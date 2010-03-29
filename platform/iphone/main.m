//
//  main.m
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#include <signal.h>

#import <UIKit/UIKit.h>
#import "ServerHost.h"
#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "main"

int main(int argc, char *argv[]) {
    
    signal(SIGPIPE, SIG_IGN);
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"Rhodes");
    [pool release];

    RAWLOG_INFO("Exiting the Runner");
    return retVal;
}
