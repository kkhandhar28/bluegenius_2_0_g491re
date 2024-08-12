/*
 * lin.c
 *
 *  Created on: Aug 8, 2024
 *      Author: kkhandhar
 */


/*
 * lin.c
 *
 *  Created on: Apr 25, 2024
 *      Author: kkhandhar
 */

#include "app.h"
#include "main.h"
#include "lin.h"
#include "uart.h"
#include "linid.h"
#include "timer.h"

volatile LIN_DetectNodeTask linDetectNodeState = LIN_IDLE_STATE;
uint8_t g_numActiveLinNodes;
uint8_t g_linId;
volatile uint8_t g_linTXLength;
volatile uint8_t g_linRxLength;
uint8_t g_getVersionData = 0;
LIN_NodeTypes g_nodeType;


uint8_t ActiveLINNodes[MAX_ACTIVE_LINNODES][18] =
{
	//   LINID, NODE_TYPE, BL_VER, BL_STATUS, APP_VER, ONLINE_STATUS, NODE_STATUS, INPUTS1, INPUTS2, OUTPUTS1, OUTPUTS2, SENSOR1H, SENSOR1L, SENSOR2H, SENSOR2L, SPARE
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


/********************************************************************
 * Function Name : lin_get_version
 *
 * Description
 * This function initiates the process of retrieving the version of a
 * LIN node
 *
 * Parameters
 * linId    : The LIN node ID
 * nodeType : The type of LIN node
 *
 * Return Value
 * ucErrCode - 0 for successful operation
 * 			 - 1 for operation failure
 *********************************************************************/
uint8_t lin_get_version(uint8_t linId , LIN_NodeTypes nodeType , uint8_t * nodeDetectedIndex) {
	uint8_t ucCalcdChksum, ucErrCode = 0;
	uint8_t refreshStatus;
	static uint8_t linInit = 1;
	switch (linDetectNodeState) {
	case LIN_IDLE_STATE:
		if (linInit == 1) {
			linDetectNodeState = LIN_SEND_STATE;
		}
		break;

	case LIN_SEND_STATE:
		linmode_getversion(linId);
		linDetectNodeState = LIN_SENDING_STATE;
		break;

	case LIN_SENDING_STATE:
		HAL_LIN_SendBreak(&huart1);
		ucErrCode = uart_transmit_data(1,&lin_out[LINCMD_SYNC] , g_linTXLength);
		set_timeout(UART_LIN_TX_TIMEOUT, 50);
		linDetectNodeState = LIN_SENDING_WAIT_STATE;
		break;

	case LIN_SENDING_WAIT_STATE:
		if(g_uartTransmitComplete == 1 )
		{
			g_uartTransmitComplete = 0;
			uart_enable_receiver(UART_LIN_MODE);
			set_timeout(UART_LIN_RX_TIMEOUT, 10);
			linDetectNodeState = LIN_RECEIVE_STATE;
		}else if(get_timeout(UART_LIN_TX_TIMEOUT) == 0){
			ucErrCode = 1; //transmit failed
			g_getVersionData = 1;
			linDetectNodeState = LIN_WAIT_STATE; //transmissin failed
			set_timeout(UART_LIN_TX_TIMEOUT, 10);

		}
		break;

	case LIN_RECEIVE_STATE:
		if (g_uartReceiveComplete == 1) {
			g_uartReceiveComplete = 0;
			uart_disable_receiver(UART_LIN_MODE);
			lin_in[LINRESP_ID] = linId;
			ucCalcdChksum = lin_checksum(&lin_in[LINRESP_ID], 4); //checksum of data including ID
			//checksum matches, data is good
			if (lin_in[LINRESP_CHK3DATA] != ucCalcdChksum) {
				g_getVersionData = 1;
				ucErrCode = LIN_ERROR_CHECKSUM;
				linDetectNodeState = LIN_SEND_STATE;
			} else if(g_numActiveLinNodes == MAX_ACTIVE_LINNODES){
				g_getVersionData = 1;
				ucErrCode = LIN_ERROR_MAXACTIVENODES;
				linDetectNodeState = LIN_SEND_STATE;
			}else {
				ucErrCode = 0;
				lin_save_versionnodedata(&lin_in[LINRESP_ID],nodeType,nodeDetectedIndex);
				linDetectNodeState = LIN_RECEIVE_COMPLETE_STATE;
			}
		} else if (get_timeout(UART_LIN_RX_TIMEOUT) == 0) { //wait 10 ms for detecting node
			g_getVersionData = 1;
			ucErrCode = 2; //receive failed //no node detected
			linDetectNodeState = LIN_SEND_STATE;
		}
		break;

	case LIN_RECEIVE_COMPLETE_STATE:
		refreshStatus = lin_refresh_node(1);
		if(refreshStatus == 1)
		{
			g_getVersionData = 1;
			linDetectNodeState = LIN_SEND_STATE;
		}
		break;

	case LIN_WAIT_STATE:
		if (get_timeout(UART_LIN_TX_TIMEOUT) == 0) {
			linDetectNodeState = LIN_SEND_STATE;
		}
		break;

	default:
		break;
	}

	return ucErrCode;
}


/***********************************************************************
   Function Name: lin_refresh_node

   Description: This function manages the refresh process for LIN
   nodes.

   Summary:
   - This function is called to refresh LIN nodes.
   - It iterates through different states of LIN refresh process.
   - It handles sending and receiving LIN commands, updating node data
   accordingly.
   - Timeout handling is implemented to manage communication failures.

   Parameters: None

   Returns: None
***********************************************************************/
uint8_t lin_refresh_node(uint8_t NodeIndex) {

	uint8_t ucCalcdChksum;
	uint8_t ucErrCode;
	uint8_t refreshDone = 0 ;
	static uint8_t lin_refresh_complete_count = 0;
	static LIN_RefreshNodeTask linRestreshCurrentState = LIN_REFRESH_INIT_STATE;

	switch (linRestreshCurrentState) {

	case LIN_REFRESH_INIT_STATE:
			g_nodeType = ActiveLINNodes[NodeIndex][LNPARAM_NODE_TYPE];
			g_linId = ActiveLINNodes[NodeIndex][LNPARAM_LINID];


			if (g_nodeType != LNT_UNKNOWN) {
				linRestreshCurrentState = LIN_REFRESH_SEND_STATE;
			} else {
				//dont need to proceed for lin transfer
			}
		break;

	case LIN_REFRESH_SEND_STATE:
		linnode_setget(NodeIndex);
		uart_disable_receiver(UART_LIN_MODE);
		linRestreshCurrentState = LIN_REFRESH_SENDING_STATE;
		break;

	case LIN_REFRESH_SENDING_STATE:
		HAL_LIN_SendBreak(&huart1);
		ucErrCode = uart_transmit_data(1,&lin_out[LINCMD_SYNC] , g_linTXLength);
		set_timeout(UART_LIN_TX_TIMEOUT, 50);
		linRestreshCurrentState = LIN_REFRESH_SENDING_WAIT_STATE;
		break;

	case LIN_REFRESH_SENDING_WAIT_STATE:
		if(g_uartTransmitComplete == 1)
		{
			g_uartTransmitComplete = 0;
			uart_enable_receiver(UART_LIN_MODE);
			set_timeout(UART_LIN_RX_TIMEOUT, 50);
			linRestreshCurrentState = LIN_REFRESH_RECEIVE_STATE;
		}else if (get_timeout(UART_LIN_TX_TIMEOUT) == 0) {
			refreshDone =  1;
			lin_refresh_complete_count = 0;
			linRestreshCurrentState = LIN_REFRESH_INIT_STATE; //transmit data timeout, reset state to initial
		}
		break;

	case LIN_REFRESH_RECEIVE_STATE:
		if (g_uartReceiveComplete == 1) {
			g_uartReceiveComplete = 0;
			uart_disable_receiver(UART_LIN_MODE);
			lin_in[LINRESP_ID] = g_linId;

			ucCalcdChksum = lin_checksum(&lin_in[LINRESP_ID], g_linRxLength);

			if (lin_refresh_complete_count == 0) {
				if (lin_in[LINRESP_CHK2DATA] == ucCalcdChksum) {
					lin_save_setgetdata(&lin_in[LINRESP_ID],g_nodeType,NodeIndex);
					linRestreshCurrentState = LIN_REFRESH_RECEIVE_COMPLETE_STATE;
				}
			} else if (lin_refresh_complete_count == 1) {
				if (lin_in[LINRESP_CHK2SENSORS] == ucCalcdChksum) {
					lin_save_angledata(&lin_in[LINRESP_ID],g_nodeType,NodeIndex);
					linRestreshCurrentState = LIN_REFRESH_RECEIVE_COMPLETE_STATE;
				}
			} else if(lin_refresh_complete_count == 2){
				if (lin_in[LINRESP_CHK1ANALOG] == ucCalcdChksum) {
					lin_save_analogdata(&lin_in[LINRESP_ID],g_nodeType,NodeIndex);
					linRestreshCurrentState = LIN_REFRESH_RECEIVE_COMPLETE_STATE;
				}
			}
			linRestreshCurrentState = LIN_REFRESH_RECEIVE_COMPLETE_STATE;
		} else if (get_timeout(UART_LIN_RX_TIMEOUT) == 0) {
			refreshDone =  1;
			ucErrCode = 1; //receive data timeout
			//linRestreshCurrentState = LIN_SENDING_WAIT_STATE; // reset state to initial //kh
			lin_refresh_complete_count = 0;
			linRestreshCurrentState = LIN_REFRESH_INIT_STATE;
			set_timeout(UART_LIN_TX_TIMEOUT, 50);
		}
		break;

	case LIN_REFRESH_RECEIVE_COMPLETE_STATE:
		lin_refresh_complete_count++;
		if (lin_refresh_complete_count == 1) {
			if ((g_linId == LINID_VLPPK) || (g_linId == LINID_GPTILT)) {
				linnode_getangles(NodeIndex);
				linRestreshCurrentState = LIN_REFRESH_SENDING_STATE;
			}
		}

		if (lin_refresh_complete_count == 2) {
			if ((g_linId == LINID_SVRPOS) || (g_linId == LINID_TRLRDET)) {
				linnode_getanalog(NodeIndex);
				linRestreshCurrentState = LIN_REFRESH_SENDING_STATE;
			}
		}

		if (lin_refresh_complete_count == 3) {
			refreshDone =  1;
			lin_refresh_complete_count = 0; // lin transmission complete
			linRestreshCurrentState = LIN_REFRESH_INIT_STATE; //reset state to initial
		}
		break;
	}
	return refreshDone;
}
/********************************************************************
 * Function Name : lin_save_versionnodedata
 *
 * Description
 * Saves the lin node version data received from a LIN node into the
 * ActiveLINNodes array
 *
 * Parameters
 * linbuf   : Pointer to the buffer containing LIN node data.
 * nodeType : The type of LIN node
 *
 * Return Value
 * NONE
 *
 *********************************************************************/
void lin_save_versionnodedata(uint8_t *linbuf , LIN_NodeTypes nodeType, uint8_t * nodeDetectedIndex) {
	//g_numActiveLinNodes++;
	g_numActiveLinNodes = 1;
	*nodeDetectedIndex = g_numActiveLinNodes;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_LINID] = linbuf[LINRESP_ID];
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_NODE_TYPE] = nodeType;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_BL_VER] =
			linbuf[LINRESP_BLVERSION];
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_BL_STATUS] =
			linbuf[LINRESP_BLSTATUS];
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_APP_VER] =
			linbuf[LINRESP_APPVER];

	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_ONLINE_STATUS] = 1;

	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_NODE_STATUS] = 0;//node specific data, filled by SetGet etc
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_INPUTH] = 0;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_INPUTL] = 0;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_OUTPUTH] = 0;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_OUTPUTL] = 0;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_SENSOR1H] = 0;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_SENSOR1L] = 0;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_SENSOR2H] = 0;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_SENSOR2L] = 0;
	ActiveLINNodes[g_numActiveLinNodes][LNPARAM_SPARE] = 0;
}


