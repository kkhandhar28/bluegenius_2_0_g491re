/*
 * lincfg.c
 *
 *  Created on: May 1, 2024
 *      Author: kkhandhar
 */

#ifndef SRC_LINCFG_C_
#define SRC_LINCFG_C_

/*******************************************************************************
 * Included headers
 *******************************************************************************/
#include "main.h"
#include "app.h"
#include "lin.h"
#include "lincfg.h"
#include "linid.h"

/**Array defining the LIN module IDs and their corresponding node types**/
uint8_t lin_module[MAX_LIN_MODULE_ID][MAX_LIN_MODULE_TYPE] = {
		{ LINID_VLPPK, LNT_LEVELER_PPK },
		{ LINID_PWRCTRL, LNT_POWER },
		{ LINID_TL85RESTR, LNT_RESTRAINT },
		{ LINID_TL85PPK, LNT_RESTRAINT_PPK },
		{ LINID_TL85SC, LNT_SC_RESTRAINT },
		{ LINID_CHOCK, LNT_CHOCK_RESTRAINT },
		{ LINID_FHPPK, LNT_LEVELER_PPK },
		{ LINID_SVRPPK, LNT_RESTRAINT_PPK },
		{ LINID_RVRRESTR, LNT_RESTRAINT },
		{ LINID_RVRPPK, LNT_RESTRAINT_PPK },
		{ LINID_XDSPPK, LNT_LEVELER_PPK },
		{ LINID_SVRXDSPPK, LNT_COMBO_PPK },
		{ LINID_GPTILT, LNT_TILTSENSOR },
		{ LINID_HVRRESTR, LNT_RESTRAINT },
		{ LINID_MVRRESTR, LNT_RESTRAINT },
		{ LINID_MVRTILT, LNT_TILTSENSOR },
		{ LINID_MVRPPK, LNT_RESTRAINT_PPK },
		{ LINID_SVRPOS, LNT_RESTRAINT },
		{ LINID_TRLRDET,LNT_DISTANCESENSOR } };


/***********************************************************************
* Function Name: lin_module_scan
*
*  Description: Scans the connected LIN modules and retrieves their
*  version information.
*
*  Parameters: None
*
*  Return : 1 : when the scan of all modules is complete,
*  			0 : otherwise
***********************************************************************/
uint8_t lin_module_scan(void) {
	uint8_t static lin_id;
	uint8_t static lin_node_type;
	uint8_t static linIdIndex = 0;
	static uint8_t mode;
	uint8_t linModuleScanComplete = 0;

	switch (mode) {
	case 0:
		lin_id = lin_module[linIdIndex][0];
		lin_node_type = lin_module[linIdIndex][1];
		mode = 1;
		break;

	case 1:
		if (g_getVersionData == 0) {
			lin_get_version(lin_id, lin_node_type);
		}
		else if (g_getVersionData == 1) {
			//HAL_Delay(1);
			g_getVersionData = 0;
			mode = 2;
		}
		break;

	case 2:
			linIdIndex++;
			if(linIdIndex == MAX_LIN_MODULE_ID){
				linModuleScanComplete = 1;
			}
			linIdIndex = linIdIndex % MAX_LIN_MODULE_ID;
			mode = 0;
		break;
	}

	return linModuleScanComplete;
}

void lin_node_configure_test(void) {
	static uint8_t mode = 0;
	uint8_t nodeStatus;

	switch (mode) {
	case 0:
		if (g_getVersionData == 0) {
			nodeStatus = lin_get_version(LINID_VLPPK, LNT_LEVELER_PPK);
		}

		else if (g_getVersionData == 1) {
			mode = 1;
		}
		break;

	case 1:
		if (lin_refresh_node(1)) {
		}
		break;
	}
}
#endif /* SRC_LINCFG_C_ */
