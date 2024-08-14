/*
 * systeminit.c
 *
 *  Created on: Aug 12, 2024
 *      Author: kkhandhar
 */


/*******************************************************************************
 * Included headers
 *******************************************************************************/
#include "main.h"
#include "systeminit.h"
#include "uart.h"

/***********************************************************************
*  Function Name: system_init
*
*  Description: This function Initializes the system components.
*
*  Parameters: None
*
*  Return : None
*
***********************************************************************/
void system_init(void){
	/** SysTick Configuration **/
	SystemCoreClockUpdate();

	/** Generate interrupt every 1ms **/
	SysTick_Config(SystemCoreClock/1000);
	SysTick ->CTRL = 0;
	SysTick ->VAL = 0;
	SysTick ->CTRL = (SysTick_CTRL_TICKINT_Msk
					 |SysTick_CTRL_ENABLE_Msk
					 |SysTick_CTRL_CLKSOURCE_Msk);

	//CS lin pin high to select lin transceiver in operational mode
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);


	uart_disable_receiver(UART_LIN_MODE);
}
