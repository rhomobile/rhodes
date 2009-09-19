//
//  BarItem.h
//  rhorunner
//
//  Created by lars on 8/25/09.
//  Copyright 2009 Home. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface BarItem : NSObject {
@private
	BOOL loaded,refresh;
	NSString* label,*location,*icon;
	UIViewController* viewController;
}

@property(nonatomic,assign) BOOL loaded;
@property(nonatomic,assign) BOOL refresh;
@property(nonatomic,retain) NSString* label;
@property(nonatomic,retain) NSString* location;
@property(nonatomic,retain) NSString* icon;
@property(nonatomic,retain) UIViewController* viewController;

@end
