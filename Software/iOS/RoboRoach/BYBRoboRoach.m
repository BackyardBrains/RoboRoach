//
//  BYBRoboRoach.m
//  RoboRoach
//
//  Created by Greg Gage on 4/17/13.
//  Copyright (c) 2013 Backyard Brains. All rights reserved.
//

#import "BYBRoboRoach.h"

@implementation BYBRoboRoach

bool turnCommandActive = NO;

- (BYBRoboRoach * ) init {
    self.isLoadingParameters = 0;
    return self;
}

// delegate bookkeeping
id <BYBRoboRoachDelegate> delegate;
- (id)delegate {
    return delegate;
}

- (void)setDelegate:(id)newDelegate {
    delegate = newDelegate;
}

- (void) updateSettings {
    NSLog(@"Updating RoboRoach Settings:");
    NSLog(@"Random Mode: %@", self.randomMode);
    NSLog(@"Frequency: %@", self.frequency);
    NSLog(@"Pulse Width: %@", self.pulseWidth);
    NSLog(@"# Pulses: %@", self.numberOfPulses);
    NSLog(@"Gain: %@", self.gain);
    
    [delegate roboRoachHasChangedSettings:self];
}

- (NSString *) getStimulationString  {
    NSString * stimSettings;
    if ([self.randomMode boolValue]){
        stimSettings = [NSString stringWithFormat:@"Randomized for %ims. %i%%", [self.duration intValue], (int)(100*[self.gain floatValue]/255) ];
    }else{
        stimSettings = [NSString stringWithFormat:@"%iHz, %ims pulse, for %ims. %i%%", [self.frequency intValue], [self.pulseWidth intValue], [self.duration intValue], (int)(100*[self.gain floatValue]/255)];
    }
    return (stimSettings);
}

- (void) goLeft {
    if (!turnCommandActive){
        if (!turnCommandActive){
            turnCommandActive = YES;
            [self.delegate roboRoach:self hasMovementCommand: moveLeft];
            [NSTimer scheduledTimerWithTimeInterval:ROBOROACH_TURN_TIMEOUT target:self selector:@selector(turnActiveTimer:) userInfo:nil repeats:NO];
        }
    }
}

- (void) goRight {
    if (!turnCommandActive){
        if (!turnCommandActive){
            turnCommandActive = YES;
            [self.delegate roboRoach:self hasMovementCommand: moveRight];
            [NSTimer scheduledTimerWithTimeInterval:ROBOROACH_TURN_TIMEOUT target:self selector:@selector(turnActiveTimer:) userInfo:nil repeats:NO];
        }
    }
}

- (void) turnActiveTimer:(NSTimer *)timer {
    turnCommandActive = NO;
}


@end
