/**************************************************************************************************
  Filename:       roboroach_profile.c
  Description:    This file contains the RoboRoach GATT profile  
                  used to configure and control the Backyard Brains Cockroach
                  cyborg: The RoboRoach.

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
#include "hal_led.h"
#include <math.h>

#include "roboRoach.h"
#include "roboroach_GATTprofile.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED      40  

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

uint16 stimulationPeriodInMilliseconds = 100;
uint8 stimulationDutyCycle = 0;
long offsetTime = 1000;
uint8  roboRoachApp_TaskID = 0;


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

// Duration of Stimulation in 5ms Increments Characteristic UUID: 0xB2B3
CONST uint8 rrCharDurationIn5msIncrementsUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_DURATION_IN_5MS_UUID), HI_UINT16(ROBOROACH_CHAR_DURATION_IN_5MS_UUID)
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

// Stimulation Gain Characteristic UUID: 0xB2B7
CONST uint8 rrCharGainUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_GAIN_UUID), HI_UINT16(ROBOROACH_CHAR_GAIN_UUID)
};

// Stimulation Random Mode Frequency Minimum Characteristic UUID: 0xB2B8
CONST uint8 rrCharFreqMinUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_FREQ_MIN_UUID), HI_UINT16(ROBOROACH_CHAR_FREQ_MIN_UUID)
};

// Stimulation Random Mode Frequency Maximum Characteristic UUID: 0xB2B9
CONST uint8 rrCharFreqMaxUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_FREQ_MAX_UUID), HI_UINT16(ROBOROACH_CHAR_FREQ_MAX_UUID)
};

// Stimulation Random Mode Pulse Width Minimum Characteristic UUID: 0xB2BA
CONST uint8 rrCharPWminUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_PW_MIN_UUID), HI_UINT16(ROBOROACH_CHAR_PW_MIN_UUID)
};

// Stimulation Random Mode Pulse Width Maximum Characteristic UUID: 0xB2BB
CONST uint8 rrCharPWmaxUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_PW_MAX_UUID), HI_UINT16(ROBOROACH_CHAR_PW_MAX_UUID)
};

// Stimulation Random Mode Gain Minimum Characteristic UUID: 0xB2BC
CONST uint8 rrCharGainMinUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_GAIN_MIN_UUID), HI_UINT16(ROBOROACH_CHAR_GAIN_MIN_UUID)
};
 
// Stimulation Random Mode Gain Maximum Characteristic UUID: 0xB2BD
CONST uint8 rrCharGainMaxUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(ROBOROACH_CHAR_GAIN_MAX_UUID), HI_UINT16(ROBOROACH_CHAR_GAIN_MAX_UUID)
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

static roboRoachProfileCBs_t *roboRoach_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// RoboRoach Service attribute
static CONST gattAttrType_t roboRoachService = { ATT_BT_UUID_SIZE, roboRoachServUUID };

// Stimulation Frequency Characteristic Properties
static uint8 rrCharFrequencyProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 rrCharFrequency = 55; //Default: 55Hz    [wjr]: when i change this the default does not... what does it affect???
static uint8 rrCharFrequencyUserDesp[22] = "Stimulation Frequency\0";

// Stimulation Gain Characteristic Properties
static uint8 rrCharGainProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 rrCharGain = 50; //Default: 50%
static uint8 rrCharGainUserDesp[17] = "Stimulation Gain\0";

// Pulse Width Characteristic Properties
static uint8 rrCharPulseWidthProps = GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 rrCharPulseWidth = 5; //Default: 5ms
static uint8 rrCharPulseWidthUserDesp[29] = "Stimulation Pulse Width (ms)\0";

// Duration of Stimulus Characteristic Properties
static uint8 rrCharDurationIn5msIncrementsProps =  GATT_PROP_READ | GATT_PROP_WRITE;
static uint8 rrCharDurationIn5msIncrements = 100; //Default: 100 * 5ms = 500ms default
static uint8 rrCharDurationIn5msIncrementsUserDesp[24] = "Duration (in 5ms units)\0";

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

// Random Mode Freq Minimum Characteristic Properties
static uint8 rrCharFreqMinProps = GATT_PROP_READ | GATT_PROP_WRITE; //GATT_PROP_NOTIFY;
static uint8 rrCharFreqMin = 40;  //Default: 40 Hz
static uint8 rrCharFreqMinUserDesp[18] = "Minimum Frequency\0";

// Random Mode Freq Maxmimum Characteristic Properties
static uint8 rrCharFreqMaxProps = GATT_PROP_READ | GATT_PROP_WRITE; //GATT_PROP_NOTIFY;
static uint8 rrCharFreqMax = 100;  //Default: 100 Hz
static uint8 rrCharFreqMaxUserDesp[18] = "Maximum Frequency\0";

// Random Mode Pulse Width Minimum Characteristic Properties
static uint8 rrCharPWminProps = GATT_PROP_READ | GATT_PROP_WRITE; //GATT_PROP_NOTIFY;
static uint8 rrCharPWmin = 1;  //Default: 1 ms
static uint8 rrCharPWminUserDesp[25] = "Minimum Pulse Width (ms)\0";

// Random Mode Pulse Width Maxmimum Characteristic Properties
static uint8 rrCharPWmaxProps = GATT_PROP_READ | GATT_PROP_WRITE; //GATT_PROP_NOTIFY;
static uint8 rrCharPWmax = 9;  //Default: 9 ms
static uint8 rrCharPWmaxUserDesp[25] = "Maximum Pulse Width (ms)\0";

// Random Mode Gain Minimum Characteristic Properties
static uint8 rrCharGainMinProps = GATT_PROP_READ | GATT_PROP_WRITE; //GATT_PROP_NOTIFY;
static uint8 rrCharGainMin = 50;  //Default: 50%
static uint8 rrCharGainMinUserDesp[13] = "Minimum Gain\0";

// Random Mode Gain Maxmimum Characteristic Properties
static uint8 rrCharGainMaxProps = GATT_PROP_READ | GATT_PROP_WRITE; //GATT_PROP_NOTIFY;
static uint8 rrCharGainMax = 50;  //Default: 50%
static uint8 rrCharGainMaxUserDesp[13] = "Maximum Gain\0";


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
      
    // Duration In 5ms Increments Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharDurationIn5msIncrementsProps },
    {{ ATT_BT_UUID_SIZE, rrCharDurationIn5msIncrementsUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharDurationIn5msIncrements },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharDurationIn5msIncrementsUserDesp },

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
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharStimulateRightUserDesp },
   
    // Gain Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharGainProps },
    {{ ATT_BT_UUID_SIZE, rrCharGainUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharGain },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharGainUserDesp },    

    // Random Mode Frequency Minimum Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharFreqMinProps },
    {{ ATT_BT_UUID_SIZE, rrCharFreqMinUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharFreqMin },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharFreqMinUserDesp },

    // Random Mode Frequency Maximum Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharFreqMaxProps },
    {{ ATT_BT_UUID_SIZE, rrCharFreqMaxUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharFreqMax },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharFreqMaxUserDesp }, 

    // Random Mode Pulse Width Minimum Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharPWminProps },
    {{ ATT_BT_UUID_SIZE, rrCharPWminUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharPWmin },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharPWminUserDesp }, 

    // Random Mode Pulse Width Maximum Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharPWmaxProps },
    {{ ATT_BT_UUID_SIZE, rrCharPWmaxUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharPWmax },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharPWmaxUserDesp }, 

    // Random Mode Gain Minimum Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharGainMinProps },
    {{ ATT_BT_UUID_SIZE, rrCharGainMinUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharGainMin },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharGainMinUserDesp }, 

    // Random Mode Gain Maximum Characteristic Declaration
    {{ ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &rrCharGainMaxProps },
    {{ ATT_BT_UUID_SIZE, rrCharGainMaxUUID }, GATT_PERMIT_READ | GATT_PERMIT_WRITE, 0, &rrCharGainMax },
    {{ ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, rrCharGainMaxUserDesp }, 
    
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 roboRoachProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t roboRoachProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );

static void roboRoachProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType );

static void roboRoachProfile_updateStimulationSettings( void );
static void roboRoachProfile_Stimulate( uint16 uuid );
 
/*********************************************************************
 * PROFILE CALLBACKS
 */
