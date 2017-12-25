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

#ifndef STIMULUS_GENERATOR_H
#define STIMULUS_GENERATOR_H
    
#include "project.h"
    
//global constants for default values
extern const uint32 DEFAULT_FREQUENCY;
    
extern const uint32 MAX_FREQUENCY;
    
extern const uint32 DEFAULT_WIDTH;
    
extern const uint32 DEFAULT_DURATION;
    
extern const uint32 MAX_DURATION;
    
extern const uint8 DEFAULT_GAIN;
    
extern const uint8 DEFAULT_RANDOM;
    
//global constants for general use
    
//100 ms to one second    
extern const uint32 SEC;

//maximum microamps fomc the DAC    
extern const uint32 DAC_MAX;
 
//factor for mapping from 0-255 to 0-150    
extern const double RAND_FREQ_MAPFACTOR;

    
enum Direction { Left = 0, Right = 1};
    
struct StimulusGenerator {
    
    //PARAMETERS
    
    //stimulation frequency, in Hz
    uint32 pulseFrequency;
    
    //stimulation pulse width, in ms
    uint32 pulseWidth;
    
    //duration of pulsing, in ms
    uint32 pulseDuration;
    
    //voltage gain of pluse, in %
    uint8 pulseGain;
    
    //boolean for randomMode state
    uint8 randomMode;
    
    //VARIABLES
    
    //stimulation status
    uint8 stimActive;
    
    //peak microamps for pulse
    uint32 currentPeak;
    
    //if current is high this will be true
    uint8 currentHigh;
    
    //current prng value;
    uint8 prngCurrent;
    
};

void StimulusGenerator_Initialize(struct StimulusGenerator * this);

void StimulusGenerator_SetFrequency(struct StimulusGenerator * this, uint8 freqParam);

void StimulusGenerator_SetPulseWidth(struct StimulusGenerator * this, uint8 widthParam);

void StimulusGenerator_SetPulseDuration(struct StimulusGenerator * this, uint8 durationParam);

void StimulusGenerator_SetPulseGain(struct StimulusGenerator * this, uint8 gainParam);

void StimulusGenerator_SetRandomMode(struct StimulusGenerator * this, uint8 randParam);

void StimulusGenerator_Randomize(struct StimulusGenerator * this);

//helper functions
uint8 RandomChar(struct StimulusGenerator * this);

uint8 mapPercentToChar(uint8 userGain);

#endif
/* [] END OF FILE */
