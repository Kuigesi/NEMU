#include "cpu/exec.h"

make_EHelper(test) {
  TODO();

  print_asm_template2(test);
}

make_EHelper(and) {
  uint32_t temp,temp1,temp2;
  uint32_t *p,*p1,*p2;
  temp1 = id_dest->val;
  temp2 = id_src->val;
  p = &temp;
  p1 = &temp1;
  p2 = &temp2;
  printf("%x\n",temp1);
  printf("%x\n",temp2);
  rtl_and(p,p1,p2);
  printf("%d\n",temp);
  operand_write(id_dest,p);
  cpu.OF = cpu.CF = 0;
  rtl_update_ZFSF(p,id_dest->width);
  print_asm_template2(and);
}

make_EHelper(xor) {
  uint32_t temp,temp1,temp2;
  uint32_t *p,*p1,*p2;
  temp1 = id_dest->val;
  temp2 = id_src->val;
  p = &temp;
  p1 = &temp1;
  p2 = &temp2;
  rtl_xor(p,p1,p2);
  operand_write(id_dest,p);
  cpu.OF = cpu.CF = 0;
  rtl_update_ZFSF(p,id_dest->width);
  print_asm_template2(xor);
}

make_EHelper(or) {
  TODO();

  print_asm_template2(or);
}

make_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
