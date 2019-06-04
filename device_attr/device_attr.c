#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/sysfs.h>

struct data_t
{
	struct device *dev;
	struct class *cls;
	int major;
};

//DEVICE_ATTR(_name, _mode, _show, _store)

static int __init attr_init(void)
{
	//register_chrdev(unsigned int major, const char *name, const struct file_operations *fops);
	//class_create(owner, name);
	//device_create(struct class *class, struct device *parent, dev_t devt, void *drvdata, const char *fmt, ...);
	//sysfs_create_file(struct kobject *kobj, const struct attribute *attr);
	return 0;
}

static void __exit attr_exit(void)
{
	//device_destroy(struct class *class, dev_t devt);
	//class_destroy(struct class *cls);
	//unregister_chrdev(unsigned int major, const char *name);
}

module_init(attr_init);
module_exit(attr_exit);

MODULE_LICENSE("GPL");

