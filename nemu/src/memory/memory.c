#include "nemu.h"
#include "device/mmio.h"
#include "memory/mmu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound cpu.eip = %x", addr,cpu.eip); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
	int temp;
	temp = is_mmio(addr);
	if(temp==-1)
	{
		return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
	}
        else
	{
          return  mmio_read(addr,len,temp);
	}
}

void paddr_write(paddr_t addr, int len, uint32_t data) {
	int temp;
	temp = is_mmio(addr);
	if(temp==-1)
	{
		memcpy(guest_to_host(addr), &data, len);
	}
	else
	{
             mmio_write(addr,len,data,temp);
	}
}

uint32_t vaddr_read(vaddr_t addr, int len) {
	uint32_t tp;
	tp = (addr<<20)>>20;
	tp = tp + (uint32_t)len -1;
	if(tp>0xfff)
	{
		//printf("addr = %x len = %d\n",addr,len);
	}
               paddr_t pa_tt = page_translate (addr,0);
	       return paddr_read(pa_tt, len);
}

void vaddr_write(vaddr_t addr, int len, uint32_t data) {
	uint32_t tp;
	tp = (addr<<20)>>20;
	tp = tp + (uint32_t)len -1;
	if(tp>0xfff)
	{
		//printf("addr = %x len = %d\n",addr,len);
	}
		paddr_t pa_tt = page_translate (addr,1);
		paddr_write(pa_tt, len, data);
}
paddr_t page_translate (vaddr_t addr,int w)
{
    if(cpu.cr0.paging==0)
    {
	    return addr;
    }
    uint32_t tp0,tp1,tpbase0,tpbase1,tpaddr0,tpaddr1,addr_t,temp;
    addr_t = (uint32_t)addr;
    tp0 = addr_t>>22;
    tp1 = ((addr_t<<10)>>10)>>12;
    tpbase0 = cpu.cr3.page_directory_base;
    tpbase0 = tpbase0<<12;
    tpaddr0 = tpbase0 + (tp0<<2);
    PDE pde_t;
    pde_t = (PDE)paddr_read(tpaddr0,4);
    if(pde_t.present!=1)
    {
        assert(0);
    }
    pde_t.accessed = 1;
    temp = pde_t.val;
    //temp= (uint32_t) pde_t;
    paddr_write(tpaddr0,4,temp);
    tpbase1 = pde_t.page_frame;
    tpbase1 = tpbase1<<12;
    tpaddr1 = tpbase1 + (tp1<<2);
    PTE pte_t;
    pte_t = (PTE)paddr_read(tpaddr1,4);;
    if(pte_t.present!=1)
    {
        assert(0);
    }
    pte_t.accessed = 1;
    if(w==1)
    {
        pte_t.dirty = 1;
    }
    temp= pte_t.val;
    paddr_write(tpaddr1,4,temp);
    uint32_t pa_tt;
    pa_tt =  pte_t.page_frame;
    pa_tt = pa_tt <<12;
    pa_tt = pa_tt + (addr_t&0xfff);
    return (paddr_t)pa_tt;
    
}
