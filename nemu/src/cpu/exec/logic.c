#include "cpu/exec.h"

make_EHelper(test) {
  uint32_t temp,temp1,temp2;
  uint32_t *p,*p1,*p2;
  temp1 = id_dest->val;
  temp2 = id_src->val;
  p = &temp;
  p1 = &temp1;
  p2 = &temp2;
  rtl_and(p,p1,p2);
  cpu.OF = cpu.CF = 0;
  rtl_update_ZFSF(p,id_dest->width);
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
  rtl_and(p,p1,p2);
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
  uint32_t temp,temp1,temp2;
  uint32_t *p,*p1,*p2;
  temp1 = id_dest->val;
  temp2 = id_src->val;
  p = &temp;
  p1 = &temp1;
  p2 = &temp2;
  rtl_or(p,p1,p2);
  operand_write(id_dest,p);
  cpu.OF = cpu.CF = 0;
  rtl_update_ZFSF(p,id_dest->width);
  print_asm_template2(or);
}

make_EHelper(sar) {
  // unnecessary to update CF and OF in NEMU
  uint32_t temp,temp1,temp2;
  uint32_t *p,*p1,*p2;
  temp1 = id_dest->val;
  temp2 = id_src->val;
  p = &temp;
  p1 = &temp1;
  p2 = &temp2;
  rtl_sext(p1,p1,id_dest->width);
  rtl_sar(p,p1,p2);
  operand_write(id_dest,p);
  rtl_update_ZFSF(p,id_dest->width);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  uint32_t temp,temp1,temp2;
  uint32_t *p,*p1,*p2;
  temp1 = id_dest->val;
  temp2 = id_src->val;
  p = &temp;
  p1 = &temp1;
  p2 = &temp2;
  rtl_shl(p,p1,p2);
  operand_write(id_dest,p);
  rtl_update_ZFSF(p,id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}
make_EHelper(rol) {
 uint32_t temp1,temp2;
 uint32_t *p1;
 temp1 = id_dest->val;
 temp2 = id_src->val;
 p1 = &temp1;
 while(temp2!=0)
 {
     rtl_msb(&t0,p1,id_dest->width);
     temp1 = temp1 << 1;
     temp1 = temp1 + t0;
     temp2 = temp2 -1;
 }
 operand_write(id_dest,p1);
 print_asm_template2(rol);
}

make_EHelper(shr) {
  // unnecessary to update CF and OF in NEMU
  uint32_t temp,temp1,temp2;
  uint32_t *p,*p1,*p2;
  temp1 = id_dest->val;
  temp2 = id_src->val;
  p = &temp;
  p1 = &temp1;
  p2 = &temp2;
  rtl_shr(p,p1,p2);
  operand_write(id_dest,p);
  rtl_update_ZFSF(p,id_dest->width);
  print_asm_template2(shr);
}
make_EHelper(ror) {
  uint32_t temp1,temp2;
  uint32_t *p1;
  temp1 = id_dest->val;
  temp2 = id_src->val;
  p1 = &temp1;
  while(temp2!=0)
  {
   t0 = temp1&0x1;
   temp1 = temp1 >> 1;
   t0 = t0<<(8*id_dest->width - 1);
   temp1 = temp1 + t0;
   temp2 = temp2 -1;
  }
  operand_write(id_dest,p1);
  print_asm_template2(ror);
}
make_EHelper(setcc) {
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not) {
  uint32_t temp;
  uint32_t *p;
  p = &temp;
  temp = id_dest->val;
  rtl_not(p);
  operand_write(id_dest,p);
  print_asm_template1(not);
}
