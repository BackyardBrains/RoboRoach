/**************************************************************************************************
  Filename:       roboRoachProfile.h

  Description:    This file contains the RoboRoach profile definitions and
                  prototypes. 

  Copyright 2013 Backyard Brains Incorporated. Some rights reserved (just the good ones).

**************************************************************************************************/

#ifndef ROBOROACHPROFILE_H
#define ROBOROACHPROFILE_H

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
 * RoboRoach_AddService- Initializes the Simple GATT Profile service by registering
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
extern bStatus_t RoboRoach_RegisterAppCBs( RoboRoachCBs_t *appCallbacks );

/*
 * RoboRoach_SetParameter - Set a Simple GATT Profile parameter.
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
 * RoboRoach_GetParameter - Get a Simple GATT Profile parameter.
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
