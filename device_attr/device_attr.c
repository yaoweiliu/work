#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/miscdevice.h>

/*
struct device_attribute {                                                  
	struct attribute    attr;                                              
	ssize_t (*show)(struct device *dev, struct device_attribute *attr, char *buf);                                                    
	ssize_t (*store)(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);                               
};
*/

struct data_t
{
	struct device *dev;
	struct class *cls;
	int major;
};

#define	NAME		"attr_demo_name"
#define	MODE 		(0666)
#define	DEVNAME		"misc_demo_device"

//static ssize_t attr_show(struct kobject *kob, struct attribute *attr, char *str)
static ssize_t attr_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk("%s\n", __func__);

	return 0;
}

//static ssize_t attr_store(struct kobject *kob, struct attribute *attr, const char *str, size_t count)
ssize_t attr_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	printk("%s\n", __func__);

	return 0;
}

DEVICE_ATTR(attr_demo_name, MODE, attr_show, attr_store);

static ssize_t misc_demo_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	printk("%s\n", __func__);

	return 0;
}

static ssize_t misc_demo_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	printk("%s\n", __func__);

	return 0;
}

static int misc_demo_open(struct inode *inode, struct file *file)
{
	printk("%s\n", __func__);

	return 0;
}

static int misc_demo_close(struct inode *inode, struct file *file)
{
	printk("%s\n", __func__);

	return 0;
}

static const struct file_operations misc_demo_ops = {
	.open = misc_demo_open,
	.release = misc_demo_close,
	.read = misc_demo_read,
	.write = misc_demo_write,
};

static struct miscdevice attr_misc = {
	.name = DEVNAME,
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &misc_demo_ops,
};

static int __init attr_init(void)
{
	//register_chrdev(unsigned int major, const char *name, const struct file_operations *fops);
	//class_create(owner, name);
	//device_create(struct class *class, struct device *parent, dev_t devt, void *drvdata, const char *fmt, ...);
	//sysfs_create_file(struct kobject *kobj, const struct attribute *attr);

	misc_register(&attr_misc);
	device_create_file(attr_misc.this_device, &dev_attr_attr_demo_name); //dev_attr_attr_demo_name--->#define DEVICE_ATTR(_name, _mode, _show, _store)	\
	//																									struct device_attribute dev_attr_##_name = __ATTR(_name, _mode, _show, _store) 
	// /sys/devices/virtual/misc/misc_demo_device/attr_demo_name
	return 0;
}

static void __exit attr_exit(void)
{
	//device_destroy(struct class *class, dev_t devt);
	//class_destroy(struct class *cls);
	//unregister_chrdev(unsigned int major, const char *name);

	misc_deregister(&attr_misc);
	device_remove_file(attr_misc.this_device, &dev_attr_attr_demo_name);
}

module_init(attr_init);
module_exit(attr_exit);

MODULE_LICENSE("GPL");