/********************************************************************
   Function: lin_save_setgetdata

   Description: This function stores LIN node data retrieved after
   sending a LIN SetGet command.

   Parameters:
   - linbuf: Pointer to the LIN buffer containing the received LIN
   response.
   - nodeType: Type of the LIN node.
   - nodeIndex: Index of the LIN node in the ActiveLINNodes array.

   Returns: None

   Notes:
   - This function updates the status and input data of the
   specified LIN node.
   - It is typically called after receiving a response to a
   LIN SetGet command.
********************************************************************/
void lin_save_setgetdata(uint8_t *linbuf, LIN_NodeTypes nodeType,
		uint8_t nodeIndex) {

	ActiveLINNodes[nodeIndex][LNPARAM_NODE_STATUS] = linbuf[LINRESP_STATUS];
	ActiveLINNodes[nodeIndex][LNPARAM_INPUTL] = linbuf[LINRESP_DATA];

}


/********************************************************************
   Function: lin_save_angledata

   Description: This function stores LIN node data retrieved after
   sending a LIN GetAngle command.

   Parameters:
   - linbuf: Pointer to the LIN buffer containing the received LIN
   response.
   - nodeType: Type of the LIN node.
   - nodeIndex: Index of the LIN node in the ActiveLINNodes array.

   Returns: None

   Notes:
   - This function updates the angle sensor data of the
   specified LIN node.
   - It is typically called after receiving a response to a
   LIN GetAngle command.

********************************************************************/
void lin_save_angledata(uint8_t *linbuf, LIN_NodeTypes nodeType,
		uint8_t nodeIndex) {

	ActiveLINNodes[nodeIndex][LNPARAM_SENSOR1H] = linbuf[LINRESP_SENSOR1H];
	ActiveLINNodes[nodeIndex][LNPARAM_SENSOR1L] = linbuf[LINRESP_SENSOR1L];
	ActiveLINNodes[nodeIndex][LNPARAM_SENSOR2H] = linbuf[LINRESP_SENSOR2H];
	ActiveLINNodes[nodeIndex][LNPARAM_SENSOR2L] = linbuf[LINRESP_SENSOR2L];

}


