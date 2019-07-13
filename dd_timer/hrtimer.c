/*--------------------------------------------
 * file: hrtimer.c
 * brief: test demo code.
 *------------------------------------------*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/jiffies.h>

static struct hrtimer hrtimer_test;

static enum hrtimer_restart callback_function(struct hrtimer *timer)
{
	ktime_t kt;

	printk("%s\n", __func__);

	kt = ktime_set(1, 0);
	hrtimer_forward_now(timer, kt);

	return HRTIMER_RESTART;
}

static __init int hrtimer_init_demo(void)
{
	ktime_t kt;
	unsigned long a, b;

	printk("%s\n", __func__);

	kt = ktime_set(10, 0);
	a = 300;
	b = 200;

	if(time_after(a, b))
		printk("a > b\n");
	else
		printk("a < b\n");

	/*
	 *CLOCK_MONOTONIC: 以绝对时间为准，获取的时间为系统重启到现在的时间，更改系统时间对它没有影响。
	 *CLOCK_REALTIME: 相对时间，从1970.1.1到目前的时间。更改系统时间会更改获取的值，它以系统时间为坐标。
	 */
	hrtimer_init(&hrtimer_test, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hrtimer_start(&hrtimer_test, kt, HRTIMER_MODE_REL);
	hrtimer_test.function = callback_function;

	return 0;
}

static __exit void hrtimer_exit_demo(void)
{
	printk("%s\n", __func__);

	hrtimer_cancel(&hrtimer_test);
}

module_init(hrtimer_init_demo);
module_exit(hrtimer_exit_demo);

MODULE_LICENSE("GPL");

