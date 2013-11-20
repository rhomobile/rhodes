
#include <signal.h>

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    signal(SIGPIPE, SIG_IGN);
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"CApplicationDelegate");
    [pool release];

    return retVal;
}
