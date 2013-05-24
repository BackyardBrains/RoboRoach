/**************************************************************************************************
  Filename:       roboRoachProfile.c

  Description:    This file contains the RoboRoach profile GATT service 
                  profile for use with the Backyard Brains RoboRoach.

  Copyright 2013 Backyard Brains Incorporated. All rights reserved.
 
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "roboRoachProfile.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        17

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

uint16 stimulationPeriodInMilliseconds = 100;
uint8 stimulationDutyCycle = 0;
long offsetTime = 1000;

// RoboRoach GATT Profile Service UUID: 0xB2B0
CONST uint8 roboRoachServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_SERV_UUID), HI_UINT16(ROBOROACH_SERV_UUID)
};

// Stimulation Frequency Characteristic UUID: 0xB2B1
CONST uint8 rrCharFrequencyUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_FREQUENCY_UUID), HI_UINT16(ROBOROACH_CHAR_FREQUENCY_UUID)
};

// Stimulation Pulse Width Characteristic UUID: 0xB2B2
CONST uint8 rrCharPulseWidthUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_PULSE_WIDTH_UUID), HI_UINT16(ROBOROACH_CHAR_PULSE_WIDTH_UUID)
};

// Number Of Stimulation Pulses Characteristic UUID: 0xB2B3
CONST uint8 rrCharNumberOfPulsesUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_NUM_PULSES_UUID), HI_UINT16(ROBOROACH_CHAR_NUM_PULSES_UUID)
};

// Random Mode Characteristic UUID: 0xB2B4
CONST uint8 rrCharRandomModeUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_RANDOM_MODE_UUID), HI_UINT16(ROBOROACH_CHAR_RANDOM_MODE_UUID)
};

// Set Stimulation Left Begin Characteristic UUID: 0xB2B5
CONST uint8 rrCharStimulateLeftUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_STIMULATE_LEFT_UUID), HI_UINT16(ROBOROACH_CHAR_STIMULATE_LEFT_UUID)
};

// Set Stimulation Right Begin Characteristic UUID: 0xB2B6
CONST uint8 rrCharStimulateRightUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_STIMULATE_RIGHT_UUID), HI_UINT16(ROBOROACH_CHAR_STIMULATE_RIGHT_UUID)
};


/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static RoboRoachCBs_t *RoboRoach_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */


// RoboRoach Service attribute
static CONST gattAttrType_t roboRoachService = { ATT_BT_UUID_SIZE, roboRoachServUUID };

// Stimulation Frequency Characteristic Properties
static uint8 rrCharFrequencyProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 rrCharFrequency = 55; //Default: 55Hz
static uint8 rrCharFrequencyUserDesp[22] = "Stimulation Frequency\0";

// Pulse Width Characteristic Properties
static uint8 rrCharPulseWidthProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 rrCharPulseWidth = 5; //Default: 5ms
static uint8 rrCharPulseWidthUserDesp[29] = "Stimulation Pulse Width (ms)\0";

// Number of Pulses Characteristic Properties
static uint8 rrCharNumPulsesProps =  GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 rrCharNumPulses = 10; //Default: 10 Pulses
static uint8 rrCharNumPulsesUserDesp[17] = "Number of Pulses\0";

// Random Mode Characteristic Properties
static uint8 rrCharRandomModeProps =  GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 rrCharRandomMode = 0; //Default: Off
static uint8 rrCharRandomModeUserDesp[24] = "Random Mode (enabled=1)\0";

// Stimulate Left Characteristic Properties
static uint8 rrCharStimulateLeftProps = GATT_PROP_WRITE; //GATT_PROP_NOTIFY;
static uint8 rrCharStimulateLeft = 0;
static uint8 rrCharStimulateLeftUserDesp[15] = "Stimulate Left\0";

// Stimulate Right Characteristic Properties
static uint8 rrCharStimulateRightProps = GATT_PROP_WRITE; //GATT_PROP_NOTIFY;
static uint8 rrCharStimulateRight = 0;
static uint8 rrCharStimulateRightUserDesp[16] = "Stimulate Right\0";




/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t RoboRoachAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&roboRoachService            /* pValue */
  },

    // Characteristic 1 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &RoboRoachChar1Props 
    },

      // Characteristic Value 1
      { 
        { ATT_BT_UUID_SIZE, RoboRoachchar1UUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        &RoboRoachChar1 
      },

      // Characteristic 1 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        RoboRoachChar1UserDesp 
      },      

    // Characteristic 2 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &RoboRoachChar2Props 
    },

      // Characteristic Value 2
      { 
        { ATT_BT_UUID_SIZE, RoboRoachchar2UUID },
        GATT_PERMIT_READ, 
        0, 
        &RoboRoachChar2 
      },

      // Characteristic 2 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        RoboRoachChar2UserDesp 
      },           
      
    // Characteristic 3 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &RoboRoachChar3Props 
    },

      // Characteristic Value 3
      { 
        { ATT_BT_UUID_SIZE, RoboRoachchar3UUID },
        GATT_PERMIT_WRITE, 
        0, 
        &RoboRoachChar3 
      },

      // Characteristic 3 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        RoboRoachChar3UserDesp 
      },

    // Characteristic 4 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &RoboRoachChar4Props 
    },

      // Characteristic Value 4
      { 
        { ATT_BT_UUID_SIZE, RoboRoachchar4UUID },
        0, 
        0, 
        &RoboRoachChar4 
      },

      // Characteristic 4 configuration
      { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)RoboRoachChar4Config 
      },
      
      // Characteristic 4 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        RoboRoachChar4UserDesp 
      },
      
    // Characteristic 5 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &RoboRoachChar5Props 
    },

      // Characteristic Value 5
      { 
        { ATT_BT_UUID_SIZE, RoboRoachchar5UUID },
        GATT_PERMIT_AUTHEN_READ, 
        0, 
        RoboRoachChar5 
      },

      // Characteristic 5 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        RoboRoachChar5UserDesp 
      },


};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 RoboRoach_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t RoboRoach_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );

