#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>

static struct uio_info kpart_info = {
	.name = "kpart",
	.version = "0.0.1",
};

static int drv_kpart_probe(struct platform_device *pdev)
{

	return 0;
}

static int drv_kpart_remove(struct platform_device *pdev)
{

	return 0;
}

static struct platform_device *uio_dummy_device = NULL;

static struct platform_driver uio_dummy_driver = {
	.driver = {
		.name = "kpart",
		.owner = THIS_MODULE,
		.of_match_table = NULL,
	},
	.probe = drv_kpart_probe,
	.remove = drv_kpart_remove,
};

static int __init uio_demo_init(void)
{
	uio_dummy_device = platform_device_register_simple("kpart", -1, NULL, 0);
	if(!uio_dummy_device) {
		printk("platform_device_register_simple error\n");
		return -ENOMEM;
	}

	return platform_driver_register(&uio_dummy_driver);
}

static void __exit uio_demo_exit(void)
{
	platform_device_unregister(uio_dummy_device);
	platform_driver_unregister(&uio_dummy_driver);
}

module_init(uio_demo_init);
module_exit(uio_demo_exit);

MODULE_LICENSE("GPL");



