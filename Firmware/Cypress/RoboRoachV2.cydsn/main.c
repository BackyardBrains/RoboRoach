/* ========================================
 *
 * Copyright BACKYARD BRAINS, 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF BACKYARD BRAINS.
 *
 * Theory of operation:
 *
 * Serving BT and batery measurements is done in main() function in infinite loop for(;;). 
 * Code periodicaly checks if there are any new BT packages to serve
 *
 * Stimulation, PWM for LEDs and general time measurement is done in mainTimerInterruptHandler
 * Main timer handler is called every 100uS which gives us enough time resolution to do PWM 
 * for LEDs and stimulation.
 *
 * Updated by Stanislav Mircic Jan. 2018
 * ========================================
*/
#include "project.h"
#include "StimulusGenerator.h"

//Timer period is 100uS
#define ONE_SECOND_IN_TIMER_PULSES 10000            //how many periods of main timer is one second

#define LED_CONNECTION_PULSE_LENGTH 1000            //Pulse length of connection LED expressed in timer periods
#define LED_CONNECTION_PERIOD 10000                 //period of pulse of connection LED in timer periods
int ledConnectionCounter = 0;                       //control counter used to pulse connection LED


//we use PWM for LEDs to lower power consumption
#define LED_PWM_PERIOD 20                           //PWM period for all LEDs expressed in timer periods
#define LED_PWM_ON_INTERVAL 5                       //ON interval expressed in timer periods
uint8 ledPWMCounter = 0;                            //control counter for PWM of LEDs
uint8 ledPWMSignal = 0;                             //"logic" variable that is equal to PWM signal for LEDs

int connectionStatus = 0;                           //1- connected; 0- not connected to BT
int initial;                                        //"logic" variable that flags if we already finished 
                                                    //initialization after connecting to BT

uint8 batteryLevel = 70;                            //battery level expressed in %
int16 batteryMeasurement = 0;                       //battery level in AD units
int batteryTimer = 0;                               //control counter used for periodic battery measurements
#define BATTERY_TIMER_PERIOD 2666                     //period of battery measurements expressed in seconds
int sendBatteryLevel = 1;                           //flag that signals to main loop to send battery level

CYBLE_API_RESULT_T apiResult;                       //Variable holds result of BT notification operation
CYBLE_CONN_HANDLE_T connectionHandle;               //Handle for BT connection

struct StimulusGenerator generator;                 //Stimulus generator struct. Holds all stimmulation parameters

const int SLEEP_TIMER_READY = 450;                   //Sleep timer variables sec
const int SLEEP_TIMER_ACTIVE = 19700;                 //Sleep timer variables sec
int sleepCountdown;                                 //Sleep countdown counter, decrement each Sleep_interrupt handler call
uint8 gotoSleep;                                    //flag that signals main loop that it should put micro to sleep

                       
uint32 stimDurationMax = 0;                         //variable holds how length of stimulation in periods of main timer 
uint32 stimLengthCounter = 0;                       //control counter for length of stimulation
uint32 stimPeriodCounter = 0;                       //control counter for one period of PWM in stimulation
uint32 stimPeriodMax = 0;                           //length of one period of PWM during stimulation in periods of main timer
uint32 stimPulseONMax = 0;                          //length of ON period inside one period of PWM of stimulation in timer periods
int direction = Left;                               //flag left/right stimulation
int stimulationActive = 0;                          //flag 1- stimulation active; 0 - stimulation inactive




