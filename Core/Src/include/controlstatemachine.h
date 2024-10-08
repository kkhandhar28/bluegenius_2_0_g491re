/*
 * controlstatemachine.h
 *
 *  Created on: Aug 8, 2024
 *      Author: kkhandhar
 */

#ifndef SRC_INCLUDE_CONTROLSTATEMACHINE_H_
#define SRC_INCLUDE_CONTROLSTATEMACHINE_H_

/*******************************************************************************
*   System States
*******************************************************************************/
typedef enum{
	SS_STARTING_STATE_CHECK = 0,
	SS_RESTR_FAULT_CHECK,
	SS_RESTR_HOMED,
	SS_RESTR_ENGAGING,
	SS_RESTR_ICCBARMISSED,
	SS_RESTR_ENGAGED,
	SS_RESTR_HOMING,
	SS_RESTR_OVERRIDE,
	SS_SEAL_INFLATING,
	SS_SEAL_DEFLATING,
	SS_DOOR_MOVING,
	SS_DOOR_OPEN,
	SS_DOOR_CLOSE,
	SS_DECK_HOME,
	SS_DECK_MOVING,
	SS_DECK_RAISING,
	SS_DECK_SINKING,
	SS_DECK_LOWERING,
	SS_DECK_OVERRIDE,
	SS_LIP_MOVING,
	SS_DECK_DEPLOYED,
}SYSYEM_CONTROL_STATE_TYPE;

/*******************************************************************************
*   Function Prototypes
*******************************************************************************/
void system_fsm();

#endif /* SRC_INCLUDE_CONTROLSTATEMACHINE_H_ */
