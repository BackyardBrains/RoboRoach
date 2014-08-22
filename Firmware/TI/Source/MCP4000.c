/**************************************************************************************************
  Filename:       MCP4000.c
  Revised:        $Date: 2013-02-08 05:16:52 -0800 (Fri, 08 Feb 2013) $
  Revision:       $Revision: 33023 $

  Description:    Control of the digital potentiometer MCP4251 on the RoboRoach board

  Copyright 2014  Backyard Brains

**************************************************************************************************/

#include <ioCC2540.h>
#include "MCP4000.h"


//***********************************************************************************
// Defines

// Digipot connected at:
// P0_2 = MISO
// P0_3 = MOSI
// P0_5 = SCK
// P0_4 = SSN (aka CS)

#define MISO            P0_2  //BLE pin 5
#define CS              P0_4  //BLE pin 6
#define SCK             P0_5  //BLE pin 7
#define MOSI            P0_3  //BLE pin 8


#define CS_DISABLED     1
#define CS_ENABLED      0


//***********************************************************************************
// Function prototypes
void spiWriteByte(uint8 write);
void spiReadByte(uint8 *read, uint8 write);


//***********************************************************************************
// Local variables


//***********************************************************************************
// functions

/**  Initialize SPI interface and MCP4251 digipot
*
* This will initialize the SPI interface and MCP4251 digipot
*/
void potInit(void)
{
    //*** Setup USART 0 SPI at alternate location 1 ***

    // USART 0 at alternate location 1
    // PERCFG.U0CFG=0 (hopefully... maybe &= ~0x01 OR &= 0xFE instead)
    PERCFG &= ~0x01;
    // Peripheral function on SCK, MISO and MOSI (P0_2, P0_3, P0_5)
    P0SEL |= 0x2C;
    //GPIO function on SSN (P0_4)
    P0SEL &= ~0x10;
    // Configure CS, SSN is set as output
    P0DIR |= 0x10;

    CS = CS_DISABLED;
    
    //*** Setup the SPI interface ***
    // SPI master mode  ??? 
    U0CSR = 0x00;
    // Negative clock polarity, Phase: data out on CPOL -> CPOL-inv
    //                                 data in on CPOL-inv -> CPOL
    // MSB first
    U0GCR = 0x20;
    // SCK frequency = 480.5kHz (max 500kHz)
    U0GCR |= 0x0D;
    U0BAUD = 0xEC;

    //initalize TCON bits 
    potWriteReg(0x04, 0xFF);
    
}


/** \brief	Writes one byte to digipot register
*
* Write one byte to a sensor register
*
* \param[in]       reg
*     Register address
* \param[in]       val
*     Value to write
*/
void potWriteReg(uint8 reg, uint8 val)
{
    CS = CS_ENABLED;
    //bit shift by 4
    spiWriteByte(reg << 4);
    //WRITE VALUE
    spiWriteByte(val);
    CS = CS_DISABLED;
}

/* \brief    Sets the gain level on digipot
*     
*  takes an uint8 (0 to 100) from user characteristic
*      and writes to the digipot an uint8 (0 to 255)
*/
void Gain_SetLevel(uint8 userGain)
{
    //initialize variables for calculation
    uint8   gain8bit = 0;
    float   floatingGain = 0;
    
    //mapping userGain (ints: 0 - 100) to gain8bit (ints: 0 - 255)
    floatingGain = (255.0/100.0) * userGain;
    gain8bit = (uint8) (floatingGain + 0.5); 
    
    //update the MCP4250
    potWriteReg(0x00, gain8bit);  
    potWriteReg(0x01, gain8bit); 
}
     

/** \brief	Read one byte from digipot register
*
* Read one byte from a sensor register
*
* \param[in]       reg
*     Register address
* \param[in]       *pVal
*     Pointer to variable to put read out value
*/
void potReadReg(uint8 reg, uint8 *pVal)
{
    CS = CS_ENABLED;
    WAIT_1_3US(2);
    spiWriteByte(reg << 4 | 0x0C);
    spiReadByte(pVal, 0xff);
    CS = CS_DISABLED;
}


/** \brief	Write one byte to SPI interface
*
* Write one byte to SPI interface
*
* \param[in]       write
*     Value to write
*/

void spiWriteByte(uint8 write)
{
        U0CSR &= ~0x02;                 // Clear TX_BYTE
        U0DBUF = write;
        while (!(U0CSR & 0x02));        // Wait for TX_BYTE to be set
}

/** \brief	Read one byte from SPI interface
*
* Read one byte from SPI interface
*
* \param[in]       read
*     Read out value
* \param[in]       write
*     Value to write
*/

void spiReadByte(uint8 *read, uint8 write)
{
        U0CSR &= ~0x02;                 // Clear TX_BYTE
        U0DBUF = write;
        while (!(U0CSR & 0x02));        // Wait for TX_BYTE to be set
        *read = U0DBUF;
}

