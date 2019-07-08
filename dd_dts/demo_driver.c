#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/of_platform.h>

#define DEMO_NAME	"demo_driver"
#define DEVICE_NAME	"DTS_demo"

static int demo_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int demo_close(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t demo_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	return 0;
}

static ssize_t demo_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	return 0;
}

static const struct file_operations demo_ops = 
{
	.open = demo_open,
	.release = demo_close,
	.read = demo_read,
	.write = demo_write,
};

static struct miscdevice demo_device = 
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEMO_NAME,
	.fops = &demo_ops,
};

/*
static __init int demo_init(void)
{
	return misc_register(&demo_device);
}

static __exit void demo_exit(void)
{
	misc_deregister(&demo_device);
}
*/

static int dts_probe(struct platform_device *pdev)
{
	struct device_node *node = NULL;

	printk("%s: entence.\n", __func__);

	for_each_compatible_node(node, NULL, "DTS_demo, BiscuitOS") {
		if(node)
			printk("Find %s\n", node->full_name);
	}

	return misc_register(&demo_device);
}

static int dts_remove(struct platform_device *pdev)
{
	misc_deregister(&demo_device);

	return 0;
}

static const struct of_device_id DTS_demo_of_match[] = {
	{.compatible = "ilitek,ili9341",},
	{ },
};

static struct platform_driver DTS_demo_driver = {
	.probe = dts_probe,
	.remove = dts_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = DEVICE_NAME,
		.of_match_table = DTS_demo_of_match,
	},
};

module_platform_driver(DTS_demo_driver);

//module_init(demo_init);
//module_exit(demo_exit);

MODULE_LICENSE("GPL");

