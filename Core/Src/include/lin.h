/*
 * lin.h
 *
 *  Created on: Aug 8, 2024
 *      Author: kkhandhar
 */

#ifndef SRC_INCLUDE_LIN_H_
#define SRC_INCLUDE_LIN_H_

typedef enum{
	LIN_IDLE_STATE = 0,
	LIN_SEND_STATE ,
	LIN_SENDING_STATE,
	LIN_SENDING_WAIT_STATE,
	LIN_RECEIVE_STATE ,
	LIN_RECEIVE_COMPLETE_STATE,
	LIN_WAIT_STATE,
}LIN_DetectNodeTask;

typedef enum
{
	LNT_UNKNOWN			= 0,
	LNT_POWER			= 1,
	LNT_RESTRAINT 		= 2,
	LNT_LEVELER 		= 3,
	LNT_RESTRAINT_PPK	= 4,
	LNT_LEVELER_PPK		= 5,
	LNT_COMBO_PPK		= 6,
	LNT_TILTSENSOR		= 7,
	LNT_DISTANCESENSOR	= 8,
	LNT_INTTRAFFICLIGHT	= 9,
	LNT_EXTTRAFFICLIGHT	= 10
}LIN_NodeTypes;

typedef enum
{
	LIN_REFRESH_INIT_STATE,
	LIN_REFRESH_SEND_STATE,
	LIN_REFRESH_SENDING_STATE,
	LIN_REFRESH_SENDING_WAIT_STATE,
	LIN_REFRESH_RECEIVE_STATE,
	LIN_REFRESH_RECEIVE_COMPLETE_STATE,

}LIN_RefreshNodeTask;


extern uint8_t lin_in[15];
extern uint8_t lin_out[15];

#define MAX_ACTIVE_LINNODES 16

#define PCU_NODE_INDEX  1

//-----LIN PARAMETER CONSTANTS--------------
#define LNPARAM_LINID 			0
#define LNPARAM_NODE_TYPE		1
#define LNPARAM_BL_VER			2
#define LNPARAM_BL_STATUS		3
#define LNPARAM_APP_VER			4
#define LNPARAM_ONLINE_STATUS	5
#define LNPARAM_NODE_STATUS		6
#define LNPARAM_INPUTH			7
#define LNPARAM_INPUTL			8
#define LNPARAM_OUTPUTH			9
#define LNPARAM_OUTPUTL			10
#define LNPARAM_SENSOR1H		11
#define LNPARAM_SENSOR1L		12
#define LNPARAM_SENSOR2H		13
#define LNPARAM_SENSOR2L		14
#define LNPARAM_ANALOG1H		15
#define LNPARAM_ANALOG1L		16
#define LNPARAM_SPARE			17

#define LIN_ERROR_CHECKSUM			0x80
#define LIN_ERROR_MAXACTIVENODES	0x81
#define LIN_ERROR_NODENOTFOUND		0x82
#define LIN_ERROR_FIRSTCHAR			0x40

//-----COMMAND IDs--------------------------
#define LINCOMMAND_GETVERSION		'v'
#define LINCOMMAND_ENTERBOOTLOADER	'b'
#define LINCOMMAND_SETCONFIG		'c'
#define LINCOMMAND_SETGET1			'1'
#define LINCOMMAND_SETGET2			'2'
#define LINCOMMAND_GETTILTANGLE		't'
#define LINCOMMAND_GETANGLES		'a'
#define LINCOMMAND_GETANALOG		'A'

//-----COMMANDS-----------------------------
#define LINCMD_SYNC		0
#define LINCMD_ID		1
#define LINCMD_COMMAND	2
#define LINCMD_PARAM1	3
#define LINCMD_PARAM2	4
#define LINCMD_CHK_2PARAMS	5
#define LINCMD_CHK_1PARAMS	4
#define LINCMD_CHK_NOPARAMS	3

//-----RESPONSES----------------------------
#define LINRESP_ID			0
#define LINRESP_BLVERSION	1		//Get Version
#define LINRESP_BLSTATUS	2
#define LINRESP_APPVER		3
#define LINRESP_CHK3DATA	4

#define LINRESP_K			1		//Enter BL
#define LINRESP_CHK1DATA	2

#define LINRESP_K			1		//Set Config

#define LINRESP_STATUS		1		//Set/Get 1 Byte
#define LINRESP_DATA		2
#define LINRESP_CHK2DATA	3

#define LINRESP_DATAH		2
#define LINRESP_DATAL		3

#define LINRESP_SENSOR1H	1		//Get Sensor data, 2 uint8_ts
#define LINRESP_SENSOR1L	2
#define LINRESP_CHK1SENSOR	3

#define LINRESP_SENSOR2H	3
#define LINRESP_SENSOR2L	4
#define LINRESP_CHK2SENSORS	5

//#define LINRESP_ID		0
#define LINRESP_ANALOG1H	1		//Get Analog data, 2 uint8_ts
#define LINRESP_ANALOG1L	2
#define LINRESP_CHK1ANALOG	3


//------------------------------------------

#define LIN_IGNORE   0
#define LIN_SEND     1
#define LIN_RECEIVE  2

#define LIN_SYNC	0x55


extern uint8_t g_numActiveLinNodes;
extern uint8_t g_getVersionData;
extern uint8_t g_refreshDone;
extern volatile uint8_t g_linTXLength;
extern volatile uint8_t g_linRxLength;


uint8_t lin_get_version(uint8_t linId , LIN_NodeTypes nodeType, uint8_t * nodeDetectedIndex);
uint8_t lin_refresh_node(uint8_t NodeIndex);
void lin_save_versionnodedata(uint8_t *linbuf , LIN_NodeTypes nodeType, uint8_t * nodeDetectedIndex);
void lin_save_setgetdata(uint8_t *linbuf, LIN_NodeTypes nodeType,
		uint8_t nodeIndex);
void lin_save_angledata(uint8_t *linbuf, LIN_NodeTypes nodeType,
		uint8_t nodeIndex);
void lin_save_analogdata(uint8_t *linbuf, LIN_NodeTypes nodeType,
		uint8_t nodeIndex);
void linnode_setget(uint8_t NodeIndex);
void linnode_getangles(uint8_t NodeIndex);
void linnode_getanalog(uint8_t NodeIndex);
void linmode_getversion(uint8_t linId);
uint8_t lin_checksum(uint8_t *array, uint8_t length);
uint8_t linnode_getdata(uint8_t NodeIndex, uint8_t Param);


#endif /* SRC_INCLUDE_LIN_H_ */
