#include "contiki.h"
#include "sys/platform.h"

unsigned int dbg_send_bytes(const unsigned char *s, unsigned int len);

/*---------------------------------------------------------------------------*/
void
platform_init_stage_one(void)
{
  dbg_send_bytes("\nSERVANT!\n", 10);
}
/*---------------------------------------------------------------------------*/
void
platform_init_stage_two(void)
{
}
/*---------------------------------------------------------------------------*/
void
platform_init_stage_three(void)
{
}
/*---------------------------------------------------------------------------*/
void
platform_idle(void)
{
}
/*---------------------------------------------------------------------------*/
