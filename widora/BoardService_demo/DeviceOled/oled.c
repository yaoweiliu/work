#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/mod_devicetable.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <uapi/asm-generic/errno-base.h>
#include <asm/uaccess.h>

#include "oled.h"

static struct i2c_client *oled_client = NULL;

static void oled_reg_init(void)
{
	char init_cmd[] = {0x00, 0xae, 0x20, 0x10, 0xb0, 0xc8, \
		0x00, 0x10, 0x40, 0x81, 0xff, 0xa1, 0xa6, 0xa8, 0x3F, \
		0xa4, 0xd3, 0x00, 0xd5, 0xf0, 0xd9, 0x22, 0xda, 0x12, \
		0xdb, 0x20, 0x8d, 0x14, 0xaf};

	msleep(100);

	i2c_master_send(oled_client, init_cmd, sizeof(init_cmd));
}

int oled_open(struct inode *node, struct file *fp)
{
	printk("%s.\n", __func__);

	return 0;
}

int oled_close(struct inode *node, struct file *fp)
{
	printk("%s.\n", __func__);

	return 0;
}

ssize_t oled_write(struct file *fp, const char __user *buf, size_t count, loff_t *offset)
{
	//i2c_master_send(const struct i2c_client *client, const char *buf, int count)
	char *send_data = NULL;

	send_data = kzalloc(count+1, GFP_KERNEL);//first send data is slave device address, so count+1.
	if(!send_data) {
		printk("%s: alloc memory error.\n", __func__);
		return -ENOMEM;
	}

	if(!copy_from_user(send_data[1], buf, count))
		*offset += count;
	else {
		printk("%s: copy_from_user error.\n");
		kfree(send_data);
		return -1;
	}

	send_data[0] = 0x78;//TODO. slave device address: 0x3c/0x3d + RW bit. 1:read 0:write
	i2c_master_send(oled_client, send_data, count+1);
	
	kfree(send_data);

	return count;
}

long oled_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
	case :
	case :
	case :
	default:
		break;
	}

	return 0;
}

static const struct file_operations oled_ops = {
	.open = oled_open,
	.release = oled_close,
	.write = oled_write,
	.unlocked_ioctl = oled_ioctl,
};

static struct miscdevice oled_misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &oled_ops,
};

static int oled_probe(struct i2c_client *ssd1306_client, const struct i2c_device_id *id)
{
	int ret;

	printk("%s: %s probe.\n", __func__, DEVICE_NAME);

	oled_client = ssd1306_client;
	ret = misc_register(&oled_misc_dev);
	if(unlikely(ret != 0)) {
		printk("oled_misc_dev register failed.\n")
		return ret;
	}

	/* oled init. */
	oled_reg_init();

	return 0;
}

static int oled_remove(struct i2c_client *ssd1306_client)
{
	printk("%s: %s remove.\n", __func__, DEVICE_NAME);

	misc_deregister(&oled_misc_dev);

	return 0;
}

static struct i2c_device_id oled_id_table[] = {
	{DEVICE_NAME, 0x56},
	{"oled_ssd1308", 0x62},
	{},
};

static struct i2c_driver oled_driver = {
	.probe = oled_probe,
	.remove = oled_remove,
	.driver = {
		.name = DEVICE_NAME,
		.owner = THIS_MODULE,
	},
	.id_table = oled_id_table,
}

static int __init oled_init(void)
{
	return i2c_add_driver(&oled_driver);
}

static void __exit oled_exit(void)
{
	i2c_del_driver(&oled_driver);
}

module_init(oled_init);
module_exit(oled_exit);

MODULE_LICENSE("GPL");

