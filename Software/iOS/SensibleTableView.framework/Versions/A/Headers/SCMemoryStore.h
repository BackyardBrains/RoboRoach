/*
 *  SCMemoryStore.h
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

#import "SCDataStore.h"


/****************************************************************************************/
/*	class SCMemoryStore	*/
/****************************************************************************************/ 
/**	
 SCMemoryStore is an SCDataStore subclass that encapsulates heap memory storage, providing means for the SC framework to communicate with this storage to fetch, add, update and remove data objects.

 @note It is very rare when you'll need to create an SCMemoryStore instance yourself, as it's typically automatically created for you when you use data definitions such as SCClassDefinition and SCDictionaryDefinition. For example, when you use the SCArrayOfObjectsSection initializer method called [SCArrayOfObjectsSection sectionWithHeaderTitle:items:itemsDefinition:], SCArrayOfObjectsSection automatically sets its dataStore property by calling your itemsDefinition's [SCDataDefinition generateCompatibleDataStore:] method.
 
 @note For more information on data stores, check out the SCDataStore base class documentation.
*/
@interface SCMemoryStore : SCDataStore


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCMemoryStore given an array of objects and their default data definition. */
+ (id)storeWithObjectsArray:(NSMutableArray *)array defaultDefiniton:(SCDataDefinition *)definition;

/** Returns an initialized SCMemoryStore given an array of objects and their default data definition. */
- (id)initWithObjectsArray:(NSMutableArray *)array defaultDefiniton:(SCDataDefinition *)definition;

/** The objects array storage managed by the memory store. */
@property (nonatomic, strong) NSMutableArray *objectsArray;

@end
