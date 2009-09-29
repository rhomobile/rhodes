//
//  BarItem.h
//  rhorunner
//
//  Created by lars on 8/25/09.
//  Copyright 2009 Home. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WebViewController.h"


@interface BarItem : NSObject {
@private
	BOOL loaded,reload;
	NSString* label,*location,*icon;
	WebViewController* viewController;
}

@property(nonatomic,assign) BOOL loaded;
@property(nonatomic,assign) BOOL reload;
@property(nonatomic,retain) NSString* label;
@property(nonatomic,retain) NSString* location;
@property(nonatomic,retain) NSString* icon;
@property(nonatomic,retain) WebViewController* viewController;

@end
