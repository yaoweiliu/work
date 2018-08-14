#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/kernel.h>

/*
 *input_report_rel()
 *input_report_key()
 *...
 *input_sync()
 */

static struct input_dev *dev;
static int id = 1314.0;

static int __init input_init(void)
{
	int ret = 0;

	printk("Input init\n");
	
	dump_stack();

	dev = input_allocate_device();
	if(dev == NULL)
		return -ENOMEM;
	dev->name = "test_input_dev";
	dev->phys = "20171124";

	set_bit(EV_REL, dev->evbit);
	set_bit(REL_X, dev->evbit);

	set_bit(EV_KEY, dev->keybit);
	set_bit(BTN_1, dev->keybit);
	
	ret = input_register_device(dev);
	if(ret) {
		printk("register input_dev error\n");
		goto register_error;
	}

	input_set_drvdata(dev, (void *)&id);

	return ret;
register_error:
	input_free_device(dev);
	return ret;
}

static void __exit input_exit(void)
{
	void *get_val;

	printk("Input exit\n");

	get_val = input_get_drvdata(dev);

	printk("get_val = %d\n", *((int *)get_val));

	input_unregister_device(dev);
}

module_init(input_init);
module_exit(input_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yaowei.liu");

