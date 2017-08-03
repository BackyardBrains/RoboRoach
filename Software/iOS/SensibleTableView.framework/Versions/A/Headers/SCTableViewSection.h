/*
 *  SCTableViewSection.h
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
#import "SCDataDefinition.h"
#import "SCDataStore.h"
#import "SCSectionActions.h"
#import "SCTableViewCell.h"
#import "SCTableViewController.h"
#import "SCDetailViewControllerOptions.h"
#import "SCExpandCollapseCell.h"
#import "SCFetchItemsCell.h"


/****************************************************************************************/
/*	class SCTableViewSection	*/
/****************************************************************************************/ 
/**
 This class functions as a section for SCTableViewModel. Every 'SCTableViewSection' can contain
 any number of SCTableViewCell(s).
 */
@interface SCTableViewSection : NSObject
{	
    __weak SCTableViewModel *_ownerTableViewModel;
	NSObject *boundObject;
    SCDataStore *boundObjectStore;
	NSString *boundPropertyName;
    
    BOOL commitChangesLive;
	BOOL commitCellChangesLive;
	
	NSString *headerTitle;
	CGFloat headerHeight;
	UIView *headerView;
	NSString *footerTitle;
	CGFloat footerHeight;
	UIView *footerView;
	
    SCSectionActions *sectionActions;
	NSMutableArray *cells;
    SCCellActions *cellActions;
	NSArray *cellsImageViews;
    
    SCExpandCollapseCell *expandCollapseCell;
    
    SCDetailViewControllerOptions *detailViewControllerOptions;
    SCDetailViewControllerOptions *newItemDetailViewControllerOptions;
    
