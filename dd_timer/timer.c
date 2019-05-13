#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/gfp.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>
//#include <uapi/linux/time.h>

struct data_priv
{
	struct timer_list timer;
	struct timespec time;
};

static struct data_priv *private = NULL;

static void check_time(unsigned long data)
{
	int64_t printk_time;

	ktime_get_real_ts(&private->time);
	printk_time = timespec_to_ns(&private->time);

	printk("%s: the time is %ld.\n", __func__, printk_time);

	mod_timer(&private->timer, jiffies + 1*HZ);
}

static int __init timer_demo_init(void)
{
	private = kzalloc(sizeof(struct data_priv), GFP_KERNEL);
	if(unlikely(!private)) {
		printk("%s: no memory.\n", __func__);
		return -ENOMEM;
	}

	init_timer(&private->timer);
	private->timer.function = check_time;
	private->timer.expires = jiffies + 1*HZ;
	add_timer(&private->timer);

	return 0;
}

static void __exit timer_demo_exit(void)
{
	del_timer(&private->timer);
	kfree(private);
	private = NULL;
}

device_initcall(timer_demo_init);
module_exit(timer_demo_exit);

MODULE_LICENSE("GPL");
