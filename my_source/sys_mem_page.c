/*
 * mem_page system call code
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#include <asm/page.h>

//walk_page_table modified
static pte_t *walk_page_table(unsigned long addr)
{
   pgd_t *pgdp;
   pud_t *pudp;
   pmd_t *pmdp;
   pte_t *ptep;
   pgdp = pgd_offset_k(addr);
   if (pgd_none(*pgdp))
      return NULL;
   pudp = pud_offset(pgdp,addr);
   if (pud_none(*pudp) || pud_large(*pudp))
      return NULL;
   pmdp = pmd_offset(pudp, addr);
   if (pmd_none(*pmdp) || pmd_large(*pmdp))
      return NULL;
   ptep = pte_offset_kernel(pmdp, addr);
   if (pte_none(*ptep))
      return NULL;
   return ptep;
}

asmlinkage int sys_mem_page(long addr)
{
   struct task_struct *task;
   task=current;
   struct page *pageptr;
   pte_t *page_entry = walk_page_table(addr);
   pageptr = pte_page(*page_entry);
   printk("REFERENCED: %d\n", test_bit(PG_referenced, &pageptr->flags));
   printk("DIRTY: %d\n", test_bit(PG_dirty, &pageptr->flags));
   printk("IN MEMORY: %d\n", pte_present(*page_entry));
   return 0;
}
