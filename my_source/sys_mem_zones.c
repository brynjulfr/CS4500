/*
 * mem_zones system call code
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <linux/mmzone.h>
#include <asm-generic/atomic-long.h>

asmlinkage int sys_mem_zones(void)
{
   struct zone *zones;
   for_each_zone(zones)
   {
      atomic_long_t active = zones->vm_stat[NR_ACTIVE_FILE];// + zones->vm_stat[NR_ACTIVE_ANON];
      atomic_long_t inactive = zones->vm_stat[NR_INACTIVE_FILE];// + zones->vm_stat[NR_INACTIVE_ANON];
      printk("ZONE:%s ACTIVE:%ld INACTIVE:%ld\n", zones->name, active, inactive);
      
   }

   return 0;
}
