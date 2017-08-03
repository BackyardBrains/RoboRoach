/*
 *  SCSectionActions.h
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
@class SCTableViewSection;
@class SCArrayOfItemsSection;
@class SCCustomCell;

typedef void(^SCDidAddToModelSectionAction_Block)(SCTableViewSection *section, NSUInteger sectionIndex);

typedef void(^SCDetailModelSectionAction_Block)(SCTableViewSection *section, SCTableViewModel *detailModel, NSIndexPath *indexPath);

typedef void(^SCDidFetchSectionItemsAction_Block)(SCArrayOfItemsSection *itemsSection, NSMutableArray *items);
typedef void(^SCFetchSectionItemsFailedAction_Block)(SCArrayOfItemsSection *itemsSection, NSError *error);
typedef void(^SCSectionItemDidCreateAction_Block)(SCArrayOfItemsSection *itemsSection, NSObject *item);
typedef BOOL(^SCSectionItemWillInsertAction_Block)(SCArrayOfItemsSection *itemsSection, NSObject *item, SCTableViewModel *itemModel);
typedef void(^SCSectionItemDidInsertAction_Block)(SCArrayOfItemsSection *itemsSection, NSObject *item, NSIndexPath *indexPath);
typedef BOOL(^SCSectionItemWillUpdateAction_Block)(SCArrayOfItemsSection *itemsSection, NSObject *item, NSIndexPath *indexPath, SCTableViewModel *itemModel);
typedef void(^SCSectionItemDidUpdateAction_Block)(SCArrayOfItemsSection *itemsSection, NSObject *item, NSIndexPath *indexPath);
typedef BOOL(^SCSectionItemWillDeleteAction_Block)(SCArrayOfItemsSection *itemsSection, NSObject *item, NSIndexPath *indexPath);
typedef void(^SCSectionItemDidDeleteAction_Block)(SCArrayOfItemsSection *itemsSection, NSIndexPath *indexPath);
typedef void(^SCDidAddSpecialCellsAction_Block)(SCArrayOfItemsSection *itemsSection, NSMutableArray *items);

typedef UIViewController*(^SCDetailViewControllerForRowAtIndexPathAction_Block)(SCTableViewSection *section, NSIndexPath *indexPath);
typedef SCTableViewModel*(^SCDetailTableViewModelForRowAtIndexPathAction_Block)(SCTableViewSection *section, NSIndexPath *indexPath);

typedef SCCustomCell*(^SCCellForRowAtIndexPathAction_Block)(SCArrayOfItemsSection *itemsSection, NSIndexPath *indexPath);
typedef NSString*(^SCReuseIdForRowAtIndexPathAction_Block)(SCArrayOfItemsSection *itemsSection, NSIndexPath *indexPath);
typedef CGFloat(^SCCustomHeightForRowAtIndexPathAction_Block)(SCArrayOfItemsSection *itemsSection, NSIndexPath *indexPath);

/****************************************************************************************/
/*	class SCSectionActions	*/
/****************************************************************************************/ 
/**	
 This class hosts a set of section action blocks. Once an action is set to a desired code block, it will execute the block as soon as the action occurs.
 
 @see SCCellActions, SCModelActions.
 */

@interface SCSectionActions : NSObject


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Section Actions
//////////////////////////////////////////////////////////////////////////////////////////

/** Action gets called right after the section has been added to a model.
 
 This action is typically used to get hold of the section at data definition time and perform any additional configuration.
 
 Example:
    myArrayOfObjectsAttributes.expandContentInCurrentView = YES;
    myArrayOfObjectsAttributes.expandedContentSectionActions.didAddToModel = ^(SCTableViewSection *section, NSUInteger sectionIndex)
    {
        // set all expanded cells to a different background color
        section.cellActions.willDisplay = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
        {
            cell.backgroundColor = [UIColor yellowColor];
        };
    };
 
  */
@property (nonatomic, copy) SCDidAddToModelSectionAction_Block didAddToModel;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Detail Model Actions
//////////////////////////////////////////////////////////////////////////////////////////

