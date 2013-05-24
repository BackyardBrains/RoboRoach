/**************************************************************************************************
  Filename:       roboroach_GATTprofile.c
  Description:    This file contains the RoboRoach GATT profile  
                  used to configure and control the Backyard Brains Cockroach
                  cyborg: The RoboRoach.

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
//#include "gapbondmgr.h"
#include <math.h>
#include "at_lib.h"
#include "roboroach_profile.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED      23  // 17

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

static roboRoachCBs_t *roboRoach_AppCBs = NULL;

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

static gattAttribute_t roboRoachAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  // RoboRoach Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&roboRoachService                /* pValue */
  },

    // Frequency Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharFrequencyProps },
    {{ ATT_BT_UUID_SIZE, rrCharFrequencyUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharFrequency },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharFrequencyUserDesp },      

    // Pulse Width Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharPulseWidthProps},
    {{ ATT_BT_UUID_SIZE, rrCharPulseWidthUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharPulseWidth},
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ,0,rrCharPulseWidthUserDesp},           
      
    // Number of Pulses Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharNumPulsesProps },
    {{ ATT_BT_UUID_SIZE, rrCharNumberOfPulsesUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharNumPulses },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharNumPulsesUserDesp },

    // Random Mode Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharRandomModeProps },
    {{ ATT_BT_UUID_SIZE, rrCharRandomModeUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharRandomMode },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharRandomModeUserDesp },           
      
    // Stimulate Left Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharStimulateLeftProps },
    {{ ATT_BT_UUID_SIZE, rrCharStimulateLeftUUID }, GATT_PERMIT_WRITE, 0, &rrCharStimulateLeft },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharStimulateLeftUserDesp },
      
    // Stimulate Right Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharStimulateRightProps },
    {{ ATT_BT_UUID_SIZE, rrCharStimulateRightUUID }, GATT_PERMIT_WRITE, 0, &rrCharStimulateRight },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharStimulateRightUserDesp }
   
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 roboRoach_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t roboRoach_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );

static void roboRoach_HandleConnStatusCB( uint16 connHandle, uint8 changeType );

static void roboRoach_updateStimulationSettings( void );
static void roboRoach_Stimulate( uint16 uuid );
 
/*********************************************************************
 * PROFILE CALLBACKS
 */
