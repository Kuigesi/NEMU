#include "nemu.h"
#include "monitor/monitor.h"
#include "monitor/watchpoint.h"
#include "monitor/expr.h"

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 0xffffffffffffffff

int nemu_state = NEMU_STOP;

void exec_wrapper(bool);

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
  if (nemu_state == NEMU_END) {
    printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
    return;
  }
  nemu_state = NEMU_RUNNING;

  bool print_flag = n < MAX_INSTR_TO_PRINT;

  for (; n > 0; n --) {
    /* Execute one instruction, including instruction fetch,
     * instruction decode, and the actual execution. */
    exec_wrapper(print_flag);
    bool trigger;
    uint32_t temp;
    bool success = true;
    bool *suc;
    char *str;
    trigger = false;
    suc = &success;
    WP *wp;
    wp = wphead();
    while(wp!=NULL)
    {
       str = &wp->expr[0];
       temp = expr(str,suc);
      if(temp!=wp->key)
      {
	      trigger = true;
	      printf("The value of watchpoint%d has been changed from %dto %d\n",wp->NO,wp->key,temp);
	      wp->key = temp;
      }
      wp = wp->next;
    }
    if(trigger)
    {
	nemu_state = NEMU_STOP;
    }
#ifdef DEBUG
    /* TODO: check watchpoints here. */

#endif

#ifdef HAS_IOE
    extern void device_update();
    device_update();
#endif

    if (nemu_state != NEMU_RUNNING) { return; }
  }

  if (nemu_state == NEMU_RUNNING) { nemu_state = NEMU_STOP; }
}
