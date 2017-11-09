#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  uint32_t addr_base,addr_offset,temp,temp2,index,l1,l2,l3;
  l1 = cpu.EFLAGS;
  l2 = cpu.CS;
  l3 = ret_addr;
  rtl_push(&l1);
  rtl_push(&l2);
  rtl_push(&l3);
  index = NO;
  addr_base = cpu.IDTR.BASE;
  temp = addr_base + 8*index;
  addr_offset = vaddr_read(temp,2);
  temp = temp+6;
  temp2 = vaddr_read(temp,2);
  temp2 = temp2 << 16;
  addr_offset = addr_offset + temp2;
  decoding.is_jmp = 1;
  decoding.jmp_eip = addr_offset;




}

void dev_raise_intr() {
}
