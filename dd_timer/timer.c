#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/gfp.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
//#include <uapi/linux/time.h>

#define DEVNAME	"wait"

struct data_priv
{
	struct timer_list timer;
	struct timespec time;
	wait_queue_head_t wait;
	struct miscdevice miscdev;
};

static struct data_priv *private = NULL;

static void check_time(unsigned long data)
{
	unsigned long printk_time;

	ktime_get_real_ts(&private->time);
	printk_time = timespec_to_ns(&private->time);

	printk("%s: the time is %lu.\n", __func__, printk_time);

	mod_timer(&private->timer, jiffies + 5*HZ);
}

static int wait_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int wait_close(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t wait_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	if(file->f_flags & O_NONBLOCK)
		return -EAGAIN;

	return 0;
}

static ssize_t wait_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	if(file->f_flags & O_NONBLOCK)
		return -EAGAIN;

	return 0;
}

static long wait_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	return 0;
}

static unsigned int wait_poll(struct file *file, struct poll_table_struct *wait_table)
{
	return 0;
}

static const struct file_operations wait_ops = {
	.open = wait_open,
	.release = wait_close,
	.read = wait_read,
	.write = wait_write,
	.unlocked_ioctl = wait_ioctl,
	.poll = wait_poll,
};

static int __init timer_demo_init(void)
{
	int ret;

	private = kzalloc(sizeof(struct data_priv), GFP_KERNEL);
	if(unlikely(!private)) {
		printk("%s: no memory.\n", __func__);
		return -ENOMEM;
	}

	init_timer(&private->timer);
	private->timer.function = check_time;
	private->timer.expires = jiffies + 1*HZ;
	add_timer(&private->timer);

	init_waitqueue_head(&private->wait);
	private->miscdev.minor = MISC_DYNAMIC_MINOR;
	private->miscdev.name = DEVNAME;
	private->miscdev.fops = &wait_ops;

	ret = misc_register(&private->miscdev);
	if(unlikely(ret != 0)) {
		printk("%s: misc_register error.\n", __func__);
		goto misc_error;
	}

	return ret;

misc_error:
	del_timer(&private->timer);
	kfree(private);
	private = NULL;
	return ret;
}

static void __exit timer_demo_exit(void)
{
	del_timer(&private->timer);
	misc_deregister(&private->miscdev);
	kfree(private);
	private = NULL;
}

device_initcall(timer_demo_init);
module_exit(timer_demo_exit);

MODULE_LICENSE("GPL");
