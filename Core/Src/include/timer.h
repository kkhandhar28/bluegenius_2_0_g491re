/*
 * timer.h
 *
 *  Created on: Mar 27, 2024
 *      Author: kkhandhar
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_
#include "main.h"

typedef enum{
	UART_LIN_TX_TIMEOUT = 0,
	UART_LIN_RX_TIMEOUT	   ,
	RESTR_1_MS_TIMEOUT	   ,
	RETSR_2_MS_TIMEOUT	   ,
	GPT_INCREMENT_TIMER    ,
	LIN_REFRESH_POLLING_TIMROUT,
}sysTimeout;

extern volatile uint64_t Millisecond_Tick;

void set_timeout(sysTimeout selector , uint32_t timeout_interval);
uint32_t get_timeout(sysTimeout selector);
void decrement_timeout(void);
void increment_timer(void);
#endif /* SRC_TIMER_H_ */
