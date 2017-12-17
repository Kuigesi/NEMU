#include "common.h"

#define DEFAULT_ENTRY ((void *)0x8048000)

extern void* new_page(void);
uintptr_t loader(_Protect *as, const char *filename) {
  size_t temp;
  int fd;
  fd = fs_open(filename,0,0);
  Log("fd = %d\n",fd);
  temp = fs_filesz(fd);
  if(temp==0)
  {
	  assert(0);
  }
  int nr_page_t;
  nr_page_t = temp/0x1000;
  if((nr_page_t*0x1000)!=temp)
  {
	  nr_page_t++;
  }
  int i;
  char* va_t;
  void* ptr_t;
  for(i=0;i<nr_page_t;i++)
  {
     ptr_t = new_page();
     va_t = (char *)DEFAULT_ENTRY;
     va_t = DEFAULT_ENTRY + i*0x1000; 
     _map(as,va_t,ptr_t);
     fs_read(fd,ptr_t,0x1000);
  }
//  temp = get_ramdisk_size();
//  ramdisk_read(DEFAULT_ENTRY,0,temp);
  return (uintptr_t)DEFAULT_ENTRY;
}
