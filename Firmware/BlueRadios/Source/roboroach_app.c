/***************************************************************************************************
* roboroach_app.c
* 
* Description: Bluetooth RoboRoach Application
* 
* Project: Backyard Brains RoboRoach
* 
* Copyright (c) 2013 Backyard Brains, Inc.  
*
* This file is part of roboroach_app.h:
*
* The BYB Roboroach App is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* BYB Roboroach App is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with BYB Roboroach App.  If not, see <http://www.gnu.org/licenses/>.
* 
* The following liscence may proceed ours, and is from the original code we used in 
* the BlueRadio SDK.
* 
* BUGS:
*    ATSBAS Does not Work.
*    LED Blink Not calculated
*    Response to Stimulation LEFT/RIGHT Write    
*    LED Blink Needs to Turn Off.
*    
*
* ---------------------------------------------------------------------------
* 
* Copyright (c) 2009-2012 BlueRadios, Inc. All Rights Reserved. 
* 
* IMPORTANT: Permission is granted to modify and use on BlueRadios hardware only. Other than for 
* the foregoing purpose, you may not use, reproduce, copy, prepare derivative works of, modify, 
* distribute, perform, display or sell this Software and/or its documentation for any purpose
* without written permission from BlueRadios. 
* 
* DISCLAIMER: YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED 
* “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, 
* ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. 
* IN NO EVENT SHALL BLUERADIOS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, 
* STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT 
* OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, 
* PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE 
* GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY 
* DEFENSE THEREOF), OR OTHER SIMILAR COSTS. 
* 
* This notice must stay with all deviations to this source. 
***************************************************************************************************/

/***************************************************************************************************
 * INCLUDES
 */
#include "osal.h"
#include "osal_pwrmgr.h"
#include "osal_timers.h"

#include "hci.h"

#include "hal_pio.h"
#include "hal_uart.h"

#include "at_lib.h"
#include "roboroach_app.h"
#include "roboroach_devinfoservice.h"
#include "roboroach_profile.h"

#define BUILD_TIME __TIME__
#define BUILD_DATE __DATE__

/***************************************************************************************************
 * MACROS
 */

/***************************************************************************************************
 * CONSTANTS
 */

// App OSAL Events (0x0001 - 0x0800 allowed / 0x1000, 0x2000, 0x4000, 0x8000 are reserved by AT.e)
#define CONNECT_OSAL_EVT	0x0001


/***************************************************************************************************
 * TYPEDEFS
 */

/***************************************************************************************************
 * GLOBAL VARIABLES
 */

/***************************************************************************************************
 * EXTERNAL VARIABLES
 */

/***************************************************************************************************
 * EXTERNAL FUNCTIONS
 */

/***************************************************************************************************
 * LOCAL VARIABLES
 */

/***************************************************************************************************
 * LOCAL FUNCTIONS
 */

/** 
 * Main - Do Not Modify
 *  
 * @return int
 */
int main( void )
{
  ATApp_Main();

  return 0;
}

/***************************************************************************************************
 * PUBLIC FUNCTIONS
 */

/** 
 * Initialization function for the AT.e application. This should contain any application specific 
 * initialization (ie. hardware initialization/setup, GATT services, power up notificaiton ...). 
 *  
 * All ATS command settings will be default to the factory reset values listed in the command 
 * documentation, so settings only need to be applied here if a non default value is needed.  The 
 * defaults match the factory defaults of the AT.s command set with the exception of ATSBRSP and 
 * ATSBAS, which are disabled by default and must be added in this function if needed. 
 *  
 * Any services other than BAS and BRSP can be added using the ServiceName_AddService command 
 * defined in the service header file.  Services can be found in the Profiles directory. 
 * 
 */
