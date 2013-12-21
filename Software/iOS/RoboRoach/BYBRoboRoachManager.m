//
//  BYBRoboRoachManager.m
//  RoboRoach
//
//  Created by Greg Gage on 4/14/13.
//  Copyright (c) 2013 Backyard Brains. All rights reserved.
//

#import "BYBRoboRoachManager.h"

@implementation BYBRoboRoachManager

// delegate bookkeeping
id <BYBRoboRoachManagerDelegate> delegate;
- (id)delegate {
    return delegate;
}

- (void)setDelegate:(id)newDelegate {
    delegate = newDelegate;
}

- (id)init
{
    if ((self = [super init]))
    {
        self.CM = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
    }
    return self;
}

-(int) searchForRoboRoaches:(int) timeout{
    NSLog(@"searchForRoboRoaches() - Entered");
    if (self.CM.state  != CBCentralManagerStatePoweredOn) {
        NSLog(@"searchForRoboRoaches() - CoreBluetooth not correctly initialized !\r\n");
        NSLog(@"searchForRoboRoaches() - State = %d (%s)\r\n",self.CM.state,[self centralManagerStateToString:self.CM.state]);
        [[self delegate] didSearchForRoboRoaches:@[]];
        [[self delegate] hadBluetoothError: self.CM.state];
        return -1;
    }
    
    
    //UInt16 s = [self swap:BYB_BATTERY_SERVICE_UUID];
    //NSData *sd = [[NSData alloc] initWithBytes:(char *)&s length:2];
    //CBUUID *su = [CBUUID UUIDWithData:sd];
    //serviceUUIDs - An array of CBUUID objects that the app is interested in. In this case, each CBUUID object represents the UUID of a service that a peripheral is advertising.
    
    // Start scanning
    //[self.CM scanForPeripheralsWithServices:@[su] options:0];
    [self.CM scanForPeripheralsWithServices:nil options:0];
    
    NSLog(@"searchForRoboRoaches() - Scanning...");
    [NSTimer scheduledTimerWithTimeInterval:(float)timeout target:self selector:@selector(scanTimer:) userInfo:nil repeats:NO];  //Set a timer to Turn Off
    
    return 0;
}


-(int) connectToRoboRoach:(BYBRoboRoach *) roboRoach{
    self.activeRoboRoach = roboRoach;
    [self connectPeripheral:self.activeRoboRoach.peripheral];
    
    
    //[self getAllServicesFromRoboRoach:self.activeRoboRoach.peripheral];
    return 0;
}

-(int) disconnectFromRoboRoach{
    
    [self.CM cancelPeripheralConnection:self.activeRoboRoach.peripheral];
    self.activeRoboRoach = nil;
    
    return 0;
}

-(void) sendMoveCommandToActiveRoboRoach: (BYBMovementCommand) command{
    switch (command) {
            char c  = 0x01;
        case moveLeft:
            [self writeValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_STIM_LEFT_UUID data:[NSData dataWithBytes: &c length: sizeof(c)]];
            break;
        case moveRight:
            [self writeValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_STIM_RIGHT_UUID data:[NSData dataWithBytes: &c length: sizeof(c)]];
            break;
            
        default:
            break;
    }
}

-(void) sendUpdatedSettingsToActiveRoboRoach {
    
    UInt8 data;
    
    data = self.activeRoboRoach.frequency.integerValue;
    [self writeValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_FREQUENCY_UUID data:[NSData dataWithBytes: &data length: sizeof(data)]];

    data = self.activeRoboRoach.pulseWidth.integerValue;
    [self writeValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_PULSEWIDTH_UUID data:[NSData dataWithBytes: &data length: sizeof(data)]];

    data = self.activeRoboRoach.duration.integerValue/5; //Note we need to divide by 5ms.
    [self writeValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_DURATION_IN_5MS_INTERVALS_UUID data:[NSData dataWithBytes: &data length: sizeof(data)]];
    
    data = self.activeRoboRoach.randomMode.integerValue;
    [self writeValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_RANDOMMODE_UUID data:[NSData dataWithBytes: &data length: sizeof(data)]];
    
    data = self.activeRoboRoach.gain.unsignedIntegerValue;
    [self writeValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_GAIN_UUID data:[NSData dataWithBytes: &data length: sizeof(data)]];

}

-(void) getAllServicesFromRoboRoach:(CBPeripheral *)p{
    NSLog(@"Entering getAllServicesFromRoboRoach");
    [p discoverServices:nil]; // Discover all services without filter
}

