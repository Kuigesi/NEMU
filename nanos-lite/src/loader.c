#include "common.h"

#define DEFAULT_ENTRY ((void *)0x4000000)

uintptr_t loader(_Protect *as, const char *filename) {
  size_t temp;
  int fd;
  fd = fs_open(filename,0,0);
  Log("fd = %d\n",fd);
  temp = fs_filesz(fd);
  fs_read(fd,DEFAULT_ENTRY,temp);
//  temp = get_ramdisk_size();
//  ramdisk_read(DEFAULT_ENTRY,0,temp);
  return (uintptr_t)DEFAULT_ENTRY;
}
