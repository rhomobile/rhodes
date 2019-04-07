
#import "../api/RhoRubySingletone.h"
#import "RhoRubyImpl.h"


static id<IRhoRuby> ourIRhoRuby = nil;

@implementation RhoRubySingletone

+(id<IRhoRuby>) getRhoRuby {
    if (ourIRhoRuby == nil) {
        ourIRhoRuby = [[RhoRubyImpl alloc] init];
    }
    return ourIRhoRuby;
}

@end

