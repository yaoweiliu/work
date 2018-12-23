/*
 * i2c_demo.c
 * i2c_read i2c_write
 * copy from https://github.com/BiscuitOS/HardStack/blob/master/bus/i2c/kernel/i2c.c
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/of.h>

#define DEV_NAME		"i2c_demo"
#define SLAVE_I2C_ADDR	0x50

/*
 * dts.
 * I2C_BUS:		I2C 0
 * slave addr:	0x50
 *
 * &i2c0 {
 * 		status = "okay";
 *				i2cdemo@50 {
 *					compatible = "i2cdemo,eeprom";
 *					reg = <0x50>;
 * 				};
 * };
 *
 */

static int i2c_demo_write(struct i2c_client *client, unsigned char reg, unsigned char *data, unsigned long count)
{
	int ret;

	struct i2c_msg msgs[] = {
		{
			.addr = client->addr,
			.flags = client->flags & I2C_M_TEN,
			.len = 1,
			.buf = &reg,
		},
		{
			.addr = client->addr,
			.flags = client->flags,
			.len = count,
			.buf = data,
		}
	};

	ret = i2c_transfer(client->adapter, msgs, 2);
	if(ret != 2) {
		printk(KERN_ERR "%s: transfer failed.\n", __func__);
		return -1;
	}

	return 0;
}

static int i2c_demo_read(struct i2c_client *client, unsigned char reg, unsigned char *data, unsigned long count)
{
	int ret;

	struct i2c_msg msgs[] = {
		{
			.addr = client->addr,
			.flags = client->flags & I2C_M_TEN,
			.len = 1,
			.buf = &reg,
		},
		{
			.addr = client->addr,
			.flags = client->flags | I2C_M_RD,
			.len = count,
			.buf = data,
		}
	};

	ret = i2c_transfer(client->adapter, msgs, 2);
	if(ret != 2) {
		printk(KERN_ERR "%s: transfer failed.", __func__);
		return -1;
	}

	return 0;
}

static int i2c_demo_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	unsigned char addr = 0x20;
	unsigned char buf;

	i2c_demo_read(client, addr, &buf, 1);
	printk(KERN_INFO "Origin-Data: %#x\n", (unsigned int)buf);

	buf = 0x68;
	i2c_demo_write(client, addr, &buf, 1);

	i2c_demo_read(client, addr, &buf, 1);
	printk(KERN_INFO "Modify-Data: %#x\n", (unsigned int)buf);

	return 0;
}

static int i2c_demo_remove(struct i2c_client *client)
{
	return 0;
}

static struct of_device_id i2c_demo_match_table[] = {
	{.compatible = "i2cdemo,eeprom",},
	{},
};

static const struct i2c_device_id i2c_demo_id[] = {
	{DEV_NAME, SLAVE_I2C_ADDR},
	{},
};

static struct i2c_driver i2c_demo_driver = {
	.driver = {
		.name = DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = i2c_demo_match_table,
	},
	.probe = i2c_demo_probe,
	.remove = i2c_demo_remove,
	.id_table = i2c_demo_id,
};

module_i2c_driver(i2c_demo_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("i2c_demo");