    NSString *themeStyle;
    NSString *firstCellThemeStyle;
    NSString *evenCellsThemeStyle;
    NSString *oddCellsThemeStyle;
    NSString *lastCellThemeStyle;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCTableViewSection. */
+ (id)section;

/** Allocates and returns an initialized SCTableViewSection given a header title.
 *
 *	@param sectionHeaderTitle A header title for the section.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle;

/** Allocates and returns an initialized SCTableViewSection given a header and a footer title.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param sectionFooterTitle A footer title for the section.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle footerTitle:(NSString *)sectionFooterTitle;

/** Returns an initialized SCTableViewSection given a header title.
 *
 *	@param sectionHeaderTitle A header title for the section.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle;

/** Returns an initialized SCTableViewSection given a header and a footer title.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param sectionFooterTitle A footer title for the section.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle footerTitle:(NSString *)sectionFooterTitle;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The owner table view model of the section. 
 *
 * @warning This property gets set automatically by the section's owner, you should never set this property manually */
@property (nonatomic, weak) SCTableViewModel *ownerTableViewModel;

/** The section header title. */
@property (nonatomic, copy) NSString *headerTitle;

/** The section header height. */
@property (nonatomic) CGFloat headerHeight;

/** The section header view. This can be any subclass of UIView (e.g. UILabel or UIImageView). The section automatically adjusts the height of its header to accommodate this view. */
@property (nonatomic, strong) UIView *headerView;

/** The section footer title. */
@property (nonatomic, copy) NSString *footerTitle;

/** The section footer height. */
@property (nonatomic) CGFloat footerHeight;

/** The section footer view. This can be any subclass of UIView (e.g. UILabel or UIImageView). The section automatically adjusts the height of its footer to accommodate this view. */
@property (nonatomic, strong) UIView *footerView;

/** The set of section action blocks. */
@property (nonatomic, readonly) SCSectionActions *sectionActions;

/** The set of cell action blocks that get applied to all the section's cells.
 @note Cell actions defined in the section's individual cells will override any actions set here.
 */
@property (nonatomic, readonly) SCCellActions *cellActions;

/** Set this property to an array of UIImageView objects to be set to each of the section's cells. */
@property (nonatomic, strong) NSArray *cellsImageViews;

/** Options for the generated detail view controller (only applicable to subclasses with a detail view controller). */
@property (nonatomic, readonly) SCDetailViewControllerOptions *detailViewControllerOptions;

/** Options for the generated detail view controller for new items (only applicable to subclasses with a detail view controller). */
@property (nonatomic, readonly) SCDetailViewControllerOptions *newItemDetailViewControllerOptions;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuring Special Cells
//////////////////////////////////////////////////////////////////////////////////////////

/** When set to a valid SCExpandCollapseCell, the cell will control if the section's content is expanded or collapsed. 
 *
 *  @note Setting this property automatically adds expandCollapseCell to the section at index 0, making it the first cell.
 */
@property (nonatomic, strong) SCExpandCollapseCell *expandCollapseCell;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuring Theme Styles
//////////////////////////////////////////////////////////////////////////////////////////

/** The theme style of the section. */
@property (nonatomic, copy) NSString *themeStyle;

/** The theme style of the first cell in the section. */
@property (nonatomic, copy) NSString *firstCellThemeStyle;

/** The theme style of the even cells in the section. */
@property (nonatomic, copy) NSString *evenCellsThemeStyle;

/** The theme style of the odd cells in the section. */
@property (nonatomic, copy) NSString *oddCellsThemeStyle;

/** The theme style of the last cell in the section. */
@property (nonatomic, copy) NSString *lastCellThemeStyle;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Cells
//////////////////////////////////////////////////////////////////////////////////////////

/** The number of cells in the section. */
@property (nonatomic, readonly) NSUInteger cellCount;

/** Adds a new cell to the section.
 *	@param cell Must be a valid non nil SCTableViewCell. */
- (void)addCell:(SCTableViewCell *)cell;

/** Inserts a new cell into the section at the specified index.
 *	@param cell Must be a valid non nil SCTableViewCell.
 *  @param index Must be less than the total number of cells. */
- (void)insertCell:(SCTableViewCell *)cell atIndex:(NSUInteger)index;

/** Returns the cell at the specified index.
 *  @param index Must be less than the total number of cells. */
- (SCTableViewCell *)cellAtIndex:(NSUInteger)index;

/** Removes the cell at the specified index.
 *  @param index Must be less than the total number of cells. */
- (void)removeCellAtIndex:(NSUInteger)index;

/** Removes all cells in section. */
- (void)removeAllCells;

/** Returns the index of the specified cell.
 *	@param cell Must be a valid non nil SCTableViewCell.
 *	@return If cell is not found, method returns NSNotFound. */
- (NSUInteger)indexForCell:(SCTableViewCell *)cell;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Cell Values
//////////////////////////////////////////////////////////////////////////////////////////

/**	This property is TRUE if all the section cells' values are valid, otherwise it's FALSE. */
@property (nonatomic, readonly) BOOL valuesAreValid;

/**	This property is TRUE if any of the section's cells need to be commited, otherwise it's FALSE. */
@property (nonatomic, readonly) BOOL needsCommit;

/** 
 Set this property to TRUE for the section cells to commit their values as soon as they
 are changed. If this value is FALSE, the user must explicitly call commitCellChanges
 for the cells to commit their value changes. Default: TRUE. 
 */
@property (nonatomic, readwrite) BOOL commitCellChangesLive;

/** Commits value changes for all cells in section. This method needs to be called only if the commitCellChangesLive property is FALSE. */
- (void)commitCellChanges;

/** Overrides optimization and sets all cells as needing to be committed. */
- (void)invalidateCellCommits;

/** Reload's the section's bound values in case the associated bound objects or keys valuea has changed by means other than the cells themselves (e.g. external custom code). */
- (void)reloadBoundValues;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Provides subclasses with the framework to bind an SCTableViewSection to an NSObject */
@property (nonatomic, strong) NSObject *boundObject;

/** The data store of the cell's bound object. */
@property (nonatomic, strong) SCDataStore *boundObjectStore;

/** Provides subclasses with the framework to bind an SCTableViewSection to an NSObject */
@property (nonatomic, readonly) NSString *boundPropertyName;

/** Provides subclasses with the framework to bind an SCTableViewSection to a value */
@property (nonatomic, strong) NSObject *boundValue;

/** Called internally by the framework to determine the cell height at the specified indexPath. */
- (CGFloat)heightForCellAtIndexPath:(NSIndexPath *)indexPath;

/** Method called internally whenever the owner table view's editing mode is about to change. */
- (void)editingModeWillChange;

/** Method called internally whenever the owner table view's editing mode has changed. */
- (void)editingModeDidChange;

/** 
 Method should be overridden by subclasses to support property attributes. 
 
 The method should be able to set the subclass' specific attributes to its corresponding SCPropertyAttributes subclass. */
- (void) setAttributesTo:(SCPropertyAttributes *)attributes;

/** Method called internally. */
- (void)setDetailViewControllerOptions:(SCDetailViewControllerOptions *)options;

/** Method called internally. */
- (void)setNewItemDetailViewControllerOptions:(SCDetailViewControllerOptions *)options;

/** Internal method that sets the section's expanded state. Never set this property to expand/collapse the section. Use the expandCollapseCell property instead. */
- (void)setExpanded:(BOOL)expanded;


@end






/****************************************************************************************/
/*	class SCObjectSection	*/
 /****************************************************************************************/ 
/**
 This class functions as an SCTableViewModel section that is able to automatically generate
 its cells from a given bound object's properties. If the bound object is given without an extended
 class definition (SCClassDefinition), then cells will only be generated for properties of type 
 NSString and NSNumber, and will be either of type SCTextFieldCell or SCNumericTextFieldCell,
 respectively. If an SCClassDefinition is provided for the bound object, a full fledged
 section of cells will be generated.
 
 @note: For your convenience, the tag property of each generated cell will have a number corresponding
 to the index of it's corresponding property in bound object.
 
 @see SCArrayOfObjectsSection, SCObjectCell.
 */
@interface SCObjectSection : SCTableViewSection
{
	SCPropertyGroup *propertyGroup;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCObjectSection' given a header title and a bound object.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param object The object that 'SCObjectSection' will use to generate its cells.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle boundObject:(NSObject *)object;

/** Allocates and returns an initialized 'SCObjectSection' given a header title, a bound object and its extended class definition.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param object The object that 'SCObjectSection' will use to generate its cells.
 *	@param definition The definition for the object.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle
                 boundObject:(NSObject *)object
       boundObjectDefinition:(SCDataDefinition *)definition;

/** Allocates and returns an initialized 'SCObjectSection' given a header title, a bound object and its extended class definition along with a property group.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param object The object that 'SCObjectSection' will use to generate its cells.
 *	@param definition The definition for the object.
 *  @param group Limits the generated properties to the ones in the property group only.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle
                 boundObject:(NSObject *)object
       boundObjectDefinition:(SCDataDefinition *)definition
               propertyGroup:(SCPropertyGroup *)group;

/** Allocates and returns an initialized 'SCObjectSection' given a header title, a bound object and its data store along with a property group.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param object The object that 'SCObjectSection' will use to generate its cells.
 *	@param definition The definition for the object.
 *  @param group Limits the generated properties to the ones in the property group only.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle
                 boundObject:(NSObject *)object
            boundObjectStore:(SCDataStore *)store
               propertyGroup:(SCPropertyGroup *)group;


/** Returns an initialized 'SCObjectSection' given a header title and a bound object.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param object The object that 'SCObjectSection' will use to generate its cells.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
              boundObject:(NSObject *)object;

/** Returns an initialized 'SCObjectSection' given a header title, a bound object and its extended class definition.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param object The object that 'SCObjectSection' will use to generate its cells.
 *	@param definition The definition for the object.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
              boundObject:(NSObject *)object
    boundObjectDefinition:(SCDataDefinition *)definition;

/** Returns an initialized 'SCObjectSection' given a header title, a bound object and its extended class definition along with a property group.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param object The object that 'SCObjectSection' will use to generate its cells.
 *	@param definition The definition for the object.
 *  @param group Limits the generated properties to the ones in the property group only.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
              boundObject:(NSObject *)object
    boundObjectDefinition:(SCDataDefinition *)definition
            propertyGroup:(SCPropertyGroup *)group;

/** Returns an initialized 'SCObjectSection' given a header title, a bound object and its data store along with a property group.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param object The object that 'SCObjectSection' will use to generate its cells.
 *	@param definition The definition for the object.
 *  @param group Limits the generated properties to the ones in the property group only.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
              boundObject:(NSObject *)object
         boundObjectStore:(SCDataStore *)store
            propertyGroup:(SCPropertyGroup *)group;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Cell Management
//////////////////////////////////////////////////////////////////////////////////////////

/**	Returns the cell associated with the given bound object's property name.
 *	@param propertyName The bound object's property name.
 *	@return Returns nil if no cell have been generated for the given property name, or if the property name does not exist within the bound object. 
 */
- (SCTableViewCell *)cellForPropertyName:(NSString *)propertyName;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Other
//////////////////////////////////////////////////////////////////////////////////////////

/** The section's bound object's group of properties that are rendered into cells. */
@property (nonatomic, strong) SCPropertyGroup *propertyGroup;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Generates cells based on boundObjectClassDefinition and the given editing state. */
- (void)generateCellsForEditingState:(BOOL)editing;

@end





/****************************************************************************************/
/*	class SCArrayOfItemsSection	*/
 /****************************************************************************************/ 
/**
 This class functions as an SCTableViewModel section that is able to represent an array
 of any kind of items and automatically generate its cells from these items. The section
 is also able to handle all end-user interaction with the generated cells, including adding,
 editing, deleting, and moving the cells. When cells are added or edited, detail views are
 automatically generated for this purpose. To enable adding cells, the class' user should
 set the addButtonItem property of the section to a valid UIBarButtonItem, the section then
 will automatically add new items when the button is tapped. 
 
 When adding new items to the array, the section starts by first asking the model's data source
 to provide a new item using its tableViewModel:newItemForArrayOfItemsSectionAtIndex: 
 SCTableViewModelDataSource protocol method.
 
 This class is an abstract base class. Subclasses of this class must override the
 buildDetailTableModel method. This method should return a model for the detail view to display.
 
 @warning This is an abstract base class,  you should never make any direct instances of it.
 
 @see SCArrayOfStringsSection, SCArrayOfObjectsSection.
 */
@interface SCArrayOfItemsSection : SCTableViewSection <SCViewControllerDelegate, SCTableViewControllerDelegate>
{
	//internal
    BOOL _isFetchingItems;
    BOOL itemsInSync;
	__weak SCTableViewModel *activeDetailModel;  // the current active detail model
	NSMutableArray *cellReuseIdentifiers;
    CGFloat _cachedCellHeight;
    NSObject *tempItem;		//used for temporarily storing newly added items
	