/********************************************************************
   Function: lin_save_analogdata

   Description: This function stores LIN node data retrieved after
   sending a LIN GetAnalog command.

   Parameters:
   - linbuf: Pointer to the LIN buffer containing the received LIN
   response.
   - nodeType: Type of the LIN node.
   - nodeIndex: Index of the LIN node in the ActiveLINNodes array.

   Returns: None

   Notes:
   - This function updates the analog sensor data of the
   specified LIN node.
   - It is typically called after receiving a response to a
   LIN GetAnalog command.

********************************************************************/

void lin_save_analogdata(uint8_t *linbuf, LIN_NodeTypes nodeType,
		uint8_t nodeIndex) {

	ActiveLINNodes[nodeIndex][LNPARAM_ANALOG1H] = lin_in[LINRESP_ANALOG1H];
	ActiveLINNodes[nodeIndex][LNPARAM_ANALOG1L] = lin_in[LINRESP_ANALOG1L];

}


/********************************************************************
   Function: linnode_setget

   Description: This function prepares a LIN message for sending a
   SetGet command to a LIN node.

   Parameters:
   - NodeIndex: Index of the LIN node in the ActiveLINNodes array.

   Returns: None

   Notes:

********************************************************************/

void linnode_setget(uint8_t NodeIndex) {

	uint8_t ucCalcdChksum;
	uint8_t ucLINID;
	uint8_t ucDataOut;

	ucLINID = ActiveLINNodes[NodeIndex][LNPARAM_LINID];
	ucDataOut = ActiveLINNodes[NodeIndex][LNPARAM_OUTPUTL];

	g_linRxLength = 3;
	g_linTXLength = 5;
	lin_out[LINCMD_SYNC] = LIN_SYNC;
	lin_out[LINCMD_ID] = ucLINID;
	lin_out[LINCMD_COMMAND] = LINCOMMAND_SETGET1;
	lin_out[LINCMD_PARAM1] = ucDataOut;
	ucCalcdChksum = lin_checksum(&lin_out[LINCMD_ID], 3); //checksum of data including ID
	lin_out[LINCMD_CHK_1PARAMS] = ucCalcdChksum;
	lin_in[LINRESP_ID] = ucLINID;
	HAL_UART_Receive_IT(&huart1, &lin_in[1], g_linRxLength);
}


