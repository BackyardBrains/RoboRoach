/*
 *  SCPullToRefreshView.h
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


#import "SCGlobals.h"


typedef enum
{
    SCPullToRefreshViewStatePull,
    SCPullToRefreshViewStateRelease,
    SCPullToRefreshViewStateLoading
} SCPullToRefreshViewState;



/****************************************************************************************/
/*	class SCPullToRefreshView	*/
/****************************************************************************************/ 
/**	
 This special view provides table views with the common pull-to-refresh functionality.
 
 Once pull-to-refresh is enabled in the table view model, this view will automatically handle displaying the pull messsage, inform the model that the table view needs refreshing, and return back to normal state after the table view has done refreshing its contents.
 
 Sample use:
    self.tableViewModel.enablePullToRefresh = TRUE;
    self.tableViewModel.pullToRefreshView.pullStateText = @"Pull to refresh tasks";
    self.tableViewModel.pullToRefreshView.releaseStateText = @"Release to refresh tasks";
    self.tableViewModel.pullToRefreshView.loadingStateText = @"Loading new tasks...";
 
 See also: SCTableViewModel
 */
@interface SCPullToRefreshView : UIView
{
    __weak UIScrollView *_boundScrollView;
    __weak id _target;
	SEL _startLoadingAction;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Configuration
//////////////////////////////////////////////////////////////////////////////////////////

/** The label that displays the current state of the view. */
@property (nonatomic, readonly) UILabel *stateLabel;

/** A detail label displayed under stateLabel. */
@property (nonatomic, readonly) UILabel *detailTextLabel;

/** A UIActivityIndicationView that appears while the table view is being refreshed. */
@property (nonatomic, readonly) UIActivityIndicatorView *activityIndicator;

/** The text that appears while the table view is being pulled down. */
@property (nonatomic, copy) NSString *pullStateText;

/** The text that appears when releasing will refresh the table view. */
@property (nonatomic, copy) NSString *releaseStateText;

/** The text that appears while the table view loads its data. */
@property (nonatomic, copy) NSString *loadingStateText;

/** The arrow image that appears next to stateLabel. This image gets rotated as the table view gets pulled down. */
@property (nonatomic, readonly) UIImageView *arrowImageView;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Internal Properties & Methods (should only be used by the framework or when subclassing)
//////////////////////////////////////////////////////////////////////////////////////////

/** Used internally to determine the current state of the pull-to-refresh view. */
 @property (nonatomic, readonly) SCPullToRefreshViewState state;

/** Used internally to bind a scroll view to the pull-to-refresh view. */
- (void)bindToScrollView:(UIScrollView *)scrollView;

/** Method called internally by the framework to set a target and an action for the refresh operation. */
- (void)setTarget:(id)target forStartLoadingAction:(SEL)action;

/** Method called internally by the framework to indicate that the bound scroll view has started scrolling. */
- (void)boundScrollViewDidScroll;

/** Method called internally by the framework to indicate that the bound scroll view has ended dragging. */
- (void)boundScrollViewDidEndDragging;

/** Method called internally by the framework to indicate that the bound scroll view has finished refreshing. */
- (void)boundScrollViewDidFinishLoading;


@end
