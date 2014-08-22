/**************************************************************************************************
  Filename:       roboRoach.h
 
  Description:    This file contains the RoboRoach application
                  definitions and prototypes.

  Copyright 2013 Backyard Brains Incorporated. All rights reserved.

**************************************************************************************************/

#ifndef ROBOROACH_H
#define ROBOROACH_H

#ifdef __cplusplus
extern "C"
{
#endif

//RoboRoach 1.0a.  Right LED = P0 0x40;
//RoboRoach 1.0a.  Left LED = P0 0x80;
//RoboRoach 1.0b.  Right LED = P1 0x01;
//RoboRoach 1.0b.  Left LED = P1 0x02;
      
// Profile Parameters
#define ROBOROACH_FREQUENCY               1   
#define ROBOROACH_PULSE_WIDTH             2  
#define ROBOROACH_DURATION                3  
#define ROBOROACH_RANDOM_MODE             4  
#define ROBOROACH_STIM_PERIOD             5  
#define ROBOROACH_STIMULATE_LEFT          6   
#define ROBOROACH_STIMULATE_RIGHT         7  
#define ROBOROACH_GAIN                    8 
#define ROBOROACH_FREQ_MIN                9   
#define ROBOROACH_FREQ_MAX                10
#define ROBOROACH_PW_MIN                  11
#define ROBOROACH_PW_MAX                  12
#define ROBOROACH_GAIN_MIN                13
#define ROBOROACH_GAIN_MAX                14
  
// RoboRoach Service UUID
#define ROBOROACH_SERV_UUID                  0xB2B0
    
// Key Pressed UUID
#define ROBOROACH_CHAR_FREQUENCY_UUID        0xB2B1
#define ROBOROACH_CHAR_PULSE_WIDTH_UUID      0xB2B2
#define ROBOROACH_CHAR_DURATION_IN_5MS_UUID  0xB2B3
#define ROBOROACH_CHAR_RANDOM_MODE_UUID      0xB2B4
#define ROBOROACH_CHAR_STIMULATE_LEFT_UUID   0xB2B5  //go left! - stimulates right ant
#define ROBOROACH_CHAR_STIMULATE_RIGHT_UUID  0xB2B6  //go right! - stimulates left ant
#define ROBOROACH_CHAR_GAIN_UUID             0xB2B7 
#define ROBOROACH_CHAR_FREQ_MIN_UUID         0xB2B8 
#define ROBOROACH_CHAR_FREQ_MAX_UUID         0xB2B9
#define ROBOROACH_CHAR_PW_MIN_UUID           0xB2BA
#define ROBOROACH_CHAR_PW_MAX_UUID           0xB2BB
#define ROBOROACH_CHAR_GAIN_MIN_UUID         0xB2BC
#define ROBOROACH_CHAR_GAIN_MAX_UUID         0xB2BD  
  
#define ROBOROACH_FIRMWARE_VERSION               "0.3"
//#define ROBOROACH_V10A
//#define ROBOROACH_V10B
//#define ROBOROACH_V10G
#define ROBOROACH_V11B
  
#if defined ( ROBOROACH_V10A )
 #define ROBOROACH_HARDWARE_VERSION               "1.0A" 
 #define ROBOROACH_HAS_CONNECTION_LEDS            0 
 #define ROBOROACH_PIO_LED_LEFT                   P0_7
 #define ROBOROACH_PIO_LED_RIGHT                  P0_6
 #define ROBOROACH_PIO_ANTENNA_LEFT               P1_0
 #define ROBOROACH_PIO_ANTENNA_RIGHT              P1_1
 #define ROBOROACH_PIO_LED_CONNECTION_1           P0_6  
 #define ROBOROACH_PIO_LED_CONNECTION_2           P0_7  
#endif
  
  #if defined ( ROBOROACH_V10B )
 #define ROBOROACH_HARDWARE_VERSION               "1.0B" 
 #define ROBOROACH_HAS_CONNECTION_LEDS            0 
 #define ROBOROACH_PIO_LED_LEFT                   P1_0
 #define ROBOROACH_PIO_LED_RIGHT                  P1_1
 #define ROBOROACH_PIO_ANTENNA_LEFT               P1_0
 #define ROBOROACH_PIO_ANTENNA_RIGHT              P1_1
 #define ROBOROACH_PIO_LED_CONNECTION_1           P1_0  
 #define ROBOROACH_PIO_LED_CONNECTION_2           P1_1  
#endif
  
#if defined ( ROBOROACH_V10G )
 #define ROBOROACH_HARDWARE_VERSION               "1.0G" 
 #define ROBOROACH_HAS_CONNECTION_LEDS            1 
 #define ROBOROACH_PIO_LED_LEFT                   P0_5
 #define ROBOROACH_PIO_LED_RIGHT                  P0_4
 #define ROBOROACH_PIO_ANTENNA_LEFT               P1_0
 #define ROBOROACH_PIO_ANTENNA_RIGHT              P1_1
 #define ROBOROACH_PIO_LED_CONNECTION_1           P0_2  
 #define ROBOROACH_PIO_LED_CONNECTION_2           P0_3  
#endif
  
#if defined ( ROBODEV )      //[wjr]--> Not 100% mapped to DK yet
 #define ROBOROACH_HARDWARE_VERSION               "EVAL" 
 #define ROBOROACH_HAS_CONNECTION_LEDS            1 
 #define ROBOROACH_PIO_LED_LEFT                   P1_0 // LED
 #define ROBOROACH_PIO_LED_RIGHT                  P1_7  
 #define ROBOROACH_PIO_ANTENNA_LEFT               P1_1  
 #define ROBOROACH_PIO_ANTENNA_RIGHT              P0_1 
 #define ROBOROACH_PIO_LED_CONNECTION_1           P1_5   
 #define ROBOROACH_PIO_LED_CONNECTION_2           P1_4  
 #define ROBOROACH_WAKE_UP_BUTTON                 P0_6 //[wjr] still requires breadboard button
#else
 #define ROBOROACH_HARDWARE_VERSION               "1.1B" 
 #define ROBOROACH_HAS_CONNECTION_LEDS            1 
 #define ROBOROACH_PIO_LED_LEFT                   P1_6  //Left turn - Lights up Right
 #define ROBOROACH_PIO_LED_RIGHT                  P1_7  //Right turn - Lights up Left
 #define ROBOROACH_PIO_ANTENNA_LEFT               P1_1  
 #define ROBOROACH_PIO_ANTENNA_RIGHT              P1_0  
 #define ROBOROACH_PIO_LED_CONNECTION_1           P1_5   
 #define ROBOROACH_PIO_LED_CONNECTION_2           P1_4  //Left?
 #define ROBOROACH_WAKE_UP_BUTTON                 P0_6 
#endif
  

// RoboRoach Services bit fields
#define ROBOROACH_SERVICE               0x000000001
 
// What happens when we connect? 
#define BYB_ADV_PULSE_PERIOD                         1000  
#define BYB_ADV_PULSE_WIDTH                            20  
#define BYB_CONNECT_PULSE_PERIOD                        5  
#define BYB_CONNECT_PULSE_WIDTH                         1 
  
#define BYB_BATTERY_CHECK_PERIOD                    10000 //Every 10s

#define POWER_SAVING  1  
#define BYB_DISCONNECT_PERIOD_B4_SLEEP              30000 //Every 30s   
  
// RoboRoach Events  
#define BYB_START_DEVICE_EVT                        0x0001
#define BYB_TURN_OFF_STIM_EVT                       0x0002
#define BYB_BATTERY_CHECK_EVT                       0x0004
#define BYB_ADV_IN_CONNECTION_EVT                   0x0008 //[wjr: not being used currently]
#define BYB_ADV_PULSE_ON_EVT                        0x0010 
#define BYB_ADV_PULSE_OFF_EVT                       0x0020 
#define BYB_CONNECT_PULSE_ON_EVT                    0x0040
#define BYB_CONNECT_PULSE_OFF_EVT                   0x0080
#define BYB_STIMULATE_LEFT_EVT                      0x0100
#define BYB_STIMULATE_RIGHT_EVT                     0x0200
#define BYB_STIMULATE_PULSE_ON_EVT                  0x0400
#define BYB_STIMULATE_PULSE_OFF_EVT                 0x0800
#define BYB_STIMULATE_FINISHED_EVT                  0x1000
#define BYB_SLEEP_EVT                               0x2000 
#define BYB_WAKE_UP_EVT                             0x4000 
                                                  //0x8000 is reserved for SYS_EVENT_MSG

/*
#define BYB_MIN_STIMULATE_PULSE_WIDTH               1
#define BYB_MAX_STIMULATE_PULSE_WIDTH               9
#define BYB_MIN_STIMULATE_PERIOD                    10
#define BYB_MAX_STIMULATE_PERIOD                    25 
#define BYB_MIN_STIMULATE_GAIN                      40
#define BYB_MAX_STIMULATE_GAIN                      60
*/

#ifdef __cplusplus
}
#endif

#endif /* ROBOROACH_H */
