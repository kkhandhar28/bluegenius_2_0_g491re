/*
 * sys_config.c
 *
 *  Created on: Aug 8, 2024
 *      Author: kkhandhar
 */
#include "main.h"
#include "app.h"
#include "sys_config.h"


void sys_AutoConfiguration()
{
	uint8_t doorSensorsDetected;

	//detect door sensors
	doorSensorsDetected = sys_detectDoorSensors();
	//write in EEPROM that how many door sensors is detected

	//detect restraint in the system
	sys_detectRestraintType();

	//detect seal is present or not
	sys_detectShelter();

	//detect deck in the system
	sys_detectDeckType();
}


uint8_t sys_detectDoorSensors(void)
{
	uint8_t ssDoorSensors;

	return ssDoorSensors;
}


void sys_detectRestraintType(void)
{

}


void sys_detectShelter(void)
{

}


void sys_detectDeckType(void)
{

}
