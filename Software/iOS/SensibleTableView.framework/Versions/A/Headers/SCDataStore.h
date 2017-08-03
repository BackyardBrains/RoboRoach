/*
 *  SCDataStore.h
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


#import "SCGlobals.h"
#import "SCDataDefinition.h"
#import "SCDataFetchOptions.h"



/* Data store notifications (used internally) */
extern NSString * const SCDataStoreWillDiscardAllUninsertedObjectsNotification;


typedef enum { SCStoreModeSynchronous, SCStoreModeAsynchronous } SCStoreMode;
typedef void(^SCDataStoreFetchSuccess_Block)(NSArray *results);
typedef void(^SCDataStoreInsertSuccess_Block)();
typedef void(^SCDataStoreUpdateSuccess_Block)();
typedef void(^SCDataStoreDeleteSuccess_Block)();
typedef void(^SCDataStoreFailure_Block)(NSError *error);


/****************************************************************************************/
/*	class SCDataStore	*/
/****************************************************************************************/ 
/**	
 SCDataStore is an abstract base class that can encapsulate any kind of data storage, providing means for the SC framework to communicate with this storage to fetch, add, update and remove data.
 
 Several SCDataStore subclasses have been implemented to enable the SC framework to communicate with Core Data, web services, iCloud storage, and even NSUserDefaults storage. Feel free to subclass SCDataStore to enable any kind of data storage not currently implemented by framework.
 
 Sample use:
    // Display all the TaskEntity objects
    SCCoreDataStore *coreDataStore = [SCCoreDataStore storeWithManagedObjectContext:context defaultEntityDefinition:TaskEntityDef];
    SCArrayOfObjectsSection *tasksSection = [SCArrayOfObjectsSection sectionWithHeaderTitle:@"Task Objects" dataStore:coreDataStore];
    [self.tableViewModel addSection:tasksSection];
 
 See also: SCMemoryStore, SCCoreDataStore, SCWebServiceStore, SCiCloudStore, SCUserDefaultsStore
 
 @note At initialization time, SCDataStore must be assigned at least one default SCDataDefinition that fully defines its stored data objects.
 
 @warning SCDataStore is an abstract base class and should not be directly instantiated.
 */
@interface SCDataStore : NSObject
{
    SCStoreMode _storeMode;
    
    NSObject *_storedData;
    SCDataDefinition *_defaultDataDefinition;
    NSMutableDictionary *_dataDefinitions;
    
    // Internal (must be managed by subclasses)
    NSMutableArray *_uninsertedObjects;
    NSObject *_boundObject;
    NSString *_boundPropertyName;
    SCDataDefinition *_boundObjectDefinition;
    
