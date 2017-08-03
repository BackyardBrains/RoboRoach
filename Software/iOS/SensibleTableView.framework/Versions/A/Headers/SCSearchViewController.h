/*
 *  SCSearchViewController.h
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

#import "SCViewController.h"



/****************************************************************************************/
/*	class SCSearchViewController */
/****************************************************************************************/
/**
 This class functions as a means to simplify creating view controllers dedicated to searching data within a table view.
 
 In addition to all the features provided by its SCViewController superclass, SCSeachViewController conveniently provides a UISearchBar control and a UITableView control aligned under each other. In addition, SCSearchViewController defaults to automatically displaying a UISearchDisplayController when a search is performed.
 
 @note Both the UISearchBar and UITableView controls can still be created in Interface Builder and assigned to their respective outlets. In case no outlets are assigned, SCSearchViewController will automatically create the missing control(s).
 
 */
@interface SCSearchViewController : SCViewController
{
    @protected
    UISearchBar *_searchBar;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The search bar control associated with the view controller. Unless assigned in Interface Builder, this property will automatically create a UISearch when accessed. */
@property (nonatomic, strong) IBOutlet UISearchBar *searchBar;

@end