/** Action gets called right after the section's detail model is created, before configuration is set or any sections are added.
 
 This action is typically used to initially configure the detail model (like set a custom tag for example). Most of the model's settings can also be configure in the detailModelConfigured action.
 
 Example:
    sectionActions.detailModelCreated = ^(SCTableViewSection *section, SCTableViewModel *detailModel, NSIndexPath *indexPath)
    {
        detailModel.tag = 100;
    };
 
 @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection. 
 
 @warning In the case where the detail model is not associated with an existing row (such as the case when 'SCArrayOfObjectsSection' creates a new item), indexPath.row will be set to NSNotFound. This is a good way to test if the detail model was generated for a new item or an already existing one.
 
 @see detailModelConfigured
 */
@property (nonatomic, copy) SCDetailModelSectionAction_Block detailModelCreated;

/** Action gets called after the section's detail model is fully configured, including the addition of all automatically generated sections.
 
 This action is typically used to add additional custom sections, or modify the already existing automatically generated ones.
 
 Example:
    sectionActions.detailModelConfigured = ^(SCTableViewSection *section, SCTableViewModel *detailModel, NSIndexPath *indexPath)
    {
        SCTableViewSection *customSection = [SCTableViewSection section];
        SCCustomCell *customCell = [SCCustomCell cellWithText:@"Custom Cell"];
        [customSection addCell:customCell];
 
        [detailModel addSection:customSection];
    };
 
 @note In general, it is easier (and more recommended) to add your custom sections and cells using the data definitions, instead of using this action to do so. For more information, please refer to SCDataDefinition and SCCustomPropertyDefinition.
 
 @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection. 
 
 @warning In the case where the detail model is not associated with an existing row (such as the case when 'SCArrayOfObjectsSection' creates a new item), indexPath.row will be set to NSNotFound. This is a good way to test if the detail model was generated for a new item or an already existing one.
 */
@property (nonatomic, copy) SCDetailModelSectionAction_Block detailModelConfigured;

/** Action gets called when the section's detail model is about to be presented in its own view controller.
 
 This action is typically used to further customize the detail model's view controller.
 
 Example:
    sectionActions.detailModelWillPresent = ^(SCTableViewSection *section, SCTableViewModel *detailModel, NSIndexPath *indexPath)
    {
        detailModel.viewController.title = @"My custom title";
    };
 
 @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection. 
 
 @warning In the case where the detail model is not associated with an existing row (such as the case when 'SCArrayOfObjectsSection' creates a new item), indexPath.row will be set to NSNotFound. This is a good way to test if the detail model was generated for a new item or an already existing one.
 */
@property (nonatomic, copy) SCDetailModelSectionAction_Block detailModelWillPresent;

/** Action gets called when the section's detail model has been presented in its own view controller.
 
 Example:
    sectionActions.detailModelDidPresent = ^(SCTableViewSection *section, SCTableViewModel *detailModel, NSIndexPath *indexPath)
    {
        NSLog(@"Detail model has been presented.");
    };
 
 @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection. 
 
 @warning In the case where the detail model is not associated with an existing row (such as the case when 'SCArrayOfObjectsSection' creates a new item), indexPath.row will be set to NSNotFound. This is a good way to test if the detail model was generated for a new item or an already existing one.
 */
@property (nonatomic, copy) SCDetailModelSectionAction_Block detailModelDidPresent;

/** Action gets called when the section's detail model's view controller is about to be dismissed.
 
 Example:
    sectionActions.detailModelWillDismiss = ^(SCTableViewSection *section, SCTableViewModel *detailModel, NSIndexPath *indexPath)
    {
        NSLog(@"Detail model will be dismissed.");
    };
 
 @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection. 
 
 @warning In the case where the detail model is not associated with an existing row (such as the case when 'SCArrayOfObjectsSection' creates a new item), indexPath.row will be set to NSNotFound. This is a good way to test if the detail model was generated for a new item or an already existing one.
 */
@property (nonatomic, copy) SCDetailModelSectionAction_Block detailModelWillDismiss;

/** Action gets called when the section's detail model's view controller has been dismissed.
 
 Example:
    sectionActions.detailModelDidDismiss = ^(SCTableViewSection *section, SCTableViewModel *detailModel, NSIndexPath *indexPath)
    {
        NSLog(@"Detail model has been dismissed.");
    };
 
 @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection. 
 
 @warning In the case where the detail model is not associated with an existing row (such as the case when 'SCArrayOfObjectsSection' creates a new item), indexPath.row will be set to NSNotFound. This is a good way to test if the detail model was generated for a new item or an already existing one.
 */
