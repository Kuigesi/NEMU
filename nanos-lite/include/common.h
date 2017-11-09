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

#endif