/********************************************************************
   Function: linnode_setget

   Description: This function prepares a LIN message for sending a
   getangles(TILT sensor angles) command to a LIN node.

   Parameters:
   - NodeIndex: Index of the LIN node in the ActiveLINNodes array.

   Returns: None

   Notes:

********************************************************************/
void linnode_getangles(uint8_t NodeIndex) {

	uint8_t ucCalcdChksum;
	uint8_t ucLINID;

	ucLINID = ActiveLINNodes[NodeIndex][LNPARAM_LINID];


	ucLINID = LINID_VLPPK;

	g_linRxLength = 5;
	g_linTXLength = 4;
	lin_out[LINCMD_SYNC] = LIN_SYNC;
	lin_out[LINCMD_ID] = ucLINID;
	lin_out[LINCMD_COMMAND] = LINCOMMAND_GETANGLES;
	ucCalcdChksum = lin_checksum(&lin_out[LINCMD_ID], 3); //checksum of data including ID
	lin_out[LINCMD_CHK_NOPARAMS] = ucCalcdChksum;
	lin_in[LINRESP_ID] = ucLINID;
	HAL_UART_Receive_IT(&huart1, &lin_in[1], g_linRxLength);
}


/********************************************************************
   Function: linnode_setget

   Description: This function prepares a LIN message for sending a
   getanalog(position of POS) command to a LIN node.

   Parameters:
   - NodeIndex: Index of the LIN node in the ActiveLINNodes array.

   Returns: None

   Notes:

********************************************************************/
void linnode_getanalog(uint8_t NodeIndex) {

	uint8_t ucCalcdChksum;
	uint8_t ucLINID;

	ucLINID = ActiveLINNodes[NodeIndex][LNPARAM_LINID];
	ucLINID = LINID_VLPPK;


	g_linRxLength = 3;
	g_linTXLength = 4;
	lin_out[LINCMD_SYNC] = LIN_SYNC;
	lin_out[LINCMD_ID] = ucLINID;
	lin_out[LINCMD_COMMAND] = LINCOMMAND_GETANALOG;
	ucCalcdChksum = lin_checksum(&lin_out[LINCMD_ID], 3); //checksum of data including ID
	lin_out[LINCMD_CHK_NOPARAMS] = ucCalcdChksum;
	lin_in[LINRESP_ID] = ucLINID;
	HAL_UART_Receive_IT(&huart1, &lin_in[1], g_linRxLength);
}


