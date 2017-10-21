#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  uint32_t *p;
  if(id_dest->type==OP_TYPE_MEM)
  {
	  printf("666666666666666\n");
  }
  if(id_dest->type==OP_TYPE_MEM)
  {
	  uint32_t temp;
	  temp = vaddr_read(id_dest->addr,id_dest->width);
	  printf("%x \n",temp);

	  p = &temp;
	  printf("%x \n",*p);
  }
  else
  {
	  p = &id_dest->val;
  }
  printf("%x \n",*p);
  if(decoding.is_operand_size_16)
  {
         uint32_t datat;
	 cpu.esp = cpu.esp - 2;
	 datat = (*p);
	 vaddr_write(cpu.esp,2,datat);
  }
  else
  {
	  rtl_push(p);
  }
  print_asm_template1(push);
}

make_EHelper(pop) {
 
 uint32_t temp;
 uint32_t *p;
 p = &temp;
 if(decoding.is_operand_size_16)
 {
	 temp = vaddr_read(cpu.esp,2);
	 cpu.esp = cpu.esp + 2;
 }
 else
 {
	 rtl_pop(p);
 }
 operand_write(id_dest,p);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  TODO();

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t2, &id_src->val, id_src->width);
  operand_write(id_dest, &t2);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  print_asm_template2(lea);
}
