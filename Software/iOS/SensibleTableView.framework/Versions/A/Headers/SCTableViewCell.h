/*
 *  SCTableViewCell.h
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

#import <Foundation/Foundation.h>

#import "SCCellActions.h"
#import "SCDataDefinition.h"
#import "SCDataStore.h"
#import "SCViewController.h"
#import "SCTableViewController.h"
#import "SCDetailViewControllerOptions.h"
#import "SCBadgeView.h"

@class SCTableViewModel;
@class SCTableViewSection;


/****************************************************************************************/
/*	class SCTableViewCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell for SCTableViewSection. SCTableViewCell is extensively subclassed to create cells with special controls such as UITextField and UISlider.
 
 SCTableViewCell provides the user with the infrastructure to access the cell's value using the Bound Object method. This works by binding the cell to an object, called the bound object, and to a property of this object, called the bound property. The cell initializes its value from the value of the bound property, and when its values changes, it sets the property back automatically.
  
 Please note that even though a cell value is not applicable to SCTableViewCell itself, SCTableViewCell still provides the bound object functionality as a framework to all its subclasses to use.
 */
@interface SCTableViewCell : UITableViewCell <SCViewControllerDelegate, SCTableViewControllerDelegate>
{
	NSString *reuseId; //used internally
    BOOL configured; //used internally
	__weak SCTableViewModel *activeDetailModel;  // the current active detail model
	
	__weak SCTableViewModel *ownerTableViewModel;
    __weak SCTableViewSection *ownerSection;
	__weak id ownerViewControllerDelegate; 
	NSObject *boundObject;
    SCDataStore *boundObjectStore;
	NSString *boundPropertyName;
    SCDataType boundPropertyDataType;
    BOOL _isCustomBoundProperty;
	
	CGFloat height;
	BOOL editable;
	BOOL movable;
	BOOL selectable;
    BOOL enabled;
    BOOL autoDeselect;
	BOOL autoResignFirstResponder;
	UITableViewCellEditingStyle cellEditingStyle;
	SCBadgeView *badgeView;
    	
	NSArray *detailCellsImageViews;
		
	BOOL valueRequired;
	BOOL autoValidateValue;
	
	BOOL commitChangesLive;
	BOOL needsCommit;
	BOOL beingReused;
	BOOL customCell;
    BOOL isSpecialCell;
    
    SCDetailViewControllerOptions *detailViewControllerOptions;
    
