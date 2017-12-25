/* ========================================
 *
 * Copyright BACKYARD BRAINS, 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF BACKYARD BRAINS.
 *
 * ========================================
*/
#include "project.h"
#include "StimulusGenerator.h"


//connection variables
#define LED_CONNECTION_PULSE_LENGTH 100
#define LED_CONNECTION_PERIOD 1000
int connectionStatus = 0;
int initial; 
int ledConnectionCounter = 0;
uint8 batteryLevel = 70;
int16 batteryMeasurement = 0;
int batteryTimer = 0;
#define BATTERY_TIMER_PERIOD 3000
int sendBatteryLevel = 1;
CYBLE_API_RESULT_T apiResult;

CYBLE_CONN_HANDLE_T connectionHandle;

//length of blink during unconnected indicator
const int STATUS_BLINK_WIDTH = 100;

//stimulus generator struct
struct StimulusGenerator generator;

//sleep timer variables
const int SLEEP_TIMER_READY = 30;//sec
const int SLEEP_TIMER_ACTIVE = 180;//sec

int sleepCountdown;

uint8 gotoSleep;


uint32 stimPeriodCounter = 0;
uint32 stimLengthCounter = 0;
uint32 stimPeriodMax = 0;
int direction = Left;
int stimulationActive = 0; 
void startStimulus(enum Direction dir)
{
    stimulationActive = 0;
    if(generator.randomMode!=0)
    {
        StimulusGenerator_Randomize(&generator);
    }
    stimPeriodMax = 1000/generator.pulseFrequency;
    stimPeriodCounter = 0;
    
    Left_Write(0);
    Right_Write(0);
    LED_R_Write(0);
    LED_L_Write(0);
    stimLengthCounter = 0;
    direction = dir;
    if(dir==Left)
    {
        LED_L_Write(1);
    }
    else
    {
        LED_R_Write(1);
    }
    stimulationActive = 1;
}

