//
//  BYBRoboRoachViewController.m
//  RoboRoach
//
//  Created by Greg Gage on 4/13/13.
//  Copyright (c) 2013 Backyard Brains. All rights reserved.
//

#import "BYBRoboRoachViewController.h"
#import "BYBRoboRoachSettingsViewController.h"

@interface BYBRoboRoachViewController() {
    
    __weak IBOutlet UIBarButtonItem *bookmarkButton;
    __weak IBOutlet UIBarButtonItem *stimulationSettingsButton;
    
    __weak IBOutlet UISegmentedControl *bookmarkBar;
    
    IBOutlet UIActivityIndicatorView *spinner;
    __weak IBOutlet UILabel *goRight;
    __weak IBOutlet UILabel *goLeft;
    IBOutlet UIImageView *roachImage;
    IBOutlet UIImageView *backpackImage;
    IBOutlet UIImageView *batteryImage;
    
    __weak IBOutlet UILabel *stimulationSettings;
    
    IBOutlet UIButton * connectButton;
    
    BYBRoboRoachManager * rr; //RoboRoach class (private)
}
@end


@implementation BYBRoboRoachViewController

BOOL isConnected = NO;

- (void)viewDidLoad
{
    [super viewDidLoad];
    NSLog(@"viewDidLoad");
    
    self.view.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"ChartBG.png"]];
    
    //CGFloat height = [[UIScreen mainScreen] bounds].size.height;
    //CGRect frame = [roachImage frame];
    //frame.origin.y = (height-frame.size.height)/2;
    //[roachImage setFrame:frame];

    [bookmarkButton setEnabled:NO];
    
    [bookmarkBar setHidden:YES];
    [bookmarkBar setSelectedSegmentIndex:5]; //Other
    [bookmarkBar addTarget:self action:@selector(bookmarkedStimulationSelected) forControlEvents:UIControlEventValueChanged];
    
    [roachImage setHidden:NO];
    [backpackImage setHidden:YES];
    [batteryImage setHidden:YES];
    [goLeft setHidden:YES];
    [goRight setHidden:YES];
    [spinner setHidden:YES];
    
    [goLeft setFont:[UIFont fontWithName:@"Comic Book" size:18]];
    [goRight setFont:[UIFont fontWithName:@"Comic Book" size:18]];
    [stimulationSettings setFont:[UIFont fontWithName:@"Comic Book" size:13]];
    
    [stimulationSettings setHidden:YES];

    

    
    rr = [[BYBRoboRoachManager alloc] init];   // Init BYBRoboRoachManager class.
    rr.delegate = self;  //Start recieveing RoboRoach updates
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    
    NSLog(@"didReceiveMemoryWarning");
}

- (void) bookmarkedStimulationSelected {
    
        switch( bookmarkBar.selectedSegmentIndex) {
            case 0: //5Hz
                rr.activeRoboRoach.frequency = @5;
                rr.activeRoboRoach.pulseWidth = @20;
                rr.activeRoboRoach.duration = @600;
                rr.activeRoboRoach.randomMode = @0;
                break;
            case 1: //15Hz
                rr.activeRoboRoach.frequency = @15;
                rr.activeRoboRoach.pulseWidth = @20;
                rr.activeRoboRoach.duration = @600;
                rr.activeRoboRoach.randomMode = @0;
                break;
            case 2: //30Hz
                rr.activeRoboRoach.frequency = @30;
                rr.activeRoboRoach.pulseWidth = @10;
                rr.activeRoboRoach.duration = @600;
                rr.activeRoboRoach.randomMode = @0;
                break;
            case 3: //55Hz
                rr.activeRoboRoach.frequency = @55;
                rr.activeRoboRoach.pulseWidth = @9;
                rr.activeRoboRoach.duration = @600;
                rr.activeRoboRoach.randomMode = @0;
                break;
            case 4: //100Hz
                rr.activeRoboRoach.frequency = @100;
                rr.activeRoboRoach.pulseWidth = @5;
                rr.activeRoboRoach.duration = @600;
                rr.activeRoboRoach.randomMode = @0;
                break;
        }
        
    [rr sendUpdatedSettingsToActiveRoboRoach];
    [stimulationSettings setText:[rr.activeRoboRoach getStimulationString]];
    
}

