/*
 *  SCExpandCollapseCell.h
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

#import "SCTableViewCell.h"




/**
 SCExpandCollapseCell is a special cell class that, once tapped, collapses its owner section's contents into a single cell. When tapped again, it will expand the section's contents to their original state.
 
 Sample use:
    mySection.expandCollapseCell = [SCExpandCollapseCell cellWithExpandText:@"Expand Section" 
        collapseText:@"Collapse Section" ownerSectionExpanded:FALSE];
 
 See also: SCTableViewSection
 
 @note SCExpandCollapseCell is always displayed at the top of the section.
 */
@interface SCExpandCollapseCell : SCCustomCell


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCExpandCollapseCell given its expandText, collapseText, an initial owner section expand state.
 @param expandText The text that will be displayed in the cell when the section is collapsed.
 @param collapseText The text that will be displayed in the cell when the section is expanded.
 @param expanded The initial expanded state of the cell's owner section.
 */
+ (id)cellWithExpandText:(NSString *)expandText collapseText:(NSString *)collapseText ownerSectionExpanded:(BOOL)expanded;

/** Returns an initialized SCExpandCollapseCell given its expandText, collapseText, an initial owner section expand state.
 @param expandText The text that will be displayed in the cell when the section is collapsed.
 @param collapseText The text that will be displayed in the cell when the section is expanded.
 @param expanded The initial expanded state of the cell's owner section.
 */
- (id)initWithExpandText:(NSString *)expandText collapseText:(NSString *)collapseText ownerSectionExpanded:(BOOL)expanded;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The text that will be displayed in the cell when the section is collapsed. */
@property (nonatomic, copy) NSString *expandText;

/** The text that will be displayed in the cell when the section is expanded. */
@property (nonatomic, copy) NSString *collapseText;

/** The expanded state of the ownerSection. Set to TRUE to expand the ownerSection's content. Default:FALSE. */
@property (nonatomic, readwrite) BOOL ownerSectionExpanded;

@end
