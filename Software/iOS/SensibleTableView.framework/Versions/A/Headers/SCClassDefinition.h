/*
 *  SCClassDefinition.h
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



/****************************************************************************************/
/*	class SCClassDefinition	*/
/****************************************************************************************/ 
/**	
 This class functions as a means to further extend the definition of user-defined classes.
 Using class definitions, classes like SCObjectCell and SCObjectSection 
 will be able to better generate user interface elements that truly represent the 
 properties of their bound objects. For more information, please refer to SCDataDefinition.
 
 Upon creation, SCClassDefinition will (optionally) automatically generate all the
 property definitions for the given class. From there, the user will be able to customize
 the generated property definitions, add new definitions, or remove generated definitions.
 
 Sample use:
    // Extend the definition of 'Task' (user defined class)
    SCClassDefinition *taskDef = [SCClassDefinition definitionWithClass:[Task class] 
        propertyNamesString:@"Task Details:(name,description,category,dueDate);Task Status:(completed)"];
    SCPropertyDefinition *namePropertyDef = [taskDef propertyDefinitionWithName:@"name"];
    namePropertyDef.required = TRUE;
    SCPropertyDefinition *descPropertyDef = [taskDef propertyDefinitionWithName:@"description"];
    descPropertyDef.type = SCPropertyTypeTextView;
    SCPropertyDefinition *categoryPropertyDef = [taskDef propertyDefinitionWithName:@"category"];
    categoryPropertyDef.type = SCPropertyTypeSelection;
    NSArray *categoryItems = [NSArray arrayWithObjects:@"Home", @"Work", @"Other", nil];
    categoryPropertyDef.attributes = [SCSelectionAttributes attributesWithItems:categoryItems 
        allowMultipleSelection:NO allowNoSelection:NO];
 
 @see SCDataDefinition.
 */
@interface SCClassDefinition : SCDataDefinition 

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCClassDefinition given a class and the option to auto generate property definitions for the given class.
 
 The method will also generate user friendly property titles from the names of the generated properties. These titles can be modified by the user later as part of the property definition customization.
 
 @param _cls A class for which the definition will be extended.
 @param autoGenerate If TRUE, SCClassDefinition will automatically generate all the property definitions for the given class. 
 */
+ (id)definitionWithClass:(Class)_cls autoGeneratePropertyDefinitions:(BOOL)autoGenerate;

/** Allocates and returns an initialized SCClassDefinition given a class and a string containing the property names. 
 
 The method will also generate user friendly property titles from the names of 
 the given properties. These titles can be modified by the user later as part of
 the property definition customization.
 
 @param _cls A class for which the definition will be extended.
 @param propertyNamesString A string with the property names separated by semi-colons. Example string: @"firstName;lastName". Property groups can also be defined in the string using the following format: @"Personal Details:(firstName, lastName); Address:(street, state, country)". The group title can also be ommitted to create a group with no title. For example: @":(firstName, lastName)".
 
 Property names string syntax options:
    @"property1;property2;property3;..."
    @"group1 header:(property1, property2,...):group1 footer;group2..."
 */
+ (id)definitionWithClass:(Class)_cls propertyNamesString:(NSString *)propertyNamesString;

/** Allocates and returns an initialized SCClassDefinition given a class and an array of the property names to generate property definitions from.
 
 The method will also generate user friendly property titles from the names of 
 the given properties. These titles can be modified by the user later as part of
 the property definition customization.
 
 @param _cls A class for which the definition will be extended.
 @param propertyNames An array of the names of properties to be generated. All array elements must be of type NSString.
 */
+ (id)definitionWithClass:(Class)_cls propertyNames:(NSArray *)propertyNames;

/** Allocates and returns an initialized SCClassDefinition given a class, an array of
 the property names to generate property definitions from, and array of titles
 for these properties.
 *
 *	@param _cls A class for which the definition will be extended.
 *	@param propertyNames An array of the names of properties to be generated. All array elements must be of type NSString.
 *	@param propertyTitles An array of titles to the properties in propertyNames. All array elements must be of type NSString.
 */
+ (id)definitionWithClass:(Class)_cls propertyNames:(NSArray *)propertyNames
		propertyTitles:(NSArray *)propertyTitles;

/** Allocates and returns an initialized SCClassDefinition given a class and an SCPropertyGroupArray.
 *
 *	@param _cls A class for which the definition will be extended.
 *	@param groups A collection of property groups. 
 */
+ (id)definitionWithClass:(Class)_cls propertyGroups:(SCPropertyGroupArray *)groups;

/** Allocates and returns an initialized SCClassDefinition given a class and the option to auto generate property definitions for the given class.
 
 The method will also generate user friendly property titles from the names of 
 the generated properties. These titles can be modified by the user later as part of
 the property definition customization.
 
 @param _cls A class for which the definition will be extended.
 @param autoGenerate If TRUE, 'SCClassDefinition' will automatically generate all the property definitions for the given class. 
 */
- (id)initWithClass:(Class)_cls autoGeneratePropertyDefinitions:(BOOL)autoGenerate;

/** Returns an initialized SCClassDefinition given a class and a string containing the property names. 
 
 The method will also generate user friendly property titles from the names of 
 the given properties. These titles can be modified by the user later as part of
 the property definition customization.
 
 @param _cls A class for which the definition will be extended.
 @param propertyNamesString A string with the property names separated by semi-colons. Example string: @"firstName;lastName". Property groups can also be defined in the string using the following format: @"Personal Details:(firstName, lastName); Address:(street, state, country)". The group title can also be ommitted to create a group with no title. For example: @":(firstName, lastName)".
 
 Property names string syntax options:
    @"property1;property2;property3;..."
    @"group1 header:(property1, property2,...):group1 footer;group2..."
 */
- (id)initWithClass:(Class)_cls propertyNamesString:(NSString *)propertyNamesString;

/** Allocates and returns an initialized SCClassDefinition given a class and an array of the property names to generate property definitions from.
 
 The method will also generate user friendly property titles from the names of 
 the given properties. These titles can be modified by the user later as part of
 the property definition customization.
 
 @param _cls A class for which the definition will be extended.
 @param propertyNames An array of the names of properties to be generated. All array elements must be of type NSString.
 */
- (id)initWithClass:(Class)_cls propertyNames:(NSArray *)propertyNames;

/** Allocates and returns an initialized SCClassDefinition given a class, an array of
 the property names to generate property definitions from, and array of titles
 for these properties.
 *
 *	@param _cls A class for which the definition will be extended.
 *	@param propertyNames An array of the names of properties to be generated. All array elements must be of type NSString.
 *	@param propertyTitles An array of titles to the properties in propertyNames. All array elements must be of type NSString.
 */
- (id)initWithClass:(Class)_cls propertyNames:(NSArray *)propertyNames propertyTitles:(NSArray *)propertyTitles;

/** Returns an initialized SCClassDefinition given a class and an SCPropertyGroupArray.
 *
 *	@param _cls A class for which the definition will be extended.
 *	@param groups A collection of property groups. 
 */
- (id)initWithClass:(Class)_cls propertyGroups:(SCPropertyGroupArray *)groups;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The class associated with the definition. */
@property (nonatomic, strong) Class cls;


@end





