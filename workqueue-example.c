#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/workqueue.h> 
#include <linux/time.h>
 
static struct workqueue_struct *queue = NULL; 
static struct work_struct work; 
static struct delayed_work delayed_work;
 
static void work_handler(struct work_struct *data) 
{   
    pr_info("[%s]: Hello LINUX.\n", __func__);
} 

static void delayed_work_handler(struct work_struct *data) 
{   
    pr_info("[%s]: Hello NCKU.\n", __func__);
}

static int __init sched_init(void) 
{ 
    ktime_t tv_start, tv_end;
    s64 nsecs;

    queue = alloc_workqueue("workqueue_example", WQ_UNBOUND, 1);
    if (!queue) {
        pr_err("Failed to allocate workqueue\n");
        return -ENOMEM;
    }

    INIT_WORK(&work, work_handler);
    tv_start = ktime_get();
    schedule_work_on(smp_processor_id(), &work);
    tv_end = ktime_get();

    nsecs = (s64) ktime_to_ns(ktime_sub(tv_end, tv_start));
    pr_info("put work in workqueue: %llu usec\n", (unsigned long long) nsecs >> 10);

    INIT_DELAYED_WORK(&delayed_work, delayed_work_handler);
    tv_start = ktime_get();
    schedule_delayed_work_on(smp_processor_id(), &delayed_work, msecs_to_jiffies(5000));
    tv_end = ktime_get();

    nsecs = (s64) ktime_to_ns(ktime_sub(tv_end, tv_start));
    pr_info("put delay_work in workqueue: %llu usec\n", (unsigned long long) nsecs >> 10);

    return 0;
}
 
static void __exit sched_exit(void)
{
    cancel_work_sync(&work);
    cancel_delayed_work_sync(&delayed_work);
    destroy_workqueue(queue);
    pr_info("sched exit.\n");
}
 
module_init(sched_init);
module_exit(sched_exit); 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("P76111262@gs.ncku.edu.tw");
MODULE_DESCRIPTION("Workqueue example");