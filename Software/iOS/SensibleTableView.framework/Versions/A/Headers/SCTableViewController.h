/*
 *  SCTableViewController.h
 *  Sensible TableView
 *  Version: 3.3.0
 *
 *
 *	THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY UNITED STATES 
 *	INTELLECTUAL PROPERTY LAW AND INTERNATIONAL TREATIES. UNAUTHORIZED REPRODUCTION OR 
 *	DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES. YOU SHALL NOT DEVELOP NOR
 *	MAKE AVAILABLE ANY WORK THAT COMPETES WITH A SENSIBLE COCOA PRODUCT DERIVED FROM THIS 
 *	SOURCE CODE. THIS SOURCE CODE MAY NOT BE RESOLD OR REDISTRIBUTED ON A STAND ALONE BASIS.
 *
 *	USAGE OF THIS SOURCE CODE IS BOUND BY THE LICENSE AGREEMENT PROVIDED WITH THE 
 *	DOWNLOADED PRODUCT.
 *
 *  Copyright 2012-2013 Sensible Cocoa. All rights reserved.
 *
 *
 *	This notice may not be removed from this file.
 *
 */

#import <UIKit/UIKit.h>
#import "SCGlobals.h"
#import "SCViewControllerTypedefs.h"
#import "SCTableViewControllerActions.h"

@class SCTableViewModel;


/****************************************************************************************/
/*	class SCTableViewController	*/
/****************************************************************************************/ 
/**
 This class functions as a means to simplify development with SCTableViewModel.
 
 SCTableViewController conveniently provides several ready made navigation
 bar types based on SCNavigationBarType, provided that it is a subview of a navigation controller. 
 SCTableViewController also defines placeholders for a tableView and a tableViewModel that
 the user can allocate and assign. If a tableViewModel is defined, SCTableViewController also
 connects its doneButton (if present) to tableViewModel's commitButton automatically.
 
 In addition, SCTableViewController fully manages memory warnings and makes sure the assigned table view is released once a memory warning occurs and reloaded once the view controller is loaded once more.
 
 Finally, SCTableViewController provides several useful actions (SCTableViewControllerActions) and delegate methods (SCTableViewControllerDelegate) that notify the delegate object of events like the view appearing or disappearing.
 
 @note You do NOT have to use SCTableViewController in order to be able to use SCTableViewModel, but it's highly recommended that you do so whenever you need a UITableViewController.
 
 */


