/*
 * uart.c
 *
 *  Created on: Apr 26, 2024
 *      Author: kkhandhar
 */
#include "main.h"
#include "app.h"
#include "uart.h"
#include "lin.h"

uint8_t lin_in[15];
uint8_t lin_out[15];

uint8_t g_uartReceiveComplete;
uint8_t g_uartTransmitComplete;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		g_uartTransmitComplete = 1;
	}
}
/********************************************************************
 * Function Name: HAL_UART_RxCpltCallback
 *
 * Description:
 * Callback function called upon completion of UART receive operation.
 *
 * Parameters:
 * huart: Pointer to the UART handle.
 *
 * Return Value: None
 *
 *********************************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		g_uartReceiveComplete = 1;
	}
}

uint8_t uart_transmit_data(uint8_t uartMode, uint8_t *uartBuf, uint8_t length) {
	uint8_t errCode = 0;
	if (uartMode == UART_LIN_MODE) {
		errCode = HAL_UART_Transmit_IT(&huart1, uartBuf, length);
	}

	return errCode;
}

void uart_enable_receiver(uint8_t uartMode) {
	if (uartMode == UART_LIN_MODE) {
		USART1->CR1 |= USART_CR1_RE; //USART Receiver enabled, line idle
	}
}

void uart_disable_receiver(uint8_t uartMode) {
	if (uartMode == UART_LIN_MODE) {
		USART1->CR1 &= ~USART_CR1_RE; //receiver disabled

	}
}
