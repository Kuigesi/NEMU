#ifndef __SYSCALL_H__
#define __SYSCALL_H__
enum {

  SYS_none,
  SYS_open,
  SYS_read,
  SYS_write,
  SYS_exit,
  SYS_kill,
  SYS_getpid,
  SYS_close,
  SYS_lseek,
  SYS_brk,
  SYS_fstat,
  SYS_time,
  SYS_signal,
  SYS_execve,
  SYS_fork,
  SYS_link,
  SYS_unlink,
  SYS_wait,
  SYS_times,
  SYS_gettimeofday
};
int sys_none();
void sys_exit(uintptr_t);
int sys_write(int , uintptr_t ,size_t );
int sys_brk(uintptr_t);
//int sys_open(uintptr_t , int ,int );
//int sys_read(int , uintptr_t ,size_t );
//int sys_close(int );

#endif
