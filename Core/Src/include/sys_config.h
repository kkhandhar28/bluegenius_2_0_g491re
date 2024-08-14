/*
 * sys_config.h
 *
 *  Created on: Aug 8, 2024
 *      Author: kkhandhar
 */

#ifndef SRC_SYS_CONFIG_H_
#define SRC_SYS_CONFIG_H_

/********************************************************************
*   Function Prototypes
*********************************************************************/
void sys_AutoConfiguration();
uint8_t sys_detectDoorSensors(void);
void sys_detectRestraintType(void);
void sys_detectShelter(void);
void sys_detectDeckType(void);
#endif /* SRC_SYS_CONFIG_H_ */
