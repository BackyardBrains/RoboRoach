/**************************************************************************************************
  Filename:       roboroach_GATTprofile.h
  Revised:        $Date: 2010-08-06 08:56:11 -0700 (Fri, 06 Aug 2010) $
  Revision:       $Revision: 23333 $

  Description:    This file contains the RoboRoach GATT profile definitions and
                  prototypes.

  Copyright 2010 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

#ifndef ROBOROACHGATTPROFILE_H
#define ROBOROACHGATTPROFILE_H

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

// Profile Parameterss
#define ROBOROACH_FREQUENCY               1  // RW uint8 
#define ROBOROACH_PULSE_WIDTH             2  // RW uint8 
#define ROBOROACH_NUM_PULSES              3  // RW uint8
#define ROBOROACH_RANDOM_MODE             4  // RW uint8 
#define ROBOROACH_STIMUATE_LEFT           5  // W uint8 
#define ROBOROACH_STIMUATE_RIGHT          6  // W uint8 
  
// RoboRoach Service UUID
#define ROBOROACH_SERV_UUID                  0xB2B0
    
// Key Pressed UUID
#define ROBOROACH_CHAR_FREQUENCY_UUID        0xB2B1
#define ROBOROACH_CHAR_PULSE_WIDTH_UUID      0xB2B2
#define ROBOROACH_CHAR_NUM_PULSES_UUID       0xB2B3
#define ROBOROACH_CHAR_RANDOM_MODE_UUID      0xB2B4
#define ROBOROACH_CHAR_STIMULATE_LEFT_UUID   0xB2B5
#define ROBOROACH_CHAR_STIMULATE_RIGHT_UUID  0xB2B6
  
  
#define ROBOROACH_PIO_LED_LEFT                   11
#define ROBOROACH_PIO_LED_RIGHT                  12
#define ROBOROACH_PIO_LED_CONNECTION             13
  
  
// RoboRoach Services bit fields
#define ROBOROACH_SERVICE               0x000000001

  
#define TURN_OFF_STIM_EVT                    0x0002
  
/*********************************************************************
 * TYPEDEFS
 */

  
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef NULL_OK void (*roboRoachChange_t)( uint8 paramID );

typedef struct
{
  roboRoachChange_t        pfnRoboRoachChange;  // Called when characteristic value changes
} roboRoachCBs_t;

    

/*********************************************************************
 * API FUNCTIONS 
 */


/*
 * RoboRoach_AddService- Initializes the RoboRoach GATT Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 */

extern bStatus_t RoboRoach_AddService( uint32 services );

/*
 * RoboRoach_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t RoboRoach_RegisterAppCBs( roboRoachCBs_t *appCallbacks );

/*
 * RoboRoach_SetParameter - Set a RoboRoach GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t RoboRoach_SetParameter( uint8 param, uint8 len, void *value );
  
/*
 * RoboRoach_GetParameter - Get a RoboRoach GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t RoboRoach_GetParameter( uint8 param, void *value );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEGATTPROFILE_H */
