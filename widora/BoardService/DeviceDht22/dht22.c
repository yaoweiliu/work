/* ERR_PTR()/IS_ERR() */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/err.h>
#include <linux/kern_levels.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/delay.h>
//#include <asm-generic/uaccess.h>
#include <asm/uaccess.h>

#include "dht22.h"

/* get data according hardware datasheet */

static void dht22_idle_state(void)
{
	gpio_direction_output(GPIO_IRQ_11, 0x1);
}

static void set_start_signal_dht22(void)
{
	//ensure dataline is high.
	gpio_direction_output(GPIO_IRQ_11, 0x1);
	udelay(10);

	//first pull down dataline 1ms.
	gpio_direction_output(GPIO_IRQ_11, 0x0);
	mdelay(20);

	//second pull up dataline. release data bus
	gpio_direction_output(GPIO_IRQ_11, 0x1);
	udelay(30);
}

static void wait_dht22_send_ack_signal(void)
{
	gpio_direction_input(GPIO_IRQ_11);

	//datasheet sequense.
	while(gpio_get_value(GPIO_IRQ_11));//if dht22 pull down, break. 80us
	while(!gpio_get_value(GPIO_IRQ_11));//if dht22 pull up, break. 80us
}

static void get_data_from_dht22(char *buf)
{
	int i, j;
	int d, count = 0;

	memset(buf, 0x0, 5);

	set_start_signal_dht22();
	wait_dht22_send_ack_signal();

	for(i = 0; i < 5; i++) {
		for(j = 0; j < 8; j++) {
			do{
				d = gpio_get_value(GPIO_IRQ_11);
				count++;
				udelay(2);
			}while(d && (count < 40));//first wait 80us, for dht22 pull up 80us.

			if(d) {
				printk("%s: timeout for wait 80us.\n", __func__);
				return ;
			}

			count = 0;
			do {
				d = gpio_get_value(GPIO_IRQ_11);
				count++;
				udelay(2);
			}while(!d && (count < 25));//first wait 50us, for dht22 pull down 50us.

			if(!d) {
				printk("%s: timeout for wait 50us.\n", __func__);
				return ;
			}

			udelay(40);//charge high or low.
			if(gpio_get_value(GPIO_IRQ_11)) {
				buf[i] |= (1 << (7-j)); //MSB
			}
		}
	}

	dht22_idle_state();
}

static irqreturn_t dht22_interrupter(int irq, void *dev_id)
{
	//TODO.
	printk("%s.\n", __func__);
	
	return IRQ_HANDLED;
}

static int dht22_open(struct inode *node, struct file *fp)
{
	int ret;

	//ret = gpio_request_one(GPIO_IRQ_11, GPIOF_IN, "dht22_irq");
	ret = gpio_request(GPIO_IRQ_11, "dht22_irq");
	if(unlikely(ret < 0)) {
		printk("%s: gpio_request_one failed.\n", __func__);
		goto gpio_request_err;
	}

	//request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags, const char *name, void *dev) 
	ret = request_irq(gpio_to_irq(GPIO_IRQ_11), dht22_interrupter, IRQF_TRIGGER_HIGH, "dht22_irq", NULL);
	if(unlikely(ret < 0)) {
		printk("%s: request_irq failed.\n", __func__);
		goto irq_request_err;
	}

	return 0;

irq_request_err:
	gpio_free(GPIO_IRQ_11);
gpio_request_err:
	return ret;
}

static int dht22_close(struct inode *node, struct file *fp)
{
	free_irq(gpio_to_irq(GPIO_IRQ_11), NULL);
	gpio_free(GPIO_IRQ_11);

	return 0;
}

static ssize_t dht22_read(struct file *fp, char __user *buf, size_t count, loff_t *offset)
{
	char data[5];
	size_t len = ARRAY_SIZE(data);

	get_data_from_dht22(data);

	len = min(count, len);
	if(!copy_to_user(buf, data, len))
		*offset += len;
	else {
		printk("%s: copy_to_user() failed.\n", __func__);
		return -EINVAL;
	}

	return len;
}

static ssize_t dht22_write(struct file *fp, const char __user *buf, size_t count, loff_t *offset)
{
	return 0;
}

static long dht22_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	return 0;
}

static unsigned int dht22_poll(struct file *fp, struct poll_table_struct *poll_table)
{
	return 0;
}

static const struct file_operations dht22_ops = {
	.open = dht22_open,
	.release = dht22_close,
	.read = dht22_read,
	.write = dht22_write,
	.unlocked_ioctl = dht22_ioctl,
	.poll = dht22_poll,
};

static struct miscdevice dht22_misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dht22",
	.fops = &dht22_ops,
};

static int __init dht22_init(void)
{
	int ret;

	ret = misc_register(&dht22_misc_dev);
	if(IS_ERR(ERR_PTR((long)ret))) { //or (ret < 0) is error.
		printk(KERN_ERR "register miscdevice error.\n");
		return ret;
	}

	return ret;
}

static void __exit dht22_exit(void)
{
	misc_deregister(&dht22_misc_dev);
}

module_init(dht22_init);
module_exit(dht22_exit);

MODULE_LICENSE("GPL");

