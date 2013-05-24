/**************************************************************************************************
  Filename:       roboroach_GATTprofile.h
  
  Description:    This file contains the RoboRoach GATT profile definitions and
                  prototypes.

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

  //RoboRoach 1.0a.  Right LED = P0 0x40;
  //RoboRoach 1.0a.  Left LED = P0 0x80;
  //RoboRoach 1.0b.  Right LED = P1 0x01;
  //RoboRoach 1.0b.  Left LED = P1 0x02;
   
   
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
  

//#define HAL_LED_1     0x01
//#define HAL_LED_2     0x02
//#define HAL_LED_3     0x04
//#define HAL_LED_4     0x08
//#define HAL_LED_ALL   (HAL_LED_1 | HAL_LED_2 | HAL_LED_3 | HAL_LED_4)


#define ROBOROACH_PIO_ANTENNA_LEFT               0x01 | 0x04
#define ROBOROACH_PIO_ANTENNA_RIGHT              0x02 | 0x08
   
#define ROBOROACH_PIO_LED_LEFT                   11
#define ROBOROACH_PIO_LED_RIGHT                  12
#define ROBOROACH_PIO_LED_CONNECTION             13
  
  
// RoboRoach Services bit fields
#define ROBOROACH_SERVICE               0x000000001

  
/*********************************************************************
 * TYPEDEFS
 */

// Callback when a characteristic value has changed
typedef NULL_OK void (*roboRoachProfileChange_t)( uint8 paramID );

typedef struct
{
  roboRoachProfileChange_t        pfnRoboRoachProfileChange;  // Called when characteristic value changes
} roboRoachProfileCBs_t;

  
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

/*********************************************************************
 * API FUNCTIONS 
 */


/*
 * RoboRoach_AddService- Initializes the RoboRoach GATT Profile service by registering
 *          GATT attributes with the GATT server.
 *
 */

extern bStatus_t RoboRoachProfile_AddService( uint8 taskID );

/*
 * RoboRoach_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t RoboRoachProfile_RegisterAppCBs( roboRoachProfileCBs_t *appCallbacks );

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
extern bStatus_t RoboRoachProfile_SetParameter( uint8 param, uint8 len, void *value );
  
/*
 * RoboRoach_GetParameter - Get a RoboRoach GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t RoboRoachProfile_GetParameter( uint8 param, void *value );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /*  ROBOROACHGATTPROFILE_H */
