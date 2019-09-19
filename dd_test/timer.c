#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

static struct timer_list timer;
static int timer_data = 0;

static void cb_func(unsigned long data)
{
	printk("%s: time is over\n", __func__);
	if(timer_data) {
		del_timer(&timer);
		printk("%s: %d del timer\n", __func__, __LINE__);
		timer_data = 0;
		return ;
	} else {
		mod_timer(&timer, jiffies+5*HZ);
		printk("%s: %d mod timer\n", __func__, __LINE__);
	}
}

static ssize_t timer_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{	
	printk("%s, before timer pending is %d\n", __func__, timer_pending(&timer));

	timer.expires = jiffies+2*HZ;
	timer.function = cb_func;
	add_timer(&timer);

	printk("%s, after timer pending is %d\n", __func__, timer_pending(&timer));

	return 0;
}

static const struct file_operations timer_ops = {
	.write = timer_write,
};

static struct miscdevice dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "timer_demo",
	.fops = &timer_ops,
};

static int __init test_init(void)
{
	timer_data = 1;
	init_timer(&timer);
	return misc_register(&dev);
}

static void __exit test_exit(void)
{
	if(timer_pending(&timer))
		del_timer(&timer);
	misc_deregister(&dev);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");