    SCDataStore *dataStore;
    SCDataFetchOptions *dataFetchOptions;
    
    BOOL autoFetchItems;
	UITableViewCellAccessoryType itemsAccessoryType;
	BOOL allowAddingItems;
	BOOL allowDeletingItems;
	BOOL allowMovingItems;
	BOOL allowEditDetailView;
	BOOL allowRowSelection;
    BOOL skipNewItemDetailView;
	BOOL autoSelectNewItemCell;
    
	NSString *cellIdentifier;
	NSIndexPath *selectedCellIndexPath;
	UIBarButtonItem *addButtonItem;
	
    SCTableViewCell *placeholderCell;
    SCTableViewCell *addNewItemCell;
    BOOL addNewItemCellExistsInNormalMode;
    BOOL addNewItemCellExistsInEditingMode;
    
    BOOL enableIdenticalCellHeight;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Allocates and returns an initialized 'SCArrayOfItemsSection' given a data store.
 
 @param sectionHeaderTitle A header title for the section.
 @param store The data store containing the section's items.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle dataStore:(SCDataStore *)store;


/** 
 Returns an initialized 'SCArrayOfItemsSection' given a data store.
 
 @param sectionHeaderTitle A header title for the section.
 @param store The data store containing the section's items.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle dataStore:(SCDataStore *)store;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The data store that's used to store and fetch the section's items. */
@property (nonatomic, strong) SCDataStore *dataStore;

/** The options used to fetch the section's items from dataStore. */
@property (nonatomic, strong) SCDataFetchOptions *dataFetchOptions;

/** The items fetched from dataStore. */
@property (nonatomic, readonly) NSArray *items;

/** Set to FALSE to disable the section from automatically fetching its items from dataStore. Default: TRUE. */
@property (nonatomic, readwrite) BOOL autoFetchItems;

/** This property is TRUE when the section is in a state of fetching its items from their dataStore. */
@property (nonatomic, readonly) BOOL isFetchingItems;

/** The accessory type of the generated cells. */
@property (nonatomic, readwrite) UITableViewCellAccessoryType itemsAccessoryType;

/** Allows/disables adding new cells/items to the items array. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowAddingItems;

/** Allows/disables deleting new cells/items from the items array. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowDeletingItems;

/** Allows/disables moving cells/items from one row to another. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowMovingItems;

/** 
 Allows/disables a detail view for editing items' values. Default: TRUE. 
 
 Detail views are automatically generated for editing new items. You can control wether the
 view appears as a modal view or gets pushed to the navigation stack using the detailViewModal
 property. Modal views have the added feature of giving the end user a Cancel and Done buttons.
 The Cancel button cancels all user's actions, while the Done button commits them. Also, if the
 cell's validation is enabled, the Done button will remain disabled until all cells' values
 are valid.
 */
@property (nonatomic, readwrite) BOOL allowEditDetailView;

/** Allows/disables row selection. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowRowSelection;


/** Set to TRUE to directly add the new item without displaying a detail view. This is useful in situations where the item details will be filled in programmatically by implementing the SCTableViewModelDelegate 'tableViewModel:itemAddedForSectionAtIndexPath:item:' method. Default: FALSE. */
@property (nonatomic, readwrite) BOOL skipNewItemDetailView;

/** Allows/disables automatic cell selection of newly created items. Default: TRUE. */
@property (nonatomic, readwrite) BOOL autoSelectNewItemCell;

/**	
 Set this property to a valid UIBarButtonItem. When addButtonItem is tapped and allowAddingItems
 is TRUE, a detail view is automatically generated for the user to enter the new items
 properties. If the properties are commited, a new item is added to the array.
 
 @see addNewItemCell
 */
@property (nonatomic, strong) UIBarButtonItem *addButtonItem;

/**	
 When the cell count of 'SCArrayOfItemsSection' reaches this limit, it will automatically calculate the first cell height and use it for all remaining cells, instead of calculating the height for each cell separately. This dratically improves performance for sections with a huge number of cells. Default: 100.
 
 @note In case the section has a huge number of cells, but their heights are not identical, consider implementing the sectionAction called customHeightForRowAtIndexPath.
 
 @see SCSectionActions.customHeightForRowAtIndexPath
 */
@property (nonatomic, readwrite) NSUInteger cellCountLimitForAutoHeight;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuring Special Cells
//////////////////////////////////////////////////////////////////////////////////////////

/** This cell is automatically displayed when more section items need to be fetched. This is typically the case when the section's dataFetchOptions has specified a batchSize. */
@property (nonatomic, strong) SCFetchItemsCell *fetchItemsCell;

/** When set to a valid cell object, 'placeholderCell' will be displayed when no items exist in the section. As soon as any items are added, this cell automatically disappears. Default: nil. */
@property (nonatomic, strong) SCTableViewCell *placeholderCell;

/** 
 When set to a valid cell object, 'addNewItemCell' will be displayed as the last cell of the section, and will add a new item to the section whenever it gets tapped by the user. This cell can be used as an alternative to addButtonItem. Default: nil.
 @see addButtonItem
 */
@property (nonatomic, strong) SCTableViewCell *addNewItemCell;

/** When TRUE, addNewItemCell will be displayed in Normal Mode. Default: TRUE. */
@property (nonatomic, readwrite) BOOL addNewItemCellExistsInNormalMode;

/** When TRUE, addNewItemCell will be displayed in Editing Mode. Default: TRUE. */
@property (nonatomic, readwrite) BOOL addNewItemCellExistsInEditingMode;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Manual Event Control
//////////////////////////////////////////////////////////////////////////////////////////

/** User can call this method to dispatch an AddNewItem event, the same event dispached when the end-user taps addButtonItem. */
- (void)dispatchEventAddNewItem;

/** User can call this method to dispatch a SelectRow event, the same event dispached when the end-user selects a cell. */
- (void)dispatchEventSelectRowAtIndexPath:(NSIndexPath *)indexPath;

/** User can call this method to dispatch a RemoveRow event, the same event dispached when the end-user taps the delete button on a cell. */
- (void)dispatchEventRemoveRowAtIndexPath:(NSIndexPath *)indexPath;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Called internally by framework to store the currently selected cell index path. */
@property (nonatomic, strong) NSIndexPath *selectedCellIndexPath;

/** Subclasses should use this property when creating new dequeable cells */
@property (nonatomic, readonly) NSString *cellIdentifier;

/** Used internally by the framework. */
- (NSMutableArray *)mutableItems;

/** Used internally by the framework. */
- (void)setMutableItems:(NSMutableArray *)mutableItems;

/** Used internally by the framework. */
- (void)fetchItems:(id)sender;

/** Used internally by the framework. */
- (void)didFetchItems:(NSArray *)fetchedItems sender:(id)sender;

/** Used internally by the framework. */
- (void)addSpecialCellsToItems;

/** Used internally by the framework. */
- (void)removeSpecialCellsFromItems;

/**	Subclasses should override this method to handle the creation of section cells */
- (SCTableViewCell *)createCellAtIndex:(NSUInteger)index;

/**	Subclasses should override this method to manage setting the properties cells after being dequed from the table view */
- (void)setPropertiesForCell:(SCTableViewCell *)cell withIndex:(NSUInteger)index;

/** Subclasses should override this method to set the text for each generated cell */
- (NSString *)textForCellAtIndex:(NSUInteger)index;

/** Subclasses should override this method to set the detail text for each generated cell */
- (NSString *)detailTextForCellAtIndex:(NSUInteger)index;

/** Method is called internally when the generated cells are selected */
- (void)didSelectCellAtIndexPath:(NSIndexPath *)indexPath;

/** Method is called internally when the generated cells will deselect */
- (void)willDeselectCellAtIndexPath:(NSIndexPath *)indexPath;

/** Subclasses should override this method to handle when generated cells are deleted */
- (void)commitEditingStyle:(UITableViewCellEditingStyle)editingStyle
		forCellAtIndexPath:(NSIndexPath *)indexPath;

/** Subclasses should override this method to handle when generated cells are about to move. */
- (NSIndexPath *)targetIndexPathForMoveFromCellAtIndexPath:(NSIndexPath *)sourceIndexPath toProposedIndexPath:(NSIndexPath *)proposedIndexPath;

/** Subclasses should override this method to handle when generated cells are moved */
- (void)moveCellAtIndexPath:(NSIndexPath *)fromIndexPath 
			   toIndexPath:(NSIndexPath *)toIndexPath;

/** Subclasses should override this method to handle when addButtonItem is tapped */
- (void)didTapAddButtonItem;

/** Subclasses should override this method to handle creating a new item */
- (NSObject *)createNewItem;

/**	Subclasses must override this method to build a model for the generated detail view.
 *	@param detailTableModel This is an empty model. Method should add sections and cells to this model.
 *	@param item The item that the model should be built for. If item is nil, a model should be buil for a new item. */
- (void)buildDetailTableModel:(SCTableViewModel *)detailTableModel forItem:(NSObject *)item;

/** Subclasses must override this method to determine the navigation bar type of the detail view for the given item. */
- (SCNavigationBarType)getDetailViewNavigationBarTypeForItem:(NSObject *)item newItem:(BOOL)newItem;

/** Commits the newly created item. */
- (void)commitNewItem:(NSObject *)newItem detailModel:(SCTableViewModel *)detailModel;

/** Subclasses should override this method to handle adding a cell for newItem (method called internally by framework) */
- (void)addCellForNewItem:(NSObject *)newItem;

/** Subclasses should override this method to handle adding an item getting modified (method called internally by framework) */
- (void)itemModified:(NSObject *)item;

/** Method gets called internally by framework. */
- (void)commitAndProcessChangesForDetailModel:(SCTableViewModel *)detailModel;

@end






/****************************************************************************************/
/*	class SCArrayOfObjectsSection	*/
/****************************************************************************************/ 
/**
 This class functions as an SCTableViewModel section that is able to represent an array
 of any kind of objects and automatically generate its cells from these objects. In addition,
 'SCArrayOfObjectsSection' generates its detail views from the properties of the corresponding
 object in its items array. Objects in the items array need not all be of the same object type, but
 they must all decend from NSObject. If more than one type of object is present in the items
 array, then their respective class definitions should be added to the itemsClassDefinitions
 set.
 
 @see SCObjectSection, SCObjectCell.
 */
@interface SCArrayOfObjectsSection : SCArrayOfItemsSection

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCArrayOfObjectsSection' given a header title and an array of objects.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param sectionItems A mutable array of objects that the section will use to generate its cells.
 *	@param definition The definition of the objects in the objects array. If the array contains more than one type of object, then their respective definitions must be added to the itemsDefinitions dictionary after initialization.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle
                       items:(NSMutableArray *)sectionItems
             itemsDefinition:(SCDataDefinition *)definition;

/** Returns an initialized 'SCArrayOfObjectsSection' given a header title and an array of objects.
 *
 *	@param sectionHeaderTitle A header title for the section.
 *	@param sectionItems A mutable array of objects that the section will use to generate its cells.
 *	@param definition The definition of the objects in the objects array. If the array contains more than one type of object, then their respective definitions must be added to the itemsDefinitions dictionary after initialization.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
                    items:(NSMutableArray *)sectionItems
          itemsDefinition:(SCDataDefinition *)definition;

@end







/****************************************************************************************/
/*	class SCArrayOfStringsSection	*/
/****************************************************************************************/ 
/**
 This class functions as an SCTableViewModel section that is able to represent an array
 of string items and automatically generate its cells from these items. The class inherits
 all its funtionality from its superclass: SCArrayOfItemsSection, except that its items
 array can only contain items of type NSString.
 */

@interface SCArrayOfStringsSection : SCArrayOfObjectsSection

/** 
 Allocates and returns an initialized 'SCArrayOfItemsSection' given a header title and an array of NSString items.
 
 @param sectionHeaderTitle A header title for the section.
 @param sectionItems An array of items that the section will use to generate its cells. This array must be of type NSMutableArray, as it must support the section's add, delete, and move operations. All array objects must be of type NSString.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle items:(NSMutableArray *)sectionItems;

/** 
 Returns an initialized 'SCArrayOfItemsSection' given a header title and an array of NSString items.
 
 @param sectionHeaderTitle A header title for the section.
 @param sectionItems An array of items that the section will use to generate its cells. This array must be of type NSMutableArray, as it must support the section's add, delete, and move operations. All array objects must be of type NSString.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle items:(NSMutableArray *)sectionItems;

@end







/****************************************************************************************/
/*	class SCSelectionSection	*/
/****************************************************************************************/ 
/**
 This class functions as an SCTableViewModel section that is able to provide selection functionality. 
 The cells in this section represent different items that the end-user can select from, and they
 are generated from NSStrings in its items array. Once a cell is selected, a checkmark appears next
 to it, similar to Apple's Settings application where a user selects a Ringtone for their
 iPhone. The section can be configured to allow multiple selection and to allow no selection at all.
 	
 There are three ways to set/retrieve the section's selection:
 - Through binding an object to the section, and specifying a property name to bind the selection index
 result to. The bound property must be of type NSMutableSet if multiple selection is allowed, otherwise
 it must be of type NSNumber or NSString.
 - Through binding a key to the section and setting/retrieving through the ownerTableViewModel modelKeyValues property.
 - Through the selectedItemsIndexes or selectedItemIndex properties.
 
 @see SCSelectionCell.
 */ 
@interface SCSelectionSection : SCArrayOfStringsSection
{	
	BOOL boundToNSNumber;	//internal
	BOOL boundToNSString;	//internal
	NSIndexPath *lastSelectedRowIndexPath; //internal
	
