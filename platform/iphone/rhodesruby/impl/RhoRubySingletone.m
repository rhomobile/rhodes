
#import "../api/RhoRubySingletone.h"
#import "RhoRubyFabrique.h"


static id<IRhoRuby> ourIRhoRuby = nil;

@implementation RhoRubySingletone

+(id<IRhoRuby>) getRhoRuby {
    if (ourIRhoRuby == nil) {
        ourIRhoRuby = [RhoRubyFabrique createRhoRuby];
    }
    return ourIRhoRuby;
}

@end


