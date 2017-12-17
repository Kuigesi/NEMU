#include "fs.h"
extern uint8_t ramdisk_start;
typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS, FD_DISPINFO, FD_NORMAL};
/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin (note that this is not the actual stdin)", 0, 0},
  {"stdout (note that this is not the actual stdout)", 0, 0},
  {"stderr (note that this is not the actual stderr)", 0, 0},
  [FD_FB] = {"/dev/fb", 0, 0},
  [FD_EVENTS] = {"/dev/events", 0, 0},
  [FD_DISPINFO] = {"/proc/dispinfo", 128, 0},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  int i,j;
  i = _screen.width;
  j = _screen.height;
  file_table[FD_FB].size = i*j*4;
  file_table[FD_FB].disk_offset = file_table[FD_FB].open_offset = 0;
}
int fs_open(const char *pathname,int flags,int mode)
{
	const char* name_t;
	char* name_p;
	int test;
	int i;
	for(i=0;i<NR_FILES;i++)
	{
		name_t = pathname;
		test = 0;
		name_p = file_table[i].name;
		while((*name_t)==(*name_p))
		{
			if((*name_t) == '\0')
			{
				test = 1;
				break;
			}
			name_t++;
			name_p++;
		}
		if(test ==1)
		{
			//printf("fd = %d\n",i);
			file_table[i].open_offset = 0;
			return i;
		}
	}
        	
	//off_t addr;
	//addr = (off_t)(&ramdisk_start);
        //printf("addr = %x\n",addr);
	assert(0);
	return -1;
}
ssize_t fs_read(int fd,void *buf,size_t len)
{
   //off_t addr;
   //Log("len = %d\n",len);
   off_t offset_t;
   off_t offset_limit;
   offset_t = file_table[fd].disk_offset + file_table[fd].open_offset;
   offset_limit = file_table[fd].disk_offset + file_table[fd].size;
   //addr = offset_t + (off_t)(&ramdisk_start)+ len ;
   //printf("addr = %x\n",addr);
   if(fd == FD_EVENTS)
   {
       //Log("read dispinfo\n");
      return events_read(buf,len);
      assert(0);
   }
   if((offset_t + len) <= offset_limit)
   {
       if(fd == FD_DISPINFO)
       {
	 dispinfo_read(buf,offset_t,len);
       }
       else
       {
	 ramdisk_read(buf,offset_t,len);
       }
   }
   else
   {
       len = offset_limit - offset_t;

       if(fd == FD_DISPINFO)
       {
	 dispinfo_read(buf,offset_t,len);
       }
       else
       { 
	 ramdisk_read(buf,offset_t,len);
       }
   }
   //printf("addr = %x len = %d\n",addr,len);
  // printf("&n = %p\n",buf);
   file_table[fd].open_offset = file_table[fd].open_offset + len;
   return len;
}
ssize_t fs_write(int fd,const void *buf,size_t len)
{
   //off_t addr;
   //Log("len = %d\n",len);   
   off_t offset_t;
   off_t offset_limit;
   
   offset_t = file_table[fd].disk_offset + file_table[fd].open_offset;
   offset_limit = file_table[fd].disk_offset + file_table[fd].size;
   //Log("offset_t = %d\n",offset_t);
   //Log("offset_limitt = %d\n",offset_limit);
   //addr = offset_t + (off_t)(&ramdisk_start)+ len ;
   //printf("addr = %x\n",addr);
   if((offset_t + len) <= offset_limit)
   {
       if(fd == FD_FB)
       { 
	 fb_write(buf,offset_t,len);	 
       }
       else
       {
	 ramdisk_write(buf,offset_t,len);
       }
   }
   else
   {
       len = offset_limit - offset_t;
       if(fd == FD_FB)
       {
	 fb_write(buf,offset_t,len);
       }
       else
       {
         ramdisk_write(buf,offset_t,len);
       }
   }
   file_table[fd].open_offset = file_table[fd].open_offset + len;
   return len;
}
off_t fs_lseek(int fd,off_t offset,int whence)
{


     //assert(fd!=FD_FB);
     if(fd==17)
     {
	     Log("1.rpg lseek\n");
     }
     switch(whence)
     {
      case  SEEK_SET : if(offset <= file_table[fd].size)
		  {
                   file_table[fd].open_offset = offset;
		  }
		  else
		  {
                   file_table[fd].open_offset = file_table[fd].size;
		  }
                  return file_table[fd].open_offset;
		  break;
      case SEEK_CUR :  if((file_table[fd].open_offset+offset) <= file_table[fd].size)
		  {
                   file_table[fd].open_offset = file_table[fd].open_offset+offset;
		  }
		  else
		  {
	           file_table[fd].open_offset = file_table[fd].size;
		  }
		  return file_table[fd].open_offset;
		  break;
      case SEEK_END :  if((file_table[fd].size+offset) <= file_table[fd].size)
		  {
                   file_table[fd].open_offset = file_table[fd].size+offset;
		  }
		  else
		  {
                   file_table[fd].open_offset = file_table[fd].size;
		  }
		  return file_table[fd].open_offset;
		  break;
      default :   assert(0);
		  return -1;
     }
}
int fs_close(int fd)
{
	
	return 0;
}
size_t fs_filesz(int fd)
{
	
	return file_table[fd].size;
}
off_t fs_filestart(int fd)
{
	return file_table[fd].disk_offset;
}