	BOOL allowMultipleSelection;
	BOOL allowNoSelection;
	NSUInteger maximumSelections;
	BOOL autoDismissViewController;
	NSMutableSet *_selectedItemsIndexes;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Allocates and returns an initialized 'SCSelectionSection' given a header title, a bound object, an NSNumber bound property name, and an array of selection items.
 
 @param sectionHeaderTitle A header title for the section.
 @param object The object the section will bind to.
 @param propertyName The property name present in the bound object that the section will bind to and
 will automatically change the value of to reflect the section's current selection. This property must
 be of type NSNumber and can't be a readonly property. The section will also initialize its selection 
 from the value present in this property.
 @param sectionItems An array of the items that the user will choose from. All items must be of
 an NSString type.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle
                 boundObject:(NSObject *)object 
   selectedIndexPropertyName:(NSString *)propertyName 
                       items:(NSArray *)sectionItems;

/** 
 Allocates and returns an initialized 'SCSelectionSection' given a header title, a bound object,
 an NSMutableSet bound property name, an array of selection items, and wether to allow multiple selection.
 
 @param sectionHeaderTitle A header title for the section.
 @param object The object the section will bind to.
 @param propertyName The property name present in the bound object that the section will bind to and
 will automatically change the value of to reflect the section's current selection(s). This property must
 be of type NSMutableSet. The section will also initialize its selection(s) from the value present
 in this property.
 @param sectionItems An array of the items that the user will choose from. All items must be of
 an NSString type.
 @param multipleSelection Determines if multiple selection is allowed.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle
                 boundObject:(NSObject *)object 
                    selectedIndexesPropertyName:(NSString *)propertyName 
                       items:(NSArray *)sectionItems 
	   allowMultipleSelection:(BOOL)multipleSelection;

/** 
 Allocates and returns an initialized 'SCSelectionSection' given a header title, a bound object,
 an NSString bound property name, and an array of selection items.
 
 @param sectionHeaderTitle A header title for the section.
 @param object The object the section will bind to.
 @param propertyName The property name present in the bound object that the section will bind to and
 will automatically change the value of to reflect the section's current selection. This property must
 be of type NSString and can't be a readonly property. The section will also initialize its selection 
 from the value present in this property.
 @param sectionItems An array of the items that the user will choose from. All items must be of
 an NSString type.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle
                 boundObject:(NSObject *)object 
                    selectionStringPropertyName:(NSString *)propertyName 
                       items:(NSArray *)sectionItems;


/** 
 Returns an initialized 'SCSelectionSection' given a header title, a bound object,
 an NSNumber bound property name, and an array of selection items.
 
 @param sectionHeaderTitle A header title for the section.
 @param object The object the section will bind to.
 @param propertyName The property name present in the bound object that the section will bind to and
 will automatically change the value of to reflect the section's current selection. This property must
 be of type NSNumber and can't be a readonly property. The section will also initialize its selection 
 from the value present in this property.
 @param sectionItems An array of the items that the user will choose from. All items must be of
 an NSString type.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
              boundObject:(NSObject *)object 
                selectedIndexPropertyName:(NSString *)propertyName 
                    items:(NSArray *)sectionItems;

/** 
 Returns an initialized 'SCSelectionSection' given a header title, a bound object,
 a bound property name, an array of selection items, and wether to allow multiple selection.
 
 @param sectionHeaderTitle A header title for the section.
 @param object The object the section will bind to.
 @param propertyName The property name present in the bound object that the section will bind to and
 will automatically change the value of to reflect the section's current selection(s). This property must
 be of type NSMutableSet. The section will also initialize its selection(s) from the value present
 in this property. Every item in this set must be an NSNumber that represent the index of the selected cell(s).
 @param sectionItems An array of the items that the user will choose from. All items must be of
 an NSString type.
 @param multipleSelection Determines if multiple selection is allowed.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
              boundObject:(NSObject *)object 
                selectedIndexesPropertyName:(NSString *)propertyName 
                    items:(NSArray *)sectionItems 
	allowMultipleSelection:(BOOL)multipleSelection;

/** 
 Returns an initialized 'SCSelectionSection' given a header title, a bound object,
 an NSString bound property name, and an array of selection items.
 
 @param sectionHeaderTitle A header title for the section.
 @param object The object the section will bind to.
 @param propertyName The property name present in the bound object that the section will bind to and
 will automatically change the value of to reflect the section's current selection. This property must
 be of type NSString and can't be a readonly property. The section will also initialize its selection 
 from the value present in this property.
 @param sectionItems An array of the items that the user will choose from. All items must be of
 an NSString type.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
              boundObject:(NSObject *)object 
                    selectionStringPropertyName:(NSString *)propertyName 
                    items:(NSArray *)sectionItems;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 This property reflects the current section's selection. You can set this property
 to define the section's selection.
 
 @note If you have bound this section to an object or a key, you can define the section's selection
 using either the bound property value or the key value, respectively. 
 @note In case of no selection, this property will be set to an NSNumber of value -1. 
 */
@property (nonatomic, copy) NSNumber *selectedItemIndex;

/** 
 This property reflects the current section's selection(s). You can add index(es) to the set
 to define the section's selection.
 
 @note If you have bound this section to an object or a key, you can define the section's selection
 using either the bound property value or the key value, respectively.
 */
@property (nonatomic, readonly) NSMutableSet *selectedItemsIndexes;

/** If TRUE, the section allows multiple selection. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowMultipleSelection;

/** If TRUE, the section allows no selection at all. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowNoSelection;

/** The maximum number of items that can be selected. Set to zero to allow an infinite number of selections. Default: 0.
 *	@note Only applicable when allowMultipleSelection is TRUE. */
@property (nonatomic, readwrite) NSUInteger maximumSelections;

/** If TRUE, the section automatically dismisses the current view controller when a value is selected. Default: FALSE. */
@property (nonatomic, readwrite) BOOL autoDismissViewController;


@end









/****************************************************************************************/
/*	class SCObjectSelectionSection	*/
/****************************************************************************************/ 
/**	
 This class functions as a section that provides the end-user with an automatically generated
 list objects to choose from. 
 
 The selection items are provided in the form of an array of
 NSObjects, called the items array. 'SCObjectSelectionSection' can be configured to allow multiple
 selection and to allow no selection at all. If allow multiple selection is disabled, then
 the bound property name of this section must be of type NSObject, otherwise
 it must be of type NSMutableSet. 
 
 @see SCObjectSelectionCell.
 */ 
@interface SCObjectSelectionSection : SCArrayOfObjectsSection
{	
	NSIndexPath *lastSelectedRowIndexPath; //internal
	
