/*
 *  SCInputAccessoryView.h
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



@protocol SCInputAccessoryViewDelegate;

/****************************************************************************************/
/*	class SCInputAccessoryView	*/
/****************************************************************************************/ 
/**	
 This special view provides an accessory view that automatically appears on top of the keyboard, providing navigation functionality between the different text controls. The view provides 'Next', 'Previous', and 'Done' buttons. It also provides a special 'Clear' button for cells such as SCDateCell.
 
 Sample use:
    self.tableViewModel.inputAccessoryView.rewind = FALSE;
 
 See also: SCTableViewModel
 */
@interface SCInputAccessoryView : UIView


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The toolbar holding the accessory's different buttons and controls. */
@property (nonatomic, readonly) UIToolbar *toolbar;

/** The segmented control displaying the 'Next' and 'Previous' buttons. */
@property (nonatomic, readonly) UISegmentedControl *previousNextSegmentedControl;

/** The 'Done' button control. */
@property (nonatomic, readonly) UIBarButtonItem *doneButton;

/** Set to TRUE to show the Prev and Next buttons. Default: TRUE. */
@property (nonatomic, readwrite) BOOL showPrevNextButtons;

/** Set to TRUE to show the 'Clear' button for the cells that need it (e.g. SCDateCell). Default: TRUE. */
@property (nonatomic, readwrite) BOOL showClearButton;

/** The 'Clear' button control. */
@property (nonatomic, readonly) UIBarButtonItem *clearButton;

/** Set to TRUE to rewind to the top or bottom text controls when the bottom or top is reached. Default: TRUE. */
@property (nonatomic, readwrite) BOOL rewind;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Used internally to set a delegate for the accessory view. */
@property (nonatomic, weak) id<SCInputAccessoryViewDelegate> delegate;

/** Method called internally when the 'Previous' button is tapped. */
- (void)previousTapped;

/** Method called internally when the 'Next' button is tapped. */
- (void)nextTapped;

/** Method called internally when the 'Clear' button is tapped. */
- (void)clearTapped;

/** Method called internally when the 'Done' button is tapped. */
- (void)doneTapped;

@end




/* Used internally */
@protocol SCInputAccessoryViewDelegate <NSObject>

@optional
- (void)inputAccessoryViewPreviousTapped:(SCInputAccessoryView *)inputAccessoryView;
- (void)inputAccessoryViewNextTapped:(SCInputAccessoryView *)inputAccessoryView;
- (void)inputAccessoryViewClearTapped:(SCInputAccessoryView *)inputAccessoryView;
- (void)inputAccessoryViewDoneTapped:(SCInputAccessoryView *)inputAccessoryView;

@end
