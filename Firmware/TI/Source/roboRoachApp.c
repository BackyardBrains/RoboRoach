/**************************************************************************************************
  Filename:       RoboRoachApp.c
 
  Description:    This file contains the RoboRoach Peripheral application
                  for use with the Backyard Brains RoboRoach.

**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "bcomdef.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"

#include "OnBoard.h"
#include "hal_adc.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_lcd.h"

#include "gatt.h"

#include "hci.h"

#include "gapgattserver.h"
#include "gattservapp.h"
#include "devinfoservice.h"
#include "battservice.h"
#include "roboRoach_GATTprofile.h"
#include "roboRoach.h"

#if defined ( PLUS_BROADCASTER )
  #include "peripheralBroadcaster.h"
#else
  #include "peripheral.h"
#endif

#include "gapbondmgr.h"

#include "roboRoach.h"
#include "roboRoachApp.h"

#if defined FEATURE_OAD
  #include "oad.h"
  #include "oad_target.h"
#endif

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */


// What is the advertising interval when device is discoverable (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL          160

// Whether to enable automatic parameter update request when a connection is formed
#define DEFAULT_ENABLE_UPDATE_REQUEST         FALSE

// Limited discoverable mode advertises for 30.72s, and then stops
// General discoverable mode advertises indefinitely

#if defined ( CC2540_MINIDK )
#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_LIMITED
#else
#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_GENERAL
#endif  // defined ( CC2540_MINIDK )

// Minimum connection interval (units of 1.25ms, 80=100ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     80

// Maximum connection interval (units of 1.25ms, 800=1000ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     800

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY         0

// Supervision timeout value (units of 10ms, 1000=10s) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_CONN_TIMEOUT          1000

// Company Identifier: Texas Instruments Inc. (13)
#define TI_COMPANY_ID                         0x000D

#define INVALID_CONNHANDLE                    0xFFFF

// Length of bd addr as a string
#define B_ADDR_STR_LEN                        15

#if defined ( PLUS_BROADCASTER )
  #define ADV_IN_CONN_WAIT                    500 // delay 500 ms
#endif

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

bool isConnected = FALSE;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

uint8   connectPulseCount = 0;   
uint8   stimulationPulseCount = 0;   
uint8   stimulationInProgress = 0;   
uint8   stimulationIsLeft = 0;
uint16  stimulationPeriod = 0;
uint8   stimulationPulseWidth = 0;
uint8   stimulationNumberOfPulses = 0;
uint8   stimulationRandomMode = 0;
   
static uint8 roboRoachApp_TaskID;   // Task ID for internal task/event processing

static gaprole_States_t gapProfileState = GAPROLE_INIT;

// GAP - SCAN RSP data (max size = 31 bytes)
static uint8 scanRspData[] =
{
  // complete name
  0x0A,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  0x52,   // 'R'
  0x6F,   // 'o'
  0x62,   // 'b'
  0x6F,   // 'o'
  0x52,   // 'R'
  0x6F,   // 'o'
  0x61,   // 'a'
  0x63,   // 'c'
  0x68,   // 'h'

  // connection interval range
  0x05,   // length of this data
  GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
  LO_UINT16( DEFAULT_DESIRED_MIN_CONN_INTERVAL ),   // 100ms
  HI_UINT16( DEFAULT_DESIRED_MIN_CONN_INTERVAL ),
  LO_UINT16( DEFAULT_DESIRED_MAX_CONN_INTERVAL ),   // 1s
  HI_UINT16( DEFAULT_DESIRED_MAX_CONN_INTERVAL ),

  // Tx power level
  0x02,   // length of this data
  GAP_ADTYPE_POWER_LEVEL,
  0       // 0dBm
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
static uint8 advertData[] =
{
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely)
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // service UUID, to notify central devices what services are included
  // in this peripheral
  0x03,   // length of this data
  GAP_ADTYPE_16BIT_MORE,      // some of the UUID's, but not all
  LO_UINT16( ROBOROACH_SERV_UUID ),
  HI_UINT16( ROBOROACH_SERV_UUID ),

};

// GAP GATT Attributes
static uint8 attDeviceName[GAP_DEVICE_NAME_LEN] = "RoboRoach";

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void roboRoachApp_ProcessOSALMsg( osal_event_hdr_t *pMsg );
static void peripheralStateNotificationCB( gaprole_States_t newState );
static void roboRoachProfileChangeCB( uint8 paramID );

