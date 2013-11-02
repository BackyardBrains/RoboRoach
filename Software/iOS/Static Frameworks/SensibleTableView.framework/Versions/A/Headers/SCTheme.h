/*
 *  SCTheme.h
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


#import <Foundation/Foundation.h>


/**
 The SCTheme class enables CSS-like styling of UIView (or any of its subclasses). Using CSS-like text files, any UIView property with a supported data type can be set. Key-path property format is fully supported (e.g. layer.cornerRadius: 5). All pointer data types accept nil as value (e.g. backgroundView: nil;).
 
 Supported property data types:
 
 Data Type   ->       Format in .sct file
 ---------            -------------------
 NSString*   ->       A string between single or double quotes.
                        Example:    detailTextLabel.text: "Hello World!";
 
 CGFloat     ->       A normal number.
                        Example:    height: 60;
 
 BOOL        ->       Possible values: TRUE, FALSE, YES, NO. All case insensitive.
                        Example:    clipsToBounds: NO;
 
 UIColor*    ->       Any UIColor color name constructor.
                        Example:    backgroundColor: blueColor;
    or                rgb(redValue, greenValue, blueValue, optionalAlphaValue)
                        Example:    backgroundColor: rgb(100, 0, 255);
    or                #hexValue
                        Example:    backgroundColor: #CC33FF
    or                A string containing an image resource.
                        Example:    backgroundColor: "background.png";
 
 CGColorRef  ->       Format identical to UIColor.
                        Example:    layer.borderColor: redColor;
 
 UIImage*    ->       A string containing the image resource.
                        Example:    backgroundImage: "background.png"
    or                A string containing the image resource and capInsets(top, left, bottom, right).
                        Example:    backgroundImage: "background.png" capInsets(0,0,0,0)
 
 UIFont*     ->       Font name and size separated by a space. A good resource for font names is: http://iosfonts.com/
                        Example:    textLabel.font: Courier-Bold 12;
 
 UIView*     ->       A string containing an image resource that will be loaded into a UIImageView.
                        Example:    backgroundView: "background.png";
 
 UITableViewCellSeparatorStyle      Any valid UITableViewCellSeparatorStyle constant.
                                        Example: separatorStyle: UITableViewCellSeparatorStyleNone;
 
 
 Sample theme file: MyTheme.sct
    UINavigationBar
    {
        backgroundImage: "navbar-background.png";
    }
  
    UITableView
    {
        backgroundView: "leather-background.png";
    }
 
    SCPullToRefreshView
    {
        backgroundColor: "leather-background.png";
    }
 
    SCTableViewSection
    {
        firstCellThemeStyle: firstCell;
        oddCellsThemeStyle: oddCell;
        evenCellsThemeStyle: evenCell;
        lastCellThemeStyle: lastCell;
    }
 
    firstCell
    {
        backgroundView: "firstCell-background.png";
        selectedBackgroundView: "selectedFirstCell-background.png";
    }
 
    oddCell
    {
        backgroundView: "oddCell-background.png";
        selectedBackgroundView: "selectedOddCell-background.png";
    }
 
    evenCell
    {
        backgroundView: "evenCell-background.png";
        selectedBackgroundView: "selectedEvenCell-background.png";
    }
 
    lastCell
    {
        backgroundView: "lastCell-background.png";
        selectedBackgroundView: "selectedLastCell-background.png";
    }
 
 
 Sample code needed to use the above theme file:
    self.tableViewModel.theme = [SCTheme themeWithPath:@"MyTheme.sct"];
 */

@interface SCTheme : NSObject

//////////////////////////////////////////////////////////////////////////////////////////
/// @name Creation and Initialization
//////////////////////////////////////////////////////////////////////////////////////////

/** Allocates and returns an initialized SCTheme given the path of the theme file. */
+ (id)themeWithPath:(NSString *)path;

/** Returns an initialized SCTheme given the path of the theme file. */
- (id)initWithPath:(NSString *)path;


//////////////////////////////////////////////////////////////////////////////////////////
/// @name Custom Styling
//////////////////////////////////////////////////////////////////////////////////////////

/** Styles the given object (typically a UIView subclass) using a style name provided in the theme's theme file. */
- (void)styleObject:(NSObject *)object usingThemeStyle:(NSString *)style;

@end
