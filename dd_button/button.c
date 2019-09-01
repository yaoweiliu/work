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

struct button_dev
{
	struct work_struct work;
	struct struct workqueue_struct *workqueue;
	int gpio;
};

static void work_func(struct work_struct *work)
{
	//turn on/off leds.
}

static irqreturn_t button_interruppt(int irq, void *dev)
{
	//top half do nothing.
	struct button_dev *button = (struct button_dev *)dev;

	INIT_WORK(&button->work, work_func);
	queue_work(button->workqueue, button->work);

	return IRQ_HANDLED;
}

static int button_open(struct inode *inode, struct file *file)
{
	int err;

	struct button_dev *button = kzalloc(sizeof(struct button_dev), GFP_KERNEL); //devm_kzalloc()
	if(!button)
		goto alloc_error;

	button->workqueue = create_singlethread_workqueue("button_wq");
	if(!button->workqueue)
		goto wq_error;
	//1.read datasheet, get the button's gpio, then request gpio.
	err = gpio_request_one(button->gpio, ); //devm_gpio_request_one();
	if(err)
		goto gpio_error;

	err = request_irq(gpio_to_irq(button->gpio), button_interruppt, IRQF_TRIGGER_LOW, "button", button); //gpio_to_irq(), devm_request_irq()
	if(err)
		goto irq_error;

	file->private_data = button;

irq_error:
	irq_free();
	gpio_free(button->gpio);
gpio_error:
	destroy_workqueue("button_wq");
wq_error:
	kfree(button);
alloc_error:
	return err;
}

static int button_close(struct inode *inode, struct file *file)
{

}

static ssize_t button_read(struct file *file, char __user *buffer, size_t count, loff_t *offset)
{

}

static ssize_t button_write(struct file *file, const char __user *buffer, size_t count, loff_t *offset)
{

}

static long button_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	//turn on/off leds.
}

static unsigned int button_poll(struct file *file, struct poll_table_struct *table)
{

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
	return misc_register(&button_misc_device);
}

static void __exit button_exit(void)
{
	misc_deregister(&button_misc_device);
}

module_init(button_init);
module_exit(button_exit);

MODULE_LICENSE("GPL");

