/*
 * linrefresh.c
 *
 *  Created on: Aug 8, 2024
 *      Author: kkhandhar
 */

/*******************************************************************************
 * Included headers
 *******************************************************************************/
#include "main.h"
#include "lin.h"

uint8_t lin_pollSliceNum = 1;

/********************************************************************
 * Function Name: lin_refresh
 *
 * Description:
 * This function refreshes the LIN module at specific time intervals.
 *
 * Parameters: None
 *
 * Return Value: None
 *
 *********************************************************************/

void lin_refresh(void)
{
	uint8_t errCode = 0;

	errCode = lin_refresh_node(lin_pollSliceNum);
	if(errCode == 0 && g_refreshDone == 1)
	{

	}

	if(g_refreshDone == 1)
	{
		g_refreshDone = 0;
		if(++lin_pollSliceNum > g_numActiveLinNodes)
		{
			lin_pollSliceNum =  1;
		}
	}
}