@property (nonatomic, copy) SCDetailModelSectionAction_Block detailModelDidDismiss;

/** Action gets called to give you the chance to return a custom detail view controller for the section.
 
 This action is typically used to provide your own custom detail view controller, instead of the one automatically generated by the section.
 
 @return The custom view controller. *Must only be of type SCViewController or SCTableViewController*. Note: returning nil ignores the implementation of this action.
 
 Example:
    sectionActions.detailViewController = ^UIViewController*(SCTableViewSection *section, NSIndexPath *indexPath)
    {
        MyCustomViewController *customVC = [[MyCustomViewController alloc] initWithNib:@"MyCustomViewController" bundle:nil];
 
        return customVC;
    };
 
 @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection.
 */
@property (nonatomic, copy) SCDetailViewControllerForRowAtIndexPathAction_Block detailViewControllerForRowAtIndexPath;

/** Action gets called to give you the chance to return a custom detail model for the section's detail view controller.
 
 This action is typically used to provide your own custom detail model, instead of the one automatically generated by the section.
 
 @note It is much more common to use the detailViewController action instead, assigning the custom model in the custom view controller's viewDidLoad method.
 
 @return The custom detail model. The returned detail model should not be associated with any table views, as the framework will automatically handle this on your behalf. Note: returning nil ignores the implementation of this action.
 
 Example:
    sectionActions.detailTableViewModel = ^SCTableViewModel*(SCTableViewSection *section, NSIndexPath *indexPath)
    {
        SCTableViewModel *detailModel = myCustomModel;
 
        return detailModel;
    };
 
    @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection.
 */
@property (nonatomic, copy) SCDetailTableViewModelForRowAtIndexPathAction_Block detailTableViewModelForRowAtIndexPath;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name SCArrayOfItemsSection Actions
//////////////////////////////////////////////////////////////////////////////////////////

/** Action gets called to give you the chance to return a custom cell for the section's item, instead of the automatically generated standard SCTableViewCell.
 
 @return The custom cell. *Must only be of type SCCustomCell or subclass*. Note: returning nil ignores the implementation of this action.
 
 Example:
    sectionActions.cellForRowAtIndexPath = ^SCCustomCell*(SCArrayOfItemsSection *itemsSection, NSIndexPath *indexPath)
    {
        // '1' and '2' are the tags of the labels corresponding to 
        // the firstName and lastName object properties
        NSString *bindingsString = @"1:firstName;2:lastName";
 
        SCCustomCell *customCell = [SCCustomCell cellWithText:nil objectBindingsString:bindingsString nibName:@"MyCustomCell"];
 
        return customCell;
    };
 
 @warning If more than one type of custom cell is returned (e.g.: depending on the indexPath), then you *must* also use the reuseIdentifierForRowAtIndexPath action to return a unique reuse id for each different cell you return. Otherwise, there is no need to set the reuseIdentifierForRowAtIndexPath action.
 
 @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection.
 
 @see reuseIdentifierForRowAtIndexPath
 */
@property (nonatomic, copy) SCCellForRowAtIndexPathAction_Block cellForRowAtIndexPath;

/** In case more than one type of custom cell is returned in cellForRowAtIndexPath, this action gets called to give you the chance to return a custom cell reuse identifier for each different type. 
 
 @note You only need to set this action if more than one type of custom cell is returned in cellForRowAtIndexPath.
 
 @return An NSString containing the custom cell reuse identifier.
 
 Example:
    sectionActions.cellForRowAtIndexPath = ^SCCustomCell*(SCArrayOfItemsSection *itemsSection, NSIndexPath *indexPath)
    {
        SCCustomCell *customCell;
        if(indexPath.row % 2)
            customCell = [[MyCustomEvenCell alloc] init];
        else
            customCell = [[MyCustomOddCell alloc] init];
 
        return customCell;
    };
 
    sectionActions.reuseIdentifierForRowAtIndexPath = ^NSString*(SCArrayOfItemsSection *itemsSection, NSIndexPath *indexPath)
    {
        NSString *reuseId;
        if(indexPath.row % 2)
            reuseId = @"EvenCell";
        else
            reuseId = @"OddCell";
 
        return reuseId;
    };
 
 @note This action is only applicable to sections that generate detail views, such as SCArrayOfObjectsSection.
 */