    NSMutableSet *selectedItemsIndexes;
	BOOL allowMultipleSelection;
	BOOL allowNoSelection;
	NSUInteger maximumSelections;
	BOOL autoDismissViewController;
    SCDataDefinition *intermediateEntityDefinition;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCObjectSelectionSection' given cell text, bound object, a bound property name, and a selection items store.
 *	@param sectionHeaderTitle The section's header title.
 *	@param object	The object the section will bind to.
 *	@param propertyName The section's bound property name corresponding to the object selection. If multiple selection is allowed, then property must be of an NSMutableSet type, otherwise, property must be of type NSObject and cannot be a readonly property.
 *	@param store The store containing the selection objects.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle
                 boundObject:(NSObject *)object selectedObjectPropertyName:(NSString *)propertyName
         selectionItemsStore:(SCDataStore *)store;

/** Allocates and returns an initialized 'SCObjectSelectionSection' given cell text, bound object, a bound property name, and an array of selection items.
 *	@param sectionHeaderTitle The section's header title.
 *	@param object	The object the section will bind to.
 *	@param propertyName The section's bound property name corresponding to the object selection. If multiple selection is allowed, then property must be of an NSMutableSet type, otherwise, property must be of type NSObject and cannot be a readonly property.
 *	@param sectionItems An array of the items that the user will choose from. All items must be of an NSObject type and all items must be instances of the same class.
 *	@param definition The definition of the selection items.
 */
+ (id)sectionWithHeaderTitle:(NSString *)sectionHeaderTitle
                 boundObject:(NSObject *)object selectedObjectPropertyName:(NSString *)propertyName
                       items:(NSArray *)sectionItems itemsDefintion:(SCDataDefinition *)definition;


/** Returns an initialized 'SCObjectSelectionSection' given cell text, bound object, a bound property name, and a selection items store.
 *	@param sectionHeaderTitle The section's header title.
 *	@param object	The object the section will bind to.
 *	@param propertyName The section's bound property name corresponding to the object selection. If multiple selection is allowed, then property must be of an NSMutableSet type, otherwise, property must be of type NSObject and cannot be a readonly property.
 *	@param store The store containing the selection objects.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
              boundObject:(NSObject *)object selectedObjectPropertyName:(NSString *)propertyName
      selectionItemsStore:(SCDataStore *)store;

/** Returns an initialized 'SCObjectSelectionSection' given cell text, bound object, a bound property name, and an array of selection items.
 *	@param sectionHeaderTitle The section's header title.
 *	@param object	The object the section will bind to.
 *	@param propertyName The section's bound property name corresponding to the object selection. If multiple selection is allowed, then property must be of an NSMutableSet type, otherwise, property must be of type NSObject and cannot be a readonly property.
 *	@param sectionItems An array of the items that the user will choose from. All items must be of an NSObject type and all items must be instances of the same class.
 *	@param definition The definition of the selection items.
 */
- (id)initWithHeaderTitle:(NSString *)sectionHeaderTitle
              boundObject:(NSObject *)object selectedObjectPropertyName:(NSString *)propertyName
                    items:(NSArray *)sectionItems itemsDefintion:(SCDataDefinition *)definition;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 This property reflects the current section's selection. You can set this property
 to define the section's selection.
 
