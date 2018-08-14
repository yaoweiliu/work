#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#define	NAMESIZE	5

/*
static wait_queue_head_t wq;
static wait_queue_head_t rq;
*/

typedef struct ts_data {
	int id;
	char name[NAMESIZE];
	#define HAVEDATA	1
	#define	NODATA		0
	int flags;
	wait_queue_head_t wq;
	wait_queue_head_t rq;
}ts_data;

static ts_data *data;

static int ts_open(struct inode *node, struct file *fp)
{
	int ret;

	printk("%s\n", __func__);

	data = kmalloc(sizeof(*data), GFP_KERNEL);
	if(!data) {
		ret = -ENOMEM;
		goto kmalloc_err;
	}
	
	data->id = 1314;
	strcpy(data->name, "lover");
	data->flags = HAVEDATA;

	init_waitqueue_head(&data->wq);
	init_waitqueue_head(&data->rq);

	fp->private_data = data;

	return 0;
kmalloc_err:
	return ret;
}

static int ts_release(struct inode *node, struct file *fp)
{
	ts_data *me = fp->private_data;

	printk("%s\n", __func__);

	kfree(me);

	return 0;
}

static ssize_t ts_read(struct file *fp, char __user *buffer, size_t count, loff_t *off)
{
	int ret;
	ts_data *me = fp->private_data;

	printk("%s\n", __func__);

	while(me->flags == NODATA) {
		if(fp->f_flags & O_NONBLOCK) {
			/*非阻塞访问*/
			return -EAGAIN;
		}
		else {
			/*阻塞访问*/
			wait_event(me->rq, me->flags == HAVEDATA);
		}
	}
	
	count = min(count, sizeof(me->id));
	ret = copy_to_user(buffer, (void *)&me->id, count);
	if(ret) {
		ret = -EFAULT;
		goto copyerror;
	}

	me->flags = NODATA;
	
	/*唤醒写*/
	wake_up(&me->wq);

	return count;
copyerror:
	return ret;
}

static ssize_t ts_write(struct file *fp, const char __user *buffer, size_t count, loff_t *off)
{
	int ret;
	ts_data *me = fp->private_data;

	printk("%s\n", __func__);
	
	while(me->flags == HAVEDATA) {
		if(fp->f_flags & O_NONBLOCK) {
			/*非阻塞访问*/
			return -EAGAIN;
		}
		else {
			wait_event(me->wq, me->flags == NODATA);
		}
	}

	count = min(count, sizeof(*me));
	ret = copy_from_user((void *)&me->id, (void *)buffer, count);
	if(ret) {
		ret = -EFAULT;
		goto copyerror;
	}

	me->flags = HAVEDATA;
	
	/*唤醒读*/
	wake_up(&me->rq);
	
	printk("In Kernel, the id is %d, %d\n", me->id, __LINE__);

	return count;
copyerror:
	return ret;
}

static long ts_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	//ts_data *me = fp->private_data;

	printk("%s\n", __func__);

	return 0;
}

static const struct file_operations ops = {
	.open = ts_open,
	.release = ts_release,
	.read = ts_read,
	.write = ts_write,
	.unlocked_ioctl = ts_ioctl,
};

static struct miscdevice msdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "ts_misc_dev",
	.fops = &ops,
};

static int ts_probe(struct platform_device *pdev)
{
	printk("probe\n");

	misc_register(&msdevice);

	return 0;
}

static int ts_remove(struct platform_device *pdev)
{
	printk("remove\n");

	misc_deregister(&msdevice);

	return 0;
}

static void ts_shutdown(struct platform_device *pdev)
{

}

static int ts_suspend(struct platform_device *pdev, pm_message_t state)
{
	return 0;
}

static int ts_resume(struct platform_device *pdev)
{
	return 0;
}

static void plat_dev_release(struct device *dev)
{
	
}

static struct platform_device pdev = {
	.name = "test_platform_driver",
	.id = -1,
	.resource = NULL,
	.num_resources = 0,
	.dev = {
		.release = plat_dev_release,
		.platform_data = NULL,
	},
};

static struct platform_driver pdrv = {
	.probe = ts_probe,
	.remove = ts_remove,
	.shutdown = ts_shutdown,
	.suspend = ts_suspend,
	.resume = ts_resume,
	.driver = {
		.name = "test_platform_driver",
		.owner = THIS_MODULE,
	},
	.id_table = NULL,
};

static __init int test_init(void)
{
	printk("init\n");
	
	/*if error...*/
	/*platform_device_register_simple()*/
	platform_device_register(&pdev);
	platform_driver_register(&pdrv);
	
	return 0;
}

static __exit void test_exit(void)
{
	printk("exit\n");

	platform_device_unregister(&pdev);
	platform_driver_unregister(&pdrv);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yaowei.liu");