- (IBAction)connectButtonClicked:(id)sender {
    
    if (!isConnected) {
        
        [spinner setHidden:NO];
        [spinner startAnimating];
        [connectButton setTitle:@"Searching..." forState:UIControlStateNormal];
        [connectButton setEnabled:NO];
                
        [rr searchForRoboRoaches:4];
        
    }else{
        [rr disconnectFromRoboRoach];
        isConnected = NO;
        
    }
}

- (void) didFinsihReadingRoboRoachValues {
    NSLog(@"didFinsihReadingRoboRoachValues");
    
    [stimulationSettings setText:[rr.activeRoboRoach getStimulationString]];
    [stimulationSettings setHidden:NO];
    
    [batteryImage setAlpha:1];
    
    if ([rr.activeRoboRoach.batteryLevel integerValue] > 90)
    {
        batteryImage.image = [UIImage imageNamed: @"battery-95.png"];
    }else if ([rr.activeRoboRoach.batteryLevel integerValue] > 80)
    {
        batteryImage.image = [UIImage imageNamed: @"battery-90.png"];
    }else if ([rr.activeRoboRoach.batteryLevel integerValue] > 70)
    {
        batteryImage.image = [UIImage imageNamed: @"battery-80.png"];
    }else if ([rr.activeRoboRoach.batteryLevel integerValue] > 60)
    {
        batteryImage.image = [UIImage imageNamed: @"battery-50.png"];
    }else if ([rr.activeRoboRoach.batteryLevel integerValue] > 50)
    {
        batteryImage.image = [UIImage imageNamed: @"battery-25.png"];
    }else{
        batteryImage.image = [UIImage imageNamed: @"battery-0.png"];
    }

    
}


- (void) didSearchForRoboRoaches: (NSArray*)foundRoboRoaches{
    NSLog(@"didSearchForRoboRoaches:foundRoboRoaches[%i]",foundRoboRoaches.count);
    
    [spinner setHidden:YES];
    [spinner stopAnimating];
    
    if (foundRoboRoaches.count > 0 ){
        [backpackImage setAlpha:0.25];
        [backpackImage setHidden:NO];
        [batteryImage setAlpha:0.25];
        [batteryImage setHidden:NO];
        
        [connectButton setTitle:@"Connecting..." forState: UIControlStateDisabled];
        [connectButton setEnabled:NO];
        
        
        //Select the first RoboRoach
        [rr connectToRoboRoach:foundRoboRoaches[0]];
        
    }
    else{
        [self didDisconnectFromRoboRoach];
    }
    
}

- (void) didDisconnectFromRoboRoach {
    NSLog(@"didDisconnectFromRoboRoach");
    isConnected = NO;
    
    
    [bookmarkButton setEnabled:NO];
    [stimulationSettingsButton setEnabled:NO];
    
    [bookmarkBar setHidden:YES];
    
    [backpackImage setHidden:YES];
    [batteryImage setHidden:YES];
    [stimulationSettings setHidden:YES];
    
    [connectButton setEnabled:YES];
    [connectButton setTitle:@"Find Signal Generator" forState:UIControlStateNormal];
    
    
    
}

- (void) roboRoachReady {
    NSLog(@"roboRoachReady");
    
    isConnected = YES;
    
    [connectButton setTitle:@"Disconnect" forState:UIControlStateNormal];
    [connectButton setEnabled:YES];
    
    [bookmarkButton setEnabled:YES];
    [stimulationSettingsButton setEnabled:YES];
    [backpackImage setAlpha:1];
    
    
    //[connectButtonBar setTitle:@"Disconnect"];
    //[connectButtonBar setEnabled:YES];

}


