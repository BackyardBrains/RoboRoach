/*
 *  SCPropertyDefinition.h
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

#import "SCPropertyAttributes.h"
#import "SCCellActions.h"


/** @enum The types of an SCPropertyDefinition */
typedef enum
{
	/** The object bound to the property will detect the best user interface element to generate. */
	SCPropertyTypeAutoDetect,
	/**	The object bound to the property will generate an SCLabelCell interface element */
	SCPropertyTypeLabel,
	/**	The object bound to the property will generate an SCTextViewCell interface element */
	SCPropertyTypeTextView,
	/**	The object bound to the property will generate an SCTextFieldCell interface element */
	SCPropertyTypeTextField,
	/**	The object bound to the property will generate an SCNumericTextFieldCell interface element */
	SCPropertyTypeNumericTextField,
	/**	The object bound to the property will generate an SCSliderCell interface element */
	SCPropertyTypeSlider,
	/**	The object bound to the property will generate an SCSegmentedCell interface element */
	SCPropertyTypeSegmented,
	/**	The object bound to the property will generate an SCSwitchCell interface element */
	SCPropertyTypeSwitch,
	/**	The object bound to the property will generate an SCDateCell interface element */
	SCPropertyTypeDate,
	/**	The object bound to the property will generate an SCImagePickerCell interface element */
	SCPropertyTypeImagePicker,
	/**	The object bound to the property will generate an SCSelectionCell interface element */
	SCPropertyTypeSelection,
	/**	The object bound to the property will generate an SCObjectSelectionCell interface element */
	SCPropertyTypeObjectSelection,
	/**	The object bound to the property will generate an SCObjectCell interface element */
	SCPropertyTypeObject,
	/**	The object bound to the property will generate an SCArrayOfObjectsCell interface element */
	SCPropertyTypeArrayOfObjects,
	/**	The object bound to the property will generate a custom interface element */
	SCPropertyTypeCustom,
	/**	The object bound to the property will not generate an interface element */
	SCPropertyTypeNone,
	/**	Undefined property type */
	SCPropertyTypeUndefined
	
} SCPropertyType;


/****************************************************************************************/
/*	class SCPropertyDefinition	*/
/****************************************************************************************/ 
/**	
 This class functions as a property definition for SCDataDefinition. 
 
 Every property definition should set a property type that determines which user interface element
 should be generated for the property. In addition, the generated user interface element
 can be further customized using the property's attributes.
 
 @see SCPropertyType, SCPropertyAttributes, SCClassDefinition, SCEntityDefinition, SCDictionaryDefinition.
 */