@property (nonatomic, copy) SCReuseIdForRowAtIndexPathAction_Block reuseIdentifierForRowAtIndexPath;

/** Action gets called to give you the chance to return a custom cell height for each of the section's cells, instead of the automatically calculated cell height.
 
 @return The custom cell height. Note: returning -1 ignores the implementation of this action and will have the framework automatically calculate the cell height.
 
 Example:
    sectionActions.customHeightForRowAtIndexPath = ^CGFloat(SCArrayOfItemsSection *itemsSection, NSIndexPath *indexPath)
    {
        CGFloat cellHeight;
        if(indexPath.row % 2)
            cellHeight = 44;
        else
            cellHeight = 60;
 
        return cellHeight;
    };
 
 @note Implementing this action only makes sense in situations where you have a huge number of cells. In these cases, providing the height directly drastically improves performance, versus having the framework automatically calculate the height for each single cell.
 
 @see SCArrayOfItemsSection.cellCountLimitForAutoHeight
 */
@property (nonatomic, copy) SCCustomHeightForRowAtIndexPathAction_Block customHeightForRowAtIndexPath;

/** Action gets called as soon as the section has retrieved its items from their data store.
 
 This action is typically used to customize the 'items' array after it has been fetched from the data store. Items can be added, removed, or rearranged. The added items can either be objects that are suppored by the data store, or normal SCTableViewCell (or subclass) objects.
 
 Example:
    sectionActions.didFetchItemsFromStore = ^(SCArrayOfItemsSection *itemsSection, NSMutableArray *items)
    {
        // Add a button cell at the end of the items list
        SCTableViewCell *buttonCell = [SCTableViewCell cellWithText:@"Tap me!" textAlignment:UITextAlignmentCenter];
        buttonCell.cellActions.didSelect = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
        {
            NSLog(@"buttonCell tapped!");
        };
 
        [items addObject:buttonCell];
    };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection. 
 
 */
@property (nonatomic, copy) SCDidFetchSectionItemsAction_Block didFetchItemsFromStore;

/** Action gets called if the section fails to retrieve its items from their data store.
 
 This action is typically used when the section is bound to a web service and you want to give the user feedback that their web service was not reachable.
 
 Example:
     sectionActions.fetchItemsFromStoreFailed = ^(SCArrayOfItemsSection *itemsSection, NSError *error)
     {
          NSLog(@"Failed retrieving section items with error: %@", error);
     };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection.
 
 */
@property (nonatomic, copy) SCFetchSectionItemsFailedAction_Block fetchItemsFromStoreFailed;

/** Action gets called as soon as the section has created a new item to be used in its add new item detail view (which usually apprears right after the user taps the section's Add button).
 
 This action is typically used to customize the newly created item before the new item detail view is displayed.
 
 Example:
    sectionActions.didCreateItem = ^(SCArrayOfItemsSection *itemsSection, NSObject *item)
    {
        // Set an initial value for startDate
        [item setValue:[NSDate date] forKey:@"startDate"];
    };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection.
 
 */
@property (nonatomic, copy) SCSectionItemDidCreateAction_Block didCreateItem;

/** Action gets called right before a newly created item is inserted into the section.
 
 This action is typically used to customize the newly created item before it's inserted. Furthermore, returning FALSE gives you the chance to discard adding the new item altogether.
 
 Example:
    sectionActions.willInsertItem = ^BOOL(SCArrayOfItemsSection *itemsSection, NSObject *item, SCTableViewModel *itemModel)
    {
        // Set a default description if no description is set
        if(![item valueForKey:@"description"])
            [item setValue:@"My default description" forKey:@"description"];
 
        // Accept insert
        return TRUE;
    };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection.
 
 */
@property (nonatomic, copy) SCSectionItemWillInsertAction_Block willInsertItem;

