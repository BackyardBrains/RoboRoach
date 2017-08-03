/*
 *  SCGlobals.h
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




/*
 *	The following defines global Sesible TableView defaults. You are very much discouraged from
 *	modifying the content of these values, mainly for your product to remain forward compatable
 *	with future versions of Sensible TableView. If you think you need to change any of these 
 *	values, please consider either modifying the respective property of the Sensible TableView 
 *	object, or subclassing it.
 */
/**********************************************************************************/
#define     SC_DefaultCellStyle                 UITableViewCellStyleSubtitle
#define		SC_DefaultMaxTextLabelWidth			200		// Default maximum width of SCTableViewCell textLabel
#define		SC_DefaultControlMargin				10		// Default margin between different controls
#define		SC_DefaultControlHeight				22		// Default control height
#define		SC_DefaultControlIndentation		120		// Default control indentation from table left border
#define		SC_DefaultTextViewFontSize			17		// Default font size of UITextView
#define		SC_DefaultTextFieldHeight			31		// Default height of UITextField
#define		SC_DefaultSegmentedControlHeight	29		// Default height of UISegmentedControl
/**********************************************************************************/






/**********************************************************************************/
/************************** DO NOT EDIT AFTER THIS LINE ***************************/
/**********************************************************************************/



/***************************************/
/* ARC supporting functions and macros */
/***************************************/

#ifndef __has_feature      
#define __has_feature(x) 0 // For compatibility with non-clang compilers.
#endif

#if __has_feature(objc_arc)
#define ARC_ENABLED
#endif

/***************************************/




/***************************************/
/* SCDebugLog definition               */
/***************************************/

#define SCDebugLog(...) NSLog(@"**STV** %@", [NSString stringWithFormat:__VA_ARGS__])

/***************************************/


/***************************************/
/* License                             */
/***************************************/

#define SC_STVLITE

/***************************************/





/* Data type definitions. */
typedef enum
{
    SCDataTypeBOOL,
    SCDataTypeInt,
    SCDataTypeFloat,
    SCDataTypeDouble,
	SCDataTypeNSString,
	SCDataTypeNSNumber,
	SCDataTypeNSDate,
	SCDataTypeNSMutableSet,
	SCDataTypeNSMutableArray,
	SCDataTypeNSObject,
	SCDataTypeDictionaryItem,
	SCDataTypeTransformable,
	SCDataTypeUnknown
} SCDataType;





@interface NSObject (SensibleCocoa)

/* 
 This method allows accessing array values in a regular key-path string by placing the array item index between brakets.
 
 Examples: 
    firstSaleAmount = [customer valueForSensibleKeyPath:@"salesData.invoices[0].amount"];
    lastSaleAmount = [customer valueForSensibleKeyPath:@"salesData.invoices[n].amount"];
    saleBeforeLastAmount = [customer valueForSensibleKeyPath:@"salesData.invoices[n-1].amount"];
 */
- (id)valueForSensibleKeyPath:(NSString *)keyPath;

/* Similar to valueForSensibleKeyPath, this method uses a Sensible keyPath to set a value. */
- (void)setValue:(id)value forSensibleKeyPath:(NSString *)keyPath;

@end





@interface UINavigationController (KeyboardDismiss)

- (BOOL)disablesAutomaticKeyboardDismissal;

@end





/* This class defines a set of internal helper methods */
@interface SCUtilities : NSObject
{
}

/* Method determines the system version */
+ (double)systemVersion;

/* Method determines if code is being run on an iPad */
+ (BOOL)is_iPad;

/* Method determines if the given view is inside a popover view */
+ (BOOL)isViewInsidePopover:(UIView *)view;

+ (BOOL)isURLValid:(NSString *)urlString;

+ (NSObject *)getFirstNodeInNibWithName:(NSString *)nibName;

+ (NSString *)getUserFriendlyTitleFromName:(NSString *)propertyName;

+ (BOOL)isStringClass:(Class)aClass;

+ (BOOL)isNumberClass:(Class)aClass;

+ (BOOL)isDateClass:(Class)aClass;

+ (BOOL)isDictionaryClass:(Class)aClass;

+ (BOOL)isBasicDataTypeClass:(Class)aClass;

+ (NSString *)dataStructureNameForClass:(Class)aClass;

+ (BOOL)propertyName:(NSString *)propertyName existsInObject:(NSObject *)object;

/*  Returns the the value for the given property in the given object. 
 *	@param propertyName The name of the property whose value is requested. propertyName can be
 *	given in a key-path format (e.g.: "subObject1.subObject2.subObject3"). More than one property
 *	can be given, separated by ';' (e.g.: "property1;property2;property3").
 *	@param object The owner object of the given property.  
 *	@return Returns the value of the given property. If more than one property name is given
 *	(separated by ';'), the return value is an NSArray. Returns nil if property does not exist
 *	in object (or NSNull if more than one property name is given). */
+ (NSObject *)valueForPropertyName:(NSString *)propertyName inObject:(NSObject *)object;

+ (NSString *)stringValueForPropertyName:(NSString *)propertyName inObject:(NSObject *)object
			separateValuesUsingDelimiter:(NSString *)delimiter;

+ (void)setValue:(NSObject *)value forPropertyName:(NSString *)propertyName inObject:(NSObject *)object;

+ (NSObject *)getValueCompatibleWithDataType:(SCDataType)dataType fromValue:(NSObject *)value;

+ (NSDictionary *)bindingsDictionaryForBindingsString:(NSString *)bindingsString;

+ (NSString *)bindingsStringForBindingsDictionary:(NSDictionary *)bindingsDictionary;


+ (NSString *)base64EncodedStringFromString:(NSString *)string;

@end




@class SCTableViewModel;

/* This class defines a tabel view model center.
 * IMPORTANT: This class is usually only used internally by the framework. */
@interface SCModelCenter : NSObject
{
	CFMutableSetRef modelsSet;
}

@property (nonatomic, weak) UIViewController *keyboardIssuer;

+ (SCModelCenter *)sharedModelCenter;

- (void)registerModel:(SCTableViewModel *)model;
- (void)unregisterModel:(SCTableViewModel *)model;

- (SCTableViewModel *)modelForViewController:(UIViewController *)viewController;

@end






@interface SCLicenseManager : NSObject

+ (UIView *)licenseViewForWidth:(CGFloat)width;

+ (NSObject *)unlicensedObjectWithName:(NSString *)name;
+ (NSObject *)unlicensedCoreDataObject;
+ (NSObject *)unlicensedWebServiceObject;
+ (NSObject *)unlicensediCloudObject;

+ (BOOL)isUnlicensedObject:(NSObject *)object;
+ (NSString *)nameForUnlicensedObject:(NSObject *)object;

@end






