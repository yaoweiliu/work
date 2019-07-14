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
	struct device_node *child = NULL;
	const __be32 *prop;
	int len, count;

	printk("%s: entence.\n", __func__);

	for_each_compatible_node(node, NULL, "DTS_demo, BiscuitOS") {
		if(node)
			printk("Find %s\n", node->full_name);
	}

	node = pdev->dev.of_node;

	prop = of_get_property(node, "BiscuitOS_name", &len);
	if(prop) {
		printk("%s: Property is BiscuitOS_name, len is %d\n", __func__, len);
		printk("%s: String is %s\n", __func__, prop);
	}

	prop = of_get_property(node, "BiscuitOS_int", &len);
	if(prop) {
		int value;

		value = be32_to_cpup(prop);
		printk("%s: Property is BiscuitOS_int, len is %d\n", __func__, len);
		printk("%s: Value is %#x\n", __func__, value);
	}

	prop = of_get_property(node, "BiscuitOS_mult", &len);
	if(prop) {
		unsigned long value;

		printk("%s: Property is BiscuitOS_mult, len is %d\n", __func__, len);
		value = of_read_ulong(prop, of_n_addr_cells(node));
		printk("%s: #cell 0 is %#lx\n", __func__, value);

		prop += of_n_addr_cells(node);
		value = of_read_ulong(prop, of_n_addr_cells(node));
		printk("%s: #cell 1 is %#lx\n", __func__, value);
	}

	prop = of_get_property(node, "BiscuitOS_leg", &len);
	if(prop)
		printk("%s: BiscuitOS_leg property exist, len is %d\n", __func__, len);
	else
		printk("%s: BiscuitOS_leg property is not exist\n", __func__);

	count = of_get_child_count(node);
	printk("%s: %s has %d children\n", __func__, node->name, count);

	printk("%s: %s child:\n", __func__, node->name);
	for_each_child_of_node(node, child)
		printk(" \"%s\"\n", child->name);

	printk("%s: %s available child:\n", __func__, node->name);
	for_each_available_child_of_node(node, child)
		printk(" \"%s\"\n", child->name);


	return misc_register(&demo_device);
}

static int dts_remove(struct platform_device *pdev)
{
	misc_deregister(&demo_device);

	return 0;
}

static const struct of_device_id DTS_demo_of_match[] = {
	{.compatible = "DTS_demo, BiscuitOS",},
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

