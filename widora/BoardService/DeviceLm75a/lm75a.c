#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/input.h>

#define DEVNAME		"lm75a"
#define SLAVEADDR	0x48

static int lm75a_probe(struct i2c_client *client, const struct i2c_device_id *id)
{

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
	.driver = {
		.name = "DEVNAME",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(lm75a_dts_table);
	},
	.id_table = lm75a_id_table,
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
