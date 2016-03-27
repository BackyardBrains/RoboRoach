/**************************************************************************************************
  Filename:       MCP4000.h 
  Revised:        $Date: 2013-07-17 05:16:52 -0800 (Wed, 17 Jul 2013) $
  Revision:       $Revision: 33023 $

  Description:    Header file for control of the digital potentiometer MCP4251

**************************************************************************************************/

#ifndef CMA3000_H
#define CMA3000_H

#include "hal_types.h"


//***********************************************************************************
// Defines

// HW version
#define REV_1_0
#ifndef REV_1_0
  #define REV_0_6
#endif

// CMA3000 addressing space
#define WHO_AM_I        (0x00<<2)
#define REVID           (0x01<<2)
#define CTRL            (0x02<<2)
#define STATUS          (0x03<<2)
#define RSTR            (0x04<<2)
#define INT_STATUS      (0x05<<2)
#define DOUTX           (0x06<<2)
#define DOUTY           (0x07<<2)
#define DOUTZ           (0x08<<2)
#define MDTHR           (0x09<<2)
#define MDFFTMR         (0x0A<<2)
#define FFTHR           (0x0B<<2)

// CTRL register definitions
#define RANGE_2G        0x80
#define RANGE_8G        0x00

#define INT_ACTIVE_LOW  0x40
#define INT_ACTIVE_HIGH 0x00

#define MODE_PD         0x00
#define MODE_100HZ_MEAS 0x02
#define MODE_400HZ_MEAS 0x04
#define MODE_40HZ_MEAS  0x06
#define MODE_10HZ_MD    0x08
#define MODE_100HZ_FALL 0x0A
#define MODE_400HZ_FALL 0x0C

#define INT_DIS         0x01
#define INT_EN          0x00




// Data ready pin
#ifdef REV_1_0
    #define ACC_DATA_READY  (P1_7 == 1)
#elif (defined REV_0_6)
    #define ACC_DATA_READY  (P0_6 == 1)
#endif

//***********************************************************************************
// Macros

// Wait 1+1/3*t [us]
#define WAIT_1_3US(t)                   \
    do{                                 \
        for (uint8 i = 0; i<t; i++)     \
            asm("NOP");                 \
    }while(0)



//***********************************************************************************
// Function prototypes
void potReadReg(uint8 reg, uint8 *pVal);

void potInit(void);
void potWriteReg(uint8 reg, uint8 val);
void Gain_SetLevel( uint8 userGain ); 

#endif