/** Action gets called as soon as a new item has been added to the section.
 
 This action is typically used to provide custom functionality after a new item has been added.
 
 Example:
    sectionActions.didInsertItem = ^(SCArrayOfItemsSection *itemsSection, NSObject *item, NSIndexPath *indexPath)
    {
        NSLog(@"New item inserted at indexPath: %@", indexPath);
    };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection.
 
 */
@property (nonatomic, copy) SCSectionItemDidInsertAction_Block didInsertItem;

/** Action gets called right before an existing item is updated.
 
 This action is typically used to customize the user's changes before the item is updated. Furthermore, returning FALSE gives you the chance to discard updating the item altogether.
 
 Example:
    sectionActions.willUpdateItem = ^BOOL(SCArrayOfItemsSection *itemsSection, NSObject *item, NSIndexPath *indexPath, SCTableViewModel *itemModel)
    {
        // Set a default description if no description is set
        if(![item valueForKey:@"description"])
            [item setValue:@"My default description" forKey:@"description"];
 
        // Accept update
        return TRUE;
    };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection.
 
 */
@property (nonatomic, copy) SCSectionItemWillUpdateAction_Block willUpdateItem;

/** Action gets called as soon as an existing item has been updated.
 
 This action is typically used to provide custom functionality after an item has been updated.
 
 Example:
    sectionActions.didUpdateItem = ^(SCArrayOfItemsSection *itemsSection, NSObject *item, NSIndexPath *indexPath)
    {
        NSLog(@"Item updated at indexPath: %@", indexPath);
    };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection.
 
 */
@property (nonatomic, copy) SCSectionItemDidUpdateAction_Block didUpdateItem;

/** Action gets called right before an existing item is deleted.
 
 This action is typically used to provide you with a chance to override the item delete operation by returning FALSE.
 
 Example:
    sectionActions.willDeleteItem = ^BOOL(SCArrayOfItemsSection *itemsSection, NSObject *item, NSIndexPath *indexPath)
    {
        BOOL deleteItem;
 
        if(myCondition)
            deleteItem = TRUE;
        else
            deleteItem = FALSE;
 
        return deleteItem;
    };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection.
 
 */
@property (nonatomic, copy) SCSectionItemWillDeleteAction_Block willDeleteItem;

/** Action gets called as soon as an existing item has been deleted.
 
 This action is typically used to provide custom functionality after an item has been deleted.
 
 Example:
    sectionActions.didDeleteItem = ^(SCArrayOfItemsSection *itemsSection, NSIndexPath *indexPath)
    {
        NSLog(@"Item at indexPath: %@ has been deleted.", indexPath);
    };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection.
 
 */
@property (nonatomic, copy) SCSectionItemDidDeleteAction_Block didDeleteItem;


/** Action gets called after the section has retrieved its items from their data store, and the framework has automatically added any needed special cells (e.g.: placeholder cell, load more cell, etc.) to the items array.
 
 This action is typically used to customize the 'items' array after it has been fetched from the data store and the special cells added to it.
 
 Example:
    sectionActions.didAddSpecialCells = ^(SCArrayOfItemsSection *itemsSection, NSMutableArray *items)
    {
        // Add a button cell at the end of the items list
        SCTableViewCell *buttonCell = [SCTableViewCell cellWithText:@"Tap me!" textAlignment:UITextAlignmentCenter];
        buttonCell.cellActions.didSelect = ^(SCTableViewCell *cell, NSIndexPath *indexPath)
        {
            NSLog(@"buttonCell tapped!");
        };
 
        [items addObject:buttonCell];
    };
 
 @note This action is only applicable to SCArrayOfItemsSection subclasses, such as SCArrayOfObjectsSection. 
 */
@property (nonatomic, copy) SCDidAddSpecialCellsAction_Block didAddSpecialCells;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Miscellaneous
//////////////////////////////////////////////////////////////////////////////////////////

/** Method assigns all the actions of another 'SCSectionActions' class to the current one.
 
 @param actions The source 'SCSectionActions' class.
 @param override Set to TRUE to override any existing actions, otherwise set to FALSE.
 */
- (void)setActionsTo:(SCSectionActions *)actions overrideExisting:(BOOL)override;

@end
