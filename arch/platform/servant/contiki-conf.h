#ifndef CONTIKI_CONF_H_
#define CONTIKI_CONF_H_

#include <stdint.h>
#include <string.h>
#include <inttypes.h>
/*---------------------------------------------------------------------------*/
/* Include Project Specific conf */
#ifdef PROJECT_CONF_PATH
#include PROJECT_CONF_PATH
#endif /* PROJECT_CONF_PATH */
/*---------------------------------------------------------------------------*/
#include "serv-def.h"

typedef unsigned int uip_stats_t;

#define CLOCK_CONF_SECOND 1000

#define PLATFORM_CONF_SUPPORTS_STACK_CHECK  0

#endif /* CONTIKI_CONF_H_ */
