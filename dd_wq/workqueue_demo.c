#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/timer.h>

typedef struct device_info
{
	struct timer_list timer;
	struct work_struct work;
	struct workqueue_struct *wq;
}device_info_t;

#define DEV_NAME	"wq_demo"

static void cb_work_func(struct work_struct *work)
{
	printk("%s\n", __func__);
}

static void cb_timer_func(unsigned long arg)
{
	//container_of()
	device_info_t *device_data = (device_info_t *)arg;

	if(device_data->wq)
		queue_work(device_data->wq, &device_data->work);
	else
		schedule_work(&device_data->work);

	mod_timer(&device_data->timer, jiffies + msecs_to_jiffies(1000));
}

static int wq_probe(struct platform_device *dev)
{
	device_info_t *device_data = (device_info_t *)devm_kzalloc(&dev->dev, sizeof(device_info_t), GFP_KERNEL);
	if(!device_data) {
		printk("%s: malloc error\n", __func__);
		return -ENOMEM;
	}

	setup_timer(&device_data->timer, cb_timer_func, (unsigned long)device_data);
	device_data->timer.expires = jiffies + msecs_to_jiffies(100);
	add_timer(&device_data->timer);

	INIT_WORK(&device_data->work, cb_work_func);
	device_data->wq = create_singlethread_workqueue("wq_demo");
	if(!device_data->wq)
		printk("alloc workqueue error, pelase use system wq\n");

	platform_set_drvdata(dev, device_data);

	return 0;
}

static int wq_remove(struct platform_device *dev)
{
	device_info_t *device_data = (device_info_t *)platform_get_drvdata(dev);

	del_timer(&device_data->timer);
	if(device_data->wq)
		destroy_workqueue(device_data->wq);
	kfree(device_data);

	return 0;
}

static void wq_shutdown(struct platform_device *dev)
{
	printk("%s\n", __func__);
}

static int wq_suspend(struct platform_device *dev, pm_message_t state)
{
	printk("%s\n", __func__);

	return 0;
}

static int wq_resume(struct platform_device *dev)
{
	printk("%s\n", __func__);

	return 0;
}

static struct platform_device *pdev;

static struct platform_driver pdrv = {
	.probe = wq_probe,
	.remove = wq_remove,
	.shutdown = wq_shutdown,
	.suspend = wq_suspend,
	.resume = wq_resume,
	.driver = {
		.name = DEV_NAME,
		.owner = THIS_MODULE,
	}
};

static int __init wq_init(void)
{
	pdev = platform_device_register_simple(DEV_NAME, -1, NULL, 0);
	if(IS_ERR(pdev)) {
		printk("%s: register device error\n", __func__);
		return PTR_ERR(pdev);
	}

	return platform_driver_register(&pdrv);
}

static void __exit wq_exit(void)
{
	platform_driver_unregister(&pdrv);
	platform_device_unregister(pdev);
}

module_init(wq_init);
module_exit(wq_exit);

MODULE_LICENSE("GPL");




