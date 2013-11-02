//
//  BYBRoboRoachManager.h
//
//  Core Bluetooth RoboRoach Class handles communication to the RoboRoach
//
//  Created by Greg Gage on 4/14/13.
//  Copyright (c) 2013 Backyard Brains. All rights reserved.
//

#import <CoreBluetooth/CoreBluetooth.h>
#import <CoreBluetooth/CBService.h>
#include "BYBRoboRoach.h"

#define BYB_ROBOROACH_SERVICE_UUID  0xB2B0
#define BYB_ROBOROACH_CHAR_FREQUENCY_UUID  0xB2B1
#define BYB_ROBOROACH_CHAR_PULSEWIDTH_UUID  0xB2B2
#define BYB_ROBOROACH_CHAR_NUMPULSES_UUID  0xB2B3
#define BYB_ROBOROACH_CHAR_RANDOMMODE_UUID  0xB2B4
#define BYB_ROBOROACH_STIM_LEFT_UUID  0xB2B5
#define BYB_ROBOROACH_STIM_RIGHT_UUID  0xB2B6
#define BYB_ROBOROACH_CHAR_GAIN_UUID  0xB2B7

#define BATTERY_SERVICE_UUID 0x180F
#define BATTERY_CHAR_BATTERYLEVEL_UUID 0x2A19

#define DEVICE_INFO_SERVICE_UUID 0x180A
#define DEVICE_INFO_CHAR_HARDWARE_UUID 0x2A27
#define DEVICE_INFO_CHAR_FIRMWARE_UUID 0x2A26


@protocol BYBRoboRoachManagerDelegate <NSObject>
@required
- (void) didSearchForRoboRoaches: (NSArray*)foundRoboRoaches;
- (void) didConnectToRoboRoach: (BOOL)success;
- (void) didFinsihReadingRoboRoachValues;
- (void) didDisconnectFromRoboRoach;
- (void) roboRoachReady;

@optional
- (void) hadBluetoothError: (int) CMState;
@end

@interface BYBRoboRoachManager : NSObject <CBCentralManagerDelegate , CBPeripheralDelegate >

@property (strong, nonatomic) NSMutableArray *peripherals;
@property (strong, nonatomic) CBCentralManager *CM;
@property (strong, nonatomic) BYBRoboRoach *activeRoboRoach;

//BYBRoboRoachManager Delegate
- (id)delegate;
- (void)setDelegate:(id)newDelegate;

-(int) searchForRoboRoaches:(int) timeout;
-(int) connectToRoboRoach:(BYBRoboRoach *) roboRoach;
-(int) disconnectFromRoboRoach;
-(void) sendMoveCommandToActiveRoboRoach: (BYBMovementCommand) command;
-(void) sendUpdatedSettingsToActiveRoboRoach;

-(void) readValue: (int)serviceUUID characteristicUUID:(int)characteristicUUID;
-(void) writeValue:(int)serviceUUID characteristicUUID:(int)characteristicUUID data:(NSData *)data;
-(void) notification:(int)serviceUUID characteristicUUID:(int)characteristicUUID on:(BOOL)on;


-(UInt16) swap:(UInt16)s ;
-(const char *) CBUUIDToString:(CBUUID *) UUID ;
-(const char *) UUIDToString:(CFUUIDRef)UUID ;
-(int) compareCBUUID:(CBUUID *) UUID1 UUID2:(CBUUID *)UUID2;
-(int) compareCBUUIDToInt:(CBUUID *)UUID1 UUID2:(UInt16)UUID2 ;
-(UInt16) CBUUIDToInt:(CBUUID *) UUID;
-(CBService *) findServiceFromUUID:(CBUUID *)UUID p:(CBPeripheral *)p;
-(CBCharacteristic *) findCharacteristicFromUUID:(CBUUID *)UUID service:(CBService*)service ;
- (const char *) centralManagerStateToString: (int)state;
- (void) printPeripheralInfo:(CBPeripheral*)peripheral;
- (void) printKnownPeripherals;

@end
