#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pwm.h>
#include <linux/sched.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/sizes.h>
#include <linux/delay.h>

static int rgb_irq;
#define CHIPID	0x10000000
void __iomem *base = NULL;

struct platform_data_st
{
	int id;
	char *info;
	struct pwm_device *pwm_rgb;
};

struct platform_data_st rgb_pds = 
{
	.id = 10,
	.info = "rgb platform data",
	.pwm_rgb = NULL,
};

static irqreturn_t rgb_interrupt(int id, void *dev)
{
	printk("%s: id is %d.\n", __func__, id);

	return IRQ_HANDLED;
}

static void request_rgb_irq(void)
{
	rgb_irq = gpio_to_irq(gpio_request(14, "rgb_gpio"));
	request_irq(rgb_irq, rgb_interrupt, IRQF_TRIGGER_NONE, "rgb_interrupt", NULL);

	//return rgb_irq;
}

static void release_rgb_irq(void)
{
	free_irq(rgb_irq, NULL);
	gpio_free(14);
}

struct resource rgb_resource[] = 
{
	[0] = {
		.start = 0x11000000,
		.end = 0x11000000 + 0x1000,
		.name = "rgb_regs",
		.flags = IORESOURCE_MEM,
	},

	[1] = {
		.start = 14,
		.end = 14 + 2,
		.name = "rgb_irq",
		.flags = IORESOURCE_IRQ,
	}
};

void rgb_release(struct device *dev)
{
	printk("%s: plarform data released.\n", __func__);
}

static struct platform_device rgb_device = 
{
	.name = "rgb_pwm",
	.id = -1,
	.dev = {
		.platform_data = &rgb_pds,
		.driver_data = &rgb_pds,
		.release = rgb_release,
	},
	.resource = rgb_resource,
	.num_resources = ARRAY_SIZE(rgb_resource),
};

static int rgb_probe(struct platform_device *pdev)
{
	struct resource *res = NULL;
	//struct pwm_device *rgb_pwm = NULL;
	struct platform_data_st *data = NULL;
	unsigned int chip_id, rev_id, wlen_status;

	dump_stack();

	printk("%s: %s.\n", __func__, pdev->name);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	printk("%s: mem name is %s, mem start is 0x%x, mem end is 0x%x.\n", __func__, res->name, res->start, res->end);

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	printk("%s: irq name is %s, irq start is %d, irq end is %d.\n", __func__, res->name, res->start, res->end);

	data = (struct platform_data_st *)platform_get_drvdata(pdev);
	printk("%s: id is %d, info is %s, pwm_rgb is 0x%x.\n", __func__, data->id, data->info, data->pwm_rgb);

	data->pwm_rgb = pwm_get(&pdev->dev, "rgb_pwm");
	printk("%s: pwm_rgb is 0x%x.\n", __func__, data->pwm_rgb);

	base = ioremap(CHIPID, SZ_16K);
	printk("%s: base is 0x%x.\n", __func__, base);

	chip_id = ioread32(base);
	printk("%s: chip_id is 0x%x.\n", __func__, chip_id);

	rev_id = ioread32(base + 12);
	printk("%s: rev_id is 0x%x.\n", __func__, rev_id);

	wlen_status = ioread32(base + 100);
	printk("%s: wlen_status is 0x%x.\n", __func__, wlen_status);

	iowrite32(0x552, base + 100);//off
	mdelay(2000);
	iowrite32(0x550, base + 100);//on

	return 0;
}

static int rgb_remove(struct platform_device *pdev)
{
	struct platform_data_st *data = NULL;

	printk("%s: %s.\n", __func__, pdev->name);

	data = (struct platform_data_st *)platform_get_drvdata(pdev);

	printk("%s: id is %d, info is %s, pwm_rgb is 0x%x.\n", __func__, data->id, data->info, data->pwm_rgb);
	//pwm_put(data->pwm_rgb); this call will fault. why?

	iounmap(base);
	base = NULL;
	printk("%s: base is 0x%x.\n", __func__, base);

	return 0;
}

static const struct platform_device_id rgb_id_table[] = 
{
	{"rgb_pwm", 0x10},
	{"rgb_test", 0x12},
	{},
};

static struct platform_driver rgb_driver = 
{
	.probe = rgb_probe,
	.remove = rgb_remove,
	.driver = {
		.name = "rgb_pwm",
		.owner = THIS_MODULE,
	},
	.id_table = rgb_id_table,
};

static int __init rgb_init(void)
{
	int ret;

	request_rgb_irq();

	printk("%s: irq is %d.\n", __func__, rgb_irq);

	ret = platform_device_register(&rgb_device);
	if(unlikely(ret < 0)) {
		printk("%s: platform_device_register error.\n", __func__);
		return ret;
	}

	ret = platform_driver_register(&rgb_driver);
	if(unlikely(ret < 0)) {
		printk("%s: platform_driver_register error.\n", __func__);
		platform_device_unregister(&rgb_device);
		return ret;
	}

	return 0;
}

static void __exit rgb_exit(void)
{
	platform_device_unregister(&rgb_device);
	platform_driver_unregister(&rgb_driver);
	release_rgb_irq();
}

module_init(rgb_init);
module_exit(rgb_exit);

MODULE_LICENSE("GPL");

