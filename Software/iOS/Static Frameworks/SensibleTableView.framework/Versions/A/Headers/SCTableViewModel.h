/*
 *  SCTableViewModel.h
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
#import "SCDataDefinition.h"
#import "SCUserDefaultsDefinition.h"
#import "SCDataStore.h"
#import "SCTableViewSection.h"
#import "SCPullToRefreshView.h"
#import "SCInputAccessoryView.h"
#import "SCDetailViewControllerOptions.h"
#import "SCModelActions.h"
#import "SCTheme.h"


/****************************************************************************************/
/*	class SCTableViewModel	*/
/****************************************************************************************/ 
/**
 This class is the master mind behind all of Sensible TableView's functionality.
 
 Sensible TableView provides an alternative easy way to create sophisticated table views very quickly. 
 The sophistication of these table views can range from simple text cells, to cells with controls, to
 custom cells that get automatically generated from your own classes. SCTableViewModel also automatically
 generates detail views for common tasks such as selecting cell values or creating new objects.
 Using SCTableViewModel, you can easily create full functioning applications in a matter of minutes.
 
 SCTableViewModel is designed to be loosely coupled with your user interface elements. What this
 means is that you can use SCTableViewModel with Apple's default UITableView or with any of your 
 custom UITableView subclasses. Similarly, you can use SCTableViewModel with any UIViewController, or
 any of its subclasses, including UITableViewController or your own subclasses. 
 
 Architecture:
 
 An SCTableViewModel defines a table view model with several sections, each section being of type 
 SCTableViewSection. Each SCTableViewSection can contain several cells, each cell being of type
 SCTableViewCell. 
 */

@interface SCTableViewModel : NSObject <UITableViewDataSource, UITableViewDelegate, UIScrollViewDelegate, SCInputAccessoryViewDelegate>
{
	//internal
    NSIndexPath *lastReturnedCellIndexPath;     // used for optimization
    SCTableViewCell *lastReturnedCell;          // used for optimization
    NSIndexPath *lastVisibleCellIndexPath;      // user for optimization
	__unsafe_unretained id target;
	SEL action;
	__unsafe_unretained SCTableViewModel *masterModel;
    __unsafe_unretained SCTableViewModel *activeDetailModel;
	
	__unsafe_unretained UITableView *_tableView;
	__unsafe_unretained id dataSource;
	__unsafe_unretained id delegate;
	UIBarButtonItem *editButtonItem;
	BOOL autoResizeForKeyboard;
	BOOL keyboardShown;
	CGFloat keyboardOverlap;
    SCInputAccessoryView *_inputAccessoryView;
	
	NSMutableArray *sections;
	
	NSArray *sectionIndexTitles;
	BOOL autoGenerateSectionIndexTitles;
	BOOL autoSortSections;
	BOOL hideSectionHeaderTitles;
	BOOL lockCellSelection;
	NSInteger tag;
    
    BOOL enablePullToRefresh;
    SCPullToRefreshView *pullToRefreshView;
    
    BOOL autoAssignDataSourceForDetailModels;
    BOOL autoAssignDelegateForDetailModels;
    
    SCTableViewCell *activeCell;
    NSIndexPath *activeCellIndexPath;
    UIResponder *activeCellControl;
    
	UIBarButtonItem *commitButton;
    BOOL swipeToDeleteActive;
    
    SCModelActions *_modelActions;
    SCSectionActions *_sectionActions;
    SCCellActions *_cellActions;
    