    NSString *themeStyle;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCTableViewCell. */
+ (id)cell;

/** Allocates and returns an initialized SCTableViewCell given the cell's style. */
+ (id)cellWithStyle:(UITableViewCellStyle)style;

/** Allocates and returns an initialized SCTableViewCell given cell text.
 *	@param cellText The text that will appear in the cell's textLabel.
 */
+ (id)cellWithText:(NSString *)cellText;

/** Allocates and returns an initialized SCTableViewCell given cell text and its alignment.
 *	@param cellText The text that will appear in the cell's textLabel.
 *  @param textAlignment The alignment setting for the cell's text.
 */
+ (id)cellWithText:(NSString *)cellText textAlignment:(UITextAlignment)textAlignment;

/** Allocates and returns an initialized SCTableViewCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see class overview).
 *	@param propertyName The cell's bound property name (see class overview).
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object boundPropertyName:(NSString *)propertyName;


/** Returns an initialized SCTableViewCell given cell text.
 *	@param cellText The text that will appear in the cell's textLabel.
 */
- (id)initWithText:(NSString *)cellText;

/** Returns an initialized SCTableViewCell given cell text and its alignment.
 *	@param cellText The text that will appear in the cell's textLabel.
 *  @param textAlignment The alignment setting for the cell's text.
 */
- (id)initWithText:(NSString *)cellText textAlignment:(UITextAlignment)textAlignment;

/** Returns an initialized SCTableViewCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see class overview).
 *	@param propertyName The cell's bound property name (see class overview).
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object boundPropertyName:(NSString *)propertyName;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The owner table view model of the cell. 
 *
 * @warning This property gets set automatically by the framework, you should never set this property manually */
@property (nonatomic, weak) SCTableViewModel *ownerTableViewModel;

/** The owner section of the cell. 
 *
 * @warning This property gets set automatically by the framework, you should never set this property manually */
@property (nonatomic, weak) SCTableViewSection *ownerSection;

/** The set of cell action blocks. */
@property (nonatomic, readonly) SCCellActions *cellActions;

/** The cell's bound object (see class overview). */
@property (nonatomic, strong) NSObject *boundObject;

/** The data store of the cell's bound object. */
@property (nonatomic, strong) SCDataStore *boundObjectStore;

/** The cell's bound property name (see class overview). */
@property (nonatomic, copy) NSString *boundPropertyName;

/** The height of the cell. */
@property (nonatomic, readwrite) CGFloat height;

/** The editable state of the cell. Default: FALSE. */
@property (nonatomic, readwrite) BOOL editable;

/** The movable state of the cell. Default: FALSE. */
@property (nonatomic, readwrite) BOOL movable;

/** Determines if the cell can be selected. Default: TRUE. */
@property (nonatomic, readwrite) BOOL selectable;

/** Set to FALSE to disable the cell's functionality. Usefull in situations where the cell should only be enabled in 'Editing Mode' for example. Default: TRUE. */
@property (nonatomic, readwrite) BOOL enabled;

/** The cell's badge view. When assigned a text value, the badgeView displays a badge similar to the badge displayed by the iPhone's mail application for its mail folders. 
 */
@property (nonatomic, readonly) SCBadgeView *badgeView;

/** Set this property to an array of UIImageView objects to be set to each of the cell's detail cells. 
 *	@note Only applicable to cells with detail views. 
 */
@property (nonatomic, strong) NSArray *detailCellsImageViews;

/** If property is TRUE, the cell gets automatically deselected whenever the user selects it. Default: FALSE.
 */
@property (nonatomic, readwrite) BOOL autoDeselect;

/** If property is TRUE, the cell automatically dismisses the keyboard (if applicable) when another cell is selected or when the value of another cell is changed. Default: TRUE.
 */
@property (nonatomic, readwrite) BOOL autoResignFirstResponder;

/** The cell's editing style. */
@property (nonatomic, readwrite) UITableViewCellEditingStyle cellEditingStyle;

/** Determines if the cell's value is required. If TRUE, valueIsValid will only return true if the cells value is not empty. Default: FALSE. 
 */
@property (nonatomic, readwrite) BOOL valueRequired;

/** 
 Determines whether to automatically validate the cell's value. Where applicable, each subclass provides
 its own rules for validating the cell's value. If the user chooses to provide custom validation
 using either the cell's actions, or the model's SCTableViewModelDelegate, they should
 set this property to FALSE. Default: TRUE. 
 */
@property (nonatomic, readwrite) BOOL autoValidateValue;

/** 
 This property is TRUE if the cell's value is valid. The validity of the cell's value depend
 on the valueRequired and the validateValue properties. If valueRequired is TRUE, valueIsValid is
 TRUE only if the cell contains a value. If validateValue is TRUE, valueIsValid depends on
 each subclass to provide validation of the cell's value. For validation, the user can also provide 
 custom validation using either the cell's actions, or the model's SCTableViewModelDelegate. 
 */
@property (nonatomic, readonly) BOOL valueIsValid;

/** Options for the generated detail view controller (only applicable to subclasses with a detail view controller). */
@property (nonatomic, readonly) SCDetailViewControllerOptions *detailViewControllerOptions;

/** The theme style used to style the cell. A nil 'themeStyle' value will use the style having the same name of the cell's class name. If no style with the class name is found, the name of the super class will be used, and so on. Default: nil. */
@property (nonatomic, copy) NSString *themeStyle;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Cell Value
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Set this property to TRUE for the cell to commit its value as soon as it is changed.
 If this value is FALSE, the user must explicitly call commitChanges
 for the cell to commit its value changes. Default: TRUE. 
 */
@property (nonatomic, readwrite) BOOL commitChangesLive;

/** 
 This propery is TRUE if the cell value has changed and needs to be committed. If commitChangesLive
 is TRUE, this property always remains FALSE. 
 */
@property (nonatomic, readonly) BOOL needsCommit;

/** 
 Commits any changes to the cell's value either to the bound object or the bound key where
 applicable (see class overview). 
 */
- (void)commitChanges;

/** 
 Reload's the cell's bound value in case the associated boundProperty value has changed
 by means other than the cell itself (e.g. external custom code). 
 */
- (void)reloadBoundValue;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Misc. Properties
//////////////////////////////////////////////////////////////////////////////////////////

/** Is TRUE if the cell is a special cell. Special cells are cells like addNewItemCell or placeholderCell and typically exist within an SCArrayOfItemsSection. */
@property (nonatomic, readonly) BOOL isSpecialCell;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Property should only be used internally by framework. */
@property (nonatomic, readwrite) SCDataType boundPropertyDataType;

/** 
 This property represents the bound value of the cell.  
 */
@property (nonatomic, strong) NSObject *boundValue;

/** Property used internally by framework to change reuseIdentifier after the cell has been created. */
@property (nonatomic, copy) NSString *reuseId;

/** Property used internally. */
@property (nonatomic, readwrite) BOOL configured;

/** Property used internally by framework to determine if cell is being dequed and reused. */
@property (nonatomic, readwrite) BOOL beingReused;

/** Property used internally by framework to determine if cell is a custom cell. */
@property (nonatomic, readwrite) BOOL customCell;

/** Method should be overridden by subclasses to perform any required initialization.
 @warning Subclasses must call [super performInitialization] from within the method's implementation.
 */
- (void)performInitialization;

/** Method called internally. */
- (void)setIsCustomBoundProperty:(BOOL)custom;

/** Method called internally. */
- (void)setNeedsCommit:(BOOL)needsCommit;

/**	
 Method gets called internally whenever the cell value changes. 
 
 This method should only be used
 when subclassing 'SCTableViewCell'. If what you want is to get notified when a cell value changes,
 consider using either cell's actions or SCTableViewModelDelegate methods. 
 */
- (void)cellValueChanged;

/**	
 Method gets called internally whenever the cell is about to be displayed.
 
 This method should only be used
 when subclassing 'SCTableViewCell'. If what you want is to get notified when a cell is about to be displayed,
 consider using either the cell's actions or SCTableViewModelDelegate methods. 
 */
- (void)willDisplay;

/**	
 Method gets called internally whenever the cell gets selected. 
 
 This method should only be used
 when subclassing 'SCTableViewCell'. If what you want is to get notified when a cell gets selected,
 consider using either the cell's actions or SCTableViewModelDelegate methods. 
 */
- (void)didSelectCell;

/**	Method gets called internally whenever the cell is about to be deselected. */
- (void)willDeselectCell;

/**	Method gets called internally whenever the cell is deselected. */
- (void)didDeselectCell;

/** Marks the cell as a special cell. */
- (void)markCellAsSpecial;

/** 
 Method should be overridden by subclasses to support property attributes. 
 
 The method should be able to set the subclass' specific attributes to its corresponding SCPropertyAttributes subclass. */
- (void) setAttributesTo:(SCPropertyAttributes *)attributes;

/** 
 Method should be overridden by subclasses to provide subclass specific validation of the cell's value. 
 
 If what you want is to be able to provide custom cell value validation,
 consider using either the cell's actions or SCTableViewModelDelegate methods. 
 */
- (BOOL)getValueIsValid;

/** Method is called internally by the framework before a detail view appears. */
- (void)prepareCellForDetailViewAppearing;

/** Method is called internally by the framework before a detail view disappears. */
- (void)prepareCellForDetailViewDisappearing;

/** Method called internally. */
- (void)setDetailViewControllerOptions:(SCDetailViewControllerOptions *)options;

/** Method called internally. */
- (SCTableViewModel *)modelForViewController:(UIViewController *)viewController;

/** Subclasses should override this method to change the returned default detail view controller navigation bar type. */
- (SCNavigationBarType)defaultDetailViewControllerNavigationBarType;

/** Subclasses should call this method to get a detail view controller. */
- (UIViewController *)getDetailViewControllerForCell:(SCTableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath allowUITableViewControllerSubclass:(BOOL)allowUITableViewController;

/** Subclasses should call this method to present their detail view controller. */
- (void)presentDetailViewController:(UIViewController *)detailViewController forCell:(SCTableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath withPresentationMode:(SCPresentationMode)mode;

/** Subclasses should override this method to build a detail model for the detail view controller. */
- (void)buildDetailModel:(SCTableViewModel *)detailModel;

/** Subclasses should override this method to commit the detail model changes. */
- (void)commitDetailModelChanges:(SCTableViewModel *)detailModel;

/** Subclasses should override this method to handle their detail view controller willPresent. Superclass method must be called. */
- (void)handleDetailViewControllerWillPresent:(UIViewController *)detailViewController;

/** Subclasses should override this method to handle their detail view controller didPresent. Superclass method must be called. */
- (void)handleDetailViewControllerDidPresent:(UIViewController *)detailViewController;

/** Subclasses should override this method to handle their detail view controller shouldDismiss. Superclass method must be called. */
- (BOOL)handleDetailViewControllerShouldDismiss:(UIViewController *)detailViewController cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;

/** Subclasses should override this method to handle their detail view controller willDismiss. Superclass method must be called. */
- (void)handleDetailViewControllerWillDismiss:(UIViewController *)detailViewController cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;

/** Subclasses should override this method to handle their detail view controller didDismiss. Superclass method must be called. */
- (void)handleDetailViewControllerDidDismiss:(UIViewController *)detailViewController cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;

@end








/****************************************************************************************/
/*	class SCCustomCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a base class for all user defined custom cells. 
 
 The following is a summary of each control that can be automatically bound, in addition to the corresponding boundObject property value type that it 
 must be associated with:
 
 - UILabel: NSString, NSNumber, NSDate 
 - UITextField: NSString, NSNumber, NSDate
 - UITextView: NSString, NSNumber, NSDate
 - UISlider: NSNumber, NSString
 - UISegmentedControl: NSNumber, NSString
 - UISwitch: NSNumber
 
 */

@interface SCCustomCell : SCTableViewCell <UITextViewDelegate, UITextFieldDelegate>
{
    BOOL _pauseControlEvents;
    
    NSMutableDictionary *_objectBindings;
    
    BOOL _autoResize;
    BOOL _showClearButtonInInputAccessoryView;
}


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Allocates and returns an initialized 'SCCustomCell' given cell text, custom object bindings, and a nib file name. Use this type of constructor in cases where the bound object is not yet known and will be provided later by the framework. A common case is when implementing the SCTableViewModelDataSource method called 'tableViewModel:cellForRowAtIndexPath:'.
 @note This constructor is usually used to construct custom cells that are either created in Interface Builder, or created by subclassing 'SCCustomCell'.
 @param cellText The text that will appear in the cell's textLabel.
 @param bindings The cell's object bindings. This dictionary specifies how each
 of the cell's custom controls binds itself to its boundObject's properties. Each dictionary key
 should be the tag string value of one of the cell's custom controls, and the value should be the 
 name of the boundObject's property that is bound to that control. 
 @warning All control tags must be greater than zero.
 @param nibName	The name of the nib file to load the cell from. The nib file should only contain
 one cell, and it should be a subclass of 'SCControlCell'. 
 @note it's ok for this parameter to be nil if the cell has no corresponding nib file.
 */
+ (id)cellWithText:(NSString *)cellText objectBindings:(NSDictionary *)bindings nibName:(NSString *)nibName;

/** 
 Allocates and returns an initialized 'SCCustomCell' given cell text, custom object bindings string, and a nib file name. Use this type of constructor in cases where the bound object is not yet known and will be provided later by the framework. A common case is when implementing the SCTableViewModelDataSource method called 'tableViewModel:cellForRowAtIndexPath:'.
 @note This constructor is usually used to construct custom cells that are either created in Interface Builder, or created by subclassing 'SCCustomCell'.
 @param cellText The text that will appear in the cell's textLabel.
 @param bindingsString The cell's object bindings string. This string specifies how each
 of the cell's custom controls binds itself to its boundObject's properties. The string's format should be as follows: @"tag1:propertyName1;tag2:propertyName2;tag3:propertyName3" 
 @warning All control tags must be greater than zero.
 @param nibName	The name of the nib file to load the cell from. The nib file should only contain
 one cell, and it should be a subclass of 'SCControlCell'. 
 @note it's ok for this parameter to be nil if the cell has no corresponding nib file.
 */
+ (id)cellWithText:(NSString *)cellText objectBindingsString:(NSString *)bindingsString nibName:(NSString *)nibName;

/** 
 Allocates and returns an initialized 'SCControlCell' given cell text, bound object, custom object bindings,
 and a nib file name. 
 @note This constructor is usually used to construct custom cells that are either created in Interface Builder, or created by subclassing 'SCControlCell'.
 @param cellText The text that will appear in the cell's textLabel.
 @param object	The cell's bound object (see SCTableViewCell class overview).
 @param bindings The cell's object bindings. This dictionary specifies how each
 of the cell's custom controls binds itself to the boundObject's properties. Each dictionary key
 should be the tag string value of one of the cell's custom controls, and the value should be the 
 name of the boundObject's property that is bound to that control. 
 @warning All control tags must be greater than zero.
 @param nibName	The name of the nib file to load the cell from. The nib file should only contain
 one cell, and it should be a subclass of 'SCControlCell'. 
 @note It's ok for this parameter to be nil if the cell has no corresponding nib file.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object objectBindings:(NSDictionary *)bindings nibName:(NSString *)nibName;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 This dictionary specifies how each of the cell's custom controls binds itself to the boundObject's properties. 
 
 Each dictionary key
 should be the tag string value of one of the cell's custom controls, and the value should be the 
 name of the boundObject's property that is bound to that control. 
 
 @warning All control tags must be greater than zero.
 @note This property can be used interchangibly with objectBindingsString
 */
@property (nonatomic, readonly) NSMutableDictionary *objectBindings;

/**
 This string specifies how each of the cell's custom controls binds itself to the boundObject's properties. The string should consist of a colon separated pair of control's tag and the property name, while each pair should be separated by a semi-colon.
 
 Example bindings string:
    NSString *bindingsString = @"1:firstName;2:lastName";
 @warning IMPORTANT: All control tags must be greater than zero.
 @note This property can be used interchangibly with objectBindings
 */
@property (nonatomic, copy) NSString *objectBindingsString;

/**	Determines if cell should automatically resize to fit its contents. Default: TRUE. */
@property (nonatomic, readwrite) BOOL autoResize;

/** Show the 'Clear' button in the model's inputAccessoryView. */
@property (nonatomic, readwrite) BOOL showClearButtonInInputAccessoryView;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Custom Control Values
//////////////////////////////////////////////////////////////////////////////////////////

/** Returns the control with the given tag value. Returns nil if controlTag is less than 1. */
- (UIView *)controlWithTag:(NSInteger)controlTag;

/** Returns the bound value for the control with the given tag value. Returns nil if controlTag is less than 1. */
- (NSObject *)boundValueForControlWithTag:(NSInteger)controlTag;

/** Commits the bound value for the control with the given tag value. */
- (void)commitValueForControlWithTag:(NSInteger)controlTag value:(NSObject *)controlValue;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** An array of all the cell's input controls sorted by their tag value. Property is used internally by the framework. */
@property (nonatomic, readonly) NSArray *inputControlsSortedByTag;

/**	Method must be called by subclasses to configure any added custom controls for automatic binding. */
- (void)configureCustomControls;

/** Method called internally by framework. */
- (void)loadBindingsIntoCustomControls;

/** Method gets called internally whenever the value of a UITextField control is changed. */
- (void)textFieldEditingChanged:(id)sender;

/** Method gets called internally whenever the value of a UISlider control is changed. */
- (void)sliderValueChanged:(id)sender;

/** Method gets called internally whenever the value of a UISegmentedControl is changed. */
- (void)segmentedControlValueChanged:(id)sender;

/** Method gets called internally whenever the value of a UISwitch control is changed. */
- (void)switchControlChanged:(id)sender;

/** Method gets called internally whenever the a custom button is tapped. */
- (void)customButtonTapped:(id)sender;


@end







/****************************************************************************************/
/*	class SCControlCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a base class for all cells that require user controls to be
 present inside their content view. 
 */
 
@interface SCControlCell : SCCustomCell  
{	
	UIView *control;
	
	CGFloat maxTextLabelWidth;
	CGFloat controlIndentation;
	CGFloat controlMargin;
    BOOL showClearButtonInInputAccessoryView;
}


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The control associated with the cell. Even though this property is readonly, feel free to customize any of the control's properties.
 *
 *	@note The type of the control has been choosen to be of type UIView instead of UIControl because not all user controls decend from UIControl (e.g. UITextView). 
 */
@property (nonatomic, readonly) UIView *control;

/** The value of contained in the cell's control. */
@property (nonatomic, readonly) NSObject *controlValue;

/** The maximum width of the cell's textLabel. Default: 200. */
@property (nonatomic, readwrite) CGFloat maxTextLabelWidth;

/**	
 The indentation of the control from the cell's left border. Default: 120. 
 
 This indentation keeps the control at a specific distance from the cell's border unless the textLabel's text exceeds this distance, in which case the control is moved accordingly to the right. 
 */
@property (nonatomic, readwrite) CGFloat controlIndentation;

/** The margin between the control and the cell's textLabel. Default: 10. */
@property (nonatomic, readwrite) CGFloat controlMargin;

/** Clears the control associated with the cell. */
- (void)clearControl;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/**	Method should be overridden by subclasses to load the cell's bound value into their controls. */
- (void)loadBoundValueIntoControl;



@end







/****************************************************************************************/
/*	class SCLabelCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell with a UILabel control. The bound property of this cell can be of any type the decends from NSObject.
 */

@interface SCLabelCell : SCControlCell
{
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCLabelCell' given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UILabel's text. Property must be of type NSString and can be a readonly property.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object labelTextPropertyName:(NSString *)propertyName;


/** Returns an initialized 'SCLabelCell' given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UILabel's text. Property must be of type NSString and can be a readonly property.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object labelTextPropertyName:(NSString *)propertyName;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The UILabel control associated with the cell. Even though this property is readonly, feel free to customize any of the control's properties. 
 */
@property (nonatomic, readonly) UILabel *label;

@end



/****************************************************************************************/
/*	class SCTextViewCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell with a UITextView control. The bound property of this cell must be of type NSString.
 */

@interface SCTextViewCell : SCControlCell
{
	CGFloat minimumHeight;
	CGFloat maximumHeight;
	
	@private
	BOOL initializing;
    float prevContentHeight;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCTextViewCell' given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UITextView's text. Property must be a readwrite property of type NSString.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object textViewTextPropertyName:(NSString *)propertyName;


/** Returns an initialized 'SCTextViewCell' given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UITextView's text. Property must be a readwrite property of type NSString.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object textViewTextPropertyName:(NSString *)propertyName;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The UITextView control associated with the cell. Even though this property is readonly, feel free to customize any of the control's properties. */
@property (nonatomic, readonly) UITextView *textView;

/** The minimum height for textView. */
@property (nonatomic, readwrite) CGFloat minimumHeight;

/** The maximum height for textView. */
@property (nonatomic, readwrite) CGFloat maximumHeight;


@end




/****************************************************************************************/
/*	class SCTextFieldCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell with a UITextField control. The bound property of this cell must be of type NSString.
 */

@interface SCTextFieldCell : SCControlCell 
{
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCTextFieldCell' given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param placeholder The placeholder string for UITextField.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UITextField's text. Property must be a readwrite property of type NSString.
 */
+ (id)cellWithText:(NSString *)cellText placeholder:(NSString *)placeholder boundObject:(NSObject *)object textFieldTextPropertyName:(NSString *)propertyName;


/** Returns an initialized 'SCTextFieldCell' given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param placeholder The placeholder string for UITextField.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UITextField's text. Property must be a readwrite property of type NSString.
 */
- (id)initWithText:(NSString *)cellText placeholder:(NSString *)placeholder boundObject:(NSObject *)object textFieldTextPropertyName:(NSString *)propertyName;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The UITextField control associated with the cell. Even though this property is readonly, feel free to customize any of the control's properties. 
 */
@property (nonatomic, readonly) UITextField *textField;


@end




/****************************************************************************************/
/*	class SCNumericTextFieldCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell with a UITextField control with numeric values. The bound property of this cell must be of type NSNumber, int, float, or double. SCNumericTextFieldCell defines its own validation rules according to the following criteria:
 - Value is valid only if it's a numeric value.
 - If minimumValue is not nil, then value is only valid if it's greater than or equal to this value.
 - If maximumValue is not nil, then value is only valid if it's less than or equal to this value.
 - If allowFloatValue is FALSE, then value is only valid if it's an integer value.
 */

@interface SCNumericTextFieldCell : SCTextFieldCell
{
	NSNumber *minimumValue;
	NSNumber *maximumValue;
	BOOL allowFloatValue;
	BOOL displayZeroAsBlank;
    NSNumberFormatter *numberFormatter;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The minimum valid cell value. */
@property (nonatomic, copy) NSNumber *minimumValue;

/** The maximum valid cell value. */
@property (nonatomic, copy) NSNumber *maximumValue;

/** If FALSE, only integer cell values are valid. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowFloatValue;

/** If TRUE, an empty space is displayed if the bound value equals zero. Default: FALSE. */
@property (nonatomic, readwrite) BOOL displayZeroAsBlank;

/** The number formatter responsible for converting the cell's numeric value to a string and vice versa. */
@property (nonatomic, readonly) NSNumberFormatter *numberFormatter;


@end





/****************************************************************************************/
/*	class SCSliderCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell with a UISlider control. The bound property of this cell must be of type NSNumber, int, float, or double.
 */

@interface SCSliderCell : SCControlCell
{
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCSliderCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UISlider's value. Property must be a readwrite property of type NSNumber.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object sliderValuePropertyName:(NSString *)propertyName;


/** Returns an initialized SCSliderCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UISlider's value. Property must be a readwrite property of type NSNumber.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object sliderValuePropertyName:(NSString *)propertyName;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The UISlider control associated with the cell. Even though this property is readonly, feel free to customize any of the control's properties. 
 */
@property (nonatomic, readonly) UISlider *slider;


@end




/****************************************************************************************/
/*	class SCSegmentedCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell with a UISegmentedControl. The bound property of this cell must be of type NSNumber or int.
 */
@interface SCSegmentedCell : SCControlCell
{
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCSegmentedCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UISegmentedControl's selectedSegmentIndex value. Property must be a readwrite property of type NSNumber.
 *	@param cellSegmentTitlesArray An array containing title strings for UISegmentedControl's segments.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object selectedSegmentIndexPropertyName:(NSString *)propertyName segmentTitlesArray:(NSArray *)cellSegmentTitlesArray;


/** Returns an initialized SCSegmentedCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UISegmentedControl's selectedSegmentIndex value. Property must be a readwrite property of type NSNumber.
 *	@param cellSegmentTitlesArray An array containing title strings for UISegmentedControl's segments.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object selectedSegmentIndexPropertyName:(NSString *)propertyName segmentTitlesArray:(NSArray *)cellSegmentTitlesArray;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The UISegmentedControl associated with the cell. Even though this property is readonly, feel free to customize any of the control's properties. 
 */
@property (nonatomic, readonly) UISegmentedControl *segmentedControl;

/**	Method creates segmented control segments based on the content of the segmentTitlesArray.
 *	@param segmentTitlesArray Must be an array of NSString objects. */
- (void)createSegmentsUsingArray:(NSArray *)segmentTitlesArray;


@end



/****************************************************************************************/
/*	class SCSwitchCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell with a UISwitch control. The bound property of this cell must be of type NSNumber or BOOL.
 */
@interface SCSwitchCell : SCControlCell
{
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCSwitchCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UISwitch's on value. Property must be a readwrite property of type NSNumber.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object switchOnPropertyName:(NSString *)propertyName;


/** Returns an initialized SCSwitchCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the UISwitch's on value. Property must be a readwrite property of type NSNumber.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object switchOnPropertyName:(NSString *)propertyName;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The UISwitch control associated with the cell. Even though this property is readonly, feel free to customize any of the control's properties. */
@property (nonatomic, readonly) UISwitch *switchControl;


@end





/****************************************************************************************/
/*	class SCDateCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell that provides the end-user with a UIDatePicker to choose a date with. The bound property of this cell must be of type NSDate.
 */
@interface SCDateCell : SCLabelCell <SCViewControllerDelegate>
{
	//internal
	UITextField *pickerField;
    UIViewController *_activePickerDetailViewController;
	
	UIDatePicker *datePicker;
	NSDateFormatter *dateFormatter;
	BOOL displaySelectedDate;
	BOOL displayDatePickerInDetailView;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCDateCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the cell's date selection. Property must be a readwrite property of type NSDate.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object datePropertyName:(NSString *)propertyName;


/** Returns an initialized SCDateCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the cell's date selection. Property must be a readwrite property of type NSDate.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object datePropertyName:(NSString *)propertyName;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The UIDatePicker control associated with the cell. Even though this property is readonly, feel free to customize any of the control's properties. */
@property (nonatomic, readonly) UIDatePicker *datePicker;

/** Set to customize how the cell display's the selected date. */
@property (nonatomic, strong) NSDateFormatter *dateFormatter;

/** If TRUE, the cell displays the selected date in a left aligned label. Default: TRUE. */
@property (nonatomic, readwrite) BOOL displaySelectedDate;

/** 
 Set to TRUE to display the date picker in its own detail view, instead of displaying it in
 the current view. Default: FALSE. 
 */
@property (nonatomic, readwrite) BOOL displayDatePickerInDetailView;

@end





/****************************************************************************************/
/*	class SCImagePickerCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell that provides the end-user with an automatically generated
 detail view of type UIImagePickerController to pick an image with. As soon as the image is picked,
 it will be saved to the Documents folder using either an auto generated name from the current time stamp,
 or a name provided through SCTableViewModelDelegate or the cell's actions. 
 The bound property of this cell must be of type NSString, and will be 
 bound to the name of the selected image. Once an image is selected, tapping an SCImagePickerCell
 will display the image in a detail view. To select a different image, the table view must be put in edit
 mode (make sure UITableView's allowsSelectionDuringEditing property is TRUE). Alternatively, make
 sure that displayClearImageButtonInDetailView property is TRUE, and the user will be able to
 tap a "Clear Image" button to clear the selected image.
 */
@interface SCImagePickerCell : SCTableViewCell <UINavigationControllerDelegate,
												UIImagePickerControllerDelegate,
												UIActionSheetDelegate,
												SCViewControllerDelegate>
{
	// Internal
	UIImage *cachedImage;  
	UIImageView *detailImageView; 

	UIPopoverController *popover;

	
	UIImagePickerController *imagePickerController;
	NSString *placeholderImageName;
	NSString *placeholderImageTitle;
	BOOL displayImageNameAsCellText;
	BOOL askForSourceType;
	UIButton *clearImageButton;
	BOOL displayClearImageButtonInDetailView;
	BOOL autoPositionClearImageButton;
	BOOL autoPositionImageView;
	
	CGRect textLabelFrame;
	CGRect imageViewFrame;
	
	NSString *selectedImageName;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCImagePickerCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the cell's selected image name. Property must be a readwrite property of type NSString.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object imageNamePropertyName:(NSString *)propertyName;


/** Returns an initialized SCImagePickerCell given cell text, bound object, and a bound property name. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the cell's selected image name. Property must be a readwrite property of type NSString.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object imageNamePropertyName:(NSString *)propertyName;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The UIImagePickerController associated with the cell. Even though this property is readonly, feel free to customize any of the controller's relevant properties. 
 */
@property (nonatomic, readonly) UIImagePickerController *imagePickerController;

/** The selected image. Returns nil if no image is selected. */
@property (nonatomic, readonly) UIImage *selectedImageObject;

/** The name for an image in the documents folder that will be used as a placeholder if no image is selected. 
 */
@property (nonatomic, copy) NSString *placeholderImageName;

/** The title for the image specified in the placeholderImageName property. */
@property (nonatomic, copy) NSString *placeholderImageTitle;

/** If TRUE, the selected image name will be displayed in the cell's textLabel. Default: TRUE. */
@property (nonatomic) BOOL displayImageNameAsCellText;

/** 
 If TRUE, the user will be asked for the source type, otherwise, the source type will be
 determined from imagePickerController's sourceType property. Default: TRUE.
 @note If the device has no camera, setting this to TRUE automatically sets the sourceType 
 to UIImagePickerControllerSourceTypePhotoLibrary. 
 */
@property (nonatomic) BOOL askForSourceType;

/** A button that clears the currently selected image when tapped. Feel free to customize the default look and feel of this button. */
@property (nonatomic, readonly) UIButton *clearImageButton;

/** If TRUE, clearImageButton will be displayed in the cell's detail view. Default: TRUE. */
@property (nonatomic) BOOL displayClearImageButtonInDetailView;

/** If TRUE, clearImageButton will be automatically positioned in the cell's detail view. Default: TRUE. */
@property (nonatomic) BOOL autoPositionClearImageButton;

/** Set this property to customize the placement of the cell's textLabel. */
@property (nonatomic, readwrite) CGRect textLabelFrame;

/** Set this property to customize the placement of the cell's imageView. */
@property (nonatomic, readwrite) CGRect imageViewFrame;


/** The name of the selected image. */
@property (nonatomic, copy) NSString *selectedImageName;


/** Resets the clearImageButton default layer styles such as corneRadius and borderWidth. */
- (void)resetClearImageButtonStyles;

@end







/****************************************************************************************/
/*	class SCSelectionCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell that provides the end-user with an automatically generated detail view of options to choose from, much like the Ringtone selection cell in the iPhone's sound settings. The selection items are provided in the form of an array of NSStrings, called the items array. 'SCSelectionCell' can be configured to allow multiple selection and to allow no selection at all. the bound property of this cell must be of type NSNumber, NSString, or NSMutableSet. If allowMultipleSelection is TRUE, then only NSString and NSMutableSet property types are allowed.
 
 @see SCSelectionSection.
 */
@interface SCSelectionCell : SCLabelCell
{
	SCDataStore *selectionItemsStore;
    SCDataFetchOptions *selectionItemsFetchOptions;
    
	BOOL allowMultipleSelection;
	BOOL allowNoSelection;
	NSUInteger maximumSelections;
	BOOL autoDismissDetailView;
	BOOL hideDetailViewNavigationBar;
    
    BOOL allowAddingItems;
	BOOL allowDeletingItems;
	BOOL allowMovingItems;
	BOOL allowEditDetailView;
	
	BOOL displaySelection;
	NSString *delimeter;
	
	NSMutableSet *selectedItemsIndexes;
    
    SCTableViewCell *placeholderCell;
    SCTableViewCell *addNewItemCell;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCSelectionCell given cell text, bound object, an NSNumber bound property name, and an array of selection items.
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the selected item index. Property must be of an NSNumber type and cannot be a readonly property.
 *	@param cellItems An array of the items that the user will choose from. All items must be of an NSString type.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object selectedIndexPropertyName:(NSString *)propertyName items:(NSArray *)cellItems;

/** Allocates and returns an initialized SCSelectionCell given cell text, bound object, a bound property name, an array of selection items, and whether to allow multiple selection. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the selected items indexes set. Property must be of an NSMutableSet type and can be a readonly property.
 *	@param cellItems An array of the items that the user will choose from. All items must be of an NSString type.
 *	@param multipleSelection Determines if multiple selection is allowed.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object selectedIndexesPropertyName:(NSString *)propertyName items:(NSArray *)cellItems allowMultipleSelection:(BOOL)multipleSelection;

/** Allocates and returns an initialized SCSelectionCell given cell text, bound object, an NSString bound property name, and an array of selection items.
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the selected item string. Property must be of an NSString type and cannot be a readonly property.
 *	@param cellItems An array of the items that the user will choose from. All items must be of an NSString type.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object selectionStringPropertyName:(NSString *)propertyName items:(NSArray *)cellItems;


/** Returns an initialized SCSelectionCell given cell text, bound object, an NSNumber bound property name, and an array of selection items.
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the selected item index. Property must be of an NSNumber type and cannot be a readonly property.
 *	@param cellItems An array of the items that the user will choose from. All items must be of an NSString type.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object selectedIndexPropertyName:(NSString *)propertyName items:(NSArray *)cellItems;

/** Returns an initialized SCSelectionCell given cell text, bound object, a bound property name, an array of selection items, and whether to allow multiple selection. 
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the selected items indexes set. Property must be of an NSMutableSet type and can be a readonly property.
 *	@param cellItems An array of the items that the user will choose from. All items must be of an NSString type.
 *	@param multipleSelection Determines if multiple selection is allowed.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object selectedIndexesPropertyName:(NSString *)propertyName items:(NSArray *)cellItems allowMultipleSelection:(BOOL)multipleSelection;

/** Returns an initialized SCSelectionCell given cell text, bound object, an NSString bound property name, and an array of selection items.
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the selected item string. Property must be of an NSString type and cannot be a readonly property.
 *	@param cellItems An array of the items that the user will choose from. All items must be of an NSString type.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object selectionStringPropertyName:(NSString *)propertyName items:(NSArray *)cellItems;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The data store for the selection items. */
@property (nonatomic, strong) SCDataStore *selectionItemsStore;

/** The fetch options for the selection items. */
@property (nonatomic, strong) SCDataFetchOptions *selectionItemsFetchOptions;

/** The items stored in selectionItemsStore. */
@property (nonatomic, strong) NSArray *items;

/** This property reflects the current cell's selection. You can set this property to define the cell's selection.
 
 @note If you have bound this cell to an object or a key, you can define the cell's selection using either the bound property value or the key value, respectively.
 */
@property (nonatomic, copy) NSNumber *selectedItemIndex;

/** This property reflects the current cell's selection(s). You can add index(es) to the set to define the cell's selection.
 
 @note If you have bound this cell to an object or a key, you can define the cell's selection using either the bound property value or the key value, respectively. 
 */
@property (nonatomic, readonly) NSMutableSet *selectedItemsIndexes;

/** If TRUE, the cell allows multiple selection. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowMultipleSelection;

/** If TRUE, the cell allows no selection at all. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowNoSelection;

/** The maximum number of items that can be selected. Set to zero to allow an infinite number of selections. Default: 0.
 *	@note Only applicable when allowMultipleSelection is TRUE.  
 */
@property (nonatomic, readwrite) NSUInteger maximumSelections;

/** If TRUE, the detail view is automatically dismissed when an item is selected. Default: FALSE. */
@property (nonatomic, readwrite) BOOL autoDismissDetailView;

/** If TRUE, the detail view's navigation bar is hidder. Default: FALSE. 
 *	@note Only applicable when autoDismissDetailView is TRUE. 
 */
@property (nonatomic, readwrite) BOOL hideDetailViewNavigationBar;

/**	Allows/disables adding new objects to the items array. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowAddingItems;

/** Allows/disables deleting new objects from the items array. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowDeletingItems;

/** Allows/disables moving an item from one row to another. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowMovingItems;

/** Allows/disables editing detail views for items. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowEditDetailView;

/** If TRUE, the cell displays the selected items' titles inside the cell in a left aligned label. Default: TRUE. */
@property (nonatomic, readwrite) BOOL displaySelection;

/** The delimeter that separates the titles of the selected items. Default: @" ,".
 *	
 *	@note This property is applicable only if displaySelection is TRUE. 
 */
@property (nonatomic, copy) NSString *delimeter;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuring Special Cells
//////////////////////////////////////////////////////////////////////////////////////////

/** When set to a valid cell object, 'placeholderCell' will be displayed when no items exist in the generated section. As soon as any items are added, this cell automatically disappears. Default: nil. */
@property (nonatomic, strong) SCTableViewCell *placeholderCell;

/** When set to a valid cell object, 'addNewItemCell' will be displayed as the last cell of the generated section, and will add a new item to the section whenever it gets tapped by the user. This cell can be used as an alternative to addButtonItem. Default: nil.
 */
@property (nonatomic, strong) SCTableViewCell *addNewItemCell;



@end





/****************************************************************************************/
/*	class SCObjectSelectionCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell that provides the end-user with an automatically generated
 detail view of objects to choose from. 
 
 The selection items are provided in the form of an array of
 NSObjects, called the items array. 'SCObjectSelectionCell' can be configured to allow multiple
 selection and to allow no selection at all. If allow multiple selection is disabled, then
 the bound property name of this cell must be of type NSObject, otherwise
 it must be of type NSMutableSet. 
 
 @see SCObjectSelectionSection.
*/
@interface SCObjectSelectionCell : SCSelectionCell
{
	SCDataDefinition *intermediateEntityDefinition;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCObjectSelectionCell given cell text, bound object, a bound property name, and a selection items store.
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the object selection. If multiple selection is allowed, then property must be of an NSMutableSet type, otherwise, property must be of type NSObject and cannot be a readonly property.
 *	@param store The store containing the selection objects.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object selectedObjectPropertyName:(NSString *)propertyName selectionItemsStore:(SCDataStore *)store;

/** Allocates and returns an initialized SCObjectSelectionCell given cell text, bound object, a bound property name, and an array of selection objects.
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the object selection. If multiple selection is allowed, then property must be of an NSMutableSet type, otherwise, property must be of type NSObject and cannot be a readonly property.
 *	@param items An array of the objects that the user will choose from. All objects must be of an NSObject type and must be instances of the same class.
 *	@param itemsDefinition The definition of the selection objects.
 */
+ (id)cellWithText:(NSString *)cellText boundObject:(NSObject *)object selectedObjectPropertyName:(NSString *)propertyName selectionItems:(NSArray *)items itemsDefintion:(SCDataDefinition *)definition;


/** Returns an initialized SCObjectSelectionCell given cell text, bound object, a bound property name, and a selection items store.
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the object selection. If multiple selection is allowed, then property must be of an NSMutableSet type, otherwise, property must be of type NSObject and cannot be a readonly property.
 *	@param store The store containing the selection objects.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object selectedObjectPropertyName:(NSString *)propertyName selectionItemsStore:(SCDataStore *)store;

/** Returns an initialized SCObjectSelectionCell given cell text, bound object, a bound property name, and an array of selection objects.
 *	@param cellText The text that will appear in the cell's textLabel.
 *	@param object	The cell's bound object (see SCTableViewCell class overview).
 *	@param propertyName The cell's bound property name corresponding to the object selection. If multiple selection is allowed, then property must be of an NSMutableSet type, otherwise, property must be of type NSObject and cannot be a readonly property.
 *	@param objects An array of the objects that the user will choose from. All objects must be of an NSObject type and must be instances of the same class.
 *	@param objectsDefinition The definition of the selection objects.
 */
- (id)initWithText:(NSString *)cellText boundObject:(NSObject *)object selectedObjectPropertyName:(NSString *)propertyName selectionItems:(NSArray *)items itemsDefintion:(SCDataDefinition *)definition;;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** Set this to the definition of the intermediate entity between the bound object's definition and the objectsDefinition. This is useful in complex many-to-many relationships where you have created an intermediate entity between you main two entities. Default: nil. */
@property (nonatomic, strong) SCDataDefinition *intermediateEntityDefinition;


@end





/****************************************************************************************/
/*	class SCObjectCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell that is able to automatically generate a detail view with cells generated
 from a given bound object's properties. 
 
 If the bound object is given without an extended
 class definition (SCClassDefinition), then the cells will only be generated for properties of type 
 NSString and NSNumber, and will be either of type SCTextFieldCell or SCNumericTextFieldCell,
 respectively. If an SCClassDefinition is provided for the bound object, a full fledged
 detail view of cells will be generated.
 
 When SCObjectCell is selected by the end-user, a detail view optionally fires up to to give
 the user the ability to edit the object's properties.
 
 @see SCObjectSection, SCArrayOfObjectsSection.
 */
@interface SCObjectCell : SCTableViewCell
{
	NSString *boundObjectTitleText;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCObjectCell given a bound object.
 *
 *	@param object The object that SCObjectCell will use to generate its detail view cells.
 */
+ (id)cellWithBoundObject:(NSObject *)object;

/** Allocates and returns an initialized SCObjectCell given a bound object and a class definition.
 *
 *	@param object The object that SCObjectCell will use to generate its detail view cells.
 *	@param classDefinition The class definition for the object.
 */
+ (id)cellWithBoundObject:(NSObject *)object boundObjectDefinition:(SCDataDefinition *)definition;

/** Allocates and returns an initialized SCObjectCell given a bound object and a data store.
 *
 *	@param object The object that SCObjectCell will use to generate its detail view cells.
 *	@param store The data store containing the object.
 */
+ (id)cellWithBoundObject:(NSObject *)object boundObjectStore:(SCDataStore *)store;

/** Returns an initialized 'SCObjectCell' given a bound object.
 *
 *	@param object The object that 'SCObjectCell' will use to generate its detail view cells.
 */
- (id)initWithBoundObject:(NSObject *)object;

/** Returns an initialized 'SCObjectCell' given a bound object and a class definition.
 *
 *	@param object The object that 'SCObjectCell' will use to generate its detail view cells.
 *	@param classDefinition The class definition for the object.
 */
- (id)initWithBoundObject:(NSObject *)object boundObjectDefinition:(SCDataDefinition *)definition;

/** Returns an initialized SCObjectCell given a bound object and a data store.
 *
 *	@param object The object that SCObjectCell will use to generate its detail view cells.
 *	@param store The data store containing the object.
 */
- (id)initWithBoundObject:(NSObject *)object boundObjectStore:(SCDataStore *)store;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The class definition of the bound object. */
@property (nonatomic, strong) SCDataDefinition *objectDefinition;

/** 
 The bound object title that will appear in the cell's textLabel.
 
 If this value is nil, 'SCObjectCell' uses the value of the title property found
 in objectDefinition. Set this value to override the default behaviour and
 provide your own title text for the object.
 */
@property (nonatomic, copy) NSString *boundObjectTitleText; 


@end





/****************************************************************************************/
/*	class SCArrayOfObjectsCell	*/
/****************************************************************************************/ 
/**	
 This class functions as a cell that, given an array of objects, will generate an SCArrayOfObjectsSection detail view that's displayed when the cell is tapped. 
 
 The cell can also be optionally bound to an object and generate an additional SCObjectSection detail view when the cell is tapped while the table is in edit mode.
 
 @see SCObjectCell, SCObjectSection, SCArrayOfObjectsSection.
 */
@interface SCArrayOfObjectsCell : SCTableViewCell
{
	SCDataStore *dataStore;
    SCDataFetchOptions *dataFetchOptions;
    
	BOOL allowAddingItems;
	BOOL allowDeletingItems;
	BOOL allowMovingItems;
	BOOL allowEditDetailView;
	BOOL allowRowSelection;
	BOOL autoSelectNewItemCell;
	BOOL displayItemsCountInBadgeView;
    
    SCTableViewCell *placeholderCell;
    SCTableViewCell *addNewItemCell;
    BOOL addNewItemCellExistsInNormalMode;
    BOOL addNewItemCellExistsInEditingMode;
    
    SCSectionActions *_detailSectionActions;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCArrayOfObjectsCell given a data store.
 *
 *	@param store The data store containing the cell's objects.
 */
+ (id)cellWithDataStore:(SCDataStore *)store;

/** Allocates and returns an initialized SCArrayOfObjectsCell given an array of objects and their class definition.
 *
 *	@param cellItems An array of objects that the cell will use to generate its detail cells.
 *	@param definition The definition of the objects in the objects array. If the array contains more than one type of object, then their respective definitions must be added to the itemsClassDefinitions dictionary after initialization.
 */
+ (id)cellWithItems:(NSMutableArray *)cellItems itemsDefinition:(SCDataDefinition *)definition;


/** Returns an initialized SCArrayOfObjectsCell given a data store.
 *
 *	@param store The data store containing the cell's objects.
 */
- (id)initWithDataStore:(SCDataStore *)store;

/** Returns an initialized SCArrayOfObjectsCell given an array of objects and their class definition.
 *
 *	@param cellItems An array of objects that the cell will use to generate its detail cells.
 *	@param classDefinition The class definition of the class or entity of the objects in the objects array. If the array contains more than one type of object, then their respective class definitions must be added to the itemsClassDefinitions dictionary after initialization.
 */
- (id)initWithItems:(NSMutableArray *)cellItems itemsDefinition:(SCDataDefinition *)definition;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The data store that's used to store and fetch the cell's objects. */
@property (nonatomic, strong) SCDataStore *dataStore;

/** The options used to fetch the cell's items from dataStore. */
@property (nonatomic, strong) SCDataFetchOptions *dataFetchOptions;

/** The items fetched from the dataStore. */
@property (nonatomic, readonly) NSArray *items;

/**	Allows/disables adding new objects to the items array. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowAddingItems;

/** Allows/disables deleting new objects from the items array. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowDeletingItems;

/** Allows/disables moving objects from one row to another. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowMovingItems;

/** Allows/disables editing detail views for objects. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowEditDetailView;

/** Allows/disables row selection. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowRowSelection;

/** Allows/disables automatic cell selection of newly created items. Default: FALSE. */
@property (nonatomic, readwrite) BOOL autoSelectNewItemCell;

/** Allows/disables displaying the number of objects in the cell's badgeView. Default: TRUE. */
@property (nonatomic, readwrite) BOOL displayItemsCountInBadgeView;

/** The set of actions for the cell's automatically generated detail section. */
@property (nonatomic, readonly) SCSectionActions *detailSectionActions;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuring Special Cells
//////////////////////////////////////////////////////////////////////////////////////////

/** When set to a valid cell object, 'placeholderCell' will be displayed when no items exist in the generated section. As soon as any items are added, this cell automatically disappears. Default: nil. */
@property (nonatomic, strong) SCTableViewCell *placeholderCell;

/** 
 When set to a valid cell object, 'addNewItemCell' will be displayed as the last cell of the generated section, and will add a new item to the section whenever it gets tapped by the user. This cell can be used as an alternative to the section's addButtonItem. Default: nil.
 */
@property (nonatomic, strong) SCTableViewCell *addNewItemCell;

/** When TRUE, addNewItemCell will be displayed in Normal Mode. Default: TRUE. */
@property (nonatomic, readwrite) BOOL addNewItemCellExistsInNormalMode;

/** When TRUE, addNewItemCell will be displayed in Editing Mode. Default: TRUE. */
@property (nonatomic, readwrite) BOOL addNewItemCellExistsInEditingMode;


@end