// RoboRoach Service Callbacks
CONST gattServiceCBs_t roboRoachCBs =
{
  roboRoachProfile_ReadAttrCB,  // Read callback function pointer
  roboRoachProfile_WriteAttrCB, // Write callback function pointer
  NULL                   // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      RoboRoachProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t RoboRoachProfile_AddService( uint8 taskID )
{
  uint8 status = SUCCESS;
  roboRoachApp_TaskID = taskID;

  // Initialize Client Characteristic Configuration attributes
  //GATTServApp_InitCharCfg( INVALID_CONNHANDLE, rrCharStimulateLeftConfig );

  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( roboRoachProfile_HandleConnStatusCB );  
  
 
  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( roboRoachAttrTbl, 
                                        GATT_NUM_ATTRS( roboRoachAttrTbl ),
                                        &roboRoachCBs );
 
  return ( status );
}


/*********************************************************************
 * @fn      RoboRoachProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t RoboRoachProfile_RegisterAppCBs( roboRoachProfileCBs_t *appCallbacks )
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
bStatus_t RoboRoachProfile_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case ROBOROACH_FREQUENCY:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharFrequency = *((uint8*)value);
        roboRoachProfile_updateStimulationSettings();
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
        roboRoachProfile_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ROBOROACH_DURATION:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharDurationIn5msIncrements = *((uint8*)value);
        roboRoachProfile_updateStimulationSettings();
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
        roboRoachProfile_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ROBOROACH_GAIN:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharGain = *((uint8*)value);
        roboRoachProfile_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    case ROBOROACH_STIMULATE_LEFT:
    case ROBOROACH_STIMULATE_RIGHT:
      if ( len == sizeof ( uint8 ) ) 
      {
        //Call and pass ROBOROACH_STIMULATE_LEFT or ROBOROACH_STIMULATE_RIGHT
        roboRoachProfile_Stimulate( param );   
        
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ROBOROACH_FREQ_MIN:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharFreqMin = *((uint8*)value);
        roboRoachProfile_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    case ROBOROACH_FREQ_MAX:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharFreqMax = *((uint8*)value);
        roboRoachProfile_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    case ROBOROACH_PW_MIN:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharPWmin = *((uint8*)value);
        roboRoachProfile_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    case ROBOROACH_PW_MAX:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharPWmax = *((uint8*)value);
        roboRoachProfile_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case ROBOROACH_GAIN_MIN:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharGainMin = *((uint8*)value);
        roboRoachProfile_updateStimulationSettings();
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    case ROBOROACH_GAIN_MAX:
      if ( len == sizeof ( uint8 ) ) 
      {
        rrCharGainMax = *((uint8*)value);
        roboRoachProfile_updateStimulationSettings();
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
 * @fn      RoboRoachProfile_GetParameter (Internally)
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
bStatus_t RoboRoachProfile_GetParameter( uint8 param, void *value )
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

    case ROBOROACH_DURATION:
      *((uint8*)value) = rrCharDurationIn5msIncrements;
      break;  

    case ROBOROACH_RANDOM_MODE:
      *((uint8*)value) = rrCharRandomMode;
      break;  

    case ROBOROACH_GAIN:
      *((uint8*)value) = rrCharGain;
      break;      
      
    case ROBOROACH_STIM_PERIOD:
      *((uint16*)value) = stimulationPeriodInMilliseconds;
      break;  
    
    case ROBOROACH_FREQ_MIN:
      *((uint8*)value) = rrCharFreqMin;
      break;     
      
    case ROBOROACH_FREQ_MAX:
      *((uint8*)value) = rrCharFreqMax;
      break;  
      
    case ROBOROACH_PW_MIN:
      *((uint8*)value) = rrCharPWmin;
      break;     
      
    case ROBOROACH_PW_MAX:
      *((uint8*)value) = rrCharPWmax;
      break;  
      
    case ROBOROACH_GAIN_MIN:
      *((uint8*)value) = rrCharGainMin;
      break;     
      
    case ROBOROACH_GAIN_MAX:
      *((uint8*)value) = rrCharGainMax;
      break;        
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          roboRoachProfile_ReadAttrCB
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
static uint8 roboRoachProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
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

      // Freq, Pulse, Duration, and Random Mode characteristics are all Readable.
      case ROBOROACH_CHAR_FREQUENCY_UUID:
      case ROBOROACH_CHAR_PULSE_WIDTH_UUID:
      case ROBOROACH_CHAR_DURATION_IN_5MS_UUID:
      case ROBOROACH_CHAR_RANDOM_MODE_UUID:
      case ROBOROACH_CHAR_GAIN_UUID: 
      case ROBOROACH_CHAR_FREQ_MIN_UUID:
      case ROBOROACH_CHAR_FREQ_MAX_UUID:
      case ROBOROACH_CHAR_PW_MIN_UUID:
      case ROBOROACH_CHAR_PW_MAX_UUID:
      case ROBOROACH_CHAR_GAIN_MIN_UUID:
      case ROBOROACH_CHAR_GAIN_MAX_UUID: 
      
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
 * @fn      roboRoachProfile_WriteAttrCB
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
static bStatus_t roboRoachProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
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
    //AT_DBG( "roboRoach_WriteAttrCB: connHandle=[%i], uuid=[%x]", connHandle, uuid );
 
    switch ( uuid )
    {
      case ROBOROACH_CHAR_FREQUENCY_UUID:
      case ROBOROACH_CHAR_PULSE_WIDTH_UUID:
      case ROBOROACH_CHAR_DURATION_IN_5MS_UUID:
      case ROBOROACH_CHAR_RANDOM_MODE_UUID:
      case ROBOROACH_CHAR_GAIN_UUID:
      case ROBOROACH_CHAR_FREQ_MIN_UUID:
      case ROBOROACH_CHAR_FREQ_MAX_UUID:
      case ROBOROACH_CHAR_PW_MIN_UUID:
      case ROBOROACH_CHAR_PW_MAX_UUID:
      case ROBOROACH_CHAR_GAIN_MIN_UUID:
      case ROBOROACH_CHAR_GAIN_MAX_UUID: 
      
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
          
          //Update the Stim Values
          roboRoachProfile_updateStimulationSettings();
      
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
          roboRoachProfile_Stimulate( uuid );
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
  if ( (notifyApp != 0xFF ) && roboRoach_AppCBs && roboRoach_AppCBs->pfnRoboRoachProfileChange )
  {
    roboRoach_AppCBs->pfnRoboRoachProfileChange( notifyApp );  
  }
  
  return ( status );
}


/*********************************************************************
 * @fn          roboRoachProfile_updateStimulationSettings
 * 
 * Command Format: ATSLED,<Led>,<Duty_Cycle>,<Period>
 * Led: 0 = PIO_2, 1 = PIO_5
 * Duty_Cycle: Integer decimal value from 0 to 100. If the Duty_Cycle is set to 0, the status LED functionality will be disabled and the PIO can be controlled manually using the ATSPIO command.
 * Period: Integer decimal value from 1ms to 65,535ms
 * 
 * Factory Default (PIO_2): Duty Cycle = 100, Period = 65535 (PIO_2),
 */
static void roboRoachProfile_updateStimulationSettings( void )
{
  // AT_DBG( "");
  //Convert Frequency to ms.  
  // Time_MS = 1/Freq * 1000  
  stimulationPeriodInMilliseconds = (1000/rrCharFrequency);
   
  //Calculate the Duty Cycle   
  // (Pulse Width / Period) * 100
  float tempDutyCycle = 100*rrCharPulseWidth/stimulationPeriodInMilliseconds;
  //float floatingPW = 0;
  if (tempDutyCycle > 100) {
   tempDutyCycle = 100;
  }
  stimulationDutyCycle = (uint8) tempDutyCycle;
  offsetTime = stimulationPeriodInMilliseconds * (rrCharDurationIn5msIncrements+1);
  
  //[wjr]Makes sure the duty cycle is never greater than 70%
  /*
  if(stimulationDutyCycle > 70)
  {
    floatingPW = (0.7)*rrCharPulseWidth;
    rrCharPulseWidth = (uint8)floatingPW;
  }
  */
  
  //AT_DBG( "Up: F[%i] P[%i] PW[%i] Cy[%i] Dur[%i]",  
  //            rrCharFrequency , 
  //            stimulationPeriodInMilliseconds, 
  //            rrCharPulseWidth,
  //            stimulationDutyCycle,
  //            offsetTime); 
}


/*********************************************************************
 * @fn          roboRoachProfile_Stimulate
 * 
 * Sends Stimulation signal to RoboRoach PIO_2 or PIO_5 pins (Left, Right Antenna)
 * Turns on indicator LED
 * Sets the timer to turn off Stimulation and LEDs 
 * 
 * parameter:  uuid = ROBOROACH_CHAR_STIMULATE_LEFT_UUID or 
 *                    ROBOROACH_CHAR_STIMULATE_RIGHT_UUID
 */
static void roboRoachProfile_Stimulate( uint16 uuid )
{
   if (uuid == ROBOROACH_CHAR_STIMULATE_LEFT_UUID)
   {
     osal_start_timerEx( roboRoachApp_TaskID, BYB_STIMULATE_LEFT_EVT, 1 );
   }
   else
   {
     osal_start_timerEx( roboRoachApp_TaskID, BYB_STIMULATE_RIGHT_EVT, 1 );  
   }
        
}

/*********************************************************************
 * @fn          roboRoachProfile_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void roboRoachProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
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
