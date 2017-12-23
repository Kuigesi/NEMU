#include "proc.h"
#include "memory.h"

static void *pf = NULL;

void* new_page(void) {
  assert(pf < (void *)_heap.end);
  void *p = pf;
  pf += PGSIZE;
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uint32_t new_brk) {
  //assert(0);
  if(current->cur_brk==0)
  {
	  current->cur_brk = current->max_brk = new_brk;
  }
  else
  {
	  if(new_brk > current->max_brk)
	  {
		  uint32_t nr_page;
		  nr_page = ((new_brk-1)>>12) - ((current->max_brk - 1)>>12);
		  uint32_t i;
		  uint32_t t_base,va_t;
		  t_base = (((current->max_brk-1)>>12)+1)<<12;
		  //Log("new_brk = %x current->max_brk = %x \n",new_brk,current->max_brk);
		  for(i=0;i<nr_page;i++)
		  {
			  va_t = t_base + i*0x1000;
			  void* ptr_t = new_page();
			  _map(&(current->as),(void*)va_t,ptr_t);
		  }

		  current->max_brk = new_brk;
	  }
	  current->cur_brk = new_brk;
  }
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _pte_init(new_page, free_page);
}