void ATApp_Init( void )
{
  AT_DBG( "ATApp_Init" );

  // enable an interrupt on PIO_6 - all pio interrupts are rising edge
  // [GjG] What is this for?
  HalPioSetInt( HAL_PIO_6, HAL_PIO_INT_ENABLE );
  
  // enable the LED as outputs and off. 
  AT_SetPio( ROBOROACH_PIO_LED_LEFT, ATSPIO_DIR_OUT, ATSPIO_OUT_LEVEL_LOW );
  AT_SetPio( ROBOROACH_PIO_LED_RIGHT, ATSPIO_DIR_OUT, ATSPIO_OUT_LEVEL_LOW );
   
  // Turn off antenna stimulation (High Current) outputs 
  AT_SetLed( ATSLED_LED_PIO2, 0, 100 ); // Left Antenna
  AT_SetLed( ATSLED_LED_PIO5, 0, 100 ); // Right Antenna

  // 16-bit Services - must all be added prior to any 128-bit services
  //Add Battery Service
  ATSBAS( ATSBAS_ENABLE, ATSBAS_MODE_AUTO, 60000 ); //Does not work.
  RoboRoach_AddService( ROBOROACH_SERVICE );
  RoboRoach_DevInfo_AddService();
  
  // 128-bit Services
  //Disable BRSP Serial Service
  ATSBRSP( ATSBRSP_DISABLE, ATSBRSP_MODE_DATA, ATSBRSP_SEC_MODE_NONE );
    
  // Set name
  ATSN(  "RoboRoach" , ATSN_GATT_NOT_WRITEABLE );
  
#ifdef DEBUG
  /* Keeps the module from going to sleep */
  osal_pwrmgr_task_state( AT_TaskId(), PWRMGR_HOLD );
#else
  /* Reduces the CPU clock down to 2MHz while the radio is active to bring current consumption down,
  but can cause problems if using certain on chip peripherals (e.g. timers, UART), so it is not 
  recommended for every application */
  HCI_EXT_ClkDivOnHaltCmd( HCI_EXT_ENABLE_CLK_DIVIDE_ON_HALT ); 
#endif
}   

/** 
 * Handles pio interrupts.
 * 
 * @param pioIntMask - mask of pio interrupts (HAL_PIO_#_INT) that have been triggered, this is a 
 *                   bit map and can contain more than one event
 */
void ATApp_PioIntCB( uint32 pioIntMask )

{
  if ( pioIntMask & HAL_PIO_6_INT )
  {
    AT_DBG( "ATApp_PioIntCB: HAL_PIO_6_INT" );

    // Set the CONNECT_OSAL_EVT to occur in 1 second
    osal_start_timerEx( AT_TaskId(), CONNECT_OSAL_EVT, 1000 ); 
  }
}

/** 
 * Handles incoming uart data.
 * 
 * @param port - UART port: HAL_UART_PORT_HW or HAL_UART_PORT_BRSP (hal_uart.h)
 * @param bytesAvailable - number of bytes available to be read with HalUARTRead / HalUARTTransfer 
 *                       (hal_uart.h)
 */
void ATApp_UartDataCB( uint8 port, uint8 bytesAvailable )
{
  //if ( port == HAL_UART_PORT_BRSP )
  //{
  //  // Loopback all received data
  //  HalUARTTransfer( port, port, bytesAvailable );
  //}
}

/** 
 * App osal event processor.  This function is called to process osal events for the app. Events can
 * be set using osal_set_event (osal.h) or osal_start_timerEx / osal_start_reload_timer 
 * (osal_timers.h). 
 * 
 * @param events - events to process.  This is a bit map and can contain more than one event 
 */
void ATApp_OsalEventCB( uint16 events )
{
  if ( events & CONNECT_OSAL_EVT )
  {
    AT_DBG( "ATApp_OsalEventCB: CONNECT_OSAL_EVT" );
   
  }
  
  if ( events & TURN_OFF_STIM_EVT )
  {
    AT_DBG( "ATApp_OsalEventCB: TURN_OFF_STIM_EVT" );

    //Turn off all Antenna and LED ports.
    AT_SetLed( ATSLED_LED_PIO2, 0, 1000 ); //Antenna
    AT_SetLed( ATSLED_LED_PIO5, 0, 1000 ); 
    AT_SetPio( ROBOROACH_PIO_LED_LEFT, ATSPIO_DIR_OUT, ATSPIO_OUT_LEVEL_LOW ); //Indicators
    AT_SetPio( ROBOROACH_PIO_LED_RIGHT, ATSPIO_DIR_OUT, ATSPIO_OUT_LEVEL_LOW );
   
  }
    
}

/** 
 * Handles AT events.
 * 
 * @param pEvent - pointer to the event (at_lib.h)
 */
