//
//  main.m
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ServerHost.h"

int main(int argc, char *argv[]) {

	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    int retVal = UIApplicationMain(argc, argv, nil, nil);
    
	[pool release];

    printf("Exiting the Runner\n");

	return retVal;
}