@interface SCPropertyDefinition : NSObject
{
	__weak SCDataDefinition *ownerDataStuctureDefinition;
	SCDataType dataType;
	BOOL dataReadOnly;
	NSString *name;
	NSString *title;
	SCPropertyType type;
	SCPropertyAttributes *attributes;
	SCPropertyType editingModeType;
	SCPropertyAttributes *editingModeAttributes;
	BOOL required;
	BOOL autoValidate;
    BOOL existsInNormalMode;
    BOOL existsInEditingMode;
    BOOL existsInCreationMode;
    BOOL existsInDetailMode;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCPropertyDefinition' given a property name.
 *
 *	@param propertyName The name of the property.           
 */
+ (id)definitionWithName:(NSString *)propertyName;

/** Allocates and returns an initialized 'SCPropertyDefinition' given a property name, title, and type.
 *
 *	@param propertyName The name of the property.
 *	@param propertyTitle The title of the property. Property titles are used by user interface elements to generate labels associated with the generated controls.
 *	@param propertyType The property type determines which user interface element will be generated for the property.
 */
+ (id)definitionWithName:(NSString *)propertyName 
				   title:(NSString *)propertyTitle
					type:(SCPropertyType)propertyType;

/** Returns an initialized 'SCPropertyDefinition' given a property name.
 *
 *	@param propertyName The name of the property.
 */
- (id)initWithName:(NSString *)propertyName;

/** Returns an initialized 'SCPropertyDefinition' given a property name, title, and type.
 *
 *	@param propertyName The name of the property.
 *	@param propertyTitle The title of the property. Property titles are used by user interface elements to generate labels associated with the generated controls.
 *	@param propertyType The property type determines which user interface element will be generated for the property.
 */
- (id)initWithName:(NSString *)propertyName 
			 title:(NSString *)propertyTitle
			  type:(SCPropertyType)propertyType;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The owner data structure definition of the property definition. 
 *
 * @warning This property gets set automatically by the framework, you should never set this property manually. 
 */
@property (nonatomic, weak) SCDataDefinition *ownerDataStuctureDefinition;

/** The name of the property associated with the property definition. */
@property (nonatomic, readonly) NSString *name;

/** 
 The title of the property. Property titles are used by user
 interface elements to generate labels associated with the generated controls.
 */
@property (nonatomic, copy) NSString *title;

/** The type of the property. Property types determines which user interface element will be generated for the property. */
@property (nonatomic, readwrite) SCPropertyType type;

/** 
 The attibutes set associated with the type of the property. Property attributes gives the user the ability
 to further customize the user interface element that will be generated for the given property type. 
 */
@property (nonatomic, strong) SCPropertyAttributes *attributes;

/** The set of cell action blocks. */
@property (nonatomic, readonly) SCCellActions *cellActions;

/** 
 The type of the property while in editing mode. Property types determines which user interface element will
 be generated for the property while in editing mode. 
 @note If not set (or set to SCPropertyTypeUndefined), the value set in the "type" property is used in editing mode. 
 */
@property (nonatomic, readwrite) SCPropertyType editingModeType;

/** 
 The attibutes set associated with the editingModetype of the property. Property attributes gives the user the ability to further customize the user interface element that will be generated for the given property editingModetype. 
 */
@property (nonatomic, strong) SCPropertyAttributes *editingModeAttributes;

/** Set to TRUE if property is a required property. Default: FALSE. */
@property (nonatomic, readwrite) BOOL required;

/** 
 Set to TRUE if the property value should be automatically validated by the user interface element
 before commiting it to the property. If the user chooses to provide custom validation
 using either the cell's actions, or the model's SCTableViewModelDelegate, they should
 set this property to FALSE. Default: TRUE. 
 */
@property (nonatomic, readwrite) BOOL autoValidate;

/** If set to TRUE, a user interface is generated for the property when the owning control is in Normal Mode, otherwise, no UI is generated in that mode. Default: TRUE. */
@property (nonatomic, readwrite) BOOL existsInNormalMode;

/** If set to TRUE, a user interface is generated for the property when the owning control is in Editing Mode, otherwise, no UI is generated in that mode. Default: TRUE. */
@property (nonatomic, readwrite) BOOL existsInEditingMode;

/** If set to TRUE, a user interface is generated for the property when the UI is generated for a newly created object, otherwise, no UI is generated. Default: TRUE. */
@property (nonatomic, readwrite) BOOL existsInCreationMode;

/** If set to TRUE, a user interface is generated for the property when the UI is generated for an existing object, otherwise, no UI is generated. Default: TRUE. */
@property (nonatomic, readwrite) BOOL existsInDetailMode;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Custom UI
//////////////////////////////////////////////////////////////////////////////////////////

/** The class of the custom user interface element the property will generate. Set this property to override the standard user interface element generated by the property's 'type'. */
@property (nonatomic, strong) Class uiElementClass;

/** The Nib name of the custom user interface element the property will generate. Set this property to override the standard user interface element generated by the property's 'type'.
 *
 *  @warning This setting is only applicable for type SCPropertyTypeCustom and will be ignored for all other property types.
 *  @warning No need to set uiElementClass if 'uiElementNibName' contains a valid value. */
@property (nonatomic, copy) NSString *uiElementNibName;

/** 
 This dictionary specifies how each of the uiElementClass or uiElementNibName custom controls binds itself to the definition's properties. Each dictionary key should be the tag string value of one of the uiElement's custom controls, and the value should be the name of the definition's property that is bound to that control. 
 @warning IMPORTANT: All control tags must be greater than zero.
 @note This property can be used interchangibly with objectBindingsString
 */
@property (nonatomic, strong) NSDictionary *objectBindings;

/** 
 This string specifies how each of the uiElementClass or uiElementNibName custom controls binds itself to the definition's properties. The string should consist of a colon separated pair of control's tag and the property name, while each pair should be separated by a semi-colon.
 
 Example bindings string:
    NSString *bindingsString = @"1:firstName;2:lastName";
 @warning IMPORTANT: All control tags must be greater than zero.
 @note This property can be used interchangibly with objectBindings
 */
@property (nonatomic, copy) NSString *objectBindingsString;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal
//////////////////////////////////////////////////////////////////////////////////////////

/** This property is automatically set by the framework and should never be assigned a value directly. */
@property (nonatomic, readwrite) SCDataType dataType;

/** Returns TRUE if the property's dataType is scalar. */
@property (nonatomic, readonly) BOOL dataTypeScalar;

/** This property is automatically set by the framework and should never be assigned a value directly. */
@property (nonatomic, readwrite) BOOL dataReadOnly;

@end





/****************************************************************************************/
/*	class SCCustomPropertyDefinition	*/
/****************************************************************************************/ 
/**	
 This class functions as a property definition for SCDataDefinition that will generate
 a custom user inteface element (e.g.: custom cell). 
 
 'SCCustomPropertyDefinition' does not have to represent a property that actually exists in its class (unlike an SCPropertyDefiniton), and is often used in a structure definition as a placeholder for custom user interface element generation.
 
 @see SCPropertyDefinition.
 */
@interface  SCCustomPropertyDefinition : SCPropertyDefinition

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCCustomPropertyDefinition' given a property name and a class of the  custom user interface element to generate.
 *
 *	@param propertyName The name of the property.
 *	@param elementClass The class of the custom user interface element that will be generated.
 *	@param bindings This dictionary specifies how each of the uiElement's custom controls binds itself to the data definition's properties. Each dictionary key should be the tag string value of one of the uiElement's custom controls, and the value should be the name of the class definition's property that is bound to that control. 
 *	@warning IMPORTANT: All control tags must be greater than zero.
 */
+ (id)definitionWithName:(NSString *)propertyName uiElementClass:(Class)elementClass objectBindings:(NSDictionary *)bindings;

/** Allocates and returns an initialized 'SCCustomPropertyDefinition' given a property name and a class of the  custom user interface element to generate.
 *
 *	@param propertyName The name of the property.
 *	@param elementClass The class of the custom user interface element that will be generated.
 *	@param bindingsString This string specifies how each of the uiElement's custom controls binds itself to the data definition's properties. The string should consist of a colon separated pair of control's tag and the property name, while each pair should be separated by a semi-colon. 
 
 *  Example bindings string:
        NSString *bindingsString = @"1:firstName;2:lastName";
 *	@warning IMPORTANT: All control tags must be greater than zero.
 */
+ (id)definitionWithName:(NSString *)propertyName uiElementClass:(Class)elementClass objectBindingsString:(NSString *)bindingsString;

/** Allocates and returns an initialized 'SCCustomPropertyDefinition' given a property name and the name of the nib file containing the custom user interface element to generate.
 *
 *	@param propertyName The name of the property.
 *	@param elementNibName The name of the nib file containing the custom user interface element that will be generated.
 *	@param bindings This dictionary specifies how each of the uiElement's custom controls binds itself to the data definition's properties. Each dictionary key should be the tag string value of one of the uiElement's custom controls, and the value should be the name of the class definition's property that is bound to that control. 
 *	@warning IMPORTANT: All control tags must be greater than zero.
 */
+ (id)definitionWithName:(NSString *)propertyName uiElementNibName:(NSString *)elementNibName objectBindings:(NSDictionary *)bindings;

/** Allocates and returns an initialized 'SCCustomPropertyDefinition' given a property name and the name of the nib file containing the custom user interface element to generate.
 *
 *	@param propertyName The name of the property.
 *	@param elementNibName The name of the nib file containing the custom user interface element that will be generated.
 *	@param bindingsString This string specifies how each of the uiElement's custom controls binds itself to the data definition's properties. The string should consist of a colon separated pair of control's tag and the property name, while each pair should be separated by a semi-colon. 
 
 *  Example bindings string:
        NSString *bindingsString = @"1:firstName;2:lastName"; 
 *	@warning IMPORTANT: All control tags must be greater than zero.
 */
+ (id)definitionWithName:(NSString *)propertyName uiElementNibName:(NSString *)elementNibName objectBindingsString:(NSString *)bindingsString;

/** Returns an initialized 'SCCustomPropertyDefinition' given a property name and a class of the  custom user interface element to generate.
 *
 *	@param propertyName The name of the property.
 *	@param elementClass The class of the custom user interface element that will be generated.
 *	@param bindings This dictionary specifies how each of the uiElement's custom controls binds itself to the data definition's properties. Each dictionary key should be the tag string value of one of the uiElement's custom controls, and the value should be the name of the class definition's property that is bound to that control. 
 *	@warning IMPORTANT: All control tags must be greater than zero.
 */
- (id)initWithName:(NSString *)propertyName uiElementClass:(Class)elementClass objectBindings:(NSDictionary *)bindings;

/** Returns an initialized 'SCCustomPropertyDefinition' given a property name and a class of the  custom user interface element to generate.
 *
 *	@param propertyName The name of the property.
 *	@param elementClass The class of the custom user interface element that will be generated.
 *	@param bindingsString This string specifies how each of the uiElement's custom controls binds itself to the data definition's properties. The string should consist of a colon separated pair of control's tag and the property name, while each pair should be separated by a semi-colon. 
 
 *  Example bindings string:
        NSString *bindingsString = @"1:firstName;2:lastName";
 *	@warning IMPORTANT: All control tags must be greater than zero.
 */
- (id)initWithName:(NSString *)propertyName uiElementClass:(Class)elementClass objectBindingsString:(NSString *)bindingsString;

/** Returns an initialized 'SCCustomPropertyDefinition' given a property name and the name of the nib file containing the custom user interface element to generate.
 *
 *	@param propertyName The name of the property.
 *	@param elementNibName The name of the nib file containing the custom user interface element that will be generated.
 *	@param bindings This dictionary specifies how each of the uiElement's custom controls binds itself to the class definition's properties. Each dictionary key should be the tag string value of one of the uiElement's custom controls, and the value should be the name of the class definition's property that is bound to that control. 
 *	@warning IMPORTANT: All control tags must be greater than zero.
 */
- (id)initWithName:(NSString *)propertyName uiElementNibName:(NSString *)elementNibName objectBindings:(NSDictionary *)bindings;

/** Allocates and returns an initialized 'SCCustomPropertyDefinition' given a property name and a class of the  custom user interface element to generate.
 *
 *	@param propertyName The name of the property.
 *	@param elementNibName The name of the nib file containing the custom user interface element that will be generated.
 *	@param bindingsString This string specifies how each of the uiElement's custom controls binds itself to the data definition's properties. The string should consist of a colon separated pair of control's tag and the property name, while each pair should be separated by a semi-colon. 
 
 *  Example bindings string:
        NSString *bindingsString = @"1:firstName;2:lastName";
 *	@warning IMPORTANT: All control tags must be greater than zero.
 */
- (id)initWithName:(NSString *)propertyName uiElementNibName:(NSString *)elementNibName objectBindingsString:(NSString *)bindingsString;


@end







/****************************************************************************************/
/*	class SCPropertyGroup	*/
/****************************************************************************************/ 
/**	
 This class functions as a way to define a property definition group that is used to generate
 a corresponding user interface element that groups these properties (e.g.: a table view section). 
 
 Property definitions are added to this class using their NSString 'name' property.
 
 @see SCPropertyDefinition, SCClassDefinition.
 */
@interface SCPropertyGroup : NSObject
{
    NSString *headerTitle;
    NSString *footerTitle;
    NSMutableArray *propertyDefinitionNames;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////


/** Allocates and returns an initialized 'SCPropertyGroup' given a header title, a footer title
 *  and an NSArray of property names to be included in the group.
 *
 *	@param sectionHeaderTitle A header title for the group.
 *	@param sectionFooterTitle A footer title for the group.
 */
+ (id)groupWithHeaderTitle:(NSString *)groupHeaderTitle footerTitle:(NSString *)groupFooterTitle propertyNames:(NSArray *)propertyNames;

/** Returns an initialized 'SCPropertyGroup' given a header title, a footer title
 *  and an NSArray of property names to be included in the group.
 *
 *	@param sectionHeaderTitle A header title for the group.
 *	@param sectionFooterTitle A footer title for the group.
 */
- (id)initWithHeaderTitle:(NSString *)groupHeaderTitle footerTitle:(NSString *)groupFooterTitle
        propertyNames:(NSArray *)propertyNames;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The group header title. */
@property (nonatomic, copy) NSString *headerTitle;

/** The group footer title. */
@property (nonatomic, copy) NSString *footerTitle;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Property Names
//////////////////////////////////////////////////////////////////////////////////////////

/** The count of the property names. */
@property (nonatomic, readonly) NSInteger propertyNameCount;

/**	Add the property name to the group. */
- (void)addPropertyName:(NSString *)propertyName;

/**	Add the property name to the group at the given index. */
- (void)insertPropertyName:(NSString *)propertyName atIndex:(NSInteger)index;

/**	Return the property name at the given index. */
- (NSString *)propertyNameAtIndex:(NSInteger)index;

/**	Remove the property name at the given index from the group. */
- (void)removePropertyNameAtIndex:(NSInteger)index;

/**	Remove all property names from the group. */
- (void)removeAllPropertyNames;

/**	Returns TRUE if the property name exists in the group. */
- (BOOL)containsPropertyName:(NSString *)propertyName;

@end



/****************************************************************************************/
/*	class SCPropertyGroupArray	*/
/****************************************************************************************/ 
/**	
 This class functions as an array of SCPropertyGroup objects. 
 
 @see SCPropertyGroup.
 */
@interface SCPropertyGroupArray : NSObject
{
    NSMutableArray *propertyGroups;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized 'SCPropertyGroupArray'. */
+ (id)groupArray;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Managing Groups
//////////////////////////////////////////////////////////////////////////////////////////

@property (nonatomic, readonly) NSInteger groupCount;

/**	Add a group to the array. */
- (void)addGroup:(SCPropertyGroup *)group;

/**	Add a group at the given index. */
- (void)insertGroup:(SCPropertyGroup *)group atIndex:(NSInteger)index;

/**	Return group at the given index. */
- (SCPropertyGroup *)groupAtIndex:(NSInteger)index;

/**	Return group with the given header title. */
- (SCPropertyGroup *)groupByHeaderTitle:(NSString *)headerTitle;

/**	Remove group at the given index. */
- (void)removeGroupAtIndex:(NSInteger)index;

/**	Remove all groups. */
- (void)removeAllGroups;

@end