//
// Main timer handler. Executes every 100uS
// I here we generate PWM for stimulation and for LEDs
// Also it is used for other measurements of time for ex. for periodic 
// batery measurements
//
CY_ISR(mainTimerInterruptHandler)
{
    //clear interupt 
    DurationTimer_ClearInterrupt(DurationTimer_INTR_MASK_CC_MATCH);

    //generate PWM for LEDs
    ledPWMCounter++;
    if(ledPWMCounter==LED_PWM_PERIOD)
    {
        //end of PWM period
        ledPWMCounter = 0; 
    }
    if(ledPWMCounter<LED_PWM_ON_INTERVAL)
    {
        ledPWMSignal =1;//put PWM signal to ON
    }
    else
    {
        ledPWMSignal = 0;//put PWM signal to OFF
    }

    
    //If stimulation is active generate PWM for stimulation
    if(stimulationActive==1)
    {
        stimLengthCounter++;
        if(stimLengthCounter>stimDurationMax)//end of stimulation
        {
            //turn off all outputs used for stimulation
            stimulationActive = 0;
            Left_Write(0);
            Right_Write(0);
            LED_R_Write(0);
            LED_L_Write(0);
            DurationTimer_Stop();
        }
        else
        {
            stimPeriodCounter++;
            if(stimPeriodCounter<=stimPulseONMax)
            {
                //Inside ON part of period
                if(direction==Left)
                {
                    Left_Write(1);
                    LED_L_Write(ledPWMSignal);
                }
                else
                {
                    Right_Write(1);
                    LED_R_Write(ledPWMSignal);
                }
                
            }
            else
            {
                //Inside OFF part of period
                if(direction==Left)
                {
                    Left_Write(0);
                    LED_L_Write(ledPWMSignal);
                }
                else
                {
                    Right_Write(0);
                    LED_R_Write(ledPWMSignal);
                }
                if(stimPeriodCounter>stimPeriodMax)//end of one period 
                {
                    stimPeriodCounter =0;
                    //if we are using RND generated stim. generate PWM stim. parameters after each period 
                    //of stimulation
                    if(generator.randomMode!=0)
                    {
                        StimulusGenerator_Randomize(&generator);
                        stimPeriodMax = ONE_SECOND_IN_TIMER_PULSES/generator.pulseFrequency;
                        
                        stimPeriodMax = ONE_SECOND_IN_TIMER_PULSES/generator.pulseFrequency;
                        stimPulseONMax = (ONE_SECOND_IN_TIMER_PULSES/1000)*generator.pulseWidth;
                    }
                }
            }
        }
    
    }

    
   
}






