
#import "INavbar.h"
#import "NavbarFactoryBase.h"

static NavbarFactoryBase* ourNavbarFactory = nil;

@implementation NavbarFactorySingleton

+(id<INavbarFactory>) getNavbarFactoryInstance {
    if (ourNavbarFactory == nil) {
        ourNavbarFactory = [[NavbarFactoryBase alloc] init];
    }
    return ourNavbarFactory;
}

@end