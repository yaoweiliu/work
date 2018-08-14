#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/workqueue.h>

static struct workqueue_struct *wq = NULL;
static struct work_struct wk; //如果此处定义为指针，必须要为他申请内存，不然就是野指针。\
                              就像create_singlethread_workqueue()一样，会为指针wq创建内存

static void work_func(struct work_struct *work)
{
    printk("work function\n");
}

static int __init wq_test_init(void)
{
    printk("work init\n");
    
    wq = create_singlethread_workqueue("wq_test");
    INIT_WORK(&wk, work_func);
    queue_work(wq, &wk);

    return 0;
}

static void __exit wq_test_exit(void)
{
    printk("work exit\n");
    
    destroy_workqueue(wq);
}

module_init(wq_test_init);
module_exit(wq_test_exit);

MODULE_LICENSE("GPL");
