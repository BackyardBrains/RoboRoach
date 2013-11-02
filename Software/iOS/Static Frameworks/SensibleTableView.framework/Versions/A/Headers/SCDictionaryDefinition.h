/*
 *  SCDictionaryDefinition.h
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

#import "SCDataDefinition.h"


/****************************************************************************************/
/*	class SCDictionaryDefinition	*/
/****************************************************************************************/ 
/**	
 This class functions as a means to further extend the key definitions of an NSMutableDictionary.
 
 Using dictionary definitions, classes like SCObjectCell and SCObjectSection 
 will be able to better generate user interface elements that truly represent the 
 keys of their bound mutable dictionaries. 
 
 Sample use:
    // Define the dictionary using SCDictionaryDefinition
    SCDictionaryDefinition *dictionaryDef = [SCDictionaryDefinition definitionWithDictionaryKeyNamesString:@"key1;key2;key3"];
    [dictionaryDef propertyDefinitionWithName:@"key2"].type = SCPropertyTypeSwitch;
    [dictionaryDef propertyDefinitionWithName:@"key3"].type = SCPropertyTypeSlider;
    
    // Create some sample data
    NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
    [dictionary setValue:@"Text" forKey:@"key1"];
    [dictionary setValue:[NSNumber numberWithBool:YES] forKey:@"key2"];
    [dictionary setValue:[NSNumber numberWithFloat:0.7f] forKey:@"key3"];
    
    // Generate sections for the dictionary
    [self.tableViewModel generateSectionsForObject:dictionary withDefinition:dictionaryDef];
 
 @see SCPropertyDefinition.
 */

@interface SCDictionaryDefinition : SCDataDefinition

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCDictionaryDefinition' given the key names string of the mutable dictionary to be defined. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNamesString A string with the key names separated by semi-colons. Example string: @"firstName;lastName". Property groups can also be defined in the string using the following format: @"Personal Details:(firstName, lastName); Address:(street, state, country)". The group title can also be ommitted to create a group with no title. For example: @":(firstName, lastName)".
 
 Key names string syntax options:
    @"key1;key2;key3;..."
    @"group1 header:(key1, key2,...):group1 footer;group2..."
 */
+ (id)definitionWithDictionaryKeyNamesString:(NSString *)keyNamesString;

/** Allocates and returns an initialized 'SCDictionaryDefinition' given the key names of the mutable dictionary to be defined. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNames An array of the dictionary key names. All array elements must be of type NSString.
 */
+ (id)definitionWithDictionaryKeyNames:(NSArray *)keyNames;

/** Allocates and returns an initialized 'SCDictionaryDefinition' given the key names and titles of the mutable dictionary to be defined. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNames An array of the dictionary key names. All array elements must be of type NSString.
 @param keyTitles An array of titles to the keys in keyNames. All array elements must be of type NSString.
 */
+ (id)definitionWithDictionaryKeyNames:(NSArray *)keyNames keyTitles:(NSArray *)keyTitles;


/** Returns an initialized 'SCDictionaryDefinition' given the key names string of the mutable dictionary to be defined. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNamesString A string with the key names separated by semi-colons. Example string: @"firstName;lastName". Property groups can also be defined in the string using the following format: @"Personal Details:(firstName, lastName); Address:(street, state, country)". The group title can also be ommitted to create a group with no title. For example: @":(firstName, lastName)".
 
 Key names string syntax options:
    @"key1;key2;key3;..."
    @"group1 header:(key1, key2,...):group1 footer;group2..."
 */
- (id)initWithDictionaryKeyNamesString:(NSString *)keyNamesString;

/** Returns an initialized 'SCDictionaryDefinition' given the key names of the mutable dictionary to be defined. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNames An array of the dictionary key names. All array elements must be of type NSString.
 */
- (id)initWithDictionaryKeyNames:(NSArray *)keyNames;

/** Returns an initialized 'SCDictionaryDefinition' given the key names and titles of the mutable dictionary to be defined. 
 
 By default, all property definitions generated for the given keyNames will have a type of SCPropertyTypeTextField. This can be fully customized after initialization.
 
 @param keyNames An array of the dictionary key names. All array elements must be of type NSString.
 @param keyTitles An array of titles to the keys in keyNames. All array elements must be of type NSString.
 */
- (id)initWithDictionaryKeyNames:(NSArray *)keyNames keyTitles:(NSArray *)keyTitles;

@end