- (void) hadBluetoothError: (int) CMState{
    
    NSLog( @"hadBluetoothError delegate in RRViewController");

    NSString *errorText;
    
    
    switch(CMState) {
        case 0: //CBCentralManagerStateUnknown:
            errorText = @"Strange... Your bluetooth is in an unknown state.  Try restarting the application or your phone.";
            break;
        case 1: //CBCentralManagerStateResetting:
            errorText = @"It seems your BlueTooth on your phone is resetting.  Wait a few seconds and try again.";
            break;
        case 2: //CBCentralManagerStateUnsupported:
            errorText = @"Sadly, BlueTooth is Not Supported on this device.  You need an iPhone 4s or iPod Touch 5th Generation or later.";
            break;
        case 3: //CBCentralManagerStateUnauthorized:
             errorText = @"Your phone is not authorized to use Bluetooth";
            break;
        case 4: //CBCentralManagerStatePoweredOff:
             errorText = @"Your BlueTooth is currently powered off.  Please turn it on to use the RoboRoach.";
            break;
        case 5: //CBCentralManagerStatePoweredOn:
             errorText = @"State powered up and ready (CBCentralManagerStatePoweredOn)";
            break;
        default:
             errorText = @"Your BlueTooth is in an Unkown State.";
    }
    
    UIAlertView *message = [[UIAlertView alloc] initWithTitle:@"Bluetooth Error"
                                                      message:errorText
                                                     delegate:nil
                                            cancelButtonTitle:@"OK"
                                            otherButtonTitles:nil];
    [message show];
}


- (void) didConnectToRoboRoach: (BOOL)success{
    NSLog(@"didConnectToRoboRoach:success[%i]",success);
    rr.activeRoboRoach.delegate = self;
    [stimulationSettingsButton setEnabled:YES];
    isConnected = YES;
}


- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    NSLog(@"prepareForSegue");
    // Make sure your segue name in storyboard is the same as this line
    if ([[segue identifier] isEqualToString:@"roboRoachSettingsSegue"])
    {
        // Get reference to the destination view controller
        BYBRoboRoachSettingsViewController *vc = [segue destinationViewController];
        
        // Pass any objects to the view controller here, like...
        vc.roboRoach = rr.activeRoboRoach;
    }
}

- (IBAction)handlePan:(UIPanGestureRecognizer *)recognizer {
    NSLog(@"handlePan");
    

    if ( isConnected ) { 
        CGPoint translation = [recognizer translationInView:self.view];
    
        
        if (translation.x > 100 ){
            [rr.activeRoboRoach goRight];
            
        }else if (translation.x < -100 ){
            [rr.activeRoboRoach goLeft];
            
        }
        
    }
}

- (void) sideIndicatorTimer:(NSTimer *)timer {
    goRight.hidden = YES;
    goLeft.hidden = YES;
}

- (IBAction)favoritesClicked:(id)sender {
    
    if (bookmarkBar.hidden)
    {
        bookmarkBar.hidden = NO;
    }
    else
    {
        bookmarkBar.hidden = YES;
    }
    
}

//RoboRoach Delgate Methods
- (void) roboRoachHasChangedSettings:(BYBRoboRoach *)roboRoach{
    //Confusing Architecture.  Think about renaming it.
    NSLog(@"roboRoachHasChangedSettings++");
    
    [bookmarkBar setSelectedSegmentIndex:5]; //Other
    //NSLog(@"Updated Bar");
    
    [rr sendUpdatedSettingsToActiveRoboRoach];
    //NSLog(@"Finished sendUpdatedSettingsToActiveRoboRoach");
    
    [stimulationSettings setText:[rr.activeRoboRoach getStimulationString]];
    //NSLog(@"stimulationSettings Text Updated");
    
    NSLog(@"roboRoachHasChangedSettings--");
    
}

- (void) roboRoach: (BYBRoboRoach *)roboRoach hasMovementCommand:(BYBMovementCommand) command{
    
    if (command == moveLeft){
        NSLog(@"Go Left");
        goLeft.hidden = NO;
        
        [rr sendMoveCommandToActiveRoboRoach:moveLeft];
    } else if (command == moveRight){
        NSLog(@"Go Right");
        goRight.hidden = NO;
        [rr sendMoveCommandToActiveRoboRoach:moveRight];
        
    }
    
    [NSTimer scheduledTimerWithTimeInterval:ROBOROACH_TURN_TIMEOUT target:self selector:@selector(sideIndicatorTimer:) userInfo:nil repeats:NO];
}

- (void)viewDidUnload {
    NSLog(@"viewDidUnload");
    //bookmarkBar = nil;
    //bookmarkButton = nil;
    //goRight = nil;
    //goLeft = nil;
    //stimulationSettingsButton = nil;
    //stimulationSettings = nil;
    
    [super viewDidUnload];
}
@end
