//
//  BYBRoboRoachSettingsViewController.h
//  RoboRoach
//
//  Created by Greg Gage on 4/17/13.
//  Copyright (c) 2013 Backyard Brains. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <SensibleTableView/SCTableViewModel.h>
#import "BYBRoboRoachManager.h"

@interface BYBRoboRoachSettingsViewController : UIViewController <SCTableViewModelDelegate>{
    SCTableViewModel *tableViewModel;
    IBOutlet UITableView *tableView;
    __weak IBOutlet UIImageView *stimImage;
}

@property (strong, nonatomic) BYBRoboRoach * roboRoach;

@end
