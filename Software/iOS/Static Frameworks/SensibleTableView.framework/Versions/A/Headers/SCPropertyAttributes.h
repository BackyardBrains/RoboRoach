/*
 *  SCPropertyAttributes.h
 *  Sensible TableView
 *  Version: 3.1.8
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
#import "SCGlobals.h"


@class SCDataDefinition;
@class SCDataStore;
@class SCDataFetchOptions;
@class SCSectionActions;


/****************************************************************************************/
/*	class SCPropertyAttributes	*/
/****************************************************************************************/ 
/**	
 This class is an abstract base class that represents a set of SCPropertyDefinition attributes.
  
 Each subclass of 'SCPropertyAttributes' is used to extend the definition of an SCPropertyDefinition
 instance according to its respective type. Set the attributes property of an SCPropertyDefinition instance 
 to a subclass to be able to further customize the user interface element that will be generated for
 this property definition.

 @note You should never make instances of this class. Use subclasses instead.
 */
@interface SCPropertyAttributes : NSObject 
{
	UIImageView *imageView;
	NSArray *imageViewArray;
    BOOL expandContentsInCurrentView;
}

/** The image view assigned to the generated UI element. */ 
@property (nonatomic, strong) UIImageView *imageView;

/** 
 The array of image views assigned to the detail elements of the generated UI element.
 
 The property is applicable to property definitions of type SCPropertyTypeSelection,
 SCPropertyTypeObject, and SCPropertyTypeArrayOfObjects. 
 */
@property (nonatomic, strong) NSArray *imageViewArray;

/** When TRUE, the generated user interface element will expand its content in the current view, instead of generating its own detail view. Default: FALSE. 
 *  @note Not applicable for all property definition types.
 */
@property (nonatomic) BOOL expandContentInCurrentView;

@end

/****************************************************************************************/
/*	class SCTextViewAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type 
 SCPropertyTypeTextView, thus allowing further customization of the generated control by the user.
 */
