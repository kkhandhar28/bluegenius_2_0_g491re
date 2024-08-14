/*
 * uart.h
 *
 *  Created on: Apr 26, 2024
 *      Author: kkhandhar
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

/********************************************************************
*  constant Defines
*********************************************************************/
#define UART_LIN_MODE 1

/********************************************************************
*  Extern Global Variables
*********************************************************************/
extern UART_HandleTypeDef huart1;
extern uint8_t lin_in[15];
extern uint8_t lin_out[15];

extern uint8_t g_uartReceiveComplete;
extern uint8_t g_uartTransmitComplete;

/********************************************************************
*   Function Prototypes
*********************************************************************/
uint8_t uart_transmit_data(uint8_t uartMode, uint8_t *uartBuf, uint8_t length);
void uart_enable_receiver(uint8_t uartMode);
void uart_disable_receiver(uint8_t uartMode);
#endif /* SRC_UART_H_ */