-(void) getAllCharacteristicsFromRoboRoach:(CBPeripheral *)p{
    NSLog(@"Entering getAllCharacteristicsFromRoboRoach");
    
    for (int i=0; i < p.services.count; i++) {
        CBService *s = [p.services objectAtIndex:i];
        NSLog(@"Fetching characteristics for service with UUID : %s",[self CBUUIDToString:s.UUID]);
        [p discoverCharacteristics:nil forService:s];
    }
}


- (void) connectPeripheral:(CBPeripheral *)peripheral {
    NSLog(@"Entering connectPeripheral(UUID : %s)",[self UUIDToString:peripheral.UUID]);
    
    self.activeRoboRoach.peripheral = peripheral;
    self.activeRoboRoach.peripheral.delegate = self;
    [self.CM connectPeripheral:self.activeRoboRoach.peripheral options:nil];
    
}

- (void) scanTimer:(NSTimer *)timer {
    [self.CM stopScan];
    NSLog(@"[local] Stopped Scanning");
    NSLog(@"[local] Known peripherals : %d",[self->_peripherals count]);
    
    [self printKnownPeripherals];
    if ([_peripherals count] > 0 ){
        BYBRoboRoach * r = [[BYBRoboRoach alloc] init];
        r.peripheral = _peripherals[0];
        [[self delegate] didSearchForRoboRoaches:@[r]];
    }
    else{
        [[self delegate] didSearchForRoboRoaches:@[]];
    }
    
}


-(void) readValue: (int)serviceUUID characteristicUUID:(int)characteristicUUID {
    UInt16 s = [self swap:serviceUUID];
    UInt16 c = [self swap:characteristicUUID];
    NSData *sd = [[NSData alloc] initWithBytes:(char *)&s length:2];
    NSData *cd = [[NSData alloc] initWithBytes:(char *)&c length:2];
    CBUUID *su = [CBUUID UUIDWithData:sd];
    CBUUID *cu = [CBUUID UUIDWithData:cd];
    CBService *service = [self findServiceFromUUID:su p:self.activeRoboRoach.peripheral];
    if (!service) {
        NSLog(@"Could not find service with UUID %s on peripheral with UUID %s\r\n",[self CBUUIDToString:su],[self UUIDToString:self.activeRoboRoach.peripheral.UUID]);
        return;
    }
    CBCharacteristic *characteristic = [self findCharacteristicFromUUID:cu service:service];
    
    if (!characteristic) {
        NSLog(@"Could not find characteristic with UUID %s on service with UUID %s on peripheral with UUID %s\r\n",[self CBUUIDToString:cu],[self CBUUIDToString:su],[self UUIDToString:self.activeRoboRoach.peripheral.UUID]);
        return;
    }
    
    NSLog(@"Reading characteristic %s on service %s",[self CBUUIDToString:cu], [self CBUUIDToString:su]);
    [self.activeRoboRoach.peripheral readValueForCharacteristic:characteristic];
}

-(void) writeValue:(int)serviceUUID characteristicUUID:(int)characteristicUUID  data:(NSData *)data {
    NSLog(@"Entering writeValue(characteristicUUID : %i)",characteristicUUID);
    
    UInt8 bdata;
    [data getBytes:&bdata length:sizeof(UInt8)];
    
    UInt16 s = [self swap:serviceUUID];
    UInt16 c = [self swap:characteristicUUID];
    NSData *sd = [[NSData alloc] initWithBytes:(char *)&s length:2];
    NSData *cd = [[NSData alloc] initWithBytes:(char *)&c length:2];
    CBUUID *su = [CBUUID UUIDWithData:sd];
    CBUUID *cu = [CBUUID UUIDWithData:cd];
    CBService *service = [self findServiceFromUUID:su p:self.activeRoboRoach.peripheral];
    if (!service) {
        NSLog(@"Could not find service with UUID %s on peripheral with UUID %s\r\n",[self CBUUIDToString:su],[self UUIDToString:self.activeRoboRoach.peripheral.UUID]);
        return;
    }
    CBCharacteristic *characteristic = [self findCharacteristicFromUUID:cu service:service];
    if (!characteristic) {
        NSLog(@"Could not find characteristic with UUID %s on service with UUID %s on peripheral with UUID %s\r\n",[self CBUUIDToString:cu],[self CBUUIDToString:su],[self UUIDToString:self.activeRoboRoach.peripheral.UUID]);
        return;
    }
     NSLog(@"Writing [%i] characteristic %s on service %s", bdata, [self CBUUIDToString:cu], [self CBUUIDToString:su]);
    [self.activeRoboRoach.peripheral writeValue:data forCharacteristic:characteristic type:CBCharacteristicWriteWithResponse];
}

