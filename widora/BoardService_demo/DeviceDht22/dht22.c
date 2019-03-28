/* ERR_PTR()/IS_ERR() */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/err.h>
#include <linux/kern_levels.h>
#include <linux/fs.h>
#include <linux/kernel.h>

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