    NSDictionary *_defaultsDictionary;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocated and returns an initialized SCDataStore given a default SCDataDefinition of the data stored. */
+ (id)storeWithDefaultDataDefinition:(SCDataDefinition *)definition;

/** Returns an initialized SCDataStore given a default SCDataDefinition of the data stored. */
- (id)initWithDefaultDataDefinition:(SCDataDefinition *)definition;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** Specifies the mode that the SC framework should use to communicate with the store. Set to SCStoreModeSynchronous to have the framework communicate synchronously, otherwise set to SCStoreModeAsynchronous for asynchronous communication. */
@property (nonatomic, readwrite) SCStoreMode storeMode;

/** The data store's default data definition. To add more data definitions (other than the default one), use addDataDefinition:. */
@property (nonatomic, strong) SCDataDefinition *defaultDataDefinition;

/** Adds a definition to dataDefinitions. */
- (void)addDataDefinition:(SCDataDefinition *)definition;

/** Returns the data definition for the given object. */
- (SCDataDefinition *)definitionForObject:(NSObject *)object;

 
//////////////////////////////////////////////////////////////////////////////////////////
/// @name Synchronous Data Access
//////////////////////////////////////////////////////////////////////////////////////////

/** Returns a newly allocated and initialized object based on the store's default data definition.
 @return The newly created object.
 @warning Any newly created objects should be later either added to the data store using insertObject:, or discarded using discardUninsertedObject:.
 */
- (NSObject *)createNewObject;

/** Returns a newly allocated and initialized object based on the given data definition. 
 @param definition The data definition that describes the object to be created.
 @return The newly created object.
 @warning Any newly created objects should be later either added to the data store using insertObject:, or discarded using discardUninsertedObject:.
 */
- (NSObject *)createNewObjectWithDefinition:(SCDataDefinition *)definition;

/** Discards the given uninserted object.
 
 Any object created with createNewObject: or createNewObjectWithDefinition: and not later inserted to the store using insertObject: must be discarded using this method.
 
 @param object The uninserted newly created object to be discarded.
 @return Returns TRUE if successful.
 */
- (BOOL)discardUninsertedObject:(NSObject *)object;

/** Inserts the given object into the data store.
 @param object The object to be inserted.
 @return Returns TRUE if successful.
 */
- (BOOL)insertObject:(NSObject *)object;

/** Inserts the given object into the data store at the specified order.
 @param object The object to be inserted.
 @param order The order index that the object should be inserted at.
 @return Returns TRUE if successful.
 @note Only applicable for data stores that maintain ordered object storage.
 */
- (BOOL)insertObject:(NSObject *)object atOrder:(NSUInteger)order;

/** Changes the order of the given object to the specified order.
 @param object The object that the order of will change.
 @param toOrder The new order index of the object.
 @return Returns TRUE if successful.
 @note Only applicable for data stores that maintain ordered object storage.
 */
- (BOOL)changeOrderForObject:(NSObject *)object toOrder:(NSUInteger)toOrder;

/** Updates the given object in the data store. 
 @param object The object to be updated.
 @return Returns TRUE if successful.*/
- (BOOL)updateObject:(NSObject *)object;

/** Deletes the given object from the data store. 
 @param object The object to be deleted.
 @return Returns TRUE if successful.*/
- (BOOL)deleteObject:(NSObject *)object;

/** Fetches objects from the data store that satisfy the given fetch options.
 @param fetchOptions The fetch options that the data store must satisfy when returning the objects.
 @return An array of the fetched objects.
 @see SCDataFetchOptions
 */
- (NSArray *)fetchObjectsWithOptions:(SCDataFetchOptions *)fetchOptions;

/** Returns the value for the given property name in the given object. 
 @param propertyName The name of the property.
 @param object The object containing propertyName.
 @return The value for the given propertyName.
 */
- (NSObject *)valueForPropertyName:(NSString *)propertyName inObject:(NSObject *)object;

/** Returns the string value for the given property name in the given object. 
 @param propertyName The name of the property.
 @param object The object containing propertyName.
 @param delimiter If the property returns more than one value, this delimiter is used to separate the values in the returned string.
 @return The value for the given propertyName.
 */
- (NSString *)stringValueForPropertyName:(NSString *)propertyName inObject:(NSObject *)object
			separateValuesUsingDelimiter:(NSString *)delimiter;

/** Sets the value for the given property name in the given object.
 @param value The value of the property.
 @param propertyName The name of the property.
 @param object The object containing propertyName.
 */
- (void)setValue:(NSObject *)value forPropertyName:(NSString *)propertyName inObject:(NSObject *)object;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Asynchronous Data Access
//////////////////////////////////////////////////////////////////////////////////////////

/** Asynchronously inserts the given object into the data store.
 @param object The object to be inserted.
 @param success_block The code block called after the object has been successfully inserted.
 
 SCDataStoreInsertSuccess_Block syntax:
    ^()
    {
        // Your code here
    }

 @param failure_block The code block called in case the object could not be inserted.
 
 SCDataStoreFailure_Block syntax:
    ^(NSError *error)
    {
        // Your code here
    }
 */
- (void)asynchronousInsertObject:(NSObject *)object success:(SCDataStoreInsertSuccess_Block)success_block failure:(SCDataStoreFailure_Block)failure_block;

/** Asynchronously updates the given object into the data store.
 @param object The object to be updated.
 @param success_block The code block called after the object has been successfully updated.
 
 SCDataStoreUpdateSuccess_Block syntax:
    ^()
    {
        // Your code here
    }
 
 @param failure_block The code block called in case the object could not be updated.
 
 SCDataStoreFailure_Block syntax:
    ^(NSError *error)
    {
        // Your code here
    }
 */
- (void)asynchronousUpdateObject:(NSObject *)object success:(SCDataStoreUpdateSuccess_Block)success_block failure:(SCDataStoreFailure_Block)failure_block;

/** Asynchronously deletes the given object from the data store.
 @param object The object to be deleted.
 @param success_block The code block called after the object has been successfully deleted.
 
 SCDataStoreDeleteSuccess_Block syntax:
    ^()
    {
        // Your code here
    }
 
 @param failure_block The code block called in case the object could not be deleted.
 
 SCDataStoreFailure_Block syntax:
    ^(NSError *error)
    {
        // Your code here
    }
 */
- (void)asynchronousDeleteObject:(NSObject *)object success:(SCDataStoreDeleteSuccess_Block)success_block failure:(SCDataStoreFailure_Block)failure_block;

/** Asynchronously fetches objects from the data store that satisfy the given fetch options.
 @param fetchOptions The fetch options that the data store must satisfy when returning the objects.
 @param success_block The code block called after the data has been successfully fetched.
 
 SCDataStoreFetchSuccess_Block syntax:
    ^(NSArray *results)
    {
        // Your code here
    }
 Where 'results' is the fetched data array.
 @param failure_block The code block called in case the data could not be fetched.
 
 SCDataStoreFailure_Block syntax:
    ^(NSError *error)
    {
        // Your code here
    }
 
 @see SCDataFetchOptions
 */
- (void)asynchronousFetchObjectsWithOptions:(SCDataFetchOptions *)fetchOptions success:(SCDataStoreFetchSuccess_Block)success_block failure:(SCDataStoreFailure_Block)failure_block;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Data Validation
//////////////////////////////////////////////////////////////////////////////////////////

/** Returns TRUE if the given object can be inserted into the data store, otherwise returns FALSE. */
- (BOOL)validateInsertForObject:(NSObject *)object;

/** Returns TRUE if the given object can be updated into the data store, otherwise returns FALSE. */
- (BOOL)validateUpdateForObject:(NSObject *)object;

/** Returns TRUE if the given object can be deleted from the data store, otherwise returns FALSE. */
- (BOOL)validateDeleteForObject:(NSObject *)object;

/** Returns TRUE if the order of the given object can be changed in the data store, otherwise returns FALSE. */
- (BOOL)validateOrderChangeForObject:(NSObject *)object;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Data Management
//////////////////////////////////////////////////////////////////////////////////////////

/** Commits the data store objects to the persistent store. This method is only applicable to data stores that store their objects in memory before persisting them to a permenant persistant storage. */
- (void)commitData;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Should be used by subclasses to store their data when heap memory storage is needed. */
@property (nonatomic, strong) NSObject *storedData;

/** Should be used by subclasses to store any default values needed. */
@property (nonatomic, strong) NSDictionary *defaultsDictionary;

/** Binds the store to a property name that will be used to fetch all the store's data.
 
 Should only be used by the framework all must be implemented by all subclasses. */
- (void)bindStoreToPropertyName:(NSString *)propertyName forObject:(NSObject *)object withDefinition:(SCDataDefinition *)definition;

/** This method is typically called internally by the framework when all unadded objects must be discarded. The method will issue the 'SCDataStoreWillDiscardAllUnaddedObjectsNotification' notification to inform all classes using the store that this will happen. */
- (void)forceDiscardAllUnaddedObjects;

/** Method called when the application is about to leave the background state. Subclasses should override this method when any initialization is needed at this point. */
- (void)applicationWillEnterForeground;

@end


