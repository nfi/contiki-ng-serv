/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
/*---------------------------------------------------------------------------*/
#define reg_uart_data (*(volatile uint32_t*)UART_BITBANG_BASE)
/*---------------------------------------------------------------------------*/
int
dbg_putchar(int c)
{
  static bool initialized;
  if(!initialized) {
    initialized = true;
    reg_uart_data = 1;
  }

  int cout = (((unsigned char)c)|0x100) << 1;
  do {
    reg_uart_data = cout;
    cout >>= 1;
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");
  } while (cout);
  return c;
}
/*---------------------------------------------------------------------------*/
unsigned int
dbg_send_bytes(const unsigned char *s, unsigned int len)
{
  unsigned int i = 0;

  while(s && *s != 0) {
    if(i >= len) {
      break;
    }
    dbg_putchar(*s++);
    i++;
  }
  return i;
}
/*---------------------------------------------------------------------------*/
