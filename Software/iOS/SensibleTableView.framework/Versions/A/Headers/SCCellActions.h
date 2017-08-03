/*
 *  SCCellActions.h
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

@class SCTableViewModel;
@class SCTableViewCell;

typedef void(^SCCellAction_Block)(SCTableViewCell *cell, NSIndexPath *indexPath);
typedef BOOL(^SCCellValueIsValidAction_Block)(SCTableViewCell *cell, NSIndexPath *indexPath);
typedef NSObject*(^SCCellBoundValueAction_Block)(SCTableViewCell *cell, NSIndexPath *indexPath, NSObject *value);
typedef void(^SCCellCustomButtonTappedAction_Block)(SCTableViewCell *cell, NSIndexPath *indexPath, UIButton *button);

typedef UIViewController*(^SCCellDetailViewControllerAction_Block)(SCTableViewCell *cell, NSIndexPath *indexPath);
typedef SCTableViewModel*(^SCCellDetailTableViewModelAction_Block)(SCTableViewCell *cell, NSIndexPath *indexPath);

typedef void(^SCDetailModelCellAction_Block)(SCTableViewCell *cell, NSIndexPath *indexPath, SCTableViewModel *detailModel);


/****************************************************************************************/
/*	class SCCellActions	*/
/****************************************************************************************/ 
/**	
 This class hosts a set of cell action blocks. Once an action is set to a desired code block, it will execute the block as soon as the action occurs.
 
 @see SCSectionActions, SCModelActions.
 */
@interface SCCellActions : NSObject

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Actions
//////////////////////////////////////////////////////////////////////////////////////////

/** Action gets called before the cell gets automatically styled using the provided theme.
 
 This action is typically used to set a custom themeStyle for the cell that is defined in the model's theme file.
 
 Example:
    cellActions.willStyle = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        cell.themeStyle = @"MyCustomStyle";
    };
 
 @see SCTheme 
 */
@property (nonatomic, copy) SCCellAction_Block willStyle;

/** Action gets called before the cell is configured. 
 
 This action is typically used to set the height of the cell, or any other attribute that will affect the cell's configuration and layout.
 
 Example:
    cellActions.willConfigure = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        cell.height = 60;
    };
 */
@property (nonatomic, copy) SCCellAction_Block willConfigure;

/** Action gets called after the cell has laid out all its subviews.
 
 This action is typically used to change the subviews' layout.
 
 Example:
    cellActions.didLayoutSubviews = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        cell.textLabel.frame = CGRect(40, 20, 100, 40);
    };
 */
@property (nonatomic, copy) SCCellAction_Block didLayoutSubviews;

/** Action gets called before the cell is displayed.
 
 This action is typically used to set any attributes that will affect how the cell is displayed.
 
 Example:
    cellActions.willDisplay = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        cell.backgroundColor = [UIColor yellowColor];
    };
 
 @note Changing the cell's height property cannot be set here, and must be set in the willConfigure action instead.
 */
@property (nonatomic, copy) SCCellAction_Block willDisplay;

/** Action gets called after the cell has been displayed and the table view has stopped scrolling.
 
 This action is typically used to load any cell content that is too expensive to load in willDisplay, such as retrieving data from a web service. This guarantees smooth and uninterrupted scrolling of the table view.
 
 Example:
    cellActions.lazyLoad = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        cell.imageView.image = [self retrieveImageForRowAtIndexPath:indexPath];
    };
 */
@property (nonatomic, copy) SCCellAction_Block lazyLoad;

/** Action gets called when the cell is about to be selected.
 
 Example:
    cellActions.willSelect = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        NSLog(@"Cell at indexPath:%@ is about to be selected.", indexPath);
    };
 */
@property (nonatomic, copy) SCCellAction_Block willSelect;

/** Action gets called when the cell has been selected.
 
 Example:
    cellActions.didSelect = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        NSLog(@"Cell at indexPath:%@ has been selected.", indexPath);
    };
 */
@property (nonatomic, copy) SCCellAction_Block didSelect;

/** Action gets called when the cell is about to be deselected.
 
 Example:
    cellActions.willDeselect = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        NSLog(@"Cell at indexPath:%@ is about to be deselected.", indexPath);
    };
 */
@property (nonatomic, copy) SCCellAction_Block willDeselect;

/** Action gets called when the cell has been deselected.
 
 Example:
    cellActions.didDeselect = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        NSLog(@"Cell at indexPath:%@ has been selected.", indexPath);
    };
 */
@property (nonatomic, copy) SCCellAction_Block didDeselect;

/** Action gets called when the cell's accessory button has been tapped.
 
 Example:
    cellActions.accessoryButtonTapped = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        NSLog(@"Cell at indexPath:%@ accessory button has been tapped.", indexPath);
    };
 
 @note For this action to get called, you must first have the cell's accessory button appear by setting its accessoryType property to UITableViewCellAccessoryDetailDisclosureButton.
 */
