//
//  main.m
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Classes/RhoConnectEngine.h"

int main(int argc, char *argv[]) {

    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	[RhoConnectEngine create];// init RhoConnectEngine singleton

    int retVal = UIApplicationMain(argc, argv, nil, nil);

    [pool drain];
    return retVal;
}
