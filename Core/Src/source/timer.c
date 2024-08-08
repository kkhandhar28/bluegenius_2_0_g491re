/*
 * timer.c
 *
 *  Created on: Mar 27, 2024
 *      Author: kkhandhar
 */
//Include files
#include "timer.h"
#include "main.h"


//global variables
volatile uint64_t Millisecond_Tick = 0;
volatile uint32_t gui_uartTxTimeoutMs;
volatile uint32_t gui_uartRxTimeoutMs;
volatile uint32_t gui_restrTimeoutMs1;
volatile uint32_t gui_restrTimeoutMs2;
volatile uint32_t gui_linRefreshtimeoutMs;
volatile uint32_t gpt_timer_ms_1;
volatile uint32_t i2c_timeout;

void set_timeout(sysTimeout selector , uint32_t timeout_interval){
	switch(selector)
	{
	case UART_LIN_TX_TIMEOUT:
		gui_uartTxTimeoutMs = timeout_interval;
		break;

	case UART_LIN_RX_TIMEOUT:
		gui_uartRxTimeoutMs = timeout_interval;
		break;

	case RESTR_1_MS_TIMEOUT:
		gui_restrTimeoutMs1 = timeout_interval;
		break;

	case RETSR_2_MS_TIMEOUT:
		gui_restrTimeoutMs2 = timeout_interval;
		break;

	case GPT_INCREMENT_TIMER:
		gpt_timer_ms_1 = timeout_interval;
		break;

	case LIN_REFRESH_POLLING_TIMROUT:
		gui_linRefreshtimeoutMs = timeout_interval;
		break;

	default:
		break;
	}
}

uint32_t get_timeout(sysTimeout selector)
{
	switch(selector)
	{
	case UART_LIN_TX_TIMEOUT:
		return gui_uartTxTimeoutMs ;
		break;

	case UART_LIN_RX_TIMEOUT:
		return gui_uartRxTimeoutMs;
		break;

	case RESTR_1_MS_TIMEOUT:
		return gui_restrTimeoutMs1;
		break;

	case RETSR_2_MS_TIMEOUT:
		return gui_restrTimeoutMs2;
		break;

	case GPT_INCREMENT_TIMER:
		return gpt_timer_ms_1;
		break;

	case LIN_REFRESH_POLLING_TIMROUT:
		return gui_linRefreshtimeoutMs;
		break;

	default:
			break;
	}

return 0;
}


void decrement_timeout(void){
	if(gui_uartTxTimeoutMs > 0){
		gui_uartTxTimeoutMs--;
	}

	if(gui_uartRxTimeoutMs > 0){
		gui_uartRxTimeoutMs--;
	}

	if(gui_restrTimeoutMs1 > 0){
		gui_restrTimeoutMs1--;
	}

	if(gui_restrTimeoutMs2 > 0){
		gui_restrTimeoutMs2--;
	}

	if(gui_linRefreshtimeoutMs > 0){
		gui_linRefreshtimeoutMs--;
	}
}


void increment_timer(void)
{
	gpt_timer_ms_1++;
}
