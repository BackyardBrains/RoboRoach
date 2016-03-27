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
