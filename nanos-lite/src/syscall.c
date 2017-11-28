#include "common.h"
#include "syscall.h"

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);
  a[2] = SYSCALL_ARG3(r);
  a[3] = SYSCALL_ARG4(r);

  switch (a[0]) {
    case 0:  SYSCALL_ARG1(r)=sys_none();break;
    case SYS_exit:sys_exit(a[1])  ; break;
    case SYS_write:SYSCALL_ARG1(r)=sys_write(a[1],a[2],a[3])  ; break;
    case SYS_brk:SYSCALL_ARG1(r)=sys_brk(a[1])  ; break;
    case SYS_open:SYSCALL_ARG1(r)=fs_open((char *)(a[1]),a[2],a[3]) ; break;
    case SYS_read:SYSCALL_ARG1(r)=fs_read(a[1],(void*)(a[2]),a[3])  ; break;
    case SYS_lseek:SYSCALL_ARG1(r)=fs_lseek(a[1],a[2],a[3])  ; break;
    case SYS_close:SYSCALL_ARG1(r)=fs_close(a[1]);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
int sys_none()
{
	return 1;
}
void sys_exit(uintptr_t i)
{
	int temp;
	temp = i;
	_halt(temp);
}
int  sys_write(int fd, uintptr_t buf,size_t count)
{
    int i;
    char* pch;
    char ch;
    pch = (char *)buf;
    
    if(fd==1||fd==2)
    {
     for(i=0;i<count;i++)
     { 
       ch = *(pch + i); 
       _putc(ch);
     }
     return count;
    }
    else
    {

     return fs_write(fd,pch,count);
    }
}  
int sys_brk(uintptr_t i)
{    
   return 0;
}
