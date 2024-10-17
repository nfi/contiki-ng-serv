#ifndef SERV_DEF_H
#define SERV_DEF_H

/* HALT address - any write to this address will exit simulation */
#define HALT_ADDR 0x90000000

/* Bitbang UART configuration */
#define UART_BITBANG_BASE 0x40000000

/* Timer configuration */
#define SERV_TIMER_BASE   0x80000000
#define SERV_TIMER_IRQ    7

#define RTIMER_ARCH_SECOND 32768

/*
 * Work around for issue with dividing 64 bit values.
 * Configure dbg-io to use 32 bit values.
*/
#define LARGEST_SIGNED long int
#define LARGEST_UNSIGNED unsigned long int

#endif /* SERV_DEF_H */