void ATApp_AtEventCB( atEvent_t *pEvent )
{
  switch ( pEvent->eventType )
  {
    case AT_EVENT_INIT_DONE:   
      {
        AT_DBG( "AT_EVENT_INIT_DONE: addr=%s", 
                AT_AddrToStr(pEvent->initDone.addr) );
      }
      break;

    case AT_EVENT_DONE:
      { 
        AT_DBG( "AT_EVENT_DONE: cT=%u, cC=%u", 
                pEvent->done.commandType, pEvent->done.completedCommand );
      }
      break;

    case AT_EVENT_DISCOVERY:
      {
        AT_DBG( "AT_EVENT_DISCOVERY: dT=%u, addr=%s, aT=%u, rssi=%d, dL=%u", 
                pEvent->discovery.discoveryType, AT_AddrToStr(pEvent->initDone.addr), 
                pEvent->discovery.addrType, pEvent->discovery.rssi, pEvent->discovery.dataLen );
      }
      break;

    case AT_EVENT_CONNECT:
      {
        AT_DBG( "AT_EVENT_CONNECT: cH=%u, addr=%s, aT=%u, cI=%u, cL=%u, cT=%u, pS=%u", 
                pEvent->connect.connHandle, AT_AddrToStr(pEvent->connect.addr), 
                pEvent->connect.addrType, pEvent->connect.connInterval, 
                pEvent->connect.connLatency, pEvent->connect.connTimeout, 
                pEvent->connect.pairState );
        
        AT_SetPio( ROBOROACH_PIO_LED_CONNECTION, ATSPIO_DIR_OUT, ATSPIO_OUT_LEVEL_HIGH );
        AT_SetLed( ATSLED_LED_PIO2, 0, 1000 ); // Left Antenna
        AT_SetLed( ATSLED_LED_PIO5, 0, 1000 ); // Right Antenna

      }
      break;

    case AT_EVENT_DISCONNECT:
      {
        AT_DBG( "AT_EVENT_DISCONNECT: cH=%u, r=%u", 
                pEvent->disconnect.connHandle, pEvent->disconnect.reason );

        AT_SetPio( ROBOROACH_PIO_LED_CONNECTION, ATSPIO_DIR_OUT, ATSPIO_OUT_LEVEL_LOW );
        AT_SetLed( ATSLED_LED_PIO2, 0, 1000 ); // Left Antenna
        AT_SetLed( ATSLED_LED_PIO5, 0, 1000 ); // Right Antenna

      }
      break;

    case AT_EVENT_CPU_STATUS:
      {
        AT_DBG( "AT_EVENT_CPU_STATUS: ch=%u, s=%u", 
                pEvent->cpuStatus.connHandle, pEvent->cpuStatus.status );
      }
      break;

    case AT_EVENT_CPU:
      {
        AT_DBG( "AT_EVENT_CPU: s=%u, cH=%u, cI=%u, cL=%u, cT=%u", 
                pEvent->cpu.connHandle, pEvent->cpu.connInterval, 
                pEvent->cpu.connLatency, pEvent->cpu.connTimeout );
      }
      break;

    case AT_EVENT_RSSI:
      {
        AT_DBG( "AT_EVENT_RSSI: cH=%u, v=%d", 
                pEvent->rssi.connHandle, pEvent->rssi.value );
      }
      break;

    case AT_EVENT_PAIR_REQ:
      {
        AT_DBG( "AT_EVENT_PAIR_REQ: cH=%u, addr=%s", 
                pEvent->pairReq.connHandle, AT_AddrToStr(pEvent->pairReq.addr) );
      }
      break;

    case AT_EVENT_PAIRED:
      {
        AT_DBG( "AT_EVENT_PAIRED: cH=%u, addr=%s, pS=%u", 
                pEvent->paired.connHandle, AT_AddrToStr(pEvent->paired.addr), pEvent->paired.pairState );
      }
      break;

    case AT_EVENT_PAIR_FAIL:
      {
        AT_DBG( "AT_EVENT_PAIR_FAIL: cH=%u, addr=%s, r=%u", 
                pEvent->pairFail.connHandle, AT_AddrToStr(pEvent->pairFail.addr), pEvent->pairFail.reason );
      }
      break;

    case AT_EVENT_PK_REQ:
      {
        AT_DBG( "AT_EVENT_PK_REQ: cH=%u, addr=%s", 
                pEvent->pkReq.connHandle, AT_AddrToStr(pEvent->pkReq.addr) );
      }
      break;

    case AT_EVENT_PK_DIS:
      {
        AT_DBG( "AT_EVENT_PK_DIS: cH=%u, addr=%s, pk=%u", 
                pEvent->pkReq.connHandle, AT_AddrToStr(pEvent->pkDis.addr), pEvent->pkDis.passkey );
      }
      break;

    case AT_EVENT_BRSP:
      {
        AT_DBG( "AT_EVENT_BRSP: cH=%u, s=%u", 
                pEvent->brsp.connHandle, pEvent->brsp.status );
      }
      break;

    default:
      {
      }
      break;
  }
}

/***************************************************************************************************
***************************************************************************************************/
