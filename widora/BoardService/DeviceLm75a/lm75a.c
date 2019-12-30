#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/of.h>

#include "lm75a.h"

static int lm75a_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct lm75a_pdata *pdata;
	struct input_dev *input;
	int ret;

	pdata = (struct lm75a_pdata *)kzalloc(sizeof(struct lm75a_pdata), GFP_KERNEL);
	if(!pdata) {
		printk("%s: malloc error\n", __func__);
		ret = -ENOMEM;
		goto alloc_error;
	}

	input = devm_input_allocate_device(&client->dev);
	if(!input) {
		printk("%s: input alloc error", __func__);
		ret = -ENOMEM;
		goto input_allocate_error;
	}

	input_set_drvdata(input, pdata);
	input->name = DEVNAME;
	input->open = lm75a_open;
	input->close = lm75a_close;
	input->id.bustype = BUS_HOST;

	input->evbit[0] = BIT_MASK(BIT_MASK(EV_REL));
	input_set_capability(input, EV_REL, REL_X);
	

input_allocate_error:
	kfree(pdata);
alloc_error:
	return ret;
}

static int lm75a_remove(struct i2c_client *client)
{

}

static const struct of_device_id lm75a_dts_table = {
	{.compatible = "lm75a"},
	{ },
}

static const struct i2c_device_id lm75a_id_table = {
	{"lm75a", SLAVEADDR},
	{ },
};

static struct i2c_driver lm75a_driver = {
	.probe = lm75a_probe,
	.remove = lm75a_remove,
	.id_table = lm75a_id_table,
	.driver = {
		.name = "DEVNAME",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(lm75a_dts_table);
	},
};

/*
static int __init lm75a_init(void)
{

	return i2c_add_driver(&lm75a_driver);
}

static void __exit lm75a_exit(void)
{
	i2c_del_driver(&lm75a_driver);
}

module_init(lm75a_init);
module_exit(lm75a_exit);
*/

module_i2c_driver(lm75a_driver);

MODULE_LICENSE("GPL");
