//
//  NativeBar.h
//  rhorunner
//
//  Created by lars on 8/21/09.
//  Copyright 2009 Home. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef enum {
	RhoNativeBar,
	RhoTabBar,
} RhoBarType;

@interface NativeBar : NSObject 
{
@private
	RhoBarType barType;
	NSArray* barItems;
}

@property(assign) RhoBarType barType;
@property(nonatomic, retain) NSArray* barItems;

@end
