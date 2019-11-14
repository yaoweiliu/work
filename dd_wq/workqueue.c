#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/timer.h>

#define DEV_NAME	"wq_demo"

static struct platform_device *pdev;

static int __init wq_init(void)
{
	pdev = platform_device_register_simple(DEV_NAME, -1, NULL, 0);
}


