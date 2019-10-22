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
#include <linux/list.h>
#include <linux/mutex.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/kthread.h>
//#include <uapi/linux/time.h>

#define DEVNAME	"wait"

struct device_info
{
	struct timer_list timer;
	struct timespec time;
	struct miscdevice miscdev;
};

struct data_info
{
	struct list_head list;
	unsigned long data;
};

static struct device_info *private = NULL;
static struct list_head data_queue;
static struct mutex lock;
static wait_queue_head_t event_wait;
static struct task_struct *task = NULL;

static void check_time(unsigned long arg)
{
	unsigned long printk_time;
	struct data_info *data = NULL;

	ktime_get_real_ts(&private->time);
	printk_time = timespec_to_ns(&private->time);

	data = kzalloc(sizeof(struct data_info), GFP_KERNEL);
	if(unlikely(data == NULL)) {
		printk("%s: alloc memory error.\n", __func__);
		goto memory_error;
	}
	data->data = printk_time;

	mutex_lock(&lock);
	list_add_tail(&data->list, &data_queue);
	mutex_unlock(&lock);

	wake_up_interruptible(&event_wait);

	printk("%s: the time is %lu.\n", __func__, printk_time);
    del_timer(&private->timer);
	//mod_timer(&private->timer, jiffies + 5*HZ);
memory_error:
	return ;
}

static unsigned long get_rdata(void)
{
	struct data_info *data = NULL;
	unsigned long value;

	mutex_lock(&lock);
	if(!list_empty(&data_queue)) {
		data = list_first_entry(&data_queue, struct data_info, list);
		list_del_init(&data->list);
	}
	mutex_unlock(&lock);

	if(data == NULL) {
		printk("%s: no data.\n", __func__);
		return -EINVAL;
	}

	value = data->data;
	kfree(data);

	return value;
}

static int kthread_func(void *data)
{
	printk("%s: kthread starting.\n", __func__);

	return 0;
}

static int wait_open(struct inode *inode, struct file *file)
{
	task = kthread_run(kthread_func, NULL, "kthread_test");
	if(IS_ERR(task))
		printk("%s: kthread error.\n", __func__);
	else
		printk("%s: kthread create.\n", __func__);

	return 0;
}

static int wait_close(struct inode *inode, struct file *file)
{
	if(task) {
		kthread_stop(task);
		task = NULL;
		printk("%s: kthread stoped.\n", __func__);
	}
	return 0;
}

static ssize_t wait_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	unsigned long value;

	if((file->f_flags & O_NONBLOCK) && list_empty(&data_queue))
		return -EAGAIN;

	value = get_rdata();

	count = min(count, sizeof(unsigned long));
	if(!copy_to_user(buf, &value, count))
		*offset += count;

	//printk("%s: value is %lu.\n", __func__, value);

	mdelay(6000);

	return count;
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
	unsigned int mask = 0;

	poll_wait(file, &event_wait, wait_table);
	if(!list_empty(&data_queue))
		mask = POLLIN | POLLRDNORM; 

	return mask;
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

	INIT_LIST_HEAD(&data_queue);

	private = kzalloc(sizeof(struct device_info), GFP_KERNEL);
	if(unlikely(!private)) {
		printk("%s: no memory.\n", __func__);
		return -ENOMEM;
	}

	init_timer(&private->timer);
	private->timer.function = check_time;
	private->timer.expires = jiffies + 1*HZ;
	private->timer.data = 0;
	add_timer(&private->timer);

	init_waitqueue_head(&event_wait);
	mutex_init(&lock);
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


