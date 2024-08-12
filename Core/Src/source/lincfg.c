/*
 * lincfg.c
 *
 *  Created on: May 1, 2024
 *      Author: kkhandhar
 */

#ifndef SRC_LINCFG_C_
#define SRC_LINCFG_C_

#include "main.h"
#include "app.h"
#include "lin.h"
#include "lincfg.h"
#include "linid.h"

uint8_t g_restraintModuleIndex;
uint8_t g_restraintPowerPackModuleIndex;


void lin_node_configure_test(void) {
	static uint8_t mode = 0;
	uint8_t nodeStatus;
	static uint8_t nodeDetectedIndex;

	switch (mode) {
	case 0:
		if (g_getVersionData == 0) {
			nodeStatus = lin_get_version(LINID_VLPPK, LNT_LEVELER_PPK, &nodeDetectedIndex);
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
