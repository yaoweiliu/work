#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/i2c.h>
#include <linux/input.h>

/*
 *input_report_rel()
 *input_report_key()
 *...
 *input_sync()
 *
 *i2c_msg[]
 *i2c_transfer()
 */

static struct input_dev *dev;
static int id = 1314;

static int test_probe(struct i2c_client *client, const struct i2c_device_id *id_table)
{
	int ret = 0;

	printk("%s\n", __func__);

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

static int test_remove(struct i2c_client *client)
{
	void *get_val;

	printk("%s\n", __func__);

	get_val = input_get_drvdata(dev);

	printk("get_val = %d\n", *((int *)get_val));

	input_unregister_device(dev);

	return 0;
}

static struct i2c_driver test_driver = {
	.probe = test_probe,
	.remove = test_remove,
	.driver = {
		.name = "test_i2c",
		.owner = THIS_MODULE,
	},
	.id_table = NULL,
};

static __init int test_init(void)
{
	printk("init\n");
	
	/*i2c_add_driver*/
	i2c_register_driver(THIS_MODULE, &test_driver);
	
	/*print kernel stack infomation, for debug*/
	dump_stack();

	return 0;
}

static __exit void test_exit(void)
{
	printk("exit\n");

	i2c_del_driver(&test_driver);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yaowei.liu");
