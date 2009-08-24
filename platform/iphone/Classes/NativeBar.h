//
//  NativeBar.h
//  rhorunner
//
//  Created by lars on 8/21/09.
//  Copyright 2009 Home. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define TOOLBAR_TYPE 0
#define TABBAR_TYPE 1

@interface NativeBar : NSObject 
{
@private
	int barType;
	NSArray* barItems;
}

@property(assign) int barType;
@property(nonatomic, retain) NSArray* barItems;

@end