//
// Reset all variables/counters before stimulation
//
void startStimulus(enum Direction dir)
{
    
    DurationTimer_WriteCounter(0);
    DurationTimer_Start();
    mainTimerInterrupt_StartEx(mainTimerInterruptHandler); 
    
    stimulationActive = 0;
    
    //if stim. is random randomize PWM parameters 
    if(generator.randomMode!=0)
    {
        StimulusGenerator_Randomize(&generator);
    }
    //calculate PWM parameters
    stimPeriodMax = ONE_SECOND_IN_TIMER_PULSES/generator.pulseFrequency;
    stimPeriodCounter = 0;
    stimDurationMax = (ONE_SECOND_IN_TIMER_PULSES/1000)*generator.pulseDuration;
    stimPulseONMax = (ONE_SECOND_IN_TIMER_PULSES/1000)*generator.pulseWidth;
    
    //init IO pins
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

//
// Bluetooth communications handler
//
void StackHandler(uint32 eventCode, void* eventParam) {
    
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReq;
    CYBLE_BLESS_CLK_CFG_PARAMS_T clockConfig;

    switch(eventCode) {
        case CYBLE_EVT_STACK_ON: 
            CyBle_GetBleClockCfgParam(&clockConfig);

             /* C8. Set the device sleep-clock accuracy (SCA) based on the tuned ppm
             of the WCO */
             clockConfig.bleLlSca = CYBLE_LL_SCA_000_TO_020_PPM;

             /* C8. Set the clock parameter of BLESS with updated values */
             CyBle_SetBleClockCfgParam(&clockConfig);

        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
        
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
             
            //set connection bool to false
            connectionStatus = 0;
            LED_Conn_Write(0);
            SPIM_Stop();
            //start sleep countdown
            sleepCountdown = SLEEP_TIMER_READY;
            batteryTimer=BATTERY_TIMER_PERIOD;//trigger measurement of battery
            break;
            
        case CYBLE_EVT_GATT_CONNECT_IND:
            
            connectionHandle = *(CYBLE_CONN_HANDLE_T*)eventParam;
            
            //set connection bool to true
            connectionStatus = 1;
            LED_Conn_Write(1);
            
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

//
// Turn off modules in SoC
//
void SleepComponents() {
    SPIM_Stop();
    DurationTimer_Stop();
    CyBle_Stop();
}

//
// Callback handler for BT notifications.
// We use BT notifications to send batery level info
//
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

//
// Functions to put all components to sleep and wake them up
//
#define CONN_LED_TIMER_MAX 25
int conneLedOn = CONN_LED_TIMER_MAX;

void sleepFunction() {   
   
    //clear sleep interrupts
  
    if(stimulationActive==0)
    {
           //decrement sleep countdown, ~1 time per second
           sleepCountdown--;

           
           if(sleepCountdown == 0) {
                //set sleep bool
                gotoSleep = 1;
           }  

            if(connectionStatus==1)
            {
                batteryTimer++;
                if(batteryTimer>=BATTERY_TIMER_PERIOD)
                {
                    //signal to main loop that we need to send batery level notification
                    //by setting sendBatteryLevel to 1
                     ADCForBattery_Start();
                    batteryTimer = 0;
                    sendBatteryLevel = 1;
                }
            }
            else
            {
                conneLedOn--;
                if(conneLedOn==0)
                {
                    LED_Conn_Write(1);
                    CyDelay(20);
                    LED_Conn_Write(0);
                    conneLedOn = CONN_LED_TIMER_MAX;
                }
              
                
                
                
                
            }
    }
    
}

static void LowPowerImplementation(void)
{
    CYBLE_LP_MODE_T bleMode;
    uint8 interruptStatus;
    
    /* For advertising and connected states, implement deep sleep 
     * functionality to achieve low power in the system. For more details
     * on the low power implementation, refer to the Low Power Application 
     * Note.
     */
    if((CyBle_GetState() == CYBLE_STATE_ADVERTISING) || 
       (CyBle_GetState() == CYBLE_STATE_CONNECTED))
    {
        /* Request BLE subsystem to enter into Deep-Sleep mode between connection and advertising intervals */
        bleMode = CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
        /* Disable global interrupts */
        interruptStatus = CyEnterCriticalSection();
        /* When BLE subsystem has been put into Deep-Sleep mode */
        if(bleMode == CYBLE_BLESS_DEEPSLEEP)
        {
            /* And it is still there or ECO is on */
            if((CyBle_GetBleSsState() == CYBLE_BLESS_STATE_ECO_ON) || 
               (CyBle_GetBleSsState() == CYBLE_BLESS_STATE_DEEPSLEEP))
            {
                /* Put the CPU into the Deep-Sleep mode when all debug information has been sent */
               /*if((UART_DEB_SpiUartGetTxBufferSize() + UART_DEB_GET_TX_FIFO_SR_VALID) == 0u)
                {*/
                if(connectionStatus ==0 || stimulationActive==0)
                {
                    CySysPmDeepSleep();
                 }
                else
                {
                    CySysPmSleep();
                }
                /*}
                else 
                {
                    CySysPmSleep();
                }*/
            }
        }
        else /* When BLE subsystem has been put into Sleep mode or is active */
        {
            /* And hardware doesn't finish Tx/Rx opeation - put the CPU into Sleep mode */
            if(CyBle_GetBleSsState() != CYBLE_BLESS_STATE_EVENT_CLOSE)
            {
                CySysPmSleep();
            }
        }
        /* Enable global interrupt */
        CyExitCriticalSection(interruptStatus);
    }
}




//
// Main function that contains main infinite loop
// 
//
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /* C1. Stop the ILO to reduce current consumption */
    CySysClkIloStop();
    /* C2. Configure the divider values for the ECO, so that a 3-MHz ECO divided
    clock can be provided to the CPU in Sleep mode */
    CySysClkWriteEcoDiv(CY_SYS_CLK_ECO_DIV8);


    //initialize sleep/wake interrupts
    //Sleep_StartEx(Sleep_interrupt);
    gotoSleep = 0;
    
    //initalize BLE
    CyBle_Start(StackHandler);
    CyBle_BasRegisterAttrCallback(BasCallBack);
    
    //initialize comms w DigiPot
    LED_R_Write(0);
    LED_L_Write(0); 
    
    VCC_OUT_IO_PIN_Write(0);

    
    /*DurationTimer_WriteCounter(0);
    DurationTimer_Start();
    mainTimerInterrupt_StartEx(mainTimerInterruptHandler); 
    */
    
    
    
    //initalize parameters to defaults
    for(;;)
    {
        //process bluetooth communications
        
        CyBle_ProcessEvents();

       // CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
        
        if (connectionStatus == 0){
            initial = 1; 
            VCC_OUT_IO_PIN_Write(0);
           
            /*CyDelay(250); 
            CyBle_ProcessEvents();
             */
            
        }
        else if (initial == 1){
            VCC_OUT_IO_PIN_Write(1);
            SPIM_Start(); 
            //seed random generator to some random number from 0 to 255
            //using batery time 
            generator.prngCurrent = batteryTimer%255;
            StimulusGenerator_Initialize(&generator);
            initial = 0;  
        }
      
        
        //send batery level notification
        if(sendBatteryLevel==1)
        {
           
            //measure voltage on ADC (ADC has 1.024V reference inside)
            ADCForBattery_StartConvert();
            ADCForBattery_IsEndConversion(ADCForBattery_WAIT_FOR_RESULT);
            //get value from first analog channel
            batteryMeasurement = ADCForBattery_GetResult16(0x00u);
            ADCForBattery_Stop();
            //divide by 20 (it is 11bit ADC so it will return 0-2048 value)
            //and we want to convert it to percents
            batteryLevel = batteryMeasurement/20;
            if(batteryLevel>100)
            {
                batteryLevel = 100;
            }
           
            sendBatteryLevel = 0;        
            //send BT notification
            apiResult = CyBle_BassSendNotification(connectionHandle, CYBLE_BATTERY_SERVICE_INDEX, 
                CYBLE_BAS_BATTERY_LEVEL, sizeof(batteryLevel), &batteryLevel);
            
            if(apiResult == CYBLE_ERROR_OK) 
            {
                //The request handled successfully
            }
            else if(apiResult == CYBLE_ERROR_INVALID_PARAMETER)
            {
                //Validation of the input parameter failed 
            }else if(apiResult == CYBLE_ERROR_INVALID_OPERATION)
            {
                //This operation is not permitted
            }else if(apiResult == CYBLE_ERROR_INVALID_STATE)
            {
                //Connection with the client is not established
            }else if(apiResult == CYBLE_ERROR_MEMORY_ALLOCATION_FAILED)
            {
                // Memory allocation failed.
            }else if(apiResult == CYBLE_ERROR_NTF_DISABLED)
            {
                //Notification is not enabled by the client.
            }
        }
        
        //go to sleep if idle
        if(gotoSleep) {         
            VCC_OUT_IO_PIN_Write(0);
            DurationTimer_Stop();
            //Clock_Stop();
            LED_Conn_Write(0);
            CS_Write(0);
            SleepComponents();
            CySysPmFreezeIo();
            CySysPmHibernate(); 
            //CySysPmStop();
        }
        
       
        // LED_L_Write(1);
        // LED_L_Write(0); 
        sleepFunction();
         //CySysPmSleep();
         //CySysPmDeepSleep();
       LowPowerImplementation();
    }
}

/* [] END OF FILE */
