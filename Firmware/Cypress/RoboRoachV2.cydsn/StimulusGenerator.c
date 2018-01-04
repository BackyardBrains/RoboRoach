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
 * 
 * Structure holds parameters of stimulation
 * This file also implements setters for parameters and RND generator for stimulation
 *
*/

#include "StimulusGenerator.h"

//definitions of constants declared in StimulusGenerator.h
const uint32 DEFAULT_FREQUENCY = 55;//hz

const uint32 MAX_FREQUENCY = 150;//hz
    
const uint32 DEFAULT_WIDTH = 18;//ms
    
const uint32 DEFAULT_DURATION = 500;//ms
    
const uint32 MAX_DURATION = 1000;//ms
    
const uint8 DEFAULT_GAIN = 50;//%
    
const uint8 DEFAULT_RANDOM = 0;//bool
   
const uint32 SEC = 1000;//ms
    
const uint32 DAC_MAX = 612;//microA
    
const double RAND_FREQ_MAPFACTOR = 0.575;
    
//definitions of functions declared in StimulusGenerator.h
void StimulusGenerator_Initialize(struct StimulusGenerator * this) {
    
    //initialize parameters to defaults
    this->pulseFrequency = DEFAULT_FREQUENCY;
    
    this->pulseWidth = DEFAULT_WIDTH;
    
    this->pulseDuration = DEFAULT_DURATION;
    
    this->pulseGain = DEFAULT_GAIN;
    
    this->randomMode = DEFAULT_RANDOM;
    
    //initalize status to off
    this->stimActive = 0;
    
    //intialize peak current to 0
    this->currentPeak = 0;
    
    this->currentHigh = 0;
    
    //seed prng
    //this->prngCurrent = Clock_GetTime() % 255;
    

    
    //setup digipot   
    SPIM_WriteByte(0x04 << 4);//tcon register
    //value 255:
    //connect all terminals to resistance network 
    //Resistor 0 and 1 are NOT forced to the hardware pin “shutdown” configuration
    SPIM_WriteByte(255);
    
    CyDelay(1);
    uint8 val = mapPercentToChar(this->pulseGain);
    
    SPIM_WriteByte(0x00 << 4);//write to wiper 0
    SPIM_WriteByte(val);
    
    CyDelay(1);
    
    SPIM_WriteByte(0x01 << 4);//write to wiper 1
    SPIM_WriteByte(val);
    
    
    CyDelay(1);
    SPIM_ClearTxBuffer();
   
}//Initialize


void StimulusGenerator_SetFrequency(struct StimulusGenerator * this, uint8 freqParam) {
    
    //frequency cannot be 0, we will get divide by 0 errors!!!
    if (freqParam == 0) {
        
        freqParam = 1;
        
    }
    
    this->pulseFrequency = freqParam;
    
}//SetFrequency

void StimulusGenerator_SetPulseWidth(struct StimulusGenerator * this, uint8 widthParam) {
    
    //map 0-255 onto 0-(wave period)
    //double period = SEC / this->pulseFrequency;
    
    //double map = period / 255;
    
    this->pulseWidth = widthParam;
    
}//SetPulseWidth

void StimulusGenerator_SetPulseDuration(struct StimulusGenerator * this, uint8 durationParam) {
    
    //duration comes in in 5ms units, so we multiply by 5 here
    uint32 newDur = durationParam;
    
    newDur *= 5;
    
    this->pulseDuration = newDur;
    
}//SetPulseDuration

void StimulusGenerator_SetPulseGain(struct StimulusGenerator * this, uint8 gainParam) {
    
    this->pulseGain = gainParam;
    
    //set amplification 
    
    uint8 val = mapPercentToChar(this->pulseGain);
    SPIM_WriteByte(0x00 << 4);
    SPIM_WriteByte(val);
    
    CyDelay(1);
    
    SPIM_WriteByte(0x01 << 4);
    SPIM_WriteByte(val);
    
    CyDelay(1);
    
    SPIM_ClearTxBuffer();
    
    
    
}//SetGain

void StimulusGenerator_SetRandomMode(struct StimulusGenerator * this, uint8 randParam){
    
    this->randomMode = randParam;
    
}//SetRandomMode

void StimulusGenerator_Randomize(struct StimulusGenerator * this) {
    
    //map the random value onto the frequency range
    double newFreq = RAND_FREQ_MAPFACTOR * (double)RandomChar(this);
    
    StimulusGenerator_SetFrequency(this, newFreq);
    
    double mapfactor = (double)(SEC / newFreq) / 255;
    
    double newPulseWidth = (double)RandomChar(this) * mapfactor;
    
    StimulusGenerator_SetPulseWidth(this, newPulseWidth);
    
}//Randomize

uint8 RandomChar(struct StimulusGenerator * this) {
    //simple xorshift. well distributed but period is only ~500
    //seeded with modulated system time it should work well enough
    uint8 x = this->prngCurrent;
    
    x ^= x << 7;
    
    x ^= x >> 5;
    
    x ^= x << 3;
    
    this->prngCurrent = x;
    
    return x;
}

uint8 mapPercentToChar(uint8 userGain) {
   return userGain * 2.55; 
    
}

/* [] END OF FILE */
