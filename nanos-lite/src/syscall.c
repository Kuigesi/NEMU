#include "common.h"
#include "syscall.h"

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);

  switch (a[0]) {
    case 0:  SYSCALL_ARG1(r)=sys_none();break;
    case SYS_exit:sys_exit(a[1])  ; break;
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
