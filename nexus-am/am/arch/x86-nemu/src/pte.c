#include <x86.h>
#include <stdio.h>

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void* (*palloc_f)();
static void (*pfree_f)(void*);

_Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

void _pte_init(void* (*palloc)(), void (*pfree)(void*)) {
  palloc_f = palloc;
  pfree_f = pfree;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
    }
  }

  set_cr3(kpdirs);
  //printf("cr3 = %x\n",kpdirs);
  set_cr0(get_cr0() | CR0_PG);
}

void _protect(_Protect *p) {
  PDE *updir = (PDE*)(palloc_f());
  p->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  p->area.start = (void*)0x8000000;
  p->area.end = (void*)0xc0000000;
}

void _release(_Protect *p) {
}

void _switch(_Protect *p) {
  set_cr3(p->ptr);
}

void _map(_Protect *p, void *va, void *pa) {
  uintptr_t temp,tpa1,tb0,tb1;
  temp = (uintptr_t)va;
  tb0 = (temp>>22)&0x3ff;
  tb1 = (temp>>12)&0x3ff;
  PDE *pde_ptr = (PDE*)(p->ptr);
  PTE *pte_base;
  if(pde_ptr[tb0]!=(pde_ptr[tb0] | PTE_P))
  {
     //printf(" va = %x pde =%x \n",va,pde_ptr[tb0]);
     pte_base = (PTE*)(palloc_f());
     pde_ptr[tb0] = ((uintptr_t)(pte_base)) | PTE_P;
     //printf(" va = %x pde =%x \n",va,pde_ptr[tb0]);
     pte_base[tb1] = (((uintptr_t)(pa))&0xfffff000) | PTE_P;
     return;
  }
  else
  {
     tpa1 = (uintptr_t)(pde_ptr[tb0]&0xfffff000);
     pte_base = (PTE*)tpa1;
     pte_base[tb1] = (((uintptr_t)(pa))&0xfffff000) | PTE_P;
     return;
  }
}

void _unmap(_Protect *p, void *va) {
}

_RegSet *_umake(_Protect *p, _Area ustack, _Area kstack, void *entry, char *const argv[], char *const envp[]) {
  return NULL;
}
