#include "cpu/exec.h"

void diff_test_skip_qemu();
void diff_test_skip_nemu();

make_EHelper(lidt) {
  t0 = id_dest->addr;
  rtl_lm(&t1,&t0,2);
  t0 = t0 + 2;
  rtl_lm(&t2,&t0,4);
  cpu.IDTR.BASE = t2;
  cpu.IDTR.LIMIT = t1;
  
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(int) {
  t2 = (*eip);
  uint8_t NO;
  NO = id_dest->val; 
  raise_intr(NO,t2);
  print_asm("int %s", id_dest->str);

#ifdef DIFF_TEST
  diff_test_skip_nemu();
#endif
}

make_EHelper(iret) {
  rtl_pop(&t0);
  rtl_pop(&t1);
  rtl_pop(&t2);
  cpu.CS = t1;
  cpu.EFLAGS = t2;
  decoding.is_jmp = 1;
  decoding.jmp_eip = t0;

  print_asm("iret");
}

uint32_t pio_read(ioaddr_t, int);
void pio_write(ioaddr_t, int, uint32_t);

make_EHelper(in) {
  t0 = pio_read(id_src->val,id_dest->width);
  rtl_sr(id_dest->reg,id_dest->width,&t0);

  print_asm_template2(in);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(out) {
  pio_write(id_dest->val,id_src->width,id_src->val);


  print_asm_template2(out);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}
