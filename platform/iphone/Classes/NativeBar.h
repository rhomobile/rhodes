//
//  NativeBar.h
//  rhorunner
//
//  Created by lars on 8/21/09.
//  Copyright 2009 Home. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define TOOLBAR_TYPE		0
#define TABBAR_TYPE			1
#define NOBAR_TYPE			2
#define VTABBAR_TYPE		3

#define NATIVE_BAR_PROPERTIES						@"properties"
#define NATIVE_BAR_ITEMS							@"items"

#define NATIVE_BAR_BACKGOUND_COLOR					@"bkg_color"

#define NATIVE_BAR_ITEM_LABEL						@"label"
#define NATIVE_BAR_ITEM_ACTION						@"action"
#define NATIVE_BAR_ITEM_ICON						@"icon"
#define NATIVE_BAR_ITEM_RELOAD						@"reload"
#define NATIVE_BAR_ITEM_COLORED_ICON				@"colored_icon"
#define NATIVE_BAR_ITEM_SELECTED_COLOR				@"selected_color"
#define NATIVE_BAR_ITEM_DISABLED					@"disabled"
#define NATIVE_BAR_ITEM_WEB_BACKGROUND_COLOR		@"web_bkg_color"
#define NATIVE_BAR_ITEM_USE_CURRENT_VIEW_FOR_TAB	@"use_current_view_for_tab"

/*
 
struture of data with Toolbar/TabBar description
 
NSDictioanry
  "properties" - NSDictionary
                   "bkg_color" - NSString
  "items" - NSArray
			  NSDictionary
                "label" - NSString
                "action" - NSString
                "icon" - NSString
                "reload" - NSString
                "colored_icon" - NSString
                "selected_color" - NSString
                "disabled" - NSString
                "bkg_color" - NSString
 
*/
