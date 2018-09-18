#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm-generic/uaccess.h>

//static int test = 7;

ssize_t ts_read(struct file *fp, char __user *buffer, size_t size, loff_t *off)
{
	int test = 7;

	copy_to_user(buffer, &test, sizeof(int));

	return sizeof(int);
}

long ts_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	int test = 7;
	
	switch(cmd) {
	case 101:
		if(copy_to_user(&arg, &test, sizeof(int)))
			printk("failed\n");
		//put_user(test, &arg);
		else
			printk("success, test = %d, arg = %ld\n", test, arg);
		break;
	default:
		break;
	}

	return 0;
}

static const struct file_operations ts_ops = 
{
	.read = ts_read,
	.unlocked_ioctl = ts_ioctl,
	//.compat_ioctl = ts_ioctl,
};

static struct miscdevice ts = 
{
	.name = "test",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &ts_ops,
};

static int __init test_init(void)
{
	misc_register(&ts);
	return 0;
}

static void __exit test_exit(void)
{
	misc_deregister(&ts);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
