#include "common.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t len) {
  int keycode;
  unsigned long  timecode;
  bool down; 
  keycode = _read_key();
  if(keycode != _KEY_NONE)
  {
    down = false;
    if(keycode & 0x8000)
    {
      keycode = keycode ^ 0x8000;
      down = true;
    }
    if(down)
    {
	sprintf(buf,"kd %s\n",keyname[keycode]);
    }
    else
    {
	sprintf(buf,"ku %s\n",keyname[keycode]);
    }
    //printf(" %s\n",buf);
    return strlen(buf);
  }
  else
  {
    timecode = _uptime();
    sprintf(buf,"t %d\n",timecode);
    //printf(" %s\n",buf);
    return strlen(buf);
  }

  
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) {
	char * cbuf;
	size_t i;
	cbuf = (char *)buf;
	for(i=0;i<len;i++)
	{
		*(cbuf + i) = dispinfo[offset + i];
	}
}

void fb_write(const void *buf, off_t offset, size_t len) {
	int x,y,screenwidth;
	screenwidth = _screen.width;
	offset=offset/4;
	x = offset%screenwidth;
	y = offset/screenwidth;
	//Log("len = %d\n",len);
	//Log("good");
	_draw_rect(buf,x,y,len/4,1);	
}

void init_device() {
  _ioe_init();
  int result;
  result = sprintf(dispinfo,"WIDTH:400\nHEIGHT:300");
  assert(result);
  //int len;
  //len = strlen(dispinfo);
  //Log("dispinfo  = %s\n",dispinfo);

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
