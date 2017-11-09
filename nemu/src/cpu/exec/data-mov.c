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
make_EHelper(pusha) {
uint32_t a[8];
uint32_t *ptr[8];
int i;
for ( i=0;i<8;i++)
{
	ptr[i] = &(a[i]);
}
	a[0] = cpu.eax;
	a[1] = cpu.ecx;
	a[2] = cpu.edx;
	a[3] = cpu.ebx;
	a[4] = vaddr_read(cpu.esp,4);
	a[5] = cpu.ebp;
	a[6] = cpu.esi;
	a[7] = cpu.edi;
if(decoding.is_operand_size_16)
{
	for ( i=0;i<8;i++)
	{
		rtl_push_16(ptr[i]);
	}
}
else
{
        for ( i=0;i<8;i++)
	{
		rtl_push(ptr[i]);
	}
}
print_asm("pusha");
}
make_EHelper(popa) {
uint32_t a[8];
uint32_t *ptr[8];
int i;
for ( i=0;i<8;i++)
{
	ptr[i] = &(a[i]);
}

if(decoding.is_operand_size_16)
{
       for ( i=7;i>=0;i--)
       {
	       if(i!=4)
	       {
		       rtl_pop_16(ptr[i]);
		       rtl_sr(i,2,ptr[i]);
	       }
               else
	       {
		       rtl_pop_16(ptr[i]);
	       }
       }
}
else
{
       for ( i=7;i>=0;i--)
       {
	       if(i!=4)
	       {
		       rtl_pop(ptr[i]);
		       rtl_sr(i,4,ptr[i]);
	       }
	       else
	       {
		       rtl_pop(ptr[i]);
	       }
       }
}
print_asm("popa");
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
    rtl_lr(&t0,0,1);
    rtl_sext(&t0,&t0,1);
    rtl_sr(0,2,&t0);
  }
  else {
    rtl_lr(&t0,0,2);
    rtl_sext(&t0,&t0,2);
    rtl_sr(0,4,&t0);
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