/********************************************************************
   Function: linnode_setget

   Description: This function prepares a LIN message for sending a
   get version command to a LIN node.

   Parameters:
   - NodeIndex: Index of the LIN node in the ActiveLINNodes array.

   Returns: None

   Notes:

********************************************************************/
void linmode_getversion(uint8_t linId) {
	uint8_t ucCalcdChksum;

	g_linRxLength = 4;
	g_linTXLength = 4;
	lin_out[LINCMD_SYNC] = LIN_SYNC;
	lin_out[LINCMD_ID] = linId;
	lin_out[LINCMD_COMMAND] = LINCOMMAND_GETVERSION;
	ucCalcdChksum = lin_checksum(&lin_out[LINCMD_ID], 2); //checksum of data including ID
	lin_out[LINCMD_CHK_NOPARAMS] = ucCalcdChksum;
	lin_in[LINRESP_ID] = linId;
	HAL_UART_Receive_IT(&huart1, &lin_in[1], g_linRxLength);
}


/********************************************************************
 * Function Name : lin_checksum
 *
 * Description
 * Calculates the LIN checksum for a given data array.
 *
 * Parameters
 * array    : Pointer to the array containing data for checksum
 * calculation.
 * length   : Length of the array.
 *
 * Return Value
 * Calculated LIN checksum.
 *
 *********************************************************************/
uint8_t lin_checksum(uint8_t *array, uint8_t length) {
	uint8_t ucLoop;
	unsigned int uiCheckSum = 0;

	for (ucLoop = 0; ucLoop < length; ucLoop++) {
		uiCheckSum += array[ucLoop];
		if (uiCheckSum > 0xFF) {
			uiCheckSum = uiCheckSum - 0xFF;
		}
	}

	uiCheckSum = ~uiCheckSum;
	uiCheckSum = (uiCheckSum & 0xFF);

	return (uint8_t) uiCheckSum;
}
