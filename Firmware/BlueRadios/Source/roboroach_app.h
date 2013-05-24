/***************************************************************************************************
* roboroach_app.h
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

#ifndef AT_APP_H
#define AT_APP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "at_lib.h"

/*********************************************************************
 * CONSTANTS
 */

void ATApp_Main(void);

void ATApp_Init(void);

void ATApp_PioIntCB( uint32 pioIntMask );

void ATApp_UartDataCB(uint8 port, uint8 event);

void ATApp_OsalEventCB( uint16 events );

void ATApp_AtEventCB( atEvent_t *pEvent );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* AT_APP_H */