@interface SCTextViewAttributes : SCPropertyAttributes
{
	CGFloat minimumHeight;
	CGFloat maximumHeight;
	BOOL editable;
	BOOL autoResize;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCTextViewAttributes'.
 *
 *	@param minHeight The minimum height of the generated UITextView control. Set to a negative value to ignore.
 *	@param maxHeight The maximum height of the generated UITextView control. Set to a negative value to ignore.
 *	@param _autoResize Determines whether the generated UITextView control will auto resize.
 *	@param _editable Determines whether the generated UITextView control will be editable.
 */
+ (id)attributesWithMinimumHeight:(CGFloat)minHeight maximumHeight:(CGFloat)maxHeight
					   autoResize:(BOOL)_autoResize editable:(BOOL)_editable;

/** Returns an initialized 'SCTextViewAttributes'.
 *
 *	@param minHeight The minimum height of the generated UITextView control. Set to a negative value to ignore.
 *	@param maxHeight The maximum height of the generated UITextView control. Set to a negative value to ignore.
 *	@param _autoResize Determines whether the generated UITextView control will auto resize.
 *	@param _editable Determines whether the generated UITextView control will be editable.
 */
- (id)initWithMinimumHeight:(CGFloat)minHeight maximumHeight:(CGFloat)maxHeight
				 autoResize:(BOOL)_autoResize editable:(BOOL)_editable;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The minimum height of the generated UITextView control. Set to a negative value to ignore. */
@property (nonatomic, readwrite) CGFloat minimumHeight;

/** The maximum height of the generated UITextView control. Set to a negative value to ignore. */
@property (nonatomic, readwrite) CGFloat maximumHeight;

/** Determines whether the generated UITextView control will auto resize. */
@property (nonatomic, readwrite) BOOL autoResize;

/** Determines whether the generated UITextView control will be editable. */
@property (nonatomic, readwrite) BOOL editable;

@end



/****************************************************************************************/
/*	class SCTextFieldAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type 
 SCPropertyTypeTextField, thus allowing further customization of the generated control by the user.
 */
@interface SCTextFieldAttributes : SCPropertyAttributes
{
	NSString *placeholder;
    BOOL secureTextEntry;
    UITextAutocorrectionType autocorrectionType;
    UITextAutocapitalizationType autocapitalizationType;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCTextFieldAttributes'.
 *
 *	@param _placeholder The placeholder of the generated UITextField control. Set to nil to ignore.
 */
+ (id)attributesWithPlaceholder:(NSString *)_placeholder;

/** Allocates and returns an initialized 'SCTextFieldAttributes'.
 *
 *	@param _placeholder The placeholder of the generated UITextField control. Set to nil to ignore.
 *  @param secure Identifies whether the text field should hide the text being entered.
 *  @param autocorrection The auto-correction style for the text field.
 *  @param autocapitalization The auto-capitalization style for the text field.
 */
+ (id)attributesWithPlaceholder:(NSString *)_placeholder secureTextEntry:(BOOL)secure autocorrectionType:(UITextAutocorrectionType)autocorrection autocapitalizationType:(UITextAutocapitalizationType)autocapitalization;

/** Returns an initialized 'SCTextFieldAttributes'.
 *
 *	@param _placeholder The placeholder of the generated UITextField control. Set to nil to ignore.
 */
- (id)initWithPlaceholder:(NSString *)_placeholder;

/** Returns an initialized 'SCTextFieldAttributes'.
 *
 *	@param _placeholder The placeholder of the generated UITextField control. Set to nil to ignore.
 *  @param secure Identifies whether the text field should hide the text being entered.
 *  @param autocorrection The auto-correction style for the text field.
 *  @param autocapitalization The auto-capitalization style for the text field.
 */
- (id)initWithPlaceholder:(NSString *)_placeholder secureTextEntry:(BOOL)secure autocorrectionType:(UITextAutocorrectionType)autocorrection autocapitalizationType:(UITextAutocapitalizationType)autocapitalization;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The placeholder of the generated UITextField control. Set to nil to ignore. */
@property (nonatomic, copy) NSString *placeholder;

/** Identifies whether the text field should hide the text being entered. Default: FALSE. */
@property (nonatomic, readwrite) BOOL secureTextEntry;

/** The auto-correction style for the text field. Default: UITextAutocorrectionTypeDefault. */
@property (nonatomic, readwrite) UITextAutocorrectionType autocorrectionType;

/** The auto-capitalization style for the text field. Default: UITextAutocapitalizationTypeSentences. */
@property (nonatomic, readwrite) UITextAutocapitalizationType autocapitalizationType;

@end



/****************************************************************************************/
/*	class SCNumericTextFieldAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type 
 SCPropertyTypeNumericTextField, thus allowing further customization of the generated control by the user.
 */
@interface SCNumericTextFieldAttributes : SCTextFieldAttributes
{
	NSNumber *minimumValue;
	NSNumber *maximumValue;
	BOOL allowFloatValue;
    NSNumberFormatter *numberFormatter;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCNumericTextFieldAttributes'.
 *
 *	@param minValue The minimum value allowed for the generated numeric text field control. Set to nil to ignore.
 *	@param maxValue The maximum value allowed for the generated numeric text field control. Set to nil to ignore.
 *	@param allowFloat Determines if the generated numeric text field control allows float values.
 */
+ (id)attributesWithMinimumValue:(NSNumber *)minValue maximumValue:(NSNumber *)maxValue
				 allowFloatValue:(BOOL)allowFloat;

/** Allocates and returns an initialized 'SCNumericTextFieldAttributes'.
 *
 *	@param minValue The minimum value allowed for the generated numeric text field control. Set to nil to ignore.
 *	@param maxValue The maximum value allowed for the generated numeric text field control. Set to nil to ignore.
 *	@param allowFloat Determines if the generated numeric text field control allows float values.
 *	@param _placeholder The placeholder of the generated numeric text field control. Set to nil to ignore.
 */
+ (id)attributesWithMinimumValue:(NSNumber *)minValue maximumValue:(NSNumber *)maxValue
				 allowFloatValue:(BOOL)allowFloat placeholder:(NSString *)_placeholder;

/** Returns an initialized 'SCNumericTextFieldAttributes'.
 *
 *	@param minValue The minimum value allowed for the generated numeric text field control. Set to nil to ignore.
 *	@param maxValue The maximum value allowed for the generated numeric text field control. Set to nil to ignore.
 *	@param allowFloat Determines if the generated numeric text field control allows float values.
 */
- (id)initWithMinimumValue:(NSNumber *)minValue maximumValue:(NSNumber *)maxValue
				 allowFloatValue:(BOOL)allowFloat;

/** Returns an initialized 'SCNumericTextFieldAttributes'.
 *
 *	@param minValue The minimum value allowed for the generated numeric text field control. Set to nil to ignore.
 *	@param maxValue The maximum value allowed for the generated numeric text field control. Set to nil to ignore.
 *	@param allowFloat Determines if the generated numeric text field control allows float values.
 *	@param _placeholder The placeholder of the generated numeric text field control. Set to nil to ignore.
 */
- (id)initWithMinimumValue:(NSNumber *)minValue maximumValue:(NSNumber *)maxValue
				 allowFloatValue:(BOOL)allowFloat placeholder:(NSString *)_placeholder;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The minimum value allowed for the generated numeric text field control. Set to nil to ignore. */
@property (nonatomic, copy) NSNumber *minimumValue;

/** The maximum value allowed for the generated numeric text field control. Set to nil to ignore. */
@property (nonatomic, copy) NSNumber *maximumValue;

/** Determines if the generated numeric text field control allows float values. */
@property (nonatomic, readwrite) BOOL allowFloatValue;

/** The number formatter responsible for converting the numeric value to a string and vice versa. */
@property (nonatomic, readonly) NSNumberFormatter *numberFormatter;

@end



/****************************************************************************************/
/*	class SCSliderAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type 
 SCPropertyTypeSlider, thus allowing further customization of the generated control by the user.
 */
@interface SCSliderAttributes : SCPropertyAttributes
{
	float minimumValue;
	float maximumValue;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCSliderAttributes'.
 *
 *	@param minValue The minimum value of the generated UISlider control. Set to nil to ignore.
 *	@param maxValue The maximum value of the generated UISlider control. Set to nil to ignore.
 */
+ (id)attributesWithMinimumValue:(float)minValue maximumValue:(float)maxValue;

/** Returns an initialized 'SCSliderAttributes'.
 *
 *	@param minValue The minimum value of the generated UISlider control. Set to nil to ignore.
 *	@param maxValue The maximum value of the generated UISlider control. Set to nil to ignore.
 */
- (id)initWithMinimumValue:(float)minValue maximumValue:(float)maxValue;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The minimum value of the generated UISlider control. Set to nil to ignore. */
@property (nonatomic, readwrite) float minimumValue;

/** The maximum value of the generated UISlider control. Set to nil to ignore. */
@property (nonatomic, readwrite) float maximumValue;


@end



/****************************************************************************************/
/*	class SCSegmentedAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type 
 SCPropertyTypeSegmented, thus allowing further customization of the generated control by the user.
 */
@interface SCSegmentedAttributes : SCPropertyAttributes
{
	NSArray *segmentTitlesArray;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCSegmentedAttributes'.
 *
 *	@param titles The segment titles of the generated UISegmentedControl. Set to nil to ignore.
 */
+ (id)attributesWithSegmentTitlesArray:(NSArray *)titles;

/** Returns an initialized 'SCSegmentedFieldAttributes'.
 *
 *	@param titles The segment titles of the generated UISegmentedControl. Set to nil to ignore.
 */
- (id)initWithSegmentTitlesArray:(NSArray *)titles;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The segment titles of the generated UISegmentedControl. Set to nil to ignore. */
@property (nonatomic, strong) NSArray *segmentTitlesArray;

@end



/****************************************************************************************/
/*	class SCDateAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type 
 SCPropertyTypeDate, thus allowing further customization of the generated control by the user.
 */
@interface SCDateAttributes : SCPropertyAttributes
{
	NSDateFormatter *dateFormatter;
	UIDatePickerMode datePickerMode;
	BOOL displayDatePickerInDetailView;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCDateAttributes'.
 *
 *	@param formatter The date formatter used to display the date of generated control. Set to nil to ignore.
 */
+ (id)attributesWithDateFormatter:(NSDateFormatter *)formatter;

/** Allocates and returns an initialized 'SCDateAttributes'.
 *
 *	@param formatter The date formatter used to display the date of generated control. Set to nil to ignore.
 *	@param mode The date picker mode.
 *	@param inDetailView Set to TRUE to display the date picker in an automatically generated detail view.
 */
+ (id)attributesWithDateFormatter:(NSDateFormatter *)formatter
				   datePickerMode:(UIDatePickerMode)mode
	displayDatePickerInDetailView:(BOOL)inDetailView;

/** Returns an initialized 'SCDateAttributes'.
 *
 *	@param formatter The date formatter used to display the date of generated control. Set to nil to ignore.
 */
- (id)initWithDateFormatter:(NSDateFormatter *)formatter;

/** Returns an initialized 'SCDateAttributes'.
 *
 *	@param formatter The date formatter used to display the date of generated control. Set to nil to ignore.
 *	@param mode The date picker mode.
 *	@param inDetailView Set to TRUE to display the date picker in an automatically generated detail view.
 */
- (id)initWithDateFormatter:(NSDateFormatter *)formatter
			 datePickerMode:(UIDatePickerMode)mode
	displayDatePickerInDetailView:(BOOL)inDetailView;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The date formatter used to display the date of generated control. Set to nil to ignore. */
@property (nonatomic, strong) NSDateFormatter *dateFormatter;

/** The date picker mode. Default:UIDatePickerModeDateAndTime. */
@property (nonatomic, readwrite) UIDatePickerMode datePickerMode;

/** Set to TRUE to display the date picker in an automatically generated detail view. Default: FALSE. */
@property (nonatomic, readwrite) BOOL displayDatePickerInDetailView;

@end



/****************************************************************************************/
/*	class SCSelectionAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type 
 SCPropertyTypeSelection, thus allowing further customization of the generated control by the user.
 */
@interface SCSelectionAttributes : SCPropertyAttributes
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
	BOOL allowEditingItems;
    NSObject *placeholderuiElement;
    NSObject *addNewObjectuiElement;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCSelectionAttributes'.
 *
 *	@param _items The items of the generated selection control. Set to nil to ignore.
 *	@param allowMultipleSel Determines if the generated selection control allows multiple selection.
 *	@param allowNoSel Determines if the generated selection control allows no selection.
 */
+ (id)attributesWithItems:(NSArray *)_items allowMultipleSelection:(BOOL)allowMultipleSel
		 allowNoSelection:(BOOL)allowNoSel;

/** Allocates and returns an initialized 'SCSelectionAttributes'.
 *
 *	@param _items The items of the generated selection control. Set to nil to ignore.
 *	@param allowMultipleSel Determines if the generated selection control allows multiple selection.
 *	@param allowNoSel Determines if the generated selection control allows no selection.
 *	@param autoDismiss Set to TRUE to automatically dismiss the selection detail view when an item is selected.
 *	@param hideNavBar Set to TRUE to hide the detail view's navigation bar. @note Only applicable if autoDismiss is TRUE.
 */
+ (id)attributesWithItems:(NSArray *)_items allowMultipleSelection:(BOOL)allowMultipleSel
		 allowNoSelection:(BOOL)allowNoSel autoDismissDetailView:(BOOL)autoDismiss
			hideDetailViewNavigationBar:(BOOL)hideNavBar;

/** Returns an initialized 'SCSelectionAttributes'.
 *
 *	@param _items The items of the generated selection control. Set to nil to ignore.
 *	@param allowMultipleSel Determines if the generated selection control allows multiple selection.
 *	@param allowNoSel Determines if the generated selection control allows no selection.
 */
- (id)initWithItems:(NSArray *)_items allowMultipleSelection:(BOOL)allowMultipleSel
		 allowNoSelection:(BOOL)allowNoSel;

/** Returns an initialized 'SCSelectionAttributes'.
 *
 *	@param _items The items of the generated selection control. Set to nil to ignore.
 *	@param allowMultipleSel Determines if the generated selection control allows multiple selection.
 *	@param allowNoSel Determines if the generated selection control allows no selection.
 *	@param autoDismiss Set to TRUE to automatically dismiss the selection detail view when an item is selected.
 *	@param hideNavBar Set to TRUE to hide the detail view's navigation bar. @note Only applicable if autoDismiss is TRUE.
 */
- (id)initWithItems:(NSArray *)_items allowMultipleSelection:(BOOL)allowMultipleSel
		 allowNoSelection:(BOOL)allowNoSel autoDismissDetailView:(BOOL)autoDismiss
			hideDetailViewNavigationBar:(BOOL)hideNavBar;


/** The data store for the selection items. */
@property (nonatomic, strong) SCDataStore *selectionItemsStore;

/** The fetch options for the selection items. */
@property (nonatomic, strong) SCDataFetchOptions *selectionItemsFetchOptions;

/** The items stored in selectionItemsStore. */
@property (nonatomic, readonly) NSArray *items;

/** Determines if the generated selection control allows multiple selection. */
@property (nonatomic, readwrite) BOOL allowMultipleSelection;

/** Determines if the generated selection control allows no selection. */
@property (nonatomic, readwrite) BOOL allowNoSelection;

/** The maximum number of items that can be selected. Set to zero to allow an infinite number of selections. Default: 0.
 *	@note Only applicable when allowMultipleSelection is TRUE.  */
@property (nonatomic, readwrite) NSUInteger maximumSelections;

/** Set to TRUE to automatically dismiss the selection detail view when an item is selected. */
@property (nonatomic, readwrite) BOOL autoDismissDetailView;

/** Set to TRUE to hide the detail view's navigation bar. @note Only applicable if autoDismissDetailView is TRUE. */
@property (nonatomic, readwrite) BOOL hideDetailViewNavigationBar;

/**	Determines if the generated control allows adding new items. Default: FALSE. */
@property (nonatomic) BOOL allowAddingItems;

/**	Determines if the generated control allows deleting existing items. Default: FALSE. */
@property (nonatomic) BOOL allowDeletingItems;

/**	Determines if the generated control allows moving items. Default: FALSE. */
@property (nonatomic) BOOL allowMovingItems;

/**	Determines if the generated control allows editing items. Default: FALSE. */
@property (nonatomic) BOOL allowEditingItems;

/** The user interface element that is used to add new objects when selected. This object is typically of type SCTableViewCell. Set to nil to ignore. Default:nil. */
@property (nonatomic, strong) NSObject *placeholderuiElement;

/** The uiElement that is displayed when no items are available to display. This object is typically of type SCTableViewCell. Set to nil to ignore. Default:nil. */
@property (nonatomic, strong) NSObject *addNewObjectuiElement;

@end







/****************************************************************************************/
/*	class SCObjectSelectionAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type 
 SCPropertyTypeObjectSelection, thus allowing further customization of the generated control by the user.
 */
@interface SCObjectSelectionAttributes : SCSelectionAttributes

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCObjectSelectionAttributes'.
 *
 *	@param objects The array of objects that will be used to select from.
 *  @param definition The data structure definition of the objects.
 *	@param allowMultipleSel Determines if the generated selection control allows multiple selection.
 *	@param allowNoSel Determines if the generated selection control allows no selection.
 */
+ (id)attributesWithSelectionObjects:(NSArray *)objects
                   objectsDefinition:(SCDataDefinition *)definition
              allowMultipleSelection:(BOOL)allowMultipleSel
                    allowNoSelection:(BOOL)allowNoSel;

/** Returns an initialized 'SCObjectSelectionAttributes'.
 *
 *	@param objects The array of objects that will be used to select from.
 *  @param definition The data structure definition of the objects.
 *	@param allowMultipleSel Determines if the generated selection control allows multiple selection.
 *	@param allowNoSel Determines if the generated selection control allows no selection.
 */
- (id)initWithSelectionObjects:(NSArray *)objects
             objectsDefinition:(SCDataDefinition *)definition
        allowMultipleSelection:(BOOL)allowMultipleSel
              allowNoSelection:(BOOL)allowNoSel;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** Set this to the definition of the intermediate entity between the property's definition and the objectsDefinition. This is useful in complex many-to-many relationships where you have created an intermediate entity between you main two entities. Default: nil. */
@property (nonatomic, strong) SCDataDefinition *intermediateEntityDefinition;

@end





/****************************************************************************************/
/*	class SCObjectAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type 
 SCPropertyTypeObject, thus allowing further customization of the generated control by the user.
 */
@interface SCObjectAttributes : SCPropertyAttributes
{
	SCDataDefinition *objectDefinition;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCObjectAttributes'.
 *
 *	@param definition The definition for the generated object control. Set to nil to ignore.
 */
+ (id)attributesWithObjectDefinition:(SCDataDefinition *)definition;

/** Allocates and returns an initialized 'SCObjectAttributes'.
 *
 *	@param definition The definition for the generated object control. Set to nil to ignore.
 *  @param expandContent When TRUE, the generated user interface element will expand its content in the current view, instead of generating its own detail view.
 */
+ (id)attributesWithObjectDefinition:(SCDataDefinition *)definition expandContentInCurrentView:(BOOL)expandContent;

/** Returns an initialized 'SCObjectAttributes'.
 *
 *	@param definition The definition for the generated object control. Set to nil to ignore.
 */
- (id)initWithObjectDefinition:(SCDataDefinition *)definition;

/** Returns an initialized 'SCObjectAttributes'.
 *
 *	@param definition The definition for the generated object control. Set to nil to ignore.
 *  @param expandContent When TRUE, the generated user interface element will expand its content in the current view, instead of generating its own detail view.
 */
- (id)initWithObjectDefinition:(SCDataDefinition *)definition expandContentInCurrentView:(BOOL)expandContent;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/**	The definition for the generated object control. Set to nil to ignore. */
@property (nonatomic, strong) SCDataDefinition *objectDefinition;

@end






/****************************************************************************************/
/*	class SCArrayOfObjectsAttributes	*/
/****************************************************************************************/ 
/**	
 This class is used to extend the definition of an 'SCPropertyAttributes' instance of type SCPropertyTypeArrayOfObjects, thus allowing further customization of the generated control by the user.
 */
@interface SCArrayOfObjectsAttributes : SCPropertyAttributes
{
    SCDataDefinition *defaultObjectsDefinition;
    SCDataFetchOptions *objectsFetchOptions;
    