 @note In case of no selection, this property will be set to an NSNumber of value -1. 
 */
@property (nonatomic, copy) NSNumber *selectedItemIndex;

/** 
 This property reflects the current section's selection(s). You can add index(es) to the set
 to define the section's selection.
 */
@property (nonatomic, readonly) NSMutableSet *selectedItemsIndexes;

/** If TRUE, the section allows multiple selection. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowMultipleSelection;

/** If TRUE, the section allows no selection at all. Default: FALSE. */
@property (nonatomic, readwrite) BOOL allowNoSelection;

/** The maximum number of items that can be selected. Set to zero to allow an infinite number of selections. Default: 0.
 *	@note Only applicable when allowMultipleSelection is TRUE. */
@property (nonatomic, readwrite) NSUInteger maximumSelections;

/** If TRUE, the section automatically dismisses the current view controller when a value is selected. Default: FALSE. */
@property (nonatomic, readwrite) BOOL autoDismissViewController;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Other
//////////////////////////////////////////////////////////////////////////////////////////

/** Set this to the definition of the intermediate entity between the bound object's class definition and the itemsEntityClassDefinition. This is useful in complex many-to-many relationships where you have created an intermediate entity between you main two entities. Default: nil. */
@property (nonatomic, strong) SCDataDefinition *intermediateEntityDefinition;


@end


