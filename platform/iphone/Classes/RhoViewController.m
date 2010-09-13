//
//  RhoViewController.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 13.09.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Rhodes.h"
#import "RhoViewController.h"

@implementation RhoViewController

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    if ([[Rhodes sharedInstance] isRotationLocked])
        return interfaceOrientation == UIInterfaceOrientationPortrait;
	return YES;
}

@end
