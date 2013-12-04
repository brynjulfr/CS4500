#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_helloworld(void)
{
   printk(KERN_EMERG "Hello World from John and Joel!\n");
   return 1;
}