@property (nonatomic, copy) SCCellAction_Block accessoryButtonTapped;

/** Action gets called when the cell keyboard's return button has been tapped.
 
 This action is typically used to override STV's behavior when the return button is tapped, and define a custom one.
 
 Example:
    cellActions.returnButtonTapped = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        [self doMyCustomAction];
        [cell.ownerTableViewModel moveToNextCellControl:YES];
    };
 
 @note Action is only applicable to cells with controls that display a keyboard.
 */
@property (nonatomic, copy) SCCellAction_Block returnButtonTapped;

/** Action gets called when the cell's bound property value has changed via a cell control or a detail model.
 
 This action is typically used to provide a custom behavior when the cell's value changes.
 
 Example:
    cellActions.valueChanged = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        NSLog(@"Cell at indexPath:%@ value has changed to: %@.", indexPath, cell.boundValue);
    };
 */
@property (nonatomic, copy) SCCellAction_Block valueChanged;

/** Action gets called when the cell's value needs to be validated.
 
 This action is typically used to provide a custom cell value validation.
 
 @return Return TRUE if the current cell value is valid, otherwise return FALSE.
 
 Example:
    cellActions.valueIsValid = ^BOOL(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        BOOL valid = FALSE;
 
        if([cell isKindOfClass:[SCTextFieldCell class]])
        {
            SCTextFieldCell *textFieldCell = (SCTextFieldCell *)cell;
 
            // Make sure the password field is at least 8 characters long
            if([textFieldCell.textField.text length] >= 8)
                valid = TRUE;
        }
 
        return valid;
    };
 */
@property (nonatomic, copy) SCCellValueIsValidAction_Block valueIsValid;

/** Action gets called whenever a cell's bound value has been loaded.
 
 This action is typically used to do any customization to the loaded bound value.
 
 Example:
    cellActions.didLoadBoundValue = ^NSObject*(SCTableViewCell *cell, NSIndexPath *indexPath, NSObject *value)
    {
        // Make sure all string spaces are trimmed before displaying string.
 
        NSString *stringValue = (NSStirng *)value;
        NSString *trimmedString = [stringValue stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
 
        return trimmedString;
    };
 */
@property (nonatomic, copy) SCCellBoundValueAction_Block didLoadBoundValue;

/** Action gets called before a cell's bound value is committed to its bound object.
 
 This action is typically used to do any customization to the bound value before being committed.
 
 Example:
    cellActions.willCommitBoundValue = ^NSObject*(SCTableViewCell *cell, NSIndexPath *indexPath, NSObject *value)
    {
        // Make sure all string spaces are trimmed before committing the string.
 
        NSString *stringValue = (NSStirng *)value;
        NSString *trimmedString = [stringValue stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
 
        return trimmedString;
    };
 */
@property (nonatomic, copy) SCCellBoundValueAction_Block willCommitBoundValue;

/** Action gets called whenever a cell's user defined custom button is tapped.
 
 This action is typically used to easily provide a custom behavior for the cell's custom button(s).
 
 Example:
    cellActions.customButtonTapped = ^(SCTableViewCell *cell, NSIndexPath *indexPath, UIButton *button)
    {
        NSLog(@"Custom button with tag:%i has been tapped for cell at indexPath:%@.", button.tag, indexPath);
    };
 */
@property (nonatomic, copy) SCCellCustomButtonTappedAction_Block customButtonTapped;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Detail Model Actions
//////////////////////////////////////////////////////////////////////////////////////////


/** Action gets called to give you the chance to return a custom detail view controller for the cell.
 
 This action is typically used to provide your own custom detail view controller, instead of the one automatically generated by the cell.
 
 @return The custom view controller. *Must only be of type SCViewController or SCTableViewController*. Note: returning nil ignores the implementation of this action.
 
 Example:
    cellActions.detailViewController = ^UIViewController*(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        MyCustomViewController *customVC = [[MyCustomViewController alloc] initWithNib:@"MyCustomViewController" bundle:nil];
        
        return customVC;
    };
 
 @note This action is only applicable to cells that generate detail views, such as SCSelectionCell and SCArrayOfObjectsCell.
 */
@property (nonatomic, copy) SCCellDetailViewControllerAction_Block detailViewController;

