#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/slab.h>

static struct button_irq_desc {
	int irq;
	int num;
	char *name;
};

static struct button_irq_desc button_irqs[] = {
	{IRQ_EINT8, 1, "KEY0"},
	{IRQ_EINT11, 2, "KEY1"},
	{IRQ_EINT13, 3, "KEY2"},
	{IRQ_EINT14, 4, "KEY3"},
	{IRQ_EINT15, 5, "KEY4"},
};

static irqreturn_t priv_interrupt(int irq, void *dev)
{
	struct uio_info *info = (struct uio_info *)dev;
	unsigned long *ret_val_add = (unsigned long *)(info->mem[1].addr);
	*ret_val_add = 10;

	printk("%s: val is %d\n", __func__, (int)(*ret_val_add));

	return IRQ_HANDLED;
}

static irqreturn_t button_interrupt(int irq, void *dev)
{

	return IRQ_HANDLED;
}

static struct uio_info kpart_info = {
	.name = "kpart",
	.version = "0.0.1",
	.irq = IRQ_EINT19, //arch/arm/mach-s3c24xx/include/mach/irqs.h
	.handler = priv_interrupt,
	.irq_flags = IRQ_TYPE_EDGE_RISING, //include/linux/irq.h
};

static int drv_kpart_probe(struct platform_device *pdev)
{
	printk("%s.\n", __func__);

	unsigned long phys = 0x56000010;//GPBCON
	kpart_info.mem[0].addr = phys;
	kpart_info.mem[0].memtype = UIO_MEM_PHYS;
	kpart_info.mem[0].size = 12;

	kpart_info.mem[1].addr = (unsigned long)kmalloc(4, GFP_KERNEL);
	if(!kpart_info[1].addr)
		return -ENOMEM;
	kpart_info.mem[1].memtype = UIO_MEM_LOGICAL;
	kpart_info.mem[1].size = 4;

	unsigned long *ret_val_add = (unsigned long *)(kpart_info.mem[1].addr);
	*ret_val_add = 999;

	if(uio_register_device(&pdev->dev, &kpart_info)) {
		kfree(kpart_info.mem[1].addr);
		return -ENODEV:
	}

	int i = 0, err = 0;
	for(i = 0; i < sizeof(button_irqs)/sizeof(button_irqs[0]); i++) {
		err = request_irq(button_irqs[i].irq, button_interrupt, IRQ_TYPE_EDGE_RISING, button_irqs[i].name, (void *)&button_irqs[i]);
		if(err)
			break;
	}

	return 0;
}

static int drv_kpart_remove(struct platform_device *pdev)
{
	kfree(kpart_info.mem[1].addr);
	uio_unregister_device(&kpart_info);

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



