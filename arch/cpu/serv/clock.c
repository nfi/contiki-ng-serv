
#include "contiki.h"
static volatile uint64_t current_time;
/*---------------------------------------------------------------------------*/
void
clock_init(void)
{
}
/*---------------------------------------------------------------------------*/
clock_time_t
clock_time(void)
{
  return current_time++;
}
/*---------------------------------------------------------------------------*/
unsigned long
clock_seconds(void)
{
  return (unsigned long)(clock_time() / RTIMER_SECOND);
}
/*---------------------------------------------------------------------------*/
void
clock_wait(clock_time_t i)
{
  clock_time_t start;

  start = clock_time();
  while(clock_time() - start < (clock_time_t)i);
}
/*---------------------------------------------------------------------------*/
void
clock_delay_usec(uint16_t dt)
{
  for(; dt > 0; dt--) {
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");
  }
}
/*---------------------------------------------------------------------------*/
rtimer_clock_t
rtimer_arch_now(void)
{
  return clock_time();
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_schedule(rtimer_clock_t t)
{
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_init(void)
{
}
/*---------------------------------------------------------------------------*/