//bluetooth communications handler
void StackHandler(uint32 eventCode, void* eventParam) {
    
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReq;
    
    switch(eventCode) {
        case CYBLE_EVT_STACK_ON: 
        
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
        
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
             
            //set connection bool to false
            connectionStatus = 0;
            LED_Conn_Write(0);
            
            //start sleep countdown
            sleepCountdown = SLEEP_TIMER_READY;
            
            break;
            
        case CYBLE_EVT_GATT_CONNECT_IND:
            
            connectionHandle = *(CYBLE_CONN_HANDLE_T*)eventParam;
            
            //set connection bool to true
            connectionStatus = 1;
            LED_Conn_Write(1);
            //turn off left and right LEDs
            //LeftLED_PWM_WriteCompare(0);
            
            //RightLED_PWM_WriteCompare(0);
            
            
            //turn on green light to indicate connection
            //DurationTimer_WriteCompare(1000);
           
            //LED_L_Write(0); 
            
            //start sleep countdown
            sleepCountdown = SLEEP_TIMER_ACTIVE;
            
            break;
            
        case CYBLE_EVT_GATTS_WRITE_REQ:
            
            wrReq = (CYBLE_GATTS_WRITE_REQ_PARAM_T*)eventParam;
            
            //handle left stimulus input, simply sets true whevenever the property is written to
            if (wrReq->handleValPair.attrHandle == CYBLE_ROBOROACH_STIMLEFT_CHAR_HANDLE) {
                
                CyBle_GattsWriteAttributeValue(&wrReq->handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                startStimulus(Left);
               
            }
            //handle right stimulus input
            if (wrReq->handleValPair.attrHandle == CYBLE_ROBOROACH_STIMRIGHT_CHAR_HANDLE) {
                
                CyBle_GattsWriteAttributeValue(&wrReq->handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                
                startStimulus(Right);
                
            }
            //handle duration input
            if (wrReq->handleValPair.attrHandle == CYBLE_ROBOROACH_DURATION_CHAR_HANDLE) {
                
                CyBle_GattsWriteAttributeValue(&wrReq->handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                
                StimulusGenerator_SetPulseDuration( &generator, wrReq->handleValPair.value.val[0]);
                
            }
            //handle frequency input
            if (wrReq->handleValPair.attrHandle == CYBLE_ROBOROACH_STIMFREQ_CHAR_HANDLE) {
                
                CyBle_GattsWriteAttributeValue(&wrReq->handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                
                StimulusGenerator_SetFrequency(&generator, wrReq->handleValPair.value.val[0]);
                
            }
            //handle pulse width input
            if (wrReq->handleValPair.attrHandle == CYBLE_ROBOROACH_STIMPULSE_CHAR_HANDLE) {
                
                CyBle_GattsWriteAttributeValue(&wrReq->handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                
                StimulusGenerator_SetPulseWidth(&generator, wrReq->handleValPair.value.val[0]); 
                
            }
            //handle gain input
            if (wrReq->handleValPair.attrHandle == CYBLE_ROBOROACH_GAIN_CHAR_HANDLE) {
                
                CyBle_GattsWriteAttributeValue(&wrReq->handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                
                StimulusGenerator_SetPulseGain(&generator, wrReq->handleValPair.value.val[0]); 
                
            }
            //handle random mode input
            if (wrReq->handleValPair.attrHandle == CYBLE_ROBOROACH_RANDOMMODE_CHAR_HANDLE) {
                
                CyBle_GattsWriteAttributeValue(&wrReq->handleValPair, 0, &connectionHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
                
                StimulusGenerator_SetRandomMode(&generator, wrReq->handleValPair.value.val[0]); 
                
            }
            
            CyBle_GattsWriteRsp(connectionHandle);
            sleepCountdown = SLEEP_TIMER_ACTIVE;
            
            break;
            
        default:
            
            break;
    }
}

void SleepComponents() {
    SPIM_Stop();
    DurationTimer_Stop();
    CyBle_Stop();
}

void BasCallBack(uint32 event, void *eventParam)
{
    uint8 locServiceIndex;
    locServiceIndex = ((CYBLE_BAS_CHAR_VALUE_T *)eventParam)->serviceIndex;
    
    switch(event)
    {
        case CYBLE_EVT_BASS_NOTIFICATION_ENABLED:
            break;
        case CYBLE_EVT_BASS_NOTIFICATION_DISABLED:
            break;
        case CYBLE_EVT_BASC_NOTIFICATION:
            break;
        case CYBLE_EVT_BASC_READ_CHAR_RESPONSE:
            break;
        case CYBLE_EVT_BASC_READ_DESCR_RESPONSE:
            break;
        case CYBLE_EVT_BASC_WRITE_DESCR_RESPONSE:
            break;
		default:
			break;
    }
}


//functions to put all components to sleep and wake them up
CY_ISR(Sleep_interrupt) {   
   //clear sleep interrupts
   CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);

   Sleep_ClearPending();

   //decrement sleep countdown, ~1 time per second
   sleepCountdown--;

   if(sleepCountdown == 0) {
        //set sleep bool
        gotoSleep = 1;
   }  
}
  int test = 0;
CY_ISR(mainTimerInterruptHandler)
{
    DurationTimer_WriteCounter(0);
    DurationTimer_ClearInterrupt(DurationTimer_INTR_MASK_CC_MATCH);


    //if we are not connected blink connection LED
    if(connectionStatus==0)
    {
        ledConnectionCounter++;
        if(ledConnectionCounter==LED_CONNECTION_PERIOD)
        {
            ledConnectionCounter = 0;//rewind period counter
        }
        if(ledConnectionCounter<LED_CONNECTION_PULSE_LENGTH)
        {
            LED_Conn_Write(!LED_Conn_Read()); 
        }
        else
        {
            LED_Conn_Write(0);
        }
    }
    else
    {
        LED_Conn_Write(!LED_Conn_Read()); 
    }
    
    //If stimulation is active generate PWM
    if(stimulationActive==1)
    {
        stimLengthCounter++;
        if(stimLengthCounter>generator.pulseDuration)//end of stimulation
        {
            //turn off all outputs used for stimulation
            stimulationActive = 0;
            Left_Write(0);
            Right_Write(0);
            LED_R_Write(0);
            LED_L_Write(0);
        }
        else
        {
            stimPeriodCounter++;
            if(stimPeriodCounter<=generator.pulseWidth)
            {
            //Inside ON part of period
                if(direction==Left)
                {
                    Left_Write(1);
                    LED_L_Write(!LED_L_Read());
                }
                else
                {
                    Right_Write(1);
                    LED_R_Write(!LED_R_Read());
                }
                
            }
            else
            {
            //Inside OFF part of period
                if(direction==Left)
                {
                    Left_Write(0);
                    LED_L_Write(!LED_L_Read());
                }
                else
                {
                    Right_Write(0);
                    LED_R_Write(!LED_R_Read());
                }
                if(stimPeriodCounter>stimPeriodMax)//end of one period 
                {
                    stimPeriodCounter =0;
                    if(generator.randomMode!=0)
                    {
                        StimulusGenerator_Randomize(&generator);
                        stimPeriodMax = 1000/generator.pulseFrequency;
                    }
                }
            }
        }
    
    }

    batteryTimer++;
    if(batteryTimer>=BATTERY_TIMER_PERIOD)
    {
        batteryTimer = 0;
        sendBatteryLevel = 1;
    }
}
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
   
    //initialize sleep/wake interrupts
    Sleep_StartEx(Sleep_interrupt);
    gotoSleep = 0;
    
    //initalize BLE
    CyBle_Start(StackHandler);
    CyBle_BasRegisterAttrCallback(BasCallBack);
    
    //initialize comms w DigiPot
    LED_R_Write(0);
    LED_L_Write(0); 
    
    POT_SHDN_Write(0);
    POT_SHDN_1_Write(0);

    //start RTC
    Clock_Start();
    DurationTimer_WriteCounter(0);
    DurationTimer_Start();
    mainTimerInterrupt_StartEx(mainTimerInterruptHandler); 
    
    ADCForBattery_Start();
    
    
    //initalize parameters to defaults
    for(;;)
    {
        //process bluetooth communications
        CyBle_ProcessEvents();
        if (connectionStatus == 0){
            initial = 1; 
            POT_SHDN_Write(0);
            POT_SHDN_1_Write(0);
            CyBle_ProcessEvents();
            CyDelay(250); 
            CyBle_ProcessEvents();
        }
        else if (initial == 1){
            POT_SHDN_Write(1);
            POT_SHDN_1_Write(1);
            SPIM_Start(); 
            StimulusGenerator_Initialize(&generator);
            initial = 0;  
        }
      
        if(sendBatteryLevel==1)
        {
            ADCForBattery_StartConvert();
            ADCForBattery_IsEndConversion(ADCForBattery_WAIT_FOR_RESULT);
            batteryMeasurement = ADCForBattery_GetResult16(0x00u);
            batteryLevel = batteryMeasurement/20;
            if(batteryLevel>100)
            {
                batteryLevel = 100;
            }
           
            sendBatteryLevel = 0;        
            apiResult = CyBle_BassSendNotification(connectionHandle, CYBLE_BATTERY_SERVICE_INDEX, 
                CYBLE_BAS_BATTERY_LEVEL, sizeof(batteryLevel), &batteryLevel);
            
            if(apiResult == CYBLE_ERROR_OK) 
            {
                //The request handled successfully
                sendBatteryLevel = 0; 
            }
            else if(apiResult == CYBLE_ERROR_INVALID_PARAMETER)
            {
                //Validation of the input parameter failed 
                sendBatteryLevel = 0; 
            }else if(apiResult == CYBLE_ERROR_INVALID_OPERATION)
            {
                //This operation is not permitted
                sendBatteryLevel = 0; 
            }else if(apiResult == CYBLE_ERROR_INVALID_STATE)
            {
                //Connection with the client is not established
                sendBatteryLevel = 0; 
            }else if(apiResult == CYBLE_ERROR_MEMORY_ALLOCATION_FAILED)
            {
                // Memory allocation failed.
                sendBatteryLevel = 0; 
            }else if(apiResult == CYBLE_ERROR_NTF_DISABLED)
            {
                //Notification is not enabled by the client.
                sendBatteryLevel = 0; 
            }
                 
           /*batteryLevel = batteryLevel+10;
           if(batteryLevel>100)
           {
                batteryLevel = 0;
           }*/
        }
        //go to sleep if idle
        if(gotoSleep) {         
            POT_SHDN_Write(0);
            POT_SHDN_1_Write(0);
            DurationTimer_Stop();
            Clock_Stop();
            LED_Conn_Write(0);
            CS_Write(0);
            SleepComponents();
            CySysPmFreezeIo();
            CySysPmHibernate(); 
        }
    }
}

/* [] END OF FILE */
