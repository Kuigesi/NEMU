#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  uint32_t *p;
  p = &id_dest->val;
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
make_EHelper(xchg)
{
	uint32_t temp;
        temp = id_src->val;
	operand_write(id_src,&id_dest->val);
	operand_write(id_dest,&temp);
	print_asm_template2("xchg");
        
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
  rtl_lr(&t0,5,4);
  rtl_sr(4,4,&t0);
  if(decoding.is_operand_size_16)
  {
       t1 = vaddr_read(cpu.esp,2);
       cpu.esp = cpu.esp + 2;
       rtl_sr(5,2,&t1);
  }
  else
  {
       t1 = vaddr_read(cpu.esp,4);
       cpu.esp = cpu.esp + 4;
       rtl_sr(5,4,&t1);
  }

  print_asm("leave");
}

make_EHelper(cltd) {
	int temp;
  if (decoding.is_operand_size_16) {
    rtl_lr(&t0,0,2);
    rtl_sext(&t0,&t0,2);
    t0 = t0 >> 16;
    rtl_sr(2,2,&t0);

  }
  else {
    rtl_lr(&t1,0,4);
    temp = t1;
     temp =  temp  >> 31;
   t1 = temp;
    rtl_sr(2,4,&t1);
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