-(void) notification:(int)serviceUUID characteristicUUID:(int)characteristicUUID on:(BOOL)on {
    NSLog(@"[local] notification(%i)",on);
    UInt16 s = [self swap:serviceUUID];
    UInt16 c = [self swap:characteristicUUID];
    NSData *sd = [[NSData alloc] initWithBytes:(char *)&s length:2];
    NSData *cd = [[NSData alloc] initWithBytes:(char *)&c length:2];
    CBUUID *su = [CBUUID UUIDWithData:sd];
    CBUUID *cu = [CBUUID UUIDWithData:cd];
    CBService *service = [self findServiceFromUUID:su p:self.activeRoboRoach.peripheral];
    if (!service) {
        NSLog(@"Could not find service with UUID %s on peripheral with UUID %s\r\n",[self CBUUIDToString:su],[self UUIDToString:self.activeRoboRoach.peripheral.UUID]);
        return;
    }
    CBCharacteristic *characteristic = [self findCharacteristicFromUUID:cu service:service];
    if (!characteristic) {
        NSLog(@"Could not find characteristic with UUID %s on service with UUID %s on peripheral with UUID %s\r\n",[self CBUUIDToString:cu],[self CBUUIDToString:su],[self UUIDToString:self.activeRoboRoach.peripheral.UUID]);
        return;
    }
    [self.activeRoboRoach.peripheral setNotifyValue:on forCharacteristic:characteristic];
}

//-------------------------------------------------------------------------
//
// CBCentralManagerDelegate protocol methods beneeth here
// Documented in CoreBluetooth documentation
//
//-------------------------------------------------------------------------

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral {
    NSLog(@"[CM] didConnectPeripheral(peripheral)");
    NSLog(@"Connection to peripheral with UUID : %s\n",[self UUIDToString:peripheral.UUID]);
   // self.activePeripheral = peripheral;
    [peripheral discoverServices:nil];
    [central stopScan];
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    NSLog(@"[CM] didUpdateState Changed %d (%s)\r\n",central.state,[self centralManagerStateToString:central.state]);

}


- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI {
    NSLog(@"[CM] didDiscoverPeripheral");
    NSLog(@"Ad data :\n%@",advertisementData);
    
    if (!self.peripherals) self.peripherals = [[NSMutableArray alloc] initWithObjects:peripheral,nil];
    
    if ([peripheral.name rangeOfString:@"RoboRoach"].location != NSNotFound) {
        NSLog(@"Found RoboRoach!...\n");
        //[self connectPeripheral:peripheral];
    } else {
        NSLog(@"Peripheral not a RoboRoach or callback was not because of a ScanResponse\n");
    }

}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error{
    NSLog(@"[CM] didDisconnectPeripheral");
    self.peripherals = nil;
    [[self delegate] didDisconnectFromRoboRoach]; //Let the UI Know it was successful.
}


//-------------------------------------------------------------------------
//
//
//CBPeripheralDelegate protocol methods beneeth here
//
//
//-------------------------------------------------------------------------

- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    UInt16 characteristicUUID = [self CBUUIDToInt:characteristic.UUID];
    if (!error) {
        switch(characteristicUUID){
            case BYB_ROBOROACH_CHAR_FREQUENCY_UUID:
            {
                char value;
                [characteristic.value getBytes:&value length:1];
                self.activeRoboRoach.frequency = [NSNumber numberWithUnsignedChar:(unsigned char)value];
                NSLog(@"[peripheral] didUpdateValueForChar Freq (%s, %@)", [self CBUUIDToString:characteristic.UUID], [NSNumber numberWithUnsignedChar:(unsigned char)value]);
                break;
            }
            case BYB_ROBOROACH_CHAR_PULSEWIDTH_UUID:
            {
                char value;
                [characteristic.value getBytes:&value length:1];
                self.activeRoboRoach.pulseWidth = [NSNumber numberWithUnsignedChar:(unsigned char)value];
                NSLog(@"[peripheral] didUpdateValueForChar PW   (%s, %@)", [self CBUUIDToString:characteristic.UUID], [NSNumber numberWithUnsignedChar:(unsigned char)value]);
                break;
            }
            case BYB_ROBOROACH_CHAR_DURATION_IN_5MS_INTERVALS_UUID:
            {
                char value;
                [characteristic.value getBytes:&value length:1];
                NSNumber *numberOf5msSteps = [NSNumber numberWithUnsignedChar:(unsigned char)value];
                self.activeRoboRoach.duration = [NSNumber numberWithInt:[numberOf5msSteps integerValue] * 5]; //Note: in 5ms steps.
                NSLog(@"[peripheral] didUpdateValueForChar Duration (%s, %@ *5ms = %@)", [self CBUUIDToString:characteristic.UUID], [NSNumber numberWithUnsignedChar:(unsigned char)value], self.activeRoboRoach.duration);
                break;
            }
            case BYB_ROBOROACH_CHAR_RANDOMMODE_UUID:
            {
                char value;
                [characteristic.value getBytes:&value length:1];
                self.activeRoboRoach.randomMode = [NSNumber numberWithBool:(bool)value];
                //[[self delegate] didFinsihReadingRoboRoachValues];
                NSLog(@"[peripheral] didUpdateValueForChar Rand (%s, %@)", [self CBUUIDToString:characteristic.UUID], [NSNumber numberWithInt:(int)value]);
                break;
            }
            case BYB_ROBOROACH_CHAR_GAIN_UUID:
            {
                char value;
                [characteristic.value getBytes:&value length:1];
                self.activeRoboRoach.gain = [NSNumber numberWithUnsignedChar:(unsigned char)value];
                NSLog(@"[peripheral] didUpdateValueForChar Gain (%s, %@)", [self CBUUIDToString:characteristic.UUID], [NSNumber numberWithUnsignedChar:(unsigned char)value]);
                break;
            }
            case BATTERY_CHAR_BATTERYLEVEL_UUID:
            {
                char value;
                [characteristic.value getBytes:&value length:1];
                self.activeRoboRoach.batteryLevel = [NSNumber numberWithInt:(int)value];
                break;
            }
            case DEVICE_INFO_CHAR_FIRMWARE_UUID:
            {
                char value;
                [characteristic.value getBytes:&value length:10];
                self.activeRoboRoach.firmwareVersion = [NSString
                    stringWithUTF8String:&value];
                break;
            }
            case DEVICE_INFO_CHAR_HARDWARE_UUID:
            {
                char value;
                [characteristic.value getBytes:&value length:10];
                self.activeRoboRoach.hardwareVersion = [NSString
                    stringWithUTF8String:&value];
                [[self delegate] didFinsihReadingRoboRoachValues];
                break;
            }
                
                
        }
    }
    else {
        NSLog(@"updateValueForCharacteristic failed !");
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error {
    NSLog(@"[peripheral] didDiscoverServices()");
    if (!error) {
        NSLog(@"Services of peripheral with UUID : %s found\r\n",[self UUIDToString:peripheral.UUID]);
        [self getAllCharacteristicsFromRoboRoach:peripheral];
    }
    else {
        NSLog(@"Service discovery was unsuccessfull !\r\n");
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error {
    NSLog(@"[peripheral] didDiscoverCharacteristicsForService(%s)",[self CBUUIDToString:service.UUID]);
    if (!error) {
        for(int i=0; i < service.characteristics.count; i++) {
            CBCharacteristic *c = [service.characteristics objectAtIndex:i];
            NSLog(@"[peripheral] Found characteristic %s",[ self CBUUIDToString:c.UUID]);
            CBService *s = [peripheral.services objectAtIndex:(peripheral.services.count - 1)];
            if([self compareCBUUID:service.UUID UUID2:s.UUID]) {
                //NSLog(@"Finished discovering characteristics");
                //[[self delegate] keyfobReady];
                NSLog(@"Finished discovering characteristics");
                
                //[self readValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_FREQUENCY_UUID];
                if ( !self.activeRoboRoach.isLoadingParameters ){
                    self.activeRoboRoach.isLoadingParameters = @1;
                    
                    [self readValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_FREQUENCY_UUID];
                    [self readValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_PULSEWIDTH_UUID];
                    [self readValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_DURATION_IN_5MS_INTERVALS_UUID];
                    [self readValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_RANDOMMODE_UUID];
                    [self readValue:BYB_ROBOROACH_SERVICE_UUID characteristicUUID:BYB_ROBOROACH_CHAR_GAIN_UUID];
                    [self readValue:BATTERY_SERVICE_UUID characteristicUUID:
                        BATTERY_CHAR_BATTERYLEVEL_UUID];
                    [self readValue:DEVICE_INFO_SERVICE_UUID characteristicUUID:
                        DEVICE_INFO_CHAR_FIRMWARE_UUID];
                    [self readValue:DEVICE_INFO_SERVICE_UUID characteristicUUID:
                        DEVICE_INFO_CHAR_HARDWARE_UUID];
                    [self.delegate roboRoachReady];
                }
            }
        }
    }
    else {
        NSLog(@"[peripheral] Error in didDiscoverCharacteristicsForService(%s)",[self CBUUIDToString:service.UUID]);
        
    }
}

/*****************************
 *                           *
 *   UUID Utility Functions  *
 *                           *
 *****************************/

-(UInt16) swap:(UInt16)s {
    UInt16 temp = s << 8;
    temp |= (s >> 8);
    return temp;
}

-(const char *) CBUUIDToString:(CBUUID *) UUID {
    return [[UUID.data description] cStringUsingEncoding:NSStringEncodingConversionAllowLossy];
}

-(const char *) UUIDToString:(CFUUIDRef)UUID {
    if (!UUID) return "NULL";
    NSLog(@"UUID To String Enter");
    //CFStringRef s = CFUUIDCreateString(NULL, UUID);
    //return CFStringGetCStringPtr(s, 0);
    return "NULL";
}

-(int) compareCBUUID:(CBUUID *) UUID1 UUID2:(CBUUID *)UUID2 {
    char b1[16];
    char b2[16];
    [UUID1.data getBytes:b1];
    [UUID2.data getBytes:b2];
    if (memcmp(b1, b2, UUID1.data.length) == 0)return 1;
    else return 0;
}

-(int) compareCBUUIDToInt:(CBUUID *)UUID1 UUID2:(UInt16)UUID2 {
    char b1[16];
    [UUID1.data getBytes:b1];
    UInt16 b2 = [self swap:UUID2];
    if (memcmp(b1, (char *)&b2, 2) == 0) return 1;
    else return 0;
}

-(UInt16) CBUUIDToInt:(CBUUID *) UUID {
    unsigned char b1[16];
    [UUID.data getBytes:b1];
    return ((b1[0] << 8) | b1[1]);
}

-(CBService *) findServiceFromUUID:(CBUUID *)UUID p:(CBPeripheral *)p {
    for(int i = 0; i < p.services.count; i++) {
        CBService *s = [p.services objectAtIndex:i];
        if ([self compareCBUUID:s.UUID UUID2:UUID]) return s;
    }
    return nil;
}

-(CBCharacteristic *) findCharacteristicFromUUID:(CBUUID *)UUID service:(CBService*)service {
    for(int i=0; i < service.characteristics.count; i++) {
        CBCharacteristic *c = [service.characteristics objectAtIndex:i];
        if ([self compareCBUUID:c.UUID UUID2:UUID]) return c;
    }
    return nil; //Characteristic not found on this service
}

- (const char *) centralManagerStateToString: (int)state{
    switch(state) {
        case CBCentralManagerStateUnknown:
            return "State unknown (CBCentralManagerStateUnknown)";
        case CBCentralManagerStateResetting:
            return "State resetting (CBCentralManagerStateUnknown)";
        case CBCentralManagerStateUnsupported:
            return "State BLE unsupported (CBCentralManagerStateResetting)";
        case CBCentralManagerStateUnauthorized:
            return "State unauthorized (CBCentralManagerStateUnauthorized)";
        case CBCentralManagerStatePoweredOff:
            return "State BLE powered off (CBCentralManagerStatePoweredOff)";
        case CBCentralManagerStatePoweredOn:
            return "State powered up and ready (CBCentralManagerStatePoweredOn)";
        default:
            return "State unknown";
    }
    return "Unknown state";
}

- (void) printPeripheralInfo:(CBPeripheral*)peripheral {
    //CFStringRef s = CFUUIDCreateString(NULL, peripheral.UUID);
    NSLog(@"------------------------------------\n");
    NSLog(@"Peripheral Info :\r\n");
    //NSLog(@"UUID : %s\n",CFStringGetCStringPtr(s, 0));
    NSLog(@"RSSI : %d\n",[peripheral.RSSI intValue]);
    NSLog(@"Name : %s\n",[peripheral.name UTF8String]);
    NSLog(@"isConnected : %d\n",peripheral.isConnected);
    NSLog(@"-------------------------------------\n");
    
}

- (void) printKnownPeripherals {
    int i;
    NSLog(@"List of currently known peripherals : \n");
    for (i=0; i < self->_peripherals.count; i++)
    {
        //CBPeripheral *p = [self->_peripherals objectAtIndex:i];
        
        //[GJG to Fix] CFUUIDCreateString crashes on new RoboRoaches.???
        
        //CFStringRef s = CFUUIDCreateString(NULL, p.UUID);
        //NSLog(@"%d  |  %s\n",i,CFStringGetCStringPtr(s, 0));
        //[self printPeripheralInfo:p];
    }
}


@end

