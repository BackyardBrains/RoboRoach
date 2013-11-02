/*
 *  SCDataFetchOptions.h
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


#import "SCGlobals.h"



/****************************************************************************************/
/*	class SCDataFetchOptions	*/
/****************************************************************************************/ 
/**	
 This class is used to control how data is fetched from SCDataStore. SCDataFetchOptions can be configured to have the data sorted, filtered, and/or returned in batches.
 
 Sample use:
    SCArrayOfObjectsModel *myObjectsModel = ...;
    myObjectsModel.dataFetchOptions.sort = TRUE;
 
 See also: SCDataStore, SCCoreDataFetchOptions, SCWebServiceFetchOptions
*/


@interface SCDataFetchOptions : NSObject
{
    BOOL _sort;
    NSString *_sortKey;
    BOOL _sortAscending;
    BOOL _filter;
    NSPredicate *_filterPredicate;
    NSUInteger _batchSize;
    NSUInteger _batchStartingOffset;
    NSUInteger _batchCurrentOffset;
}


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCDataFetchOptions object. */
+ (id)options;

/** Allocates and returns an initialized SCDataFetchOptions object given the sorting and filtering configuration.
 
 @param key The key that the data will be sorted against. Passing a non nil value for 'key' also sets the sort property to TRUE.
 @param predicate The NSPredicate that the data will be filtered against. Passing a non nil value for 'predicate' also sets the filter property to TRUE.
 */
+ (id)optionsWithSortKey:(NSString *)key sortAscending:(BOOL)ascending filterPredicate:(NSPredicate *)predicate;

/** Returns an initialized SCDataFetchOptions object given the sorting and filtering configuration.
 
 @param key The key that the data will be sorted against. Passing a non nil value for 'key' also sets the sort property to TRUE.
 @param predicate The NSPredicate that the data will be filtered against. Passing a non nil value for 'predicate' also sets the filter property to TRUE.
 */
- (id)initWithSortKey:(NSString *)key sortAscending:(BOOL)ascending filterPredicate:(NSPredicate *)predicate;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** Set to TRUE to enable data sorting, otherwise set to FALSE. Default: FALSE. 
 @note Only applicable when sortKey is set to a valid key. */
@property (nonatomic, readwrite) BOOL sort;

/** The key that the data will be sorted against. Default: nil. */
@property (nonatomic, copy) NSString *sortKey;

/** Set to TRUE to have the sorting done in ascending order, otherwise set to FALSE. Default: TRUE.
 @note Only applicable when sortKey is set to a valid key. */
@property (nonatomic, readwrite) BOOL sortAscending;

/** Set to TRUE to enable data filtering, otherwise set to FALSE. Default: TRUE. 
 @note Only applicable when filterPredicate is set to a valid NSPredicate. */
@property (nonatomic, readwrite) BOOL filter;

/** The NSPredicate that the data will be filtered against. Default: nil. */
@property (nonatomic, strong) NSPredicate *filterPredicate;

/** Set to the data batch size that should be retrieved. Setting this property to zero retrieves all avialable data. Default: 0. */
@property (nonatomic, readwrite) NSUInteger batchSize;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** The offset of the batch that should be first retrieved. Default: 0. */
@property (nonatomic, readwrite) NSUInteger batchStartingOffset;

/** The current offset of the last retrieved batch. */
@property (nonatomic, readonly) NSUInteger batchCurrentOffset;

/** Sets the current batch offset. 
 @warning Reserved for internal framework use only. */
- (void)setBatchOffset:(NSUInteger)offset;

/** Increments the current batch offset. 
 @warning Reserved for internal framework use only. */
- (void)incrementBatchOffset;

/** Resets the current batch offset. 
 @warning Reserved for internal framework use only. */
- (void)resetBatchOffset;

/** Returns an array of sort-descriptors based on the current sorting configuration. */
- (NSArray *)sortDescriptors;

/** Sorts the given array based on the current sorting configuration. */
- (void)sortMutableArray:(NSMutableArray *)array;

/** Filters the given array based on the current filtering configuration. */
- (void)filterMutableArray:(NSMutableArray *)array;



@end






