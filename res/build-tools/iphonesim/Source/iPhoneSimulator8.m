

#import "iPhoneSimulator.h"

int iossim_main(int argc, char* const argv[]);



@implementation iPhoneSimulator



- (void) runWithArgc: (int) argc argv: (char **) argv {
    iossim_main(argc, argv);
}

@end
