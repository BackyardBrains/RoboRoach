/*
 *  SCUserDefaultsStore.h
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
/*	class SCUserDefaultsStore	*/
/****************************************************************************************/ 
/**	
 SCUserDefaultsStore is an SCDataStore subclass that encapsulates the NSUserDefaults storage, providing means for the SC framework to communicate with this storage to fetch, add, update and remove user defaults.
 
 @note It is very rare when you'll need to create an SCUserDefaultsStore instance yourself, as it's typically automatically created for you when you use SCUserDefaultsDefinition. For example, when you use the SCTableViewModel method called [SCTableViewModel generateSectionsForUserDefaultsDefinition:], the model automatically sets its sections' dataStore property by calling your userDefaultsDefinition's [SCDataDefinition generateCompatibleDataStore:] method.
 
 @note For more information on data stores, check out the SCDataStore base class documentation.
 */
@interface SCUserDefaultsStore : SCDataStore

/** The user defaults object managed by the data store. */
@property (nonatomic, readonly) NSUserDefaults *standardUserDefaultsObject;

@end