/** Action gets called to give you the chance to return a custom detail model for the cell's detail view controller.
 
 This action is typically used to provide your own custom detail model, instead of the one automatically generated by the cell. This might be needed in cases where the cell generates a detail SCArrayOfObjectsSection for example, and you need an SCArrayOfObjectsModel instead (to automatically generate sections for instance).
 
 @note It is much more common to use the detailViewController action instead, assigning the custom model in the custom view controller's viewDidLoad method. This also gives you the chance to add a search bar (for example, to make use of SCArrayOfObjectsModel automatic searching functionality), or any other controls.
 
 @return The custom detail model. The returned detail model should not be associated with any table views, as the framework will automatically handle this on your behalf. Note: returning nil ignores the implementation of this action.
 
 Example:
    cellActions.detailTableViewModel = ^SCTableViewModel*(SCTableViewCell *cell, NSIndexPath *indexPath)
    {
        SCTableViewModel *detailModel = nil;
        if([cell isKindOfClass:[SCArrayOfObjectsCell class]])
        {
            detailModel = [SCArrayOfObjectsModel modelWithTableView:nil];
        }
 
        return detailModel;
    };
 
 @note This action is only applicable to cells that generate detail views, such as SCSelectionCell and SCArrayOfObjectsCell.
 */
@property (nonatomic, copy) SCCellDetailTableViewModelAction_Block detailTableViewModel;


/** Action gets called right after the cell's detail model is created, before configuration is set or any sections are added.
 
 This action is typically used to initially configure the detail model (like set a custom tag for example). Most of the model's settings can also be configure in the detailModelConfigured action.
 
 Example:
    cellActions.detailModelCreated = ^(SCTableViewCell *cell, NSIndexPath *indexPath, SCTableViewModel *detailModel)
    {
        detailModel.tag = 100;
    };
 
 @note This action is only applicable to cells that generate detail views, such as SCSelectionCell.
 
 @see detailModelConfigured
 */
@property (nonatomic, copy) SCDetailModelCellAction_Block detailModelCreated;

/** Action gets called after the cell's detail model is fully configured, including the addition of all automatically generated sections.
 
 This action is typically used to add additional custom sections, or modify the already existing automatically generated ones.
 
 Example:
    cellActions.detailModelConfigured = ^(SCTableViewCell *cell, NSIndexPath *indexPath, SCTableViewModel *detailModel)
    {
        SCTableViewSection *customSection = [SCTableViewSection section];
        SCCustomCell *customCell = [SCCustomCell cellWithText:@"Custom Cell"];
        [customSection addCell:customCell];
 
        [detailModel addSection:customSection];
    };
 
 @note In general, it is easier (and more recommended) to add your custom sections and cells using the data definitions, instead of using this action to do so. For more information, please refer to SCDataDefinition and SCCustomPropertyDefinition.
 
 @note This action is only applicable to cells that generate detail views, such as SCSelectionCell.
 
 */
@property (nonatomic, copy) SCDetailModelCellAction_Block detailModelConfigured;

/** Action gets called when the cell's detail model is about to be presented in its own view controller.
 
 This action is typically used to further customize the detail model's view controller.
 
 Example:
    cellActions.detailModelWillPresent = ^(SCTableViewCell *cell, NSIndexPath *indexPath, SCTableViewModel *detailModel)
    {
        detailModel.viewController.title = @"My custom title";
    };
 
 @note This action is only applicable to cells that generate detail views, such as SCSelectionCell.
 
 */
@property (nonatomic, copy) SCDetailModelCellAction_Block detailModelWillPresent;

/** Action gets called when the cell's detail model has been presented in its own view controller.
 
 Example:
    cellActions.detailModelDidPresent = ^(SCTableViewCell *cell, NSIndexPath *indexPath, SCTableViewModel *detailModel)
    {
        NSLog(@"Detail model has been presented.");
    };
 
 @note This action is only applicable to cells that generate detail views, such as SCSelectionCell.

 */
@property (nonatomic, copy) SCDetailModelCellAction_Block detailModelDidPresent;

/** Action gets called when the cell's detail model's view controller is about to be dismissed.
 
 Example:
    cellActions.detailModelWillDismiss = ^(SCTableViewCell *cell, NSIndexPath *indexPath, SCTableViewModel *detailModel)
    {
        NSLog(@"Detail model will be dismissed.");
    };
 
 @note This action is only applicable to cells that generate detail views, such as SCSelectionCell.

 */
@property (nonatomic, copy) SCDetailModelCellAction_Block detailModelWillDismiss;

/** Action gets called when the cell's detail model's view controller has been dismissed.
 
 Example:
    cellActions.detailModelDidDismiss = ^(SCTableViewCell *cell, NSIndexPath *indexPath, SCTableViewModel *detailModel)
    {
        NSLog(@"Detail model has been dismissed.");
    };
 
 @note This action is only applicable to cells that generate detail views, such as SCSelectionCell.
 
 */
@property (nonatomic, copy) SCDetailModelCellAction_Block detailModelDidDismiss;



//////////////////////////////////////////////////////////////////////////////////////////
/// @name Miscellaneous
//////////////////////////////////////////////////////////////////////////////////////////

/** Method assigns all the actions of another 'SCCellActions' class to the current one.
 
 @param actions The source 'SCCellActions' class.
 @param override Set to TRUE to override any existing actions, otherwise set to FALSE.
 */
- (void)setActionsTo:(SCCellActions *)actions overrideExisting:(BOOL)override;

@end
