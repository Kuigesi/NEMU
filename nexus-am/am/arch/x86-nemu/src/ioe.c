#include <am.h>
#include <x86.h>
#include <stdio.h>

#define RTC_PORT 0x48   // Note that this is not standard
static unsigned long boot_time;

void _ioe_init() {
  boot_time = inl(RTC_PORT);
}

unsigned long _uptime() {
  unsigned long temp;
  temp = inl(RTC_PORT);
  temp = temp - boot_time;
     return temp;
}

uint32_t* const fb = (uint32_t *)0x40000;

_Screen _screen = {
  .width  = 400,
  .height = 300,
};

extern void* memcpy(void *, const void *, int);

void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h) {
  int i;
  int j;
  for (i = x; i < x + w; i++)
  {
	  for (j = y; j < y + h; j++)
	  {
		  fb[j*400 + i] =pixels[(j-y)*w +i-x];
	  }
  }
}

void _draw_sync() {
}

int _read_key() {
  uint8_t status_t;
  uint32_t data_t;
  int temp;
 status_t = inb(0x64);
 if(status_t==0x1)
 {
   data_t = inl(0x60);
   temp = data_t;
   return temp;
 }
 else
 {
  return _KEY_NONE;
 }
}
