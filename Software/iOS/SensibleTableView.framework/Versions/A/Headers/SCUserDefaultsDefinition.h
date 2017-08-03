/*
 *  SCUserDefaultsDefinition.h
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


#import "SCDictionaryDefinition.h"


/****************************************************************************************/
/*	class SCUserDefaultsDefinition	*/
/****************************************************************************************/ 
/**	
 This class functions as a means to define NSUserDefaults fields that the framework can automatically generate a user interface for. As with all other types of binding, once the UI is generated, the framework will also be responsible for automatically reading and writing the values to NSUserDefaults.
 
 Sample use:
    // Create the user defaults definition
    SCUserDefaultsDefinition *userDefaultsDef = [SCUserDefaultsDefinition definitionWithUserDefaultsKeyNamesString:
        @"Login Details:(username,password):Will be automatically signed in"];
    SCPropertyDefinition *passwordDef = [userDefaultsDef propertyDefinitionWithName:@"password"];
    passwordDef.attributes = [SCTextFieldAttributes attributesWithPlaceholder:nil secureTextEntry:YES
        autocorrectionType:UITextAutocorrectionTypeNo autocapitalizationType:UITextAutocapitalizationTypeNone];
 
    // Generate the UI for the user defaults
    [self.tableViewModel generateSectionsForUserDefaultsDefinition:userDefaultsDef];
 
 @see SCPropertyDefinition.
 */

@interface SCUserDefaultsDefinition : SCDictionaryDefinition

/** Allocates and returns an initialized 'SCUserDefaultsDefinition' given the key names string of the user defaults. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNamesString A string with the key names separated by semi-colons. Example string: @"firstName;lastName". Property groups can also be defined in the string using the following format: @"Personal Details:(firstName, lastName); Address:(street, state, country)". The group title can also be ommitted to create a group with no title. For example: @":(firstName, lastName)".
 
 Key names string syntax options:
    @"key1;key2;key3;..."
    @"group1 header:(key1, key2,...):group1 footer;group2..."
 */
+ (id)definitionWithUserDefaultsKeyNamesString:(NSString *)keyNamesString;

/** Allocates and returns an initialized 'SCUserDefaultsDefinition' given the key names of the user defaults. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNames An array of the user defaults key names. All array elements must be of type NSString.
 */
+ (id)definitionWithUserDefaultsKeyNames:(NSArray *)keyNames;

/** Allocates and returns an initialized 'SCUserDefaultsDefinition' given the key names of the user defaults and their titles. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNames An array of the user defaults key names. All array elements must be of type NSString.
 @param keyTitles An array of titles to the keys in keyNames. All array elements must be of type NSString.
 */
+ (id)definitionWithUserDefaultsKeyNames:(NSArray *)keyNames keyTitles:(NSArray *)keyTitles;


/** Returns an initialized 'SCUserDefaultsDefinition' given the key names string of the user defaults. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNamesString A string with the key names separated by semi-colons. Example string: @"firstName;lastName". Property groups can also be defined in the string using the following format: @"Personal Details:(firstName, lastName); Address:(street, state, country)". The group title can also be ommitted to create a group with no title. For example: @":(firstName, lastName)".
 
 Key names string syntax options:
    @"key1;key2;key3;..."
    @"group1 header:(key1, key2,...):group1 footer;group2..."
 */
- (id)initWithUserDefaultsKeyNamesString:(NSString *)keyNamesString;

/** Returns an initialized 'SCUserDefaultsDefinition' given the key names of the user defaults. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNames An array of the user defaults key names. All array elements must be of type NSString.
 */
- (id)initWithUserDefaultsKeyNames:(NSArray *)keyNames;

/** Returns an initialized 'SCUserDefaultsDefinition' given the key names of the user defaults and their titles. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNames An array of the user defaults key names. All array elements must be of type NSString.
 @param keyTitles An array of titles to the keys in keyNames. All array elements must be of type NSString.
 */
- (id)initWithUserDefaultsKeyNames:(NSArray *)keyNames keyTitles:(NSArray *)keyTitles;


@end