@interface SCTableViewController : UITableViewController <UIPopoverControllerDelegate>
{
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** Contains a valid SCTableViewModel that is associated with tableView and ready to use. If this model is replaced by a custom one, the class will automatically take care of associating it with tableView. */
@property (nonatomic, strong) SCTableViewModel *tableViewModel;

/** The type of the navigation bar. */
@property (nonatomic, readwrite) SCNavigationBarType navigationBarType;

/** The navigation bar's Add button. Only contains a value if the button exists on the bar. */
@property (nonatomic, readonly) UIBarButtonItem *addButton;

/** The editButtonItem of 'SCTableViewController''s superclass. */
@property (nonatomic, readonly) UIBarButtonItem *editButton;

/** The navigation bar's Cancel button. Only contains a value if the button exists on the bar. */
@property (nonatomic, readonly) UIBarButtonItem *cancelButton;

/** Set to TRUE to allow the cancel button to appear when entering editing mode. Default: TRUE.
 @note: Only applicable if navigationBarType == SCNavigationBarTypeEditRight. */
@property (nonatomic, readwrite) BOOL allowEditingModeCancelButton;

/** The navigation bar's Done button. Only contains a value if the button exists on the bar. */
@property (nonatomic, readonly) UIBarButtonItem	*doneButton;

/** If the view controller is presented from within a popover controller, this property must be set to it. When set, the view controller takes over the delegate of the popover controller. */
@property (nonatomic, strong) UIPopoverController *popoverController;

/** The set of view controller action blocks. */
@property (nonatomic, readonly) SCTableViewControllerActions *actions;

/** The current state of the view controller. */
@property (nonatomic, readonly) SCViewControllerState state;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Button Events
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Property is TRUE if the view controller have been dismissed due to the user tapping the
 Cancel button. This property is useful if you do not with to subclass this view controller. 
 See also SCTableViewControllerDelegate to get notified when the view controller is dismissed. 
 */
@property (nonatomic, readonly) BOOL cancelButtonTapped;

/** 
 Property is TRUE if the view controller have been dismissed due to the user tapping the
 Done button. This property is useful if you do not with to subclass this view controller. 
 See also SCTableViewControllerDelegate to get notified when the view controller is dismissed.
 */
@property (nonatomic, readonly) BOOL doneButtonTapped;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Delegate
//////////////////////////////////////////////////////////////////////////////////////////

/** The object that acts as the delegate of 'SCTableViewController'. The object must adopt the SCTableViewControllerDelegate protocol. */
@property (nonatomic, weak) id delegate;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Returns TRUE if the view controller currently has been given focus by its master model. */
@property (nonatomic, readonly) BOOL hasFocus;

/** Method should be overridden by subclasses to perform any required initialization.
 @warning Subclasses must call [super performInitialization] from within the method's implementation.
 */
- (void)performInitialization;

/** 
 Method gets called when the Cancel button is tapped. If what you want is to get notified
 when the Cancel button gets tapped without subclassing 'SCTableViewController', consider
 using SCTableViewControllerDelegate. 
 */
- (void)cancelButtonAction;

/** 
 Method gets called when the Done button is tapped. If what you want is to get notified
 when the Cancel button gets tapped without subclassing 'SCTableViewController', consider
 using SCTableViewControllerDelegate.
 */
- (void)doneButtonAction;

/** Method gets called when the Edit button is tapped. */
- (void)editButtonAction;

/** Method gets called when the Cancel button is tapped while the table view is in editing mode. */
- (void)editingModeCancelButtonAction;

/** Dismisses the view controller with the specified values for cancel and done. */
- (void)dismissWithCancelValue:(BOOL)cancelValue doneValue:(BOOL)doneValue;

/** Called by master model to have the view controller gain focus. */
- (void)gainFocus;

/** Called by master model to have the view controller lose focus. */
- (void)loseFocus;

@end



/****************************************************************************************/
/*	protocol SCTableViewControllerDelegate	*/
/****************************************************************************************/ 
/**
 This protocol should be adopted by objects that want to mediate as a delegate for 
 SCTableViewController. All methods for this protocol are optional.
 */
@protocol SCTableViewControllerDelegate

@optional

/** Notifies the delegate that the view controller will appear.
 *	@param tableViewController The view controller informing the delegate of the event.
 */
- (void)tableViewControllerWillAppear:(SCTableViewController *)tableViewController;

/** Notifies the delegate that the view controller has appeared.
 *	@param tableViewController The view controller informing the delegate of the event.
 */
- (void)tableViewControllerDidAppear:(SCTableViewController *)tableViewController;

/** Notifies the delegate that the view controller will disappear.
 *	@param tableViewController The view controller informing the delegate of the event.
 */
- (void)tableViewControllerWillDisappear:(SCTableViewController *)tableViewController;

/** Notifies the delegate that the view controller has disappeared.
 *	@param tableViewController The view controller informing the delegate of the event.
 */
- (void)tableViewControllerDidDisappear:(SCTableViewController *)tableViewController;

/** Notifies the delegate that the view controller will be presented.
 *	@param tableViewController The view controller informing the delegate of the event.
 */
- (void)tableViewControllerWillPresent:(SCTableViewController *)tableViewController;

/** Notifies the delegate that the view controller has been presented.
 *	@param tableViewController The view controller informing the delegate of the event.
 */
- (void)tableViewControllerDidPresent:(SCTableViewController *)tableViewController;

/** Asks the delegate if the view controller should be dismissed.
 *	@param tableViewController The view controller informing the delegate of the event.
 *	@param cancelTapped TRUE if Cancel button has been tapped to dismiss the view controller.
 *	@param doneTapped TRUE if Done button has been tapped to dismiss the view controller.
 *  @return Retrun TRUE to have the view controller dismissed, otherwise return FALSE.
 */
- (BOOL)tableViewControllerShouldDismiss:(SCTableViewController *)tableViewController
                 cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;

/** Notifies the delegate that the view controller will be dismissed.
 *	@param tableViewController The view controller informing the delegate of the event.
 *	@param cancelTapped TRUE if Cancel button has been tapped to dismiss the view controller.
 *	@param doneTapped TRUE if Done button has been tapped to dismiss the view controller.
 */
- (void)tableViewControllerWillDismiss:(SCTableViewController *)tableViewController
               cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;

/** Notifies the delegate that the view controller has been dismissed.
 *	@param tableViewController The view controller informing the delegate of the event.
 *	@param cancelTapped TRUE if Cancel button has been tapped to dismiss the view controller.
 *	@param doneTapped TRUE if Done button has been tapped to dismiss the view controller.
 */
- (void)tableViewControllerDidDismiss:(SCTableViewController *)tableViewController
                   cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;



// Internal

/** Notifies the delegate that the view controller will gain focus from master model.
 *	@param tableViewController The view controller informing the delegate of the event.
 */
- (void)tableViewControllerWillGainFocus:(SCTableViewController *)tableViewController;

/** Notifies the delegate that the view controller did gain focus from master model.
 *	@param tableViewController The view controller informing the delegate of the event.
 */
- (void)tableViewControllerDidGainFocus:(SCTableViewController *)tableViewController;

/** Notifies the delegate that the view controller will lose focus to its master model.
 *	@param tableViewController The view controller informing the delegate of the event.
 *	@param cancelTapped TRUE if Cancel button has been tapped to dismiss the view controller.
 *	@param doneTapped TRUE if Done button has been tapped to dismiss the view controller.
 */
- (void)tableViewControllerWillLoseFocus:(SCTableViewController *)tableViewController
                      cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;

/** Notifies the delegate that the view controller did lose focus to its master model.
 *	@param tableViewController The view controller informing the delegate of the event.
 *	@param cancelTapped TRUE if Cancel button has been tapped to dismiss the view controller.
 *	@param doneTapped TRUE if Done button has been tapped to dismiss the view controller.
 */
- (void)tableViewControllerDidLoseFocus:(SCTableViewController *)tableViewController
                     cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;

/** Notifies the delegate that the view controller did enter editing mode. */
- (void)tableViewControllerDidEnterEditingMode:(SCTableViewController *)viewController;

/** Notifies the delegate that the view controller did exit editing mode.
 *	@param tableViewController The view controller informing the delegate of the event.
 *	@param cancelTapped TRUE if Cancel button has been tapped to exit editing mode.
 *	@param doneTapped TRUE if Done button has been tapped to exit editing mode.
 */
- (void)tableViewControllerDidExitEditingMode:(SCTableViewController *)tableViewController
                           cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;

@end