static void RoboRoach_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t RoboRoachCBs =
{
  RoboRoach_ReadAttrCB,  // Read callback function pointer
  RoboRoach_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      RoboRoach_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t RoboRoach_AddService( uint32 services )
{
  uint8 status = SUCCESS;

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, RoboRoachChar4Config );

  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( RoboRoach_HandleConnStatusCB );  
  
  if ( services & ROBOROACH_SERVICE  )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( RoboRoachAttrTbl, 
                                          GATT_NUM_ATTRS( RoboRoachAttrTbl ),
                                          &RoboRoachCBs );
  }

  return ( status );
}


/*********************************************************************
 * @fn      RoboRoach_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t RoboRoach_RegisterAppCBs( RoboRoachCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    RoboRoach_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
  

/*********************************************************************
 * @fn      RoboRoach_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t RoboRoach_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case ROBOROACH_FREQUENCY:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharFrequency = *((uint8*)value);
        roboRoach_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ROBOROACH_PULSE_WIDTH:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharPulseWidth = *((uint8*)value);
        roboRoach_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ROBOROACH_NUM_PULSES:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharNumPulses = *((uint8*)value);
        roboRoach_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ROBOROACH_RANDOM_MODE:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharRandomMode = *((uint8*)value);
        roboRoach_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ROBOROACH_STIMUATE_LEFT:
    case ROBOROACH_STIMUATE_RIGHT:
      if ( len == sizeof ( uint8 ) ) 
      {
        //Call and pass ROBOROACH_STIMUATE_LEFT or ROBOROACH_STIMUATE_RIGHT
        roboRoach_Stimulate( param );   
        
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn      RoboRoach_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t RoboRoach_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case RoboRoach_CHAR1:
      *((uint8*)value) = RoboRoachChar1;
      break;

    case RoboRoach_CHAR2:
      *((uint8*)value) = RoboRoachChar2;
      break;      

    case RoboRoach_CHAR3:
      *((uint8*)value) = RoboRoachChar3;
      break;  

    case RoboRoach_CHAR4:
      *((uint8*)value) = RoboRoachChar4;
      break;

    case RoboRoach_CHAR5:
      VOID osal_memcpy( value, RoboRoachChar5, RoboRoach_CHAR5_LEN );
      break;      
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          RoboRoach_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 RoboRoach_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
 
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
      // gattserverapp handles those reads

      // characteristics 1 and 2 have read permissions
      // characteritisc 3 does not have read permissions; therefore it is not
      //   included here
      // characteristic 4 does not have read permissions, but because it
      //   can be sent as a notification, it is included here
      case RoboRoach_CHAR1_UUID:
      case RoboRoach_CHAR2_UUID:
      case RoboRoach_CHAR4_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
        break;

      case RoboRoach_CHAR5_UUID:
        *pLen = RoboRoach_CHAR5_LEN;
        VOID osal_memcpy( pValue, pAttr->pValue, RoboRoach_CHAR5_LEN );
        break;
        
      default:
        // Should never get here! (characteristics 3 and 4 do not have read permissions)
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    *pLen = 0;
    status = ATT_ERR_INVALID_HANDLE;
  }

  return ( status );
}

/*********************************************************************
 * @fn      RoboRoach_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   complete - whether this is the last packet
 * @param   oper - whether to validate and/or write attribute value  
 *
 * @return  Success or Failure
 */
static bStatus_t RoboRoach_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset )
{
  bStatus_t status = SUCCESS;
  uint8 notifyApp = 0xFF;
  
  // If attribute permissions require authorization to write, return error
  if ( gattPermitAuthorWrite( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      case RoboRoach_CHAR1_UUID:
      case RoboRoach_CHAR3_UUID:

        //Validate the value
        // Make sure it's not a blob oper
        if ( offset == 0 )
        {
          if ( len != 1 )
          {
            status = ATT_ERR_INVALID_VALUE_SIZE;
          }
        }
        else
        {
          status = ATT_ERR_ATTR_NOT_LONG;
        }
        
        //Write the value
        if ( status == SUCCESS )
        {
          uint8 *pCurValue = (uint8 *)pAttr->pValue;        
          *pCurValue = pValue[0];

          if( pAttr->pValue == &RoboRoachChar1 )
          {
            notifyApp = RoboRoach_CHAR1;        
          }
          else
          {
            notifyApp = RoboRoach_CHAR3;           
          }
        }
             
        break;

      case GATT_CLIENT_CHAR_CFG_UUID:
        status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                 offset, GATT_CLIENT_CFG_NOTIFY );
        break;
        
      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    status = ATT_ERR_INVALID_HANDLE;
  }

  // If a charactersitic value changed then callback function to notify application of change
  if ( (notifyApp != 0xFF ) && RoboRoach_AppCBs && RoboRoach_AppCBs->pfnRoboRoachChange )
  {
    RoboRoach_AppCBs->pfnRoboRoachChange( notifyApp );  
  }
  
  return ( status );
}

/*********************************************************************
 * @fn          RoboRoach_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void RoboRoach_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset Client Char Config if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
         ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) && 
           ( !linkDB_Up( connHandle ) ) ) )
    { 
      GATTServApp_InitCharCfg( connHandle, RoboRoachChar4Config );
    }
  }
}


/*********************************************************************
*********************************************************************/
