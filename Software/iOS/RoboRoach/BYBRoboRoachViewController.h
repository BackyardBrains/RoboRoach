//
//  BYBRoboRoachViewController.h
//  RoboRoach
//
//  Created by Greg Gage on 4/13/13.
//  Copyright (c) 2013 Backyard Brains. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import <CoreBluetooth/CBService.h>
#import "BYBRoboRoachManager.h"



@interface BYBRoboRoachViewController : UIViewController <BYBRoboRoachManagerDelegate, BYBRoboRoachDelegate>

- (IBAction)connectButtonClicked:(id)sender ;
- (IBAction)favoritesClicked:(id)sender ;

- (IBAction)handlePan:(UIPanGestureRecognizer *)recognizer;


- (void) roboRoachHasChangedSettings:(BYBRoboRoach *)roboRoach;
- (void) roboRoach: (BYBRoboRoach *)roboRoach hasMovementCommand:(BYBMovementCommand) command;
- (void) didFinsihReadingRoboRoachValues;

@end

