/*
 * &soc {
 * 		UIO_intr {
 *			compatible = "am335x, uio_demo";
 *			status = "okay";
 *			u-gpio = <&gpio 25 GPIO_ACTIVE_HIGH>;
 * 		};
 * };
 *
 */


#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/uio_driver.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

/* LDD Platform Name */
#define DEV_NAME	"UIO_intr"
#define UIO_VERSION	"0.11"
static int to_user = 0;

static ssize_t uio_demo_read(struct file *file, char __user *buf, size_t count , loff_t *off)
{	
	if(copy_to_user(buf, &to_user, sizeof(int)))
		return -1;
	return 0;
}

static const struct file_operations uio_fops = {
	.read = uio_demo_read,
}; 

/* misc device */
static struct miscdevice uio_misc_dev = {
	.name = DEV_NAME,
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &uio_fops,
};

/* 0 - irq disable  1 - irq enable */
static int irq_state = 0;

/* IRQ handler */
static irqreturn_t UIO_intr_isr(int irq, struct uio_info *info)
{
	/* Disable irq and enable on userspace */
	printk("Interrupt on kernel\n");
	to_user = 1;

	return IRQ_RETVAL(IRQ_HANDLED);
}

/* IRQ controller */
static int UIO_intr_irqcontrol(struct uio_info *info, s32 irq_on)
{
	if (irq_on && !irq_state) {
		enable_irq(info->irq);
		irq_state = 1;
	} else if (!irq_on && irq_state) {
		disable_irq_nosync(info->irq);
		irq_state = 0;
	}

	return 0;
}

/* Probe: (LDD) Initialize Device */
static int UIO_intr_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct uio_info *uinfo;
	int ret, gpio;

	/* Allocate Memory */
	uinfo = kzalloc(sizeof(*uinfo), GFP_KERNEL);
	if (!uinfo) {
		printk("ERROR: No free memory\n");
		ret = -ENOMEM;
		goto err_alloc;
	}

	/* Get IRQ number from DTB */
	gpio = of_get_named_gpio(np, "u-gpio", 0);
	if (gpio < 0) {
		printk("Unable to get gpio from DTS\n");
		ret = -EINVAL;
		goto err_free;
	}

	gpio_direction_input(gpio);
	uinfo->irq = gpio_to_irq(gpio);
	if (uinfo->irq < 0) {
		printk("ERROR: Unable to get IRQ\n");
		ret = -EINVAL;
		goto err_free;
	}

	/* setup UIO */
	uinfo->name       = DEV_NAME;
	uinfo->version    = UIO_VERSION;
	uinfo->irq_flags  = IRQF_TRIGGER_FALLING | IRQF_SHARED;
	uinfo->handler    = UIO_intr_isr;
	uinfo->irqcontrol = UIO_intr_irqcontrol;

	/* Register UIO device */
	ret = uio_register_device(&pdev->dev, uinfo);
	if (ret) {
		printk("ERROR: Register UIO\n");
		ret = -ENODEV;
		goto err_free;

	}
	
	misc_register(&uio_misc_dev);

	disable_irq_nosync(uinfo->irq);
	irq_state = 0;
	platform_set_drvdata(pdev, &uinfo);
	printk("UIO Interrupt %ld Register OK...\n", uinfo->irq);

	return 0;

err_free:
	kfree(uinfo);
err_alloc:
	return ret;
}

/* Remove: (LDD) Remove Device (Module) */
static int UIO_intr_remove(struct platform_device *pdev)
{
	struct uio_info *uinfo = platform_get_drvdata(pdev);

	if (irq_state)
		disable_irq_nosync(uinfo->irq);

	misc_deregister(&uio_misc_dev);

	uio_unregister_device(uinfo);
	kfree(uinfo);
	platform_set_drvdata(pdev, NULL);

	return 0;
}

static const struct of_device_id UIO_intr_of_match[] = {
	{.compatible = "am335x, uio_demo",},
	{ },
};

MODULE_DEVICE_TABLE(of, UIO_intr_of_match);

/* Platform Driver Information */
static struct platform_driver UIO_intr_driver = {
	.probe    = UIO_intr_probe,
	.remove   = UIO_intr_remove,
	.driver	= {
		.owner	= THIS_MODULE,
		.name	= DEV_NAME,
		.of_match_table	= UIO_intr_of_match,
	},
};

module_platform_driver(UIO_intr_driver);

MODULE_LICENSE("GPL");