#if defined( CC2540_MINIDK )
static void roboRoachApp_HandleKeys( uint8 shift, uint8 keys );
#endif




/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t roboRoachApp_PeripheralCBs =
{
  peripheralStateNotificationCB,  // Profile State Change Callbacks
  NULL                            // When a valid RSSI is read from controller (not used by application)
};

// GAP Bond Manager Callbacks
static gapBondCBs_t roboRoachApp_BondMgrCBs =
{
  NULL,                     // Passcode callback (not used by application)
  NULL                      // Pairing / Bonding state Callback (not used by application)
};

// Simple GATT Profile Callbacks
static roboRoachProfileCBs_t roboRoachApp_RoboRoachProfileCBs =
{
  roboRoachProfileChangeCB    // Charactersitic value change callback
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      RoboRoachPeripheral_Init
 *
 * @brief   Initialization function for the Simple BLE Peripheral App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void RoboRoachPeripheral_Init( uint8 task_id )
{
  roboRoachApp_TaskID = task_id;

  // Setup the GAP Peripheral Role Profile
  {

    #if defined( CC2540_MINIDK )
      // For the CC2540DK-MINI keyfob, device doesn't start advertising until button is pressed
      uint8 initial_advertising_enable = FALSE;
    #else
      // For other hardware platforms, device starts advertising upon initialization
      uint8 initial_advertising_enable = TRUE;
    #endif

    // By setting this to zero, the device will go into the waiting state after
    // being discoverable for 30.72 second, and will not being advertising again
    // until the enabler is set back to TRUE
    uint16 gapRole_AdvertOffTime = 0;

    uint8 enable_update_request = DEFAULT_ENABLE_UPDATE_REQUEST;
    uint16 desired_min_interval = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
    uint16 desired_max_interval = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
    uint16 desired_slave_latency = DEFAULT_DESIRED_SLAVE_LATENCY;
    uint16 desired_conn_timeout = DEFAULT_DESIRED_CONN_TIMEOUT;

    // Set the GAP Role Parameters
    GAPRole_SetParameter( GAPROLE_ADVERT_ENABLED, sizeof( uint8 ), &initial_advertising_enable );
    GAPRole_SetParameter( GAPROLE_ADVERT_OFF_TIME, sizeof( uint16 ), &gapRole_AdvertOffTime );

    GAPRole_SetParameter( GAPROLE_SCAN_RSP_DATA, sizeof ( scanRspData ), scanRspData );
    GAPRole_SetParameter( GAPROLE_ADVERT_DATA, sizeof( advertData ), advertData );

    GAPRole_SetParameter( GAPROLE_PARAM_UPDATE_ENABLE, sizeof( uint8 ), &enable_update_request );
    GAPRole_SetParameter( GAPROLE_MIN_CONN_INTERVAL, sizeof( uint16 ), &desired_min_interval );
    GAPRole_SetParameter( GAPROLE_MAX_CONN_INTERVAL, sizeof( uint16 ), &desired_max_interval );
    GAPRole_SetParameter( GAPROLE_SLAVE_LATENCY, sizeof( uint16 ), &desired_slave_latency );
    GAPRole_SetParameter( GAPROLE_TIMEOUT_MULTIPLIER, sizeof( uint16 ), &desired_conn_timeout );
  }

  // Set the GAP Characteristics
  GGS_SetParameter( GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName );

  // Set advertising interval
  {
    uint16 advInt = DEFAULT_ADVERTISING_INTERVAL;

    GAP_SetParamValue( TGAP_LIM_DISC_ADV_INT_MIN, advInt );
    GAP_SetParamValue( TGAP_LIM_DISC_ADV_INT_MAX, advInt );
    GAP_SetParamValue( TGAP_GEN_DISC_ADV_INT_MIN, advInt );
    GAP_SetParamValue( TGAP_GEN_DISC_ADV_INT_MAX, advInt );
  }

  //[GJG] - What is this?  
  // Setup the GAP Bond Manager
  {
    uint32 passkey = 0; // passkey "000000"
    uint8 pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
    uint8 mitm = TRUE;
    uint8 ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
    uint8 bonding = TRUE;
    GAPBondMgr_SetParameter( GAPBOND_DEFAULT_PASSCODE, sizeof ( uint32 ), &passkey );
    GAPBondMgr_SetParameter( GAPBOND_PAIRING_MODE, sizeof ( uint8 ), &pairMode );
    GAPBondMgr_SetParameter( GAPBOND_MITM_PROTECTION, sizeof ( uint8 ), &mitm );
    GAPBondMgr_SetParameter( GAPBOND_IO_CAPABILITIES, sizeof ( uint8 ), &ioCap );
    GAPBondMgr_SetParameter( GAPBOND_BONDING_ENABLED, sizeof ( uint8 ), &bonding );
  }

  // Initialize GATT attributes
  GGS_AddService( GATT_ALL_SERVICES );            // GAP
  GATTServApp_AddService( GATT_ALL_SERVICES );    // GATT attributes
  DevInfo_AddService();                           // Device Information Service
  Batt_AddService();                               // Battery Service  
  RoboRoachProfile_AddService( roboRoachApp_TaskID );  // Simple GATT Profile

#if defined FEATURE_OAD
 // VOID OADTarget_AddService();                    // OAD Profile
#endif

  // Setup the RoboRoach Profile Characterisitc Values
  {
    uint8 charValue1 = 55; // ROBOROACH_FREQUENCY (55 Hz)
    uint8 charValue2 = 9;  // ROBOROACH_PULSE_WIDTH (9 ms)
    uint8 charValue3 = 55; // ROBOROACH_NUM_PULSES (1 s)
    uint8 charValue4 = 0;  // ROBOROACH_RANDOM_MODE (Disabled)
    RoboRoachProfile_SetParameter( ROBOROACH_FREQUENCY, sizeof ( uint8 ), &charValue1 );
    RoboRoachProfile_SetParameter( ROBOROACH_PULSE_WIDTH, sizeof ( uint8 ), &charValue2 );
    RoboRoachProfile_SetParameter( ROBOROACH_NUM_PULSES , sizeof ( uint8 ), &charValue3 );
    RoboRoachProfile_SetParameter( ROBOROACH_RANDOM_MODE, sizeof ( uint8 ), &charValue4 );
    
    DevInfo_SetParameter(DEVINFO_MANUFACTURER_NAME, 16, "Backyard Brains");
    
  }

/* Modes */

  //[GJG] - This is configuring the port.
  P0SEL = 0; // Configure Port 0 as GPIO
  P1SEL = 0; // Configure Port 1 as GPIO
  P2SEL = 0; // Configure Port 2 as GPIO

  P0DIR = 0xFF; // Was 0xFC Port 0 pins P0.0 and P0.1 as input (buttons),
                // all others (P0.2-P0.7) as output
  P1DIR = 0xFF; // All port 1 pins (P1.0-P1.7) as output
  P2DIR = 0x1F; // All port 1 pins (P2.0-P2.4) as output

  //Turn it all off!
  P0 = 0; P1 = 0; P2 = 0;
  
  // initialize the ADC for battery reads
  HalAdcInit();
  
  // Register callback with SimpleGATTprofile
  VOID RoboRoachProfile_RegisterAppCBs( &roboRoachApp_RoboRoachProfileCBs );

  // Enable clock divide on halt
  // This reduces active current while radio is active and CC254x MCU
  // is halted
  HCI_EXT_ClkDivOnHaltCmd( HCI_EXT_ENABLE_CLK_DIVIDE_ON_HALT );

#if defined ( DC_DC_P0_7 )

  // Enable stack to toggle bypass control on TPS62730 (DC/DC converter)
  HCI_EXT_MapPmIoPortCmd( HCI_EXT_PM_IO_PORT_P0, HCI_EXT_PM_IO_PORT_PIN7 );

#endif // defined ( DC_DC_P0_7 )

  // Setup a delayed profile startup
  osal_set_event( roboRoachApp_TaskID, BYB_START_DEVICE_EVT );

}

/*********************************************************************
 * @fn      RoboRoachPeripheral_ProcessEvent
 *
 * @brief   Simple BLE Peripheral Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16 RoboRoachPeripheral_ProcessEvent( uint8 task_id, uint16 events )
{

  VOID task_id; // OSAL required parameter that isn't used in this function

  if ( events & SYS_EVENT_MSG )
  {
    uint8 *pMsg;

    if ( (pMsg = osal_msg_receive( roboRoachApp_TaskID )) != NULL )
    {
      roboRoachApp_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );

      // Release the OSAL message
      VOID osal_msg_deallocate( pMsg );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  
  if ( events & BYB_START_DEVICE_EVT )
  {
    // Start the Device
    VOID GAPRole_StartDevice( &roboRoachApp_PeripheralCBs );

    // Start Bond Manager
    VOID GAPBondMgr_Register( &roboRoachApp_BondMgrCBs );

    // Set timer for first battery read event
    osal_start_timerEx(  roboRoachApp_TaskID, BYB_BATTERY_CHECK_EVT, BYB_BATTERY_CHECK_PERIOD );

    return ( events ^ BYB_START_DEVICE_EVT );
  }
  
  if ( events & BYB_BATTERY_CHECK_EVT )
  {
    // Restart timer
    if ( BYB_BATTERY_CHECK_PERIOD )
    {
      osal_start_timerEx( roboRoachApp_TaskID, BYB_BATTERY_CHECK_EVT, BYB_BATTERY_CHECK_PERIOD );
    }

    // perform battery level check
    Batt_MeasLevel( );

    return (events ^ BYB_BATTERY_CHECK_EVT);
  }

   
  //Connection Begin Pulse (Startup) 
  if ( events & BYB_CONNECT_STARTUP_PULSE_ON_EVT )
  {
    // Blink the Connect Lights
    if ( ++connectPulseCount  < BYB_CONNECT_STARTUP_NUM_PULSES ) //
    {
      //Load up the next Start Blink
      osal_start_timerEx( roboRoachApp_TaskID, BYB_CONNECT_STARTUP_PULSE_ON_EVT, BYB_CONNECT_STARTUP_PULSE_PERIOD );
      osal_start_timerEx( roboRoachApp_TaskID, BYB_CONNECT_STARTUP_PULSE_OFF_EVT, BYB_CONNECT_STARTUP_PULSE_WIDTH );
    }
    else
    {
      //Start the standard Connect Blink (Low Power)
      osal_start_timerEx( roboRoachApp_TaskID, BYB_CONNECT_PULSE_ON_EVT, BYB_CONNECT_PULSE_PERIOD );
    } 
    
    ROBOROACH_PIO_LED_CONNECTION_1 = 1;
    ROBOROACH_PIO_LED_CONNECTION_2 = 1;
    return (events ^ BYB_CONNECT_STARTUP_PULSE_ON_EVT);
  } 

  //Connection End Pulse (Startup) 
  if ( events & BYB_CONNECT_STARTUP_PULSE_OFF_EVT )
  {
    ROBOROACH_PIO_LED_CONNECTION_1 = 0;
    ROBOROACH_PIO_LED_CONNECTION_2 = 0;
    return (events ^ BYB_CONNECT_STARTUP_PULSE_OFF_EVT);
  } 
  
  //Connection Begin Pulse (Connected) 
  if ( events & BYB_CONNECT_PULSE_ON_EVT )
  {
    if ( isConnected & ROBOROACH_HAS_CONNECTION_LEDS ) 
    {
      osal_start_timerEx( roboRoachApp_TaskID, BYB_CONNECT_PULSE_ON_EVT, BYB_CONNECT_PULSE_PERIOD );
    }     
    osal_start_timerEx( roboRoachApp_TaskID, BYB_CONNECT_PULSE_OFF_EVT, BYB_CONNECT_PULSE_WIDTH );

    ROBOROACH_PIO_LED_CONNECTION_1 = 1;
    ROBOROACH_PIO_LED_CONNECTION_2 = 1;
    return (events ^ BYB_CONNECT_PULSE_ON_EVT);
  } 

  if ( events & BYB_CONNECT_PULSE_OFF_EVT )
  {
    ROBOROACH_PIO_LED_CONNECTION_1 = 0;
    ROBOROACH_PIO_LED_CONNECTION_2 = 0;
    return (events ^ BYB_CONNECT_PULSE_OFF_EVT);
  } 
  
  if ( events & BYB_STIMULATE_LEFT_EVT )
  {
    //Don't allow multiple stimulations
    if (stimulationInProgress){
       return (events ^ BYB_STIMULATE_LEFT_EVT);
    }
    stimulationIsLeft = 1;
    startRoboRoachStimulation();
    
    return (events ^ BYB_STIMULATE_LEFT_EVT);
  }

  if ( events & BYB_STIMULATE_RIGHT_EVT )
  {
    //Don't allow multiple stimulations
    if (stimulationInProgress){
       return (events ^  BYB_STIMULATE_RIGHT_EVT);
    }
    stimulationIsLeft = 0;
    startRoboRoachStimulation();
    
    return (events ^ BYB_STIMULATE_RIGHT_EVT);
  }

  //Handle the Stimulation Pulses
  if ( events & BYB_STIMULATE_PULSE_ON_EVT )
  {
     if (stimulationRandomMode) {
        stimulationPeriod = osal_rand() % (BYB_MAX_STIMULATE_PERIOD - BYB_MIN_STIMULATE_PERIOD) + BYB_MIN_STIMULATE_PERIOD;
        stimulationPulseWidth = osal_rand() % (BYB_MAX_STIMULATE_PULSE_WIDTH - BYB_MIN_STIMULATE_PULSE_WIDTH) + BYB_MIN_STIMULATE_PULSE_WIDTH;
     }
     
    // Restart timer
    if ( ++stimulationPulseCount  < stimulationNumberOfPulses ) //
    {
      osal_start_timerEx( roboRoachApp_TaskID, BYB_STIMULATE_PULSE_ON_EVT, stimulationPeriod );
    } else 
    { 
      //We are done stimulating.  Shut it down.
      osal_start_timerEx( roboRoachApp_TaskID, BYB_STIMULATE_FINISHED_EVT, stimulationPulseWidth + 1 );
    }
    osal_start_timerEx( roboRoachApp_TaskID, BYB_STIMULATE_PULSE_OFF_EVT, stimulationPulseWidth );

    if (stimulationIsLeft) {
      ROBOROACH_PIO_ANTENNA_LEFT = 1;
    } else {
      ROBOROACH_PIO_ANTENNA_RIGHT = 1;
    }  
    return (events ^ BYB_STIMULATE_PULSE_ON_EVT);
  } 

  if ( events & BYB_STIMULATE_PULSE_OFF_EVT )
  {
    if (stimulationIsLeft) {
      ROBOROACH_PIO_ANTENNA_LEFT = 0;
    } else {
      ROBOROACH_PIO_ANTENNA_RIGHT = 0;
    }  
    return (events ^ BYB_STIMULATE_PULSE_OFF_EVT);
  } 
  
  if (  events & BYB_STIMULATE_FINISHED_EVT)
  {
    stimulationInProgress = 0;
    #ifndef ROBOROACH_V10B
      stimulationIsLeft ? (ROBOROACH_PIO_LED_LEFT = 0) : (ROBOROACH_PIO_LED_RIGHT = 0);
    #endif
    return (events ^ BYB_STIMULATE_FINISHED_EVT);
  } 
    
  // Discard unknown events
  return 0;
}

void startRoboRoachStimulation(){
  
  stimulationInProgress = 1;
  stimulationPulseCount = 0;   

  RoboRoachProfile_GetParameter( ROBOROACH_STIM_PERIOD, &stimulationPeriod);
  RoboRoachProfile_GetParameter( ROBOROACH_NUM_PULSES,  &stimulationNumberOfPulses);
  RoboRoachProfile_GetParameter( ROBOROACH_PULSE_WIDTH, &stimulationPulseWidth);
  RoboRoachProfile_GetParameter( ROBOROACH_RANDOM_MODE, &stimulationRandomMode);

  //Start Immediately (1ms)
  osal_start_timerEx( roboRoachApp_TaskID, BYB_STIMULATE_PULSE_ON_EVT, 1 );
  #ifndef ROBOROACH_V10B
     stimulationIsLeft ? (ROBOROACH_PIO_LED_LEFT = 1) : (ROBOROACH_PIO_LED_RIGHT = 1);
  #endif
}



/*********************************************************************
 * @fn      roboRoachApp_ProcessOSALMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void roboRoachApp_ProcessOSALMsg( osal_event_hdr_t *pMsg )
{
  switch ( pMsg->event )
  {
  #if defined( CC2540_MINIDK )
    case KEY_CHANGE:
      roboRoachApp_HandleKeys( ((keyChange_t *)pMsg)->state, ((keyChange_t *)pMsg)->keys );
      break;
  #endif // #if defined( CC2540_MINIDK )

  default:
    // do nothing
    break;
  }
}


/*********************************************************************
 * @fn      peripheralStateNotificationCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void peripheralStateNotificationCB( gaprole_States_t newState )
{
  switch ( newState )
  {
    case GAPROLE_STARTED:
      {
        uint8 ownAddress[B_ADDR_LEN];
        uint8 systemId[DEVINFO_SYSTEM_ID_LEN];

        GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);

        // use 6 bytes of device address for 8 bytes of system ID value
        systemId[0] = ownAddress[0];
        systemId[1] = ownAddress[1];
        systemId[2] = ownAddress[2];

        // set middle bytes to zero
        systemId[4] = 0x00;
        systemId[3] = 0x00;

        // shift three bytes up
        systemId[7] = ownAddress[5];
        systemId[6] = ownAddress[4];
        systemId[5] = ownAddress[3];

        DevInfo_SetParameter(DEVINFO_SYSTEM_ID, DEVINFO_SYSTEM_ID_LEN, systemId);

      }
      break;

    case GAPROLE_ADVERTISING:
      {
        #if (defined HAL_LCD) && (HAL_LCD == TRUE)
          HalLcdWriteString( "Advertising",  HAL_LCD_LINE_3 );
        #endif // (defined HAL_LCD) && (HAL_LCD == TRUE)
          
        //Turn off everything.  
        P0 = 0; P1 = 0; P2 = 0;  
      }
      break;

    case GAPROLE_CONNECTED:
      {
        #if (defined HAL_LCD) && (HAL_LCD == TRUE)
          HalLcdWriteString( "Connected",  HAL_LCD_LINE_3 );
        #endif // (defined HAL_LCD) && (HAL_LCD == TRUE)
        
        connectPulseCount = 0;
        isConnected = TRUE;
        osal_start_timerEx( roboRoachApp_TaskID, BYB_CONNECT_STARTUP_PULSE_ON_EVT, 1 );
        
      }
      break;
      
    case GAPROLE_WAITING:
      {
        #if (defined HAL_LCD) && (HAL_LCD == TRUE)
          HalLcdWriteString( "Disconnected",  HAL_LCD_LINE_3 );
        #endif // (defined HAL_LCD) && (HAL_LCD == TRUE)
        isConnected = FALSE;
      }
      break;

    case GAPROLE_WAITING_AFTER_TIMEOUT:
      {
        #if (defined HAL_LCD) && (HAL_LCD == TRUE)
          HalLcdWriteString( "Timed Out",  HAL_LCD_LINE_3 );
        #endif // (defined HAL_LCD) && (HAL_LCD == TRUE)
        isConnected = FALSE;
      }
      break;

    case GAPROLE_ERROR:
      {
        #if (defined HAL_LCD) && (HAL_LCD == TRUE)
          HalLcdWriteString( "Error",  HAL_LCD_LINE_3 );
        #endif // (defined HAL_LCD) && (HAL_LCD == TRUE)
      }
      break;

    default:
      {
        #if (defined HAL_LCD) && (HAL_LCD == TRUE)
          HalLcdWriteString( "",  HAL_LCD_LINE_3 );
        #endif // (defined HAL_LCD) && (HAL_LCD == TRUE)
      }
      break;

  }

  gapProfileState = newState;

#if !defined( CC2540_MINIDK )
  VOID gapProfileState;     // added to prevent compiler warning with
                            // "CC2540 Slave" configurations
#endif


}

/*********************************************************************
 * @fn      roboRoachProfileChangeCB
 *
 * @brief   Callback from RoboRoachProfile indicating a value change
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  none
 */
static void roboRoachProfileChangeCB( uint8 paramID )
{
  uint8 newValue;

  switch( paramID )
  {
    case  ROBOROACH_FREQUENCY:
      RoboRoachProfile_GetParameter(  ROBOROACH_FREQUENCY, &newValue );

      #if (defined HAL_LCD) && (HAL_LCD == TRUE)
        HalLcdWriteStringValue( "Frequency:", (uint16)(newValue), 10,  HAL_LCD_LINE_3 );
      #endif // (defined HAL_LCD) && (HAL_LCD == TRUE)

      break;

    case ROBOROACH_PULSE_WIDTH:
      RoboRoachProfile_GetParameter( ROBOROACH_PULSE_WIDTH, &newValue );

      #if (defined HAL_LCD) && (HAL_LCD == TRUE)
        HalLcdWriteStringValue( "Pulse Width:", (uint16)(newValue), 10,  HAL_LCD_LINE_3 );
      #endif // (defined HAL_LCD) && (HAL_LCD == TRUE)

      break;

    default:
      // should not reach here!
      break;
  }
}

/*********************************************************************
*********************************************************************/
