#ifndef __COMMON_H__
#define __COMMON_H__

#include <am.h>
#include <klib.h>
#include "debug.h"

typedef char bool;
#define true 1
#define false 0
void ramdisk_read(void *,off_t , size_t );
void ramdisk_write(const void *,off_t , size_t );
size_t get_ramdisk_size();
_RegSet* do_syscall(_RegSet *);
int fs_open(const char* ,int,int);
ssize_t fs_read(int,void*,size_t);
ssize_t fs_write(int,const void*,size_t);
off_t fs_lseek(int,off_t,int);
int fs_close(int fd);
size_t fs_filesz(int fd);
void dispinfo_read(void *,off_t , size_t );
void fb_write(const void *,off_t , size_t );
size_t events_read(void *,size_t);
off_t fs_filestart(int );
#endif
