#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>
#include <linux/kernel.h>
#include <linux/delay.h>

#define DELAY1				(1000)	/* ms */
#define DELAY2				(872)	/* ms */
#define DELAY3				(1093)	/* ms */

static struct task_struct *task1, *task2, *task3;
static spinlock_t lock;
static volatile int global_value = 0;

static int threadfn1(void *data)
{
	while(1) {
		printk("%s: add\n", __func__);
		spin_lock(&lock);
		global_value += 1;
		spin_unlock(&lock);
		msleep(DELAY1);
		break;
	}

	return 0;
}

static int threadfn2(void *data)
{
	while(1) {
		spin_lock(&lock);
		printk("%s: global_value %d\n", __func__, global_value);
		spin_unlock(&lock);
		msleep(DELAY2);
		break;
	}
	
	return 0;
}

static int threadfn3(void *data)
{
	while(1) {
		printk("%s: dec\n", __func__);
		spin_lock(&lock);
		global_value -= 1;
		spin_unlock(&lock);
		msleep(DELAY3);
		break;
	}
	
	return 0;
}

static int __init thread_demo_init(void)
{
	spin_lock_init(&lock);

	task1 = kthread_run(threadfn1, NULL, "thread1");
	if(IS_ERR(task1)) {
		printk("%s: task1 create error\n", __func__);
		goto tsak1_error;
	}

	task2 = kthread_run(threadfn2, NULL, "thread2");
	if(IS_ERR(task2)) {
		printk("%s: task2 create error\n", __func__);
		goto tsak2_error;
	}

	task3 = kthread_run(threadfn3, NULL, "thread3");
	if(IS_ERR(task3)) {
		printk("%s: task3 create error\n", __func__);
		goto tsak3_error;
	}

	printk("thread_demo_init 1: %p\n", task1);
	printk("thread_demo_init 2: %p\n", task2);
	printk("thread_demo_init 3: %p\n", task3);

	dump_stack();

	return 0;

tsak3_error:
	kthread_stop(task2);
	task2 = NULL;
tsak2_error:
	kthread_stop(task1);
	task1 = NULL;
tsak1_error:
	return -ENOMEM;
}

static void __exit thread_demo_exit(void)
{
	printk("thread_demo_exit 1: %p\n", task1);
	printk("thread_demo_exit 2: %p\n", task2);
	printk("thread_demo_exit 3: %p\n", task3);

	if(task1) {
		printk("%s: task1 stoped\n", __func__);
		kthread_stop(task1);
		task1 = NULL;
	}

	if(task2) {
		printk("%s: task2 stoped\n", __func__);
		kthread_stop(task2);
		task2 = NULL;
	}

	if(task3) {
		printk("%s: task3 stoped\n", __func__);
		kthread_stop(task3);
		task3 = NULL;
	}

	dump_stack();
}


module_init(thread_demo_init);
module_exit(thread_demo_exit);

MODULE_LICENSE("GPL");