	BOOL allowAddingItems;
	BOOL allowDeletingItems;
	BOOL allowMovingItems;
	BOOL allowEditingItems;
    NSObject *placeholderuiElement;
    NSObject *addNewObjectuiElement;
    BOOL addNewObjectuiElementExistsInNormalMode;
    BOOL addNewObjectuiElementExistsInEdtingMode;
    
    SCSectionActions *_sectionActions;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCArrayOfObjectsAttributes'.
 *
 *	@param definition The definition for the generated object control. Set to nil to ignore.
 *	@param allowAdding Determines if the generated control allows adding new objects.
 *	@param allowDeleting Determines if the generated control allows deleting objects.
 *	@param allowMoving Determines if the generated control allows moving objects.
 */
+ (id)attributesWithObjectDefinition:(SCDataDefinition *)definition
						 allowAddingItems:(BOOL)allowAdding allowDeletingItems:(BOOL)allowDeleting
						 allowMovingItems:(BOOL)allowMoving;



/** Allocates and returns an initialized 'SCArrayOfObjectsAttributes'.
 *
 *	@param definition The class definition for the generated object control. Set to nil to ignore.
 *	@param allowAdding Determines if the generated control allows adding new objects.
 *	@param allowDeleting Determines if the generated control allows deleting objects.
 *	@param allowMoving Determines if the generated control allows moving objects.
 *  @param expandContent When TRUE, the generated user interface element will expand its content in the current view, instead of generating its own detail view.
 *  @param placeholderUI The uiElement that is displayed when no items are available. This object is typically of type SCTableViewCell.
 *  @param newObjectUI The uiElement that is used to add new objects when tapped. This object is typically of type SCTableViewCell.
 *  @param existsInNormalMode Determines if uiElement is displayed in 'Normal Mode'.
 *  @param existsInEditingMode Determines if uiElement is displayed in 'Editing Mode'.
 */
+ (id)attributesWithObjectDefinition:(SCDataDefinition *)definition
						 allowAddingItems:(BOOL)allowAdding allowDeletingItems:(BOOL)allowDeleting allowMovingItems:(BOOL)allowMoving 
               expandContentInCurrentView:(BOOL)expandContent
                     placeholderuiElement:(NSObject *)placeholderUI
                    addNewObjectuiElement:(NSObject *)newObjectUI
  addNewObjectuiElementExistsInNormalMode:(BOOL)existsInNormalMode 
 addNewObjectuiElementExistsInEditingMode:(BOOL)existsInEditingMode;

/** Returns an initialized 'SCArrayOfObjectsAttributes'.
 *
 *	@param definition The definition for the generated object control. Set to nil to ignore.
 *	@param allowAdding Determines if the generated control allows adding new objects.
 *	@param allowDeleting Determines if the generated control allows deleting objects.
 *	@param allowMoving Determines if the generated control allows moving objects.
 */
- (id)initWithObjectDefinition:(SCDataDefinition *)definition
				   allowAddingItems:(BOOL)allowAdding allowDeletingItems:(BOOL)allowDeleting
				   allowMovingItems:(BOOL)allowMoving;

/** Returns an initialized 'SCArrayOfObjectsAttributes'.
 *
 *	@param definition The definition for the generated object control. Set to nil to ignore.
 *	@param allowAdding Determines if the generated control allows adding new objects.
 *	@param allowDeleting Determines if the generated control allows deleting objects.
 *	@param allowMoving Determines if the generated control allows moving objects.
 *  @param expandContent When TRUE, the generated user interface element will expand its content in the current view, instead of generating its own detail view.
 *  @param placeholderUI The uiElement that is displayed when no items are available to display. This object is typically of type SCTableViewCell.
 *  @param newObjectUI The uiElement that is used to add new objects when tapped. This object is typically of type SCTableViewCell.
 *  @param existsInNormalMode Determines if uiElement is displayed in 'Normal Mode'.
 *  @param existsInEditingMode Determines if uiElement is displayed in 'Editing Mode'.
 */
- (id)initWithObjectDefinition:(SCDataDefinition *)definition
						 allowAddingItems:(BOOL)allowAdding allowDeletingItems:(BOOL)allowDeleting allowMovingItems:(BOOL)allowMoving 
               expandContentInCurrentView:(BOOL)expandContent
                     placeholderuiElement:(NSObject *)placeholderUI
                    addNewObjectuiElement:(NSObject *)newObjectUI
  addNewObjectuiElementExistsInNormalMode:(BOOL)existsInNormalMode 
 addNewObjectuiElementExistsInEditingMode:(BOOL)existsInEditingMode;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The default data definition for the objects. */
@property (nonatomic, strong) SCDataDefinition *defaultObjectsDefinition;

/** The fetch options for the objects. */
@property (nonatomic, strong) SCDataFetchOptions *objectsfetchOptions;

/**	Determines if the generated control allows adding new objects. Default: TRUE. */
@property (nonatomic) BOOL allowAddingItems;

/**	Determines if the generated control allows deleting objects. Default: TRUE. */
@property (nonatomic) BOOL allowDeletingItems;

/**	Determines if the generated control allows moving objects. Default: TRUE. */
@property (nonatomic) BOOL allowMovingItems;

/**	Determines if the generated control allows editing objects. Default: TRUE. */
@property (nonatomic) BOOL allowEditingItems;

/** The uiElement that is displayed when no items are available to display. This object is typically of type SCTableViewCell. Set to nil to ignore. Default:nil. */
@property (nonatomic, strong) NSObject *addNewObjectuiElement;

/** The user interface element that is used to add new objects when selected. This object is typically of type SCTableViewCell. Set to nil to ignore. Default:nil. */
@property (nonatomic, strong) NSObject *placeholderuiElement;

/** Determines if uiElement is displayed in 'Normal Mode'. Default: TRUE. */
@property (nonatomic) BOOL addNewObjectuiElementExistsInNormalMode;

/** Determines if uiElement is displayed in 'Editing Mode'. Default: TRUE. */
@property (nonatomic) BOOL addNewObjectuiElementExistsInEditingMode;

/** The actions of the generated SCArrayOfObjectsSection. */
@property (nonatomic, readonly) SCSectionActions *sectionActions;

@end



