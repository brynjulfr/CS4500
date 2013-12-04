/*
 * print_other system call code
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>

asmlinkage int sys_print_other(long pids)
{
   struct task_struct *task;
   for_each_process(task)
   {
      if (task->pid == pids)
      {
         printk("%s PID: [%d] STATE: %ld \n", task->comm, task->pid, task->state);
         printk("Start Time: %ld\n", task->start_time.tv_sec);
         printk("Virtual runtime: %ld\n", task->se.vruntime);
         printk("Parent Processes:\n");
         for(task=task->parent;task!=&init_task;task=task->parent)
         {
            printk("%s [%d]\n",task->comm , task->pid);
         }
         return 0;
      }
   }
   //if no thread=pids, return 1
   return 1;
}
