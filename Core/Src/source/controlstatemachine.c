/*
 * controlstatemachine.c
 *
 *  Created on: Aug 8, 2024
 *      Author: kkhandhar
 */

/*******************************************************************************
 * Included headers
 *******************************************************************************/
#include "main.h"
#include "controlstatemachine.h"
#include "app.h"

/********************************************************************
 * Global Variables
 ********************************************************************/
SYSYEM_CONTROL_STATE_TYPE sysCurrentControlState = SS_STARTING_STATE_CHECK;
SYSYEM_CONTROL_STATE_TYPE sysNextControlState;

void system_fsm() {

	switch (sysCurrentControlState) {
	case SS_STARTING_STATE_CHECK:
		 //Initial state where the system reads sensors on modules and decides what the next system state will be.
		break;

	case SS_RESTR_FAULT_CHECK:
		 //The system checks for any faults or errors in the restraint and sends a front panel display event accordingly.
		break;

	case SS_RESTR_HOMED:
		// Indicates that after the initial state, the restraint has been detected at the
		// home position and is ready for the next operation.
		// Waits for a button detection event from the back panel, and when a restraint engage
		// or override button event is detected, moves to the next state accordingly.
		break;

	case SS_RESTR_ENGAGING:
		// When the restraint engage button event is detected from the front panel,
		// the restraint engagement process begins by turning on the restraint motor.
		// After a specific timeout, it reads the restraint state and moves to the next system state accordingly.
		break;

	case SS_RESTR_ICCBARMISSED:
		// After the restraint engagement procedure, if the restraint engagement fails due to the ICC bar being missed,
		// the system enters this state and moves to the next state according to the subsequent procedure.
		break;

	case SS_RESTR_ENGAGED:
		// When the restraint is successfully engaged, the system enters this state.
	    // It will then move to the next system state according to the system configuration.
		break;

	case SS_RESTR_HOMING:
		 // The system will start releasing the restraint by turning on the motor in this state.
		 // After a specific timeout, it will move to the next system state based on the restraint state.
		break;

	case SS_RESTR_OVERRIDE:
		// System will enter in this state when restraint override event detected from front panel and
		// start restraint overriding procedure
		break;

	case SS_SEAL_INFLATING:
		// If seal is configured in the system then according to system's configuration system initiate seal inflating
		// process and upon successful seal inflation move to next system state according to system configuration.
		break;

	case SS_SEAL_DEFLATING:
		// system enters in this state to deflate seal and upon successful deflation it will move to next state according to system state.
		break;

	case SS_DOOR_MOVING:
		// When system receive door open or close event from front panel then system will enter this state
		// and start moving the door accordingly.
		break;

	case SS_DOOR_OPEN:
		// When door sensor detects the door is open system will enter in this state and move to next state according to system configuration.
		break;

	case SS_DOOR_CLOSE:
		// When door sensor detects the door is close system will enter in this state and move to next state according to system configuration
		break;

	case SS_DECK_HOME:
		// When deck is present in the system and detected at home then system will enter in this state.
		// and wait for front panel button event and move to next state accordingly.
		break;

	case SS_DECK_MOVING:
		// when system get event from back panel to move the deck system will enter in this state and wait for specific time when deck is moving.
		// and when deck stop moving according to deck state it will go to next system state
		break;

	case SS_DECK_RAISING:
		// when system enters in this state deck motor is on and deck is raising and after specific timeout move according
		// to deck state move to next system state.
		break;

	case SS_DECK_SINKING:
		break;

	case SS_DECK_LOWERING:
		break;

	case SS_DECK_OVERRIDE:
		// when Deck override event detected srom front panel system will enter in this state and override the deck.
		break;

	case SS_LIP_MOVING:
		// when lip button event detected from front panel system move in this control state and lip started moving
		break;

	case SS_DECK_DEPLOYED:
		// When deck is deployed system will enter in this state and wait in this state until any front panel button event detected.
		break;
	}
}
