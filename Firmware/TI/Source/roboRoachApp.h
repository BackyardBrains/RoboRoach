/**************************************************************************************************
  Filename:       roboRoachApp.h 
 
  Description:    This file contains the RoboRoach application
                  definitions and prototypes.

  Copyright 2013 Backyard Brains Incorporated. All rights reserved.

**************************************************************************************************/

#ifndef ROBOROACHPERIPHERAL_H
#define ROBOROACHPERIPHERAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the BLE Application
 */
extern void RoboRoachPeripheral_Init( uint8 task_id );

/*
 * Task Event Processor for the BLE Application
 */
extern uint16 RoboRoachPeripheral_ProcessEvent( uint8 task_id, uint16 events );

extern void startRoboRoachStimulation();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEBLEPERIPHERAL_H */