// RoboRoach Service Callbacks
CONST gattServiceCBs_t roboRoachCBs =
{
  roboRoach_ReadAttrCB,  // Read callback function pointer
  roboRoach_WriteAttrCB, // Write callback function pointer
  NULL                   // Authorization callback function pointer
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
  //GATTServApp_InitCharCfg( INVALID_CONNHANDLE, rrCharStimulateLeftConfig );

  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( roboRoach_HandleConnStatusCB );  
  
  if ( services & ROBOROACH_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( roboRoachAttrTbl, 
                                          GATT_NUM_ATTRS( roboRoachAttrTbl ),
                                          &roboRoachCBs );
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
bStatus_t RoboRoach_RegisterAppCBs( roboRoachCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    roboRoach_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
  

/*********************************************************************
 * @fn      RoboRoach_SetParameter (Internally)
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
  AT_DBG(" RoboRoach_SetParameter(%i,%i)", param, len);
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
 * @fn      RoboRoach_GetParameter (Internally)
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
    case ROBOROACH_FREQUENCY:
      *((uint8*)value) = rrCharFrequency;
      break;

    case ROBOROACH_PULSE_WIDTH:
      *((uint8*)value) = rrCharPulseWidth;
      break;      

    case ROBOROACH_NUM_PULSES:
      *((uint8*)value) = rrCharNumPulses;
      break;  

    case ROBOROACH_RANDOM_MODE:
      *((uint8*)value) = rrCharRandomMode;
      break;  
    
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          roboRoach_ReadAttrCB
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
static uint8 roboRoach_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
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

      // Freq, Pulse, NumPulses, and Random Mode characteristics are all Readable.
      case ROBOROACH_CHAR_FREQUENCY_UUID:
      case ROBOROACH_CHAR_PULSE_WIDTH_UUID:
      case ROBOROACH_CHAR_NUM_PULSES_UUID:
      case ROBOROACH_CHAR_RANDOM_MODE_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
        break;
  
      default:
        // Should never get here! (Stimulation characteristics do not have read permissions)
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
 * @fn      roboRoach_WriteAttrCB
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
static bStatus_t roboRoach_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
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
    AT_DBG( "roboRoach_WriteAttrCB: value=[%i = 0x%x], uuid=[%x]", pValue[0], pValue[0], uuid );
 
    switch ( uuid )
    {
      case ROBOROACH_CHAR_FREQUENCY_UUID:
      case ROBOROACH_CHAR_PULSE_WIDTH_UUID:
      case ROBOROACH_CHAR_NUM_PULSES_UUID:
      case ROBOROACH_CHAR_RANDOM_MODE_UUID:
      
        //Validate the value (Make sure it's not a blob oper)
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
        
          AT_DBG("Value changed.  Calling  roboRoach_UpdateStimSettings()");
          
          //Update the Stim Values
          roboRoach_updateStimulationSettings();
      
        }
                     
        break;
        
      case ROBOROACH_CHAR_STIMULATE_LEFT_UUID :
      case ROBOROACH_CHAR_STIMULATE_RIGHT_UUID :
     
        //Double Check this is Legit.
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
        
        //No need to write the value.  Just fire the event!
        if ( status == SUCCESS )
        {         
          roboRoach_Stimulate( uuid );
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
  if ( (notifyApp != 0xFF ) && roboRoach_AppCBs && roboRoach_AppCBs->pfnRoboRoachChange )
  {
    roboRoach_AppCBs->pfnRoboRoachChange( notifyApp );  
  }
  
  return ( status );
}


/*********************************************************************
 * @fn          roboRoach_updateStimulationSettings
 * 
 * Command Format: roboRoach_updateStimulationSettings
 * 
 * Updates all of the parameters for the RoboRoach
 */
static void roboRoach_updateStimulationSettings( void )
{
  AT_DBG( "Updating Stim Settings");
  //Convert Frequency to ms.  
  // Time_MS = 1/Freq * 1000  
  stimulationPeriodInMilliseconds = (1000/rrCharFrequency);
   
  //Calculate the Duty Cycle
  // (Pulse Width / Period) * 100
  float tempDutyCycle = 100*rrCharPulseWidth/stimulationPeriodInMilliseconds;
  if (tempDutyCycle > 100) {
   tempDutyCycle = 100;
  }
  stimulationDutyCycle = (uint8) tempDutyCycle;
  offsetTime = stimulationPeriodInMilliseconds * (rrCharNumPulses+1);
  
  AT_DBG( "Up: F[%i] P[%i] PW[%i] Cy[%i] Dur[%f]",  
              rrCharFrequency , 
              stimulationPeriodInMilliseconds, 
              rrCharPulseWidth,
              stimulationDutyCycle,
              offsetTime); 
}


/*********************************************************************
 * @fn          roboRoach_Stimulate
 * 
 * Sends Stimulation signal to RoboRoach PIO_2 or PIO_5 pins (Left, Right Antenna)
 * Turns on indicator LED
 * Sets the timer to turn off Stimulation and LEDs 
 * 
 * parameter:  uuid = ROBOROACH_CHAR_STIMULATE_LEFT_UUID or 
 *                    ROBOROACH_CHAR_STIMULATE_RIGHT_UUID
 */
static void roboRoach_Stimulate( uint16 uuid )
{
   //Assume Right
   uint8 antennaPIO = ATSLED_LED_PIO5; 
   uint8 indicaterPIO = ROBOROACH_PIO_LED_RIGHT;
   
   //Then Change if Left
   if (uuid == ROBOROACH_CHAR_STIMULATE_LEFT_UUID)
   {
     antennaPIO = ATSLED_LED_PIO2; 
     indicaterPIO = ROBOROACH_PIO_LED_LEFT;
   }

   AT_DBG( "Stimulate: Ant=[%i],LED=[%i],TO=[%i]",antennaPIO, indicaterPIO, offsetTime );

   AT_SetLed( antennaPIO,  stimulationDutyCycle, stimulationPeriodInMilliseconds); // Cockroach Antenna
   AT_SetPio( indicaterPIO, ATSPIO_DIR_OUT, ATSPIO_OUT_LEVEL_HIGH );
  
   osal_start_timerEx( AT_TaskId(), TURN_OFF_STIM_EVT, offsetTime ); 
        
}

/*********************************************************************
 * @fn          roboRoach_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void roboRoach_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset Client Char Config if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
         ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) && 
           ( !linkDB_Up( connHandle ) ) ) )
    { 
      //GATTServApp_InitCharCfg( connHandle, rrCharStimulateLeftConfig );
    }
  }
}


/*********************************************************************
*********************************************************************/
