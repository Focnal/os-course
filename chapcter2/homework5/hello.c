#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>

static struct task_struct *my_thread;

static int thread_function(void *data)
{
    while (!kthread_should_stop())
    {
        printk(KERN_INFO "My kernel thread is running\n");
        msleep(1000);  // sleep for 1 second
    }
    return 0;
}

static int __init my_module_init(void)
{
    printk(KERN_INFO "My kernel module is loaded\n");
    my_thread = kthread_create(thread_function, NULL, "my_thread");
    if (my_thread)
        wake_up_process(my_thread);
    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "My kernel module is unloaded\n");
    if (my_thread)
        kthread_stop(my_thread);
}

module_init(my_module_init);
module_exit(my_module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module with a kernel thread");
