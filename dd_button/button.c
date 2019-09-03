#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/spinlock.h>

static struct list_head event_queue;
static spinlock_t lock;
static wait_queue_head_t wait_queue;

struct button_data
{
	struct list_head head;
	char to_user[0];
};

struct button_dev
{
	struct work_struct work;
	struct workqueue_struct *workqueue;
	int led_gpio;
	int button_gpio;
};

static void work_func(struct work_struct *work)
{
	//turn on/off leds.
	struct button_dev *button = container_of(work, struct button_dev, work);
	gpio_set_value(button->led_gpio, 0);
	msleep(10); //wq can sleep.
	gpio_set_value(button->led_gpio, 1);// read datasheet.
}

static irqreturn_t button_interruppt(int irq, void *dev)
{
	//top half do nothing.
	unsigned long flags;
	struct button_dev *button = (struct button_dev *)dev;

	struct button_data *data = (struct button_data *)kzalloc(sizeof(struct button_data)+7, GFP_ATOMIC);
	if(!data)
		printk("%s: alloc memory error.\n", __func__);
	//data->to_user = "touser"; error
	memcpy(data->to_user, "touser", 7);

	spin_lock_irqsave(&lock, flags);
	list_add_tail(&data->head, &event_queue);
	spin_unlock_irqrestore(&lock, flags);

	wake_up_interruptible(&wait_queue);

	INIT_WORK(&button->work, work_func);
	queue_work(button->workqueue, &button->work);

	return IRQ_HANDLED;
}

static int button_open(struct inode *inode, struct file *file)
{
	int err;

	struct button_dev *button = kzalloc(sizeof(struct button_dev)+6, GFP_KERNEL); //devm_kzalloc()
	if(!button)
		goto alloc_error;
	button->button_gpio = 30; //GPIO0_30
	button->led_gpio = 39; //GPIO1_7

	button->workqueue = create_singlethread_workqueue("button_wq");
	if(!button->workqueue)
		goto wq_error;
	//1.read datasheet, get the button's gpio, then request gpio.
	err = gpio_request_one(button->button_gpio, GPIOF_DIR_IN, "button_gpio"); //devm_gpio_request_one();
	if(err)
		goto button_gpio_error;
	err = gpio_request_one(button->led_gpio, GPIOF_DIR_OUT, "led_gpio");
	if(err)
		goto led_gpio_error;

	err = request_irq(gpio_to_irq(button->button_gpio), button_interruppt, IRQF_TRIGGER_LOW, "button", button); //gpio_to_irq(), devm_request_irq()
	if(err)
		goto irq_error;

	file->private_data = button;

irq_error:
	free_irq(gpio_to_irq(button->button_gpio), button);
	gpio_free(button->button_gpio);
	gpio_free(button->led_gpio);
led_gpio_error:
	gpio_free(button->button_gpio);
button_gpio_error:
	destroy_workqueue(button->workqueue);
wq_error:
	kfree(button);
alloc_error:
	return err;
}

static int button_close(struct inode *inode, struct file *file)
{
	struct button_dev *button  = (struct button_dev *)file->private_data;

	free_irq(gpio_to_irq(button->button_gpio), button);
	gpio_free(button->button_gpio);
	gpio_free(button->led_gpio);
	gpio_free(button->button_gpio);
	destroy_workqueue(button->workqueue);
	kfree(button);

	return 0;
}

static ssize_t button_read(struct file *file, char __user *buffer, size_t count, loff_t *offset)
{
	//get data from event_queue; and free memory.
	return 0;
}

static ssize_t button_write(struct file *file, const char __user *buffer, size_t count, loff_t *offset)
{
	return 0;
}

static long button_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	//turn on/off leds.
	return 0;
}

static unsigned int button_poll(struct file *file, struct poll_table_struct *table)
{
	return 0;
}

static const struct file_operations button_ops = {
	.open = button_open,
	.release = button_close,
	.read = button_read,
	.write = button_write,
	.unlocked_ioctl = button_ioctl,
	.poll = button_poll,
};

static struct miscdevice button_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "button_dev",
	.fops = &button_ops,
};

static int __init button_init(void)
{
	spin_lock_init(&lock);
	INIT_LIST_HEAD(&event_queue);
	init_waitqueue_head(&wait_queue);
	return misc_register(&button_misc_device);
}

static void __exit button_exit(void)
{
	misc_deregister(&button_misc_device);
}

module_init(button_init);
module_exit(button_exit);

MODULE_LICENSE("GPL");

