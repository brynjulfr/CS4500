/*
 * mem_self system call code
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/fs.h>

asmlinkage int sys_mem_self(void)
{
   struct task_struct *task;
   task=current;
   struct mm_struct *memory;
   memory=(task->mm);
   struct vm_area_struct *vma;
   printk("%s PID: [%lu] VMEMORY: %ld\n", task->comm, task->pid, memory->total_vm);
   for(vma=memory->mmap;vma!=NULL;vma=vma->vm_next)
      {
         printk("FLAGS: %lu SIZE:%lu ",vma->vm_flags, (vma->vm_start - vma->vm_end));
         if((vma->vm_file)!=NULL)
         {
	    struct inode *inode = vma->vm_file->f_dentry->d_inode;
	    unsigned long ino = inode->i_ino;
            printk("INODE: %lu \n", ino);
         } else
            printk("No File Associated\n");
      }
   return 0;
}