    SCTheme *_theme;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCTableViewModel bound to a UITableView. 
 *
 * Upon the model's initialization, the model sets itself as the table view's dataSource and delegate, and starts providing it with its sections and cells.
 *
 * @param tableView The UITableView to be bound to the model. It's ok for this value to be nil if the table view is not yet available when the model is created.
 */
+ (id)modelWithTableView:(UITableView *)tableView;

/** Returns an initialized 'SCTableViewModel' bound to a UITableView.  
 *
 * Upon the model's initialization, the model sets itself as the table view's dataSource and delegate, and starts providing it with its sections and cells.
 *
 * @param tableView The UITableView to be bound to the model. It's ok for this value to be nil if the table view is not yet available when the model is created.
 */
- (id)initWithTableView:(UITableView *)tableView;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** Set to TRUE to enable pull-to-refresh functionality on the table view. Default: FALSE. 
 @see pullToRefreshView
 */
@property (nonatomic, readwrite) BOOL enablePullToRefresh;

/** Contains a valid SCPullToRefreshView that automatically provides pull-to-refresh functionality to the table view.
 @note enablePullToRefresh must be set to TRUE for this view to take effect. 
 */
@property (nonatomic, strong) SCPullToRefreshView *pullToRefreshView;

/**	
 When set to a valid UIBarButtonItem, SCTableViewModel automatically puts its table view
 in edit mode when the button is tapped. Note: Must be set if the model is to automatically
 show/hide editing mode sections.
 */
@property (nonatomic, strong) UIBarButtonItem *editButtonItem;

/** 
 If TRUE, SCTableViewModel will automatically resize its tableView when the
 keyboard appears. Property defualts to FALSE if viewController is a UITableViewController subclass,
 as UITableViewController will automatically handle the resizing. Otherwise, it defaults to TRUE.
 */
@property (nonatomic, readwrite) BOOL autoResizeForKeyboard;

/**
 An array of strings that serve as the title of sections in the tableView and
 appear in the index list on the right side of the tableView. tableView
 must be in plain style for the index to appear.
 */
@property (nonatomic, strong) NSArray *sectionIndexTitles;

/** 
 If TRUE, SCTableViewModel will automatically generate the sectionIndexTitles array from
 the first letter of each section's header title. Default: FALSE. 
 */
@property (nonatomic, readwrite) BOOL autoGenerateSectionIndexTitles;

/** 
 If TRUE, SCTableViewModel will automatically sort its sections alphabetically according to their header
 title value. To provide custom section sorting, implement the tableViewModel:sortSections: 
 SCTableViewDataSource method instead. Default: FALSE. 
 */
@property (nonatomic, readwrite) BOOL autoSortSections;

/** If TRUE, all section header titles will be hidden. Default: FALSE. */
@property (nonatomic, readwrite) BOOL hideSectionHeaderTitles;

/** If TRUE, SCTableViewModel will prevent any cell from being selected. Default: FALSE. 
 *	@note for preventing individual cells from being selected, use SCTableViewCell "selectable" property. */
@property (nonatomic, readwrite) BOOL lockCellSelection;

/** An integer that you can use to identify different table view models in your application. Any detail model automatically gets its tag set to be the value of its parent model's tag plus one. Default: 0. */
@property (nonatomic, readwrite) NSInteger tag;

/** 
 When set to a valid view controller, the model will use this controller to display all of its automatically generated detail views. This property is typically set in iPad applications where the model and its detailViewController co-exist in a UISplitViewController. 
 
 @warning detailViewController must be of type SCViewController or SCTableViewController only.
 */
@property (nonatomic, strong) UIViewController *detailViewController;

/** When TRUE, the model automatically assigns the dataSource of all detail models to the same value of its own data source. This option should be used carefully as setting it to TRUE will trigger all implemented dataSource methods for all detail models in the heirarchy. Default: FALSE. */
@property (nonatomic, readwrite) BOOL autoAssignDataSourceForDetailModels;

/** When TRUE, the model automatically assigns the delegate of all detail models to the same value of its own delegate. This option should be used carefully as setting it to TRUE will trigger all implemented delegate methods for all detail models in the heirarchy. Default: FALSE. */
@property (nonatomic, readwrite) BOOL autoAssignDelegateForDetailModels;

/** The set of model action blocks. */
@property (nonatomic, readonly) SCModelActions *modelActions;

/** The set of section action blocks that get applied to all the model's sections.
 @note Section actions defined in the model's individual sections will override any actions set here.
 */
@property (nonatomic, readonly) SCSectionActions *sectionActions;

/** The set of cell action blocks that get applied to all the model's cells.
 @note Cell actions defined in the model's individual cells will override any actions set here.
 */
@property (nonatomic, readonly) SCCellActions *cellActions;

/** The theme used to style the model's views. Default: nil. */
@property (nonatomic, strong) SCTheme *theme;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Sections
//////////////////////////////////////////////////////////////////////////////////////////

/** The number of sections in the model. */
@property (nonatomic, readonly) NSUInteger sectionCount;

/** Adds a new section to the model. 
 *	@param section Must be a valid non nil SCTableViewSection. */
- (void)addSection:(SCTableViewSection *)section;

/** Inserts a new section at the specified index. 
 *	@param section Must be a valid non nil SCTableViewSection.
 *	@param index Must be less than the total number of sections. */
- (void)insertSection:(SCTableViewSection *)section atIndex:(NSUInteger)index;

/** Returns the section at the specified index.
 *	@param index Must be less than the total number of sections. */
- (SCTableViewSection *)sectionAtIndex:(NSUInteger)index;

/** Returns the first section with the specified header title.
 *	@param title The header title. */
- (SCTableViewSection *)sectionWithHeaderTitle:(NSString *)title;

/** Returns the index of the specified section. 
 *	@param section Must be a valid non nil SCTableViewSection.
 *	@return If section is not found, method returns NSNotFound. */
- (NSUInteger)indexForSection:(SCTableViewSection *)section;

/** Removes the section at the specified index from the model.
 *	@param index Must be less than the total number of section. */
- (void)removeSectionAtIndex:(NSUInteger)index;

/** Removes all sections from the model. */
- (void)removeAllSections;

/** Generates sections using the given object and its data definition. The method fully utilizes the definition's groups feature by generating a section for each group. 
 *  @param object The object that the sections will be generated for.
 *  @param definition The object's definition.
 *  @warning Important: definition must be the data definition representing the given object.
 */
- (void)generateSectionsForObject:(NSObject *)object withDefinition:(SCDataDefinition *)definition;

/** Generates sections using the given object and its data definition. The method fully utilizes the definition's groups feature by generating a section for each group. 
 *  @param object The object that the sections will be generated for.
 *  @param definition The object's definition.
 *  @param newObject Set to TRUE if the generated sections are used represent a newly created fresh object, otherwise set to FALSE.
 *  @warning Important: definition must be the data definition representing the given object.
 */
- (void)generateSectionsForObject:(NSObject *)object withDefinition:(SCDataDefinition *)definition newObject:(BOOL)newObject;

/** Generates sections using the given object and its data store.  
 *  @param object The object that the sections will be generated for.
 *  @param store The object's data store.
 */
- (void)generateSectionsForObject:(NSObject *)object withDataStore:(SCDataStore *)store;

/** Generates sections using the given object and its data store.
 *  @param object The object that the sections will be generated for.
 *  @param store The object's data store.
 *  @param newObject Set to TRUE if the generated sections are used represent a newly created fresh object, otherwise set to FALSE.
 */
- (void)generateSectionsForObject:(NSObject *)object withDataStore:(SCDataStore *)store newObject:(BOOL)newObject;

/** Generates sections using a user defaults definition. */
- (void)generateSectionsForUserDefaultsDefinition:(SCUserDefaultsDefinition *)userDefaultsDefinition;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Cells
//////////////////////////////////////////////////////////////////////////////////////////

/** The current active cell. A cell becomes active if it is selected or if its value changes. */
@property (nonatomic, readonly) SCTableViewCell *activeCell;

/** The indexPath of activeCell. */
@property (nonatomic, readonly) NSIndexPath *activeCellIndexPath;

/** The current active input control. */
@property (nonatomic, readonly) UIResponder *activeCellControl;

/** Returns the cell at the specified indexPath.
 *	@param indexPath Must be a valid non nil NSIndexPath. */
- (SCTableViewCell *)cellAtIndexPath:(NSIndexPath *)indexPath;

/** Returns the index path for the specified cell.
 *	@param cell Must be a valid non nil SCTableViewCell.
 *	@return If cell is not found, method returns NSNotFound. */
- (NSIndexPath *)indexPathForCell:(SCTableViewCell *)cell;

/** Returns the indexPath of the cell that comes after the specified cell in the model.
 *	@param indexPath The indexPath of the current cell.
 *	@param rewind If TRUE and cell is the very last cell in the model, method returns the indexPath of the cell at the very top.
 *	@return Returns nil if cell is the last cell in the model and rewind is FALSE, or if cell does not exist in the model. */
- (NSIndexPath *)indexPathForCellAfterCellAtIndexPath:(NSIndexPath *)indexPath rewind:(BOOL)rewind;

/** Returns the indexPath of the cell that comes after the specified cell in the model.
 *	@param cell Must be a valid non nil SCTableViewCell.
 *	@param rewind If TRUE and cell is the very last cell in the model, method returns the indexPath of the cell at the very top.
 *	@return Returns nil if cell is the last cell in the model and rewind is FALSE, or if cell does not exist in the model. */
- (NSIndexPath *)indexPathForCellAfterCell:(SCTableViewCell *)cell rewind:(BOOL)rewind;

/** Returns the cell that comes after the specified cell in the model.
 *	@param cell Must be a valid non nil SCTableViewCell.
 *	@param rewind If TRUE and cell is the very last cell in the model, method returns the cell at the very top.
 *	@return Returns nil if cell is the last cell in the model and rewind is FALSE, or if cell does not exist in the model. */
- (SCTableViewCell *)cellAfterCell:(SCTableViewCell *)cell rewind:(BOOL)rewind;

/** Returns the indexPath of the cell that comes before the specified cell in the model.
 *	@param indexPath The indexPath of the current cell.
 *	@param rewind If TRUE and cell is the very first cell in the model, method returns the indexPath of the last cell.
 *	@return Returns nil if cell is the last cell in the model and rewind is FALSE, or if cell does not exist in the model. */
- (NSIndexPath *)indexPathForCellBeforeCellAtIndexPath:(NSIndexPath *)indexPath rewind:(BOOL)rewind;

/** Returns the indexPath of the cell that comes before the specified cell in the model.
 *	@param cell Must be a valid non nil SCTableViewCell.
 *	@param rewind If TRUE and cell is the very first cell in the model, method returns the indexPath of the last cell.
 *	@return Returns nil if cell is the last cell in the model and rewind is FALSE, or if cell does not exist in the model. 
 *  @see moveToNextCellControl:
 */
- (NSIndexPath *)indexPathForCellBeforeCell:(SCTableViewCell *)cell rewind:(BOOL)rewind;

/** Returns the cell that comes before the specified cell in the model.
 *	@param cell Must be a valid non nil SCTableViewCell.
 *	@param rewind If TRUE and cell is the very first cell in the model, method returns the last cell.
 *	@return Returns nil if cell is the last cell in the model and rewind is FALSE, or if cell does not exist in the model. 
 *  @see moveToPreviousCellControl:
 */
- (SCTableViewCell *)cellBeforeCell:(SCTableViewCell *)cell rewind:(BOOL)rewind;

/** Moves the first responder to the next cell control, automatically scrolling the table view as needed. If rewind is TRUE, the first responder is moved to the very first cell after the last cell has been reached.
 @note This method is typically used when you're overriding the framework's automatic handling of the keyboard's 'Return' button.
 */
- (void)moveToNextCellControl:(BOOL)rewind;

/** Moves the first responder to the previous cell control, automatically scrolling the table view as needed. If rewind is TRUE, the first responder is moved to the very last cell after the first cell has been reached.
 */
- (void)moveToPreviousCellControl:(BOOL)rewind;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Detail Views
//////////////////////////////////////////////////////////////////////////////////////////

/** Dismisses all detail views, commiting all changes when commit is TRUE, otherwise it will ignore all changes. */
- (void)dismissAllDetailViewsWithCommit:(BOOL)commit;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Model Values
//////////////////////////////////////////////////////////////////////////////////////////

/** TRUE if all the model's section and cell values are valid. */
@property (nonatomic, readonly) BOOL valuesAreValid;

/** 'SCTableViewModel' will automatically enable/disable the commitButton based on the valuesAreValid property, where commitButton is enabled if valuesAreValid is TRUE. */
@property (nonatomic, strong) UIBarButtonItem *commitButton;

/** Forces the commit of all section and cell values into their respective bound objects. There is usually no need to call this method manually as it's typically called by the framework when the user is ready to commit changes. */
- (void)commitChanges;

/** Reload's the model's bound values in case the associated bound objects or keys valuea has changed by means other than the cells themselves (e.g. external custom code). */
- (void)reloadBoundValues;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing the Data Source and Delegate
//////////////////////////////////////////////////////////////////////////////////////////

/** The object that acts as the data source of 'SCTableViewModel'. The object must adopt the SCTableViewModelDataSource protocol. */
@property (nonatomic, unsafe_unretained) id dataSource;

/** The object that acts as the delegate of 'SCTableViewModel'. The object must adopt the SCTableViewModelDelegate protocol. */
@property (nonatomic, unsafe_unretained) id delegate;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Miscellaneous
//////////////////////////////////////////////////////////////////////////////////////////

/**	The UITableView bound to 'SCTableViewModel'. */
@property (nonatomic, unsafe_unretained) UITableView *tableView;

/**	
 The UITableView bound to 'SCTableViewModel'.
 
 @warning This property has been deprecated. Use tableView instead.
 */
@property (nonatomic, unsafe_unretained) UITableView *modeledTableView;

/**	The UIViewController of tableView. */
@property (nonatomic, readonly) UIViewController *viewController;

/** The keyboard input accessory view responsible for providing keyboard navigation between the different responders. Set to nil to disable the accessory view functionality. */
@property (nonatomic, strong) SCInputAccessoryView *inputAccessoryView;

/** Clears all contents of the model. */
- (void)clear;

/** Sets the editing mode for tableView. */
- (void)setTableViewEditing:(BOOL)editing animated:(BOOL)animate;

/** 
 Sets the editing mode for tableView.
 
 @warning This method has been deprecated. Use setTableViewEditing:animated: instead.
 */
- (void)setModeledTableViewEditing:(BOOL)editing animated:(BOOL)animate;
 
//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Returns true if the modeled table view is live and displaying cells. */
@property (nonatomic, readonly) BOOL live;

/** Property is used internally by the framework to determine if the table view is in swipe-to-delete editing mode. */
@property (nonatomic, readonly) BOOL swipeToDeleteActive;

/** Property is used internally by the framework to set the master model in a master-detail relationship. */
@property (nonatomic, unsafe_unretained) SCTableViewModel *masterModel;

/** Holds the currently active detail model.
 @warning Property must only be set internally by the framework.
 */
@property (nonatomic, unsafe_unretained) SCTableViewModel *activeDetailModel;

/** Warning: Method must only be called internally by the framework. */
- (void)setActiveCell:(SCTableViewCell *)cell;

/** Warning: Method must only be called internally by the framework. */
- (void)setActiveCellControl:(UIResponder *)control;

/** Warning: Method must only be called internally by the framework. */
- (void)enterLoadingMode;

/** Warning: Method must only be called internally by the framework. */
- (void)exitLoadingMode;

/** Warning: Method must only be called internally by the framework. */
- (void)clearLastReturnedCellData;

/** Warning: Method must only be called internally by the framework. */
- (void)configureDetailModel:(SCTableViewModel *)detailModel;

/** Warning: Method must only be called internally by the framework. */
- (void)keyboardWillShow:(NSNotification *)aNotification;

/** Warning: Method must only be called internally by the framework. */
- (void)keyboardWillHide:(NSNotification *)aNotification;

/** 
 Method gets called internally whenever the value of a section changes. This method 
 should only be used when subclassing 'SCTableViewModel'. If what you want is to get notified
 when a section value changes, consider using SCTableViewModelDelegate methods.
 
 When subclassing 'SCTableViewModel', you can override this method to define custom behaviour when a 
 section value changes. However, you should always call "[super valueChangedForSectionAtIndex:]"
 somewhere in your subclassed method.
 
 @param index Index of the section changed.
 */
- (void)valueChangedForSectionAtIndex:(NSUInteger)index;

/** 
 Method gets called internally whenever the value of a cell changes. This method 
 should only be used when subclassing 'SCTableViewModel'. If what you want is to get notified
 when a cell value changes, consider using either SCTableViewModelDelegate or 
 the cell's actions.
 
 When subclassing 'SCTableViewModel', you can override this method to define custom behaviour when a 
 cell value changes. However, you should always call "[super valueChangedForRowAtIndexPath:]"
 somewhere in your subclassed method.
 
 @param indexPath Index path of the cell changed.
 */
- (void)valueChangedForRowAtIndexPath:(NSIndexPath *)indexPath;

/** Method used internally by the framework to monitor model modification events. */
- (void)setTargetForModelModifiedEvent:(id)_target action:(SEL)_action;

/** Subclasses should override this method to handle when editButtonItem is tapped. */
- (void)didTapEditButtonItem;

/** Method called by pullToRefreshView to initiate refresh. */
- (void)pullToRefreshViewDidStartLoading;

/** Method called internally. */
- (void)styleSections;

/** Method called internally. */
- (void)styleViews;

/** Method called internally. */
- (void)styleCell:(SCTableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath;

/** Method called internally. */
- (void)configureCell:(SCTableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath;

@end




@class SCArrayOfItemsModel;
/****************************************************************************************/
/*	protocol SCTableViewModelDataSource	*/
/****************************************************************************************/ 
/**
 This protocol should be adopted by objects that want to mediate as a data source for 
 SCTableViewModel. All methods for this protocol are optional.
 */
@protocol SCTableViewModelDataSource

@optional


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Section Management
//////////////////////////////////////////////////////////////////////////////////////////

/** Asks the dataSource to provide custom sorting for the model's sections. 
 *	@note If you just need to sort the sections alphabetically, just set your model's autoSortSections property to TRUE.
 *	@param tableModel The model requesting the sorted sections array.
 *	@param sectionsArray The array containing the sections to be sorted. All objects of the array are of type SCTableViewSection.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel sortSections:(NSMutableArray *)sectionsArray;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Custom cells
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Asks the dataSource to provide a custom cell for the specified indexPath. Implement
 this method to provide your own custom cells instead of the automatically generated cells
 by SCArrayOfItemsSection and its subclasses.
 @note Returning "nil" will have the section provide an automatically generated cell.
 @warning If more than one cell type is returned (including returning "nil"), then
 the tableViewModel:customReuseIdentifierForRowAtIndexPath: method must be implemented, returning
 a unique reuse identifier for each cell type used.
 @param tableModel The model requesting the custom cell.
 @param indexPath The index path of the row whose custom cell is requested.
 */
- (SCCustomCell *)tableViewModel:(SCTableViewModel *)tableModel
           cellForRowAtIndexPath:(NSIndexPath *)indexPath;

/** 
 Asks the dataSource to provide a custom reuse identifier for the specified indexPath. 
 Implement this method to provide your own custom reuse identifiers for cells generated
 by SCArrayOfItemsSection and its subclasses.
 @warning This method must be implemented if more than one type of cell is returned in
 tableViewModel:cellForRowAtIndexPath: (including returning "nil"), where it should
 return a unique reuse identifier for each cell type used. If only one cell type is returned
 in tableViewModel:cellForRowAtIndexPath:, then there is no need to implement this method.
 @param tableModel The model requesting the custom reuse identifier.
 @param indexPath The index path of the row whose custom reuse identifier is requested.
 */
- (NSString *)tableViewModel:(SCTableViewModel *)tableModel
        reuseIdentifierForRowAtIndexPath:(NSIndexPath *)indexPath;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Custom detail views
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Asks the dataSource to provide a custom detail table view model for the specified cell. This custom table view model will be used to render the cell's details instead of the default automatically generated detail model. The returned model is typically a blank model with no sections (all content will be generated by the requesting cell).
 @warning This method should only be implemented for cells that require a detail
 UITableView to display their contents. For cells that do not require a detail UITableView
 (e.g.: SCImagePickerCell), you should implement the tableViewModel:detailViewForRowAtIndexPath:
 method instead.
 @note This method is typically used to display the cell's details in the detail view of an
 Pad's UISplitViewController.
 
 @param tableModel The model requesting the custom detail table view model.
 @param indexPath The index path of the cell whose detail table view model is requested. 
 @return The custom detail table view model. This model should be autoreleased and is typically a blank model that the requesting cell will generate the content for.
 */
- (SCTableViewModel *)tableViewModel:(SCTableViewModel *)tableModel detailTableViewModelForRowAtIndexPath:(NSIndexPath *)indexPath;

/** 
 Asks the dataSource to provide a custom detail view controller for the specified cell. This custom detail view will be used to render the cell's details instead of the default automatically generated detail view controller.
 @warning This method should only be implemented for cells that do not require a detail
 UITableView to display their contents (e.g.: SCImagePickerCell). For all other cells that do require a detail 
 UITableView, you should implement the tableViewModel:customTableViewModelForRowAtIndexPath:
 method instead.
 @note This method is typically used to display the cell's details in the detail view of an
 iPad's UISplitViewController.
 
 @param tableModel The model requesting the custom detail view controller.
 @param indexPath The index path of the cell whose detail view controller is requested. 
 @return The custom detail view controller. This view controller is typically a blank view controller that the requesting cell will generate the content for.
 */
- (UIViewController *)tableViewModel:(SCTableViewModel *)tableModel detailViewControllerForRowAtIndexPath:(NSIndexPath *)indexPath;


- (BOOL)tableViewModel:(SCTableViewModel *)tableModel customPresentDetailViewController:(UIViewController *)detailViewController forRowAtIndexPath:(NSIndexPath *)indexPath;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Inserting, Deleting, or Moving Model Rows
//////////////////////////////////////////////////////////////////////////////////////////

/**	
 Asks the dataSource to handle to the insertion or deletion of the specified row in the 
 model. 

 @warning It is very rare when you'll need to define this method. If you are using
 an SCArrayOfItemsSection or any of its subclasses, the insertion and deletion of rows
 will be handeled for you automatically.
 
 @param tableModel The model requesting the insertion or deletion.
 @param editingStyle The cell editing style corresponding to a insertion or deletion requested 
 for the row specified by indexPath. Possible editing styles are UITableViewCellEditingStyleInsert 
 or UITableViewCellEditingStyleDelete.
 @param indexPath The index path locating the row in the model.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	commitEditingStyle:(UITableViewCellEditingStyle)editingStyle 
	 forRowAtIndexPath:(NSIndexPath *)indexPath;

/**	
 Asks the dataSource to handle to the movement of the specified row in the 
 model from a specified location to another. 
 
 @warning It is very rare when you'll need to define this method. If you are using
 an SCArrayOfItemsSection or any of its subclasses, the movement of rows
 will be handeled for you automatically.
 
 @param tableModel The model requesting the row movement.
 @param fromIndexPath The starting index path of the row to be moved.
 @param toIndexPath The destination index path of the move.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	moveRowAtIndexPath:(NSIndexPath *)fromIndexPath 
		   toIndexPath:(NSIndexPath *)toIndexPath;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name SCArrayOfItemsModel methods
//////////////////////////////////////////////////////////////////////////////////////////

/** Asks the dataSource to return the section header title for the given item index.
 *
 *	@param tableModel The model requesting the section header title.
 *	@param item The item whose setion header title is requested.
 *	@param index The index of item in SCArrayOfItemsModel.
 *	@return The method should return an autoreleased NSString header title.
 */
- (NSString *)tableViewModel:(SCArrayOfItemsModel *)tableModel
   sectionHeaderTitleForItem:(NSObject *)item AtIndex:(NSUInteger)index;

/** Asks the dataSource to return a list of section header titles.
 *
 *	@param tableModel The model requesting the section header titles.
 *	@return An array of NSString section titles.
 */
- (NSArray *)tableViewModelSectionHeaderTitles:(SCArrayOfItemsModel *)tableModel;

/** 
 Asks the dataSource to return a custom search result array given the search text and the
 results array automatically generated by the model. 
 
 @param tableModel The model requesting the custom search result.
 @param searchText The search text typed into the search bar.
 @param autoSearchResults The search results array automatically generated by the model. The type of
 objects in the results array is identical to the type of objects in the tableViewModel items array.
 @return The method should return an autoreleased NSArray results array. Important: The type of
 objects in the results array must be identical to the type of objects in the tableViewModel
 items array. Note: Return nil to ignore the custom search results and use autoSearchResults instead.
 */
- (NSArray *)tableViewModel:(SCArrayOfItemsModel *)tableModel
  customSearchResultForSearchText:(NSString *)searchText
		  autoSearchResults:(NSArray *)autoSearchResults;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name SCArrayOfItemsSection methods
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Asks the dataSource to handle the creation of a new item in an SCArrayOfItemsSection.
 
 When there is an attempt to create a new array item in an SCArrayOfItemsSection, the dataSource is asked to provide this new item. If the dataSource does not define this method, SCArrayOfItemsSection creates an item of the same class as the first item in the array (unless SCArrayOfObjectsSection is being used, in which case the provided class definition will be used to create the new object). If no items are in the array, and this method is not defined, no new objects can be created.
 
 @param tableModel The model requesting owning the array of objects section.
 @param index The index of the array of items section requesting the new item.
 @return The method should return an autoreleased new item that is a subclass of NSObject.
 */
- (NSObject *)tableViewModel:(SCTableViewModel *)tableModel
	newItemForArrayOfItemsSectionAtIndex:(NSUInteger)index;

@end





/****************************************************************************************/
/*	protocol SCTableViewModelDelegate	*/
/****************************************************************************************/ 
/**
 This protocol should be adopted by objects that want to mediate as a delegate for 
 SCTableViewModel. All methods for this protocol are optional.
 */
@protocol SCTableViewModelDelegate

@optional

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing TableView Model
//////////////////////////////////////////////////////////////////////////////////////////

/**
 Return FALSE to prevent the model from entering into editing mode.
 @note For this method to get fired, the SCTableViewModel editButtonItem property must be set.
 */
- (BOOL)tableViewModelShouldBeginEditing:(SCTableViewModel *)tableModel;

/** 
 Notifies the delegate that the table view will enter editing mode. 
 @note For this method to get fired, the SCTableViewModel editButtonItem property must be set.
 */
- (void)tableViewModelWillBeginEditing:(SCTableViewModel *)tableModel;

/** 
 Notifies the delegate that the table view did enter editing mode. 
 @note For this method to get fired, the SCTableViewModel editButtonItem property must be set.
 */
- (void)tableViewModelDidBeginEditing:(SCTableViewModel *)tableModel;

/**
 Return FALSE to prevent the model from exiting editing mode.
 @note For this method to get fired, the SCTableViewModel editButtonItem property must be set.
 */
- (BOOL)tableViewModelShouldEndEditing:(SCTableViewModel *)tableModel;

/** 
 Notifies the delegate that the table view will exit editing mode. 
 @note For this method to get fired, the SCTableViewModel editButtonItem property must be set. 
 */
- (void)tableViewModelWillEndEditing:(SCTableViewModel *)tableModel;

/** 
 Notifies the delegate that the table view did exit editing mode. 
 @note For this method to get fired, the SCTableViewModel editButtonItem property must be set.
 */
- (void)tableViewModelDidEndEditing:(SCTableViewModel *)tableModel;

/** 
 Notifies the delegate that the table view did scroll. 
 */
- (void)tableViewModelDidScroll:(SCTableViewModel *)tableModel;

/** 
 Notifies the delegate that the table view did end dragging. 
 */
- (void)tableViewModelDidEndDragging:(SCTableViewModel *)tableModel willDecelerate:(BOOL)decelerate;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Sections
//////////////////////////////////////////////////////////////////////////////////////////

/** Notifies the delegate that a section at the specified index has been added.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param section The added section.
 *	@param index The added section's index.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
	  didAddSectionAtIndex:(NSUInteger)index;

/** Notifies the delegate that a section at the specified index has been removed.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param index The index of the removed section.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
	  didRemoveSectionAtIndex:(NSUInteger)index;

/** Notifies the delegate that the value for the section at the specified index has changed.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param index The index of the section whose value has changed.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	valueChangedForSectionAtIndex:(NSUInteger)index;	

/** 
 Notifies the delegate that a new item bound cell has been created for the section at the specified index.
 @note Method usually called for SCArrayOfItemsSection's subclasses. 
 @warning Although item has been created, it's still not added to the section and can be deallocated
 if the user cancels the item's detail view.
 
 @param tableModel The model informing the delegate of the event.
 @param index The index of the section whose a new item has been created for.
 @param item The item that has been created.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
	itemCreatedForSectionAtIndex:(NSUInteger)index item:(NSObject *)item;

/** 
 Asks the delegate if a new item should be added to the section at the specified index.
 @note Method usually called for SCArrayOfItemsSection's subclasses.
 	
 @param tableModel The model informing the delegate of the event.
 @param index The index of the section the new item will be added to.
 @param item The item that will be added.
 */
- (BOOL)tableViewModel:(SCTableViewModel *)tableModel
    willAddItemForSectionAtIndex:(NSUInteger)index item:(NSObject *)item;

/** 
 Notifies the delegate that a new item bound cell has been added to a section at the specified indexPath.
 @note Method usually called for SCArrayOfItemsSection's subclasses.
 
 @param tableModel The model informing the delegate of the event.
 @param indexPath The index path the new item bound cell has been added to.
 @param item The item that has been added.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
	itemAddedForSectionAtIndexPath:(NSIndexPath *)indexPath item:(NSObject *)item;

/** 
 Notifies the delegate that an item bound cell has been edited for a section at the specified indexPath.
 @note Method usually called for SCArrayOfItemsSection's subclasses.
 
 @param tableModel The model informing the delegate of the event.
 @param indexPath The index path the new item bound cell has been added to.
 @param item The item that has been edited.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
	itemEditedForSectionAtIndexPath:(NSIndexPath *)indexPath item:(NSObject *)item;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Cells
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Notifies the delegate that the cell at the specified indexPath is about to be configured in its owner UITableView. 
 
 This is the perfect time to do any customization to the cell's height, editable, and movable properties.
 
 @param tableModel The model informing the delegate of the event.
 @param cell The cell that is about to layout controls.
 @param indexPath The index path of the cell.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	   willConfigureCell:(SCTableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath;

/** 
 Notifies the delegate that the cell at the specified indexPath did perform layout to its subviews. 
 
 This is the perfect place to do any customization to the cell's subviews' layouts/frames.
 	
 @param tableModel The model informing the delegate of the event.
 @param cell The cell that performed layout to its subviews.
 @param indexPath The index path of the cell.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	didLayoutSubviewsForCell:(SCTableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath;

/** 
 Notifies the delegate that the cell at the specified indexPath will be displayed. 
 
 This is the perfect place to do any customization to the cell's appearance.
 
 @note To change cell properties like the height, editable, or movable states, 
 use the willConfigureCell delegate method instead.
 	
 @param tableModel The model informing the delegate of the event.
 @param cell The cell that will be displayed.
 @param indexPath The index path of the cell that will be displayed.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	   willDisplayCell:(SCTableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath;

/** 
 Implement this method if you want to lazy-load the cell's contents. This method is usually 
 used when the cell's contents are expensive to retrieve inside willDisplayCell (gets retrieved via
 web services for example).
 
 @param tableModel The model informing the delegate of the event.
 @param cell The cell that will be displayed.
 @param indexPath The index path of the cell that will be lazy-loaded.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
          lazyLoadCell:(SCTableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that the cell at the specified indexPath will be selected.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell that will be selected.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	willSelectRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that the cell at the specified indexPath has been selected.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell that has been selected.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	didSelectRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that the cell at the specified indexPath has been deselected.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell that has been deselected.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	didDeselectRowAtIndexPath:(NSIndexPath *)indexPath;

/** 
 Notifies the delegate that the accessory button for the cell at the 
 specified indexPath has been selected.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell with the accessory button.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath;

/** 
 Notifies the delegate that a custom button for the cell at the 
 specified indexPath has been selected. This method gets called whenever any custom
 button placed on an SCControlCell is tapped. Note: button.tag must be greater than zero
 for this method to get called.
 	
 @param tableModel The model informing the delegate of the event.
 @param button The custom button tapped.
 @param indexPath The index path of the cell with the accessory button.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	customButtonTapped:(UIButton *)button forRowWithIndexPath:(NSIndexPath *)indexPath;

/** Requests the default title of the delete-confirmation button for the cell
 at the specified indexPath.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell requesting the delete-confirmation button title.
 *	@return A localized string to used as the title of the delete-confirmation button.
 */
- (NSString *)tableViewModel:(SCTableViewModel *)tableModel 
	titleForDeleteConfirmationButtonForRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that the value of the cell at the specified indexPath has changed.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell whose value has changed.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
    valueChangedForRowAtIndexPath:(NSIndexPath *)indexPath;

/**	
 Asks the delegate to return a new index path to retarget a proposed move of a row. 
 
 @warning It is very rare when you'll need to define this method. If you are using
 an SCArrayOfItemsSection or any of its subclasses, this will be automatically handeled for you.
 
 @param tableModel The model requesting this information.
 @param sourceIndexPath The index path identifying the original location of a row (in its section) that is being dragged.
 @param proposedDestinationIndexPath The index path identifying the currently proposed destination of the row being dragged.
 @return The index path locating the desired row destination for the move operation. Return proposedDestinationIndexPath if that location is suitable.
 */
- (NSIndexPath *)tableViewModel:(SCTableViewModel *)tableModel targetIndexPathForMoveFromRowAtIndexPath:(NSIndexPath *)sourceIndexPath toProposedIndexPath:(NSIndexPath *)proposedDestinationIndexPath;

/** Asks the delegate if editing should begin for the text view of the cell at the specified indexPath.
 *	
 *	@param tableModel The model asking the delegate.
 *	@param indexPath The index path of the cell containing the text field.
 *  @param textView The text view.
 */
- (BOOL)tableViewModel:(SCTableViewModel *)tableModel rowAtIndexPath:(NSIndexPath *)indexPath textViewShouldBeginEditing:(UITextView *)textView;

/** Asks the delegate if editing should begin for the text field of the cell at the specified indexPath.
 *	
 *	@param tableModel The model asking the delegate.
 *	@param indexPath The index path of the cell containing the text field.
 *  @param textField The text field.
 */
- (BOOL)tableViewModel:(SCTableViewModel *)tableModel rowAtIndexPath:(NSIndexPath *)indexPath textFieldShouldBeginEditing:(UITextField *)textField;

/** Asks the delegate if the specified text should be changed for the text field of the cell at the specified indexPath.
 *	
 *	@param tableModel The model asking the delegate for validation.
 *	@param indexPath The index path of the cell containing the text field.
 *  @param textField The text field containing the text.
 *  @param range The range of characters to be replaced.
 *  @param string The replacement string.
 */
- (BOOL)tableViewModel:(SCTableViewModel *)tableModel rowAtIndexPath:(NSIndexPath *)indexPath textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string;

/** Asks the delegate if the value is valid for the cell at the specified indexPath. Define this method if you want to override the cells' default value validation and provide your own custom validation.
 *	
 *	@param tableModel The model asking the delegate for validation.
 *	@param indexPath The index path of the cell whose value needs validation.
 */
- (BOOL)tableViewModel:(SCTableViewModel *)tableModel 
	valueIsValidForRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that the return keyboard button has been tapped for the cell at the specified indexPath. Define this method if you want to override the cells' default behaviour for tapping the return button.
 *	
 *	@param tableModel The model asking the delegate for validation.
 *	@param indexPath The index path of the cell whose keyboard return key has been tapped.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	returnButtonTappedForRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that cell at the specified indexPath has been newly inserted.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell who has been newly inserted.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	didInsertRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that cell at the specified indexPath will be removed.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell that will be removed.
 *	@return Return TRUE to proceed with the remove operation, otherwise return FALSE.
 */
- (BOOL)tableViewModel:(SCTableViewModel *)tableModel 
	willRemoveRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that cell at the specified indexPath has been removed.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell that has been removed.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
	didRemoveRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that the detail model for the cell at the specified index path has been created. This is the perfect time to do any customizations to the cell's detail table view model before any automatically generated sections are added, including setting its dataSource and delegate properties.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell whose detail model has been created.
 *	@param detailTableViewModel The model for the cell's detail view.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
	detailModelCreatedForRowAtIndexPath:(NSIndexPath *)indexPath
  detailTableViewModel:(SCTableViewModel *)detailTableViewModel;	

/** Notifies the delegate that the detail model for the cell at the specified index path has been configured with the appropriate sections and cells. This is the perfect time to do any customizations to the cell's detail table view model after the automatically generated sections have been added.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell whose detail model has been configured.
 *	@param detailTableViewModel The model for the cell's detail view.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
    detailModelConfiguredForRowAtIndexPath:(NSIndexPath *)indexPath
  detailTableViewModel:(SCTableViewModel *)detailTableViewModel;

/** Notifies the delegate that the detail view for the cell at the specified indexPath will be presented. This is the perfect time to do any customizations to the cell's detail view model after the automatically generated sections have been added.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell whose detail view will appear.
 *	@param detailTableViewModel The model for the cell's detail view.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
	detailViewWillPresentForRowAtIndexPath:(NSIndexPath *)indexPath
		withDetailTableViewModel:(SCTableViewModel *)detailTableViewModel;

/** Notifies the delegate that the detail view for the cell at the specified indexPath has been presented.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell whose detail view did appear.
 *	@param detailTableViewModel The model for the cell's detail view.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
    detailViewDidPresentForRowAtIndexPath:(NSIndexPath *)indexPath
    withDetailTableViewModel:(SCTableViewModel *)detailTableViewModel;

/** Asks the delegate if the detail view for the cell at the specified indexPath should be dismissed.
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index path of the cell.
 *	@param detailTableViewModel The model for the cell's detail view.
 *	@param cancelTapped TRUE if Cancel button has been tapped to dismiss the detail view.
 *	@param doneTapped TRUE if Done button has been tapped to dismiss the detail view.
 *  @return Retrun TRUE to have the detail view dismissed, otherwise return FALSE.
 */
- (BOOL)tableViewModel:(SCTableViewModel *)tableModel
    shouldDismissDetailViewForRowAtIndexPath:(NSIndexPath *)indexPath
    withDetailTableViewModel:(SCTableViewModel *)detailTableViewModel
            cancelButtonTapped:(BOOL)cancelTapped doneButtonTapped:(BOOL)doneTapped;

/** Notifies the delegate that the detail view for the cell at the specified indexPath will be dismissed.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index of the cell whose detail view will disappear.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
	detailViewWillDismissForRowAtIndexPath:(NSIndexPath *)indexPath;

/** Notifies the delegate that the detail view for the cell at the specified indexPath has been dismissed.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param indexPath The index of the cell whose detail view has disappeared.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel
	detailViewDidDismissForRowAtIndexPath:(NSIndexPath *)indexPath;

/** Asks the delegate for a new image name for the SCImagePickerCell at the specified indexPath. Define this method to provide a new name for the selected image, instead of using the auto generated one.
 *	
 *	@param tableModel The model asking the delegate for validation.
 *	@param indexPath The index path of the SCImagePickerCell who needs a new image name.
 *  @return The image name. Return value must be autoreleased.
 */
- (NSString *)tableViewModel:(SCTableViewModel *)tableModel 
	imageNameForRowAtIndexPath:(NSIndexPath *)indexPath;

/** Asks the delegate to handle saving the image for the SCImagePickerCell at the specified indexPath. Define this method to provide your own algorithm for saving the image instead of using the default one.
 *	
 *	@param tableModel The model asking the delegate for validation.
 *  @param image The image to be saved.
 *  @param path The path that the image should be saved to.
 *	@param indexPath The index path of the SCImagePickerCell that requests the image to be saved.
 *  @warning If you choose to save the image to a location other than that of the given path, then tableViewModel:loadImageFromImagePath:forRowAtIndexPath: method must also be implemented.
 */
- (void)tableViewModel:(SCTableViewModel *)tableModel 
             saveImage:(UIImage *)image toPath:(NSString *)path forRowAtIndexPath:(NSIndexPath *)indexPath;

/** Asks the delegate to handle loading the image for the SCImagePickerCell at the specified indexPath.Define this method to provide your own algorithm for loading the image instead of using the default one.
 *	
 *	@param tableModel The model asking the delegate for validation.
 *  @param path The path that the image should be loaded from.
 *	@param indexPath The index path of the SCImagePickerCell that requests the image to be loaded.
 *  @return The loaded image. This image should be autoreleased.
 */
- (UIImage *)tableViewModel:(SCTableViewModel *)tableModel 
               loadImageFromPath:(NSString *)path forRowAtIndexPath:(NSIndexPath *)indexPath;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name SCArrayOfItemsModel methods
//////////////////////////////////////////////////////////////////////////////////////////

/** Asks the delegate if editing should begin in the search bar.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@return Return YES if an editing session should be initiated, otherwise, NO.
 */
- (BOOL)tableViewModelSearchBarShouldBeginEditing:(SCArrayOfItemsModel *)tableModel;

/** Notifies the delegate that the search bar scope button selection changed.
 *	
 *	@param tableModel The model informing the delegate of the event.
 *	@param selectedScope The index of the selected scope button.
 */
- (void)tableViewModel:(SCArrayOfItemsModel *)tableModel
searchBarSelectedScopeButtonIndexDidChange:(NSInteger)selectedScope;

/** Notifies the delegate that the search bar bookmark button was tapped.
 *	
 *	@param tableModel The model informing the delegate of the event.
 */
- (void)tableViewModelSearchBarBookmarkButtonClicked:(SCArrayOfItemsModel *)tableModel;

/** Notifies the delegate that the search bar cancel button was tapped.
 *	
 *	@param tableModel The model informing the delegate of the event.
 */
- (void)tableViewModelSearchBarCancelButtonClicked:(SCArrayOfItemsModel *)tableModel;

/** Notifies the delegate that the search bar results list button was tapped.
 *	
 *	@param tableModel The model informing the delegate of the event.
 */
- (void)tableViewModelSearchBarResultsListButtonClicked:(SCArrayOfItemsModel *)tableModel;

/** Notifies the delegate that the search bar search button was tapped.
 *	
 *	@param tableModel The model informing the delegate of the event.
 */
- (void)tableViewModelSearchBarSearchButtonClicked:(SCArrayOfItemsModel *)tableModel;

@end






/****************************************************************************************/
/*	class SCArrayOfItemsModel	*/
/****************************************************************************************/ 
/**
 This class subclasses SCTableViewModel to represent an array of any kind of items and will automatically generate its cells from these items. 'SCArrayOfItemsModel will automatically generate a set of SCArrayOfItemsSection(s) if either the sectionHeaderTitleForItem modelAction or SCTableViewModelDataSource method tableViewModel:sectionHeaderTitleForItem:AtIndex: is implemented, otherwise it will only generate a single
 SCArrayOfItemsSection.
 
 @warning This is an abstract base class, you should never make any direct instances of it.
 
 @see SCArrayOfStringsModel, SCArrayOfObjectsModel, SCArrayOfStringsSection, SCArrayOfObjectsSection.
 */
@interface SCArrayOfItemsModel : SCTableViewModel <SCTableViewControllerDelegate, UISearchBarDelegate>
{
	SCArrayOfItemsSection *tempSection;		//internal
	NSArray *filteredArray;					//internal
	
	SCDataStore *dataStore;
    SCDataFetchOptions *dataFetchOptions;
    
    BOOL _loadingContents;
    BOOL sectionsInSync;
    NSMutableArray *items;
    BOOL itemsInSync;
    UITableViewCellAccessoryType itemsAccessoryType;
	BOOL allowAddingItems;
	BOOL allowDeletingItems;
	BOOL allowMovingItems;
	BOOL allowEditDetailView;
	BOOL allowRowSelection;
	BOOL autoSelectNewItemCell;

	SCDetailViewControllerOptions *detailViewControllerOptions;
    SCDetailViewControllerOptions *newItemDetailViewControllerOptions;
    
	UISearchBar *searchBar;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Returns an initialized 'SCArrayOfItemsModel given a UITableView and a data store.
 
 @param tableView The UITableView to be bound to the model.
 @param store The data store containing the model's items.
 */
- (id)initWithTableView:(UITableView *)tableView dataStore:(SCDataStore *)store;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The data store that's used to store and fetch the model's items. */
@property (nonatomic, strong) SCDataStore *dataStore;

/** The options used to fetch the model's items from dataStore. */
@property (nonatomic, strong) SCDataFetchOptions *dataFetchOptions;

/** The items fetched from dataStore. */
@property (nonatomic, readonly) NSArray *items;

/** The accessory type of the generated cells. */
@property (nonatomic, readwrite) UITableViewCellAccessoryType itemsAccessoryType;

/** Allows/disables adding new cells/items to the items array. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowAddingItems;

/** Allows/disables deleting new cells/items from the items array. Default: TRUE. */
@property (nonatomic, readwrite) BOOL allowDeletingItems;

/** Allows/disables moving cells/items from one row to another. Default: FALSE. */
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

/** Allows/disables automatic cell selection of newly created items. Default: TRUE. */
@property (nonatomic, readwrite) BOOL autoSelectNewItemCell;

/**	
 Set this property to a valid UIBarButtonItem. When addButtonItem is tapped and allowAddingItems
 is TRUE, a detail view is automatically generated for the user to enter the new items
 properties. If the properties are commited, a new item is added to the array.
 */
@property (nonatomic, strong) UIBarButtonItem *addButtonItem;

/** 
 The search bar associated with the model. Once set to a valid UISearchBar, the model will
 automatically filter its items based on the user's typed search term. 
 */
@property (nonatomic, strong) UISearchBar *searchBar;

/** Options for the generated detail view controller. */
@property (nonatomic, readonly) SCDetailViewControllerOptions *detailViewControllerOptions;

/** Options for the generated detail view controller for new items. */
@property (nonatomic, readonly) SCDetailViewControllerOptions *newItemDetailViewControllerOptions;


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

/** Subclasses should override this method to handle section creation. */
- (SCArrayOfItemsSection *)createSectionWithHeaderTitle:(NSString *)title;

/** Subclasses should override this method to set additional section properties after creation. */
- (void)setPropertiesForSection:(SCArrayOfItemsSection *)section;

/** Subclasses should override this method to handle when addButtonItem is tapped. */
- (void)didTapAddButtonItem;

/** Method called internally by framework when the model should add a new item. */
- (void)addNewItem:(NSObject *)newItem;

/** Method called internally by framework when a model item has been modified. */
- (void)itemModified:(NSObject *)item inSection:(SCArrayOfItemsSection *)section;

/** Method called internally by framework when a model item has been removed. */
- (void)itemRemoved:(NSObject *)item inSection:(SCArrayOfItemsSection *)section;

/** Method called internally by framework when the model's items are out of sync with the data store. */
- (void)invalidateItems;

/** Method called internally by framework. */
- (NSUInteger)getSectionIndexForItem:(NSObject *)item;

/** Method called internally. */
- (void)setDetailViewControllerOptions:(SCDetailViewControllerOptions *)options;

/** Method called internally. */
- (void)setNewItemDetailViewControllerOptions:(SCDetailViewControllerOptions *)options;

@end






/****************************************************************************************/
/*	class SCArrayOfObjectsModel	*/
/****************************************************************************************/ 
/**
 This class functions as a table view model that is able to represent an array of any kind of objects and automatically generate its cells from these objects. In addition, 'SCArrayOfObjectsModel' generates its detail views from the properties of the corresponding object in its items array. Objects in the items array need not all be of the same object type, but they must all decend from NSObject. 
 
 'SCArrayOfItemsModel' will automatically generate a set of SCArrayOfObjectsSection(s) if either the sectionHeaderTitleForItem modelAction or SCTableViewModelDataSource method tableViewModel:sectionHeaderTitleForItem:AtIndex: is implemented, otherwise it will only generate a single SCArrayOfObjectsSection.
 */
@interface SCArrayOfObjectsModel : SCArrayOfItemsModel
{
	NSString *searchPropertyName;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 Allocates and returns an initialized 'SCArrayOfObjectsModel' given a UITableView and an array of objects.
 
 @param tableView The UITableView to be bound to the model.
 @param items An array of objects that the model will use to generate its cells.
 This array must be of type NSMutableArray, as it must support the model's add, delete, and
 move operations. 
 @param definition The definition of the objects in the objects array.
 */
+ (id)modelWithTableView:(UITableView *)tableView items:(NSMutableArray *)items itemsDefinition:(SCDataDefinition *)definition;
   

/** 
 Returns an initialized 'SCArrayOfObjectsModel' given a UITableView and an array of objects.
 
 @param tableView The UITableView to be bound to the model.
 @param items An array of objects that the model will use to generate its cells.
 This array must be of type NSMutableArray, as it must support the model's add, delete, and
 move operations. 
 @param definition The definition of the objects in the objects array.
 */
- (id)initWithTableView:(UITableView *)tableView items:(NSMutableArray *)items itemsDefinition:(SCDataDefinition *)definition;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** 
 The name of the object's property that the value of which will be used to search the items array 
 when the user types a search term inside the model's associated search bar. To search more than one property 
 value, separate the property names by a semi-colon (e.g.: @"firstName;lastName"). To search all 
 properties in the object's class definition, set the property to an astrisk (e.g.: @"*").
 If the property is not set, it defaults to the value of the object's class definition titlePropertyName property. 
 */
@property (nonatomic, copy) NSString *searchPropertyName;

@end







/****************************************************************************************/
/*	class SCArrayOfStringsModel	*/
/****************************************************************************************/ 
/**
 This class functions as a table view model that is able to represent an array
 of string items and automatically generate its cells from these items. The class inherits
 all its funtionality from its superclass: SCArrayOfItemsModel, except that its items
 array can only contain items of type NSString. 'SCArrayOfStringsModel 
 will automatically generate a set of SCArrayOfStringsSection(s) if either the sectionHeaderTitleForItem modelAction or SCTableViewModelDataSource method tableViewModel:sectionHeaderTitleForItem:AtIndex: is implemented, otherwise it will only generate a single
 SCArrayOfStringsSection.
 */

@interface SCArrayOfStringsModel : SCArrayOfObjectsModel

/** 
 Allocates and returns an initialized 'SCArrayOfStringsModel' given a UITableView and an array of NSString objects.
 
 @param tableView The UITableView to be bound to the model.
 @param items An array of NSStrings that the model will use to generate its cells.
 This array must be of type NSMutableArray, as it must support the model's add, delete, and
 move operations. 
 */
+ (id)modelWithTableView:(UITableView *)tableView items:(NSMutableArray *)items;

/** 
 Returns an initialized 'SCArrayOfStringsModel' given a UITableView and an array of NSString objects.
 
 @param tableView The UITableView to be bound to the model.
 @param items An array of NSStrings that the model will use to generate its cells.
 This array must be of type NSMutableArray, as it must support the model's add, delete, and
 move operations. 
 */
- (id)initWithTableView:(UITableView *)tableView items:(NSMutableArray *)items;

@end









/****************************************************************************************/
/*	class SCSelectionModel	*/
/****************************************************************************************/ 
/**
 This class functions as a model that is able to provide selection functionality. 
 The cells in this model represent different items that the end-user can select from, and they
 are generated from NSStrings in its items array. Once a cell is selected, a checkmark appears next
 to it, similar to Apple's Settings application where a user selects a Ringtone for their
 iPhone. The section can be configured to allow multiple selection and to allow no selection at all.
 
 Since this model is based on SCArrayOfStringsModel, it supports automatically generated sections and automatic search functionality.
 
 There are three ways to set/retrieve the section's selection:
 - Through binding an object to the model, and specifying a property name to bind the selection index
 result to. The bound property must be of type NSMutableSet if multiple selection is allowed, otherwise
 it must be of type NSNumber or NSString.
 - Through the selectedItemsIndexes or selectedItemIndex properties.
 
 @see SCSelectionSection.
 */ 
@interface SCSelectionModel : SCArrayOfStringsModel
{	
    //internal
	BOOL boundToNSNumber;	
	BOOL boundToNSString;	
	NSIndexPath *lastSelectedRowIndexPath; 
	
    NSObject *boundObject;
    SCDataStore *boundObjectStore;
	NSString *boundPropertyName;
    
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
 Returns an initialized 'SCSelectionModel' given a table view, a bound object,
 an NSNumber bound property name, and an array of selection items.
 
 @param tableView The UITableView to be bound to the model. 
 @param object The object the model will bind to.
 @param propertyName The property name present in the bound object that the section will bind to and
 will automatically change the value of to reflect the model's current selection. This property must
 be of type NSNumber and can't be a readonly property. The model will also initialize its selection 
 from the value present in this property.
 @param sectionItems An array of the items that the user will choose from. All items must be of
 an NSString type.
 */
- (id)initWithTableView:(UITableView *)tableView
            boundObject:(NSObject *)object 
    selectedIndexPropertyName:(NSString *)propertyName 
                  items:(NSArray *)sectionItems;

/** 
 Returns an initialized 'SCSelectionModel' given a table view, a bound object,
 a bound property name, an array of selection items, and whether to allow multiple selection.
 
 @param tableView The UITableView to be bound to the model.
 @param object The object the model will bind to.
 @param propertyName The property name present in the bound object that the model will bind to and
 will automatically change the value of to reflect the model's current selection(s). This property must
 be of type NSMutableSet. The model will also initialize its selection(s) from the value present
 in this property. Every item in this set must be an NSNumber that represent the index of the selected cell(s).
 @param sectionItems An array of the items that the user will choose from. All items must be of
 an NSString type.
 @param multipleSelection Determines if multiple selection is allowed.
 */
- (id)initWithTableView:(UITableView *)tableView
            boundObject:(NSObject *)object 
    selectedIndexesPropertyName:(NSString *)propertyName 
                  items:(NSArray *)sectionItems 
 allowMultipleSelection:(BOOL)multipleSelection;

/** 
 Returns an initialized 'SCSelectionModel' given a table view, a bound object,
 an NSString bound property name, and an array of selection items.
 
 @param tableView The UITableView to be bound to the model.
 @param object The object the model will bind to.
 @param propertyName The property name present in the bound object that the model will bind to and
 will automatically change the value of to reflect the model's current selection. This property must
 be of type NSString and can't be a readonly property. The model will also initialize its selection 
 from the value present in this property.
 @param sectionItems An array of the items that the user will choose from. All items must be of
 an NSString type.
 */
- (id)initWithTableView:(UITableView *)tableView
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
 @note: Only applicable when allowMultipleSelection is TRUE. */
@property (nonatomic, readwrite) NSUInteger maximumSelections;

/** If TRUE, the section automatically dismisses the current view controller when a value is selected. Default: FALSE. */
@property (nonatomic, readwrite) BOOL autoDismissViewController;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Provides subclasses with the framework to bind an SCTableViewSection to an NSObject */
@property (nonatomic, readonly) NSObject *boundObject;

/** The data store of the cell's bound object. */
@property (nonatomic, strong) SCDataStore *boundObjectStore;

/** Provides subclasses with the framework to bind an SCTableViewSection to an NSObject */
@property (nonatomic, readonly) NSString *boundPropertyName;

/** Provides subclasses with the framework to bind an SCTableViewSection to a value */
@property (nonatomic, strong) NSObject *boundValue;


@end




