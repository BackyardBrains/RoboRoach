/*
 *  SCDateDefinition.h
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

#import "SCDataDefinition.h"


/****************************************************************************************/
/*	class SCDateDefinition	*/
/****************************************************************************************/ 
/**	
 This class functions as a means to represent the definition of an NSDate class.
 
 This class is typically used when you need to work with a collection of NSDate objects in classes like SCArrayOfObjectsSection and SCArrayOfObjectsModel. The class automatically generates only one property definition representing how the NSDate automatically generated UI should be defined.
 */

@interface SCDateDefinition : SCDataDefinition

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCDateDefinition. */
+ (id)definition;

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The property definition representing the NSDate automatically generated UI element. */
@property (nonatomic, readonly) SCPropertyDefinition *datePropertyDefinition;


@end
