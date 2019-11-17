#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/slab.h>

#define	DEV_NAME	"uio_mem_demo"
static struct uio_info uio_mem_info = {
	.name = DEV_NAME,
	.version = "1.0.0",
	.irq = UIO_IRQ_NONE,//0
};

static int uio_mem_probe(struct platform_device *dev)
{
	int ret;

	uio_mem_info.mem[0].addr = (unsigned long)devm_kzalloc(&dev->dev, 24, GFP_KERNEL);
	if(!uio_mem_info.mem[0].addr) {
		printk("%s: alloc memeory error\n", __func__);
		return -ENOMEM;
	}
	uio_mem_info.mem[0].memtype = UIO_MEM_LOGICAL;
	uio_mem_info.mem[0].size = 24;

	ret = uio_register_device(&dev->dev, &uio_mem_info);
	if(ret) {
		printk("%s: uio register error\n", __func__);
		return ret;
	}

	platform_set_drvdata(dev, &uio_mem_info);

	printk("UIO -> %#lx (length: %ld)\n",\
		(unsigned long)uio_mem_info.mem[0].addr, (unsigned long)uio_mem_info.mem[0].size);

	dump_stack();

	return ret;
}

static int uio_meme_remove(struct platform_device *dev)
{
	struct uio_info *uio = (struct uio_info *)platform_get_drvdata(dev);

	kfree((void *)uio->mem[0].addr);
	uio_unregister_device(uio);

	dump_stack();

	return 0;
}

static struct platform_device *pdev;

static struct platform_driver pdrv = 
{
	.driver = {
		.name = DEV_NAME,
		.owner = THIS_MODULE,
	},
	.probe = uio_mem_probe,
	.remove = uio_meme_remove,
};

static int __init uio_mem_init(void)
{
	pdev = platform_device_register_simple(DEV_NAME, -1, NULL, 0);
	if(IS_ERR(pdev))
		return PTR_ERR(pdev);
	return platform_driver_register(&pdrv);
}

static void __exit uio_mem_exit(void)
{
	platform_driver_unregister(&pdrv);
	platform_device_unregister(pdev);
}

module_init(uio_mem_init);
module_exit(uio_mem_exit);

MODULE_LICENSE("GPL");


