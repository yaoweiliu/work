/* https://github.com/BiscuitOS/HardStack/blob/master/Device-Tree/kernel/Demo/demo.c */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>

static int dts_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;

	printk("%s: dts demo probe.\n");

	return 0;
}

static int dts_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id DTS_demo_of_match[] = {
	{.compatible = "DTS_demo, lyw", },
	{},
};
MODULE_DEBICE_TABLE(of, DTS_demo_of_match);

static struct platform_driver dts_demo_driver = {
	.probe = dts_probe,
	.remove = dts_remove,
	.driver = {
		.name 			= "DTS_demo",
		.owner		 	= THIS_MODULE,
		.of_match_table = DTS_demo_of_match,
	},
};

module_platform_driver(dts_demo_driver);

