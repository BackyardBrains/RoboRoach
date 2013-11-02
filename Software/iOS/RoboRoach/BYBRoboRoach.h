//
//  BYBRoboRoach.h
//  RoboRoach
//
//  Created by Greg Gage on 4/17/13.
//  Copyright (c) 2013 Backyard Brains. All rights reserved.
//

#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>
#import "TestFlight.h"

#define ROBOROACH_TURN_TIMEOUT 0.4

typedef enum {
    moveLeft=1,
    moveRight,
} BYBMovementCommand;

@interface BYBRoboRoach : NSObject

@property (nonatomic, copy) NSNumber *randomMode;
@property (nonatomic, copy) NSNumber *frequency;
@property (nonatomic, copy) NSNumber *gain;
@property (nonatomic, copy) NSNumber *pulseWidth;
@property (nonatomic, copy) NSNumber *duration;
@property (strong, nonatomic) CBPeripheral *peripheral;
@property (nonatomic, copy) NSNumber *batteryLevel;

@property (nonatomic, copy) NSString *hardwareVersion;
@property (nonatomic, copy) NSString *firmwareVersion;

@property (nonatomic, copy) NSNumber *isLoadingParameters;


//- (void) readSettingsFromRoboRoach;
- (void) goLeft;
- (void) goRight;
- (void) updateSettings;
- (NSString *) getStimulationString;

//BYBRoboRoach Delegate
- (id)delegate;
- (void)setDelegate:(id)newDelegate;


@end


@protocol BYBRoboRoachDelegate <NSObject>
@required
- (void) roboRoachHasChangedSettings:(BYBRoboRoach *)roboRoach;
- (void) roboRoach: (BYBRoboRoach *)roboRoach hasMovementCommand:(BYBMovementCommand) command;
@end

