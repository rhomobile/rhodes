
#import "RhoRubyMutableBooleanImpl.h"
#import "../api/RhoRubySingletone.h"
#import "RhoRubyImpl.h"

#include "../../../shared/rhoruby/api/RhoRuby.h"


@implementation RhoRubyMutableBooleanImpl

-(rho::ruby::IBoolean*) getCppIBoolean {
    return (rho::ruby::IBoolean*)[self getCppObject];
}

-(rho::ruby::IMutableBoolean*) getCppIMutableBoolean {
    return (rho::ruby::IMutableBoolean*)[self getCppObject];
}


- (NSString*) getClassName {
    if ([self getCppIBoolean]->getBool()) {
        return @"TrueClass";
    }
    return @"FalseClass";}

-(BOOL) getBool {
    return ([self getCppIBoolean]->getBool() == true);
}

-(void) setBool:(BOOL)value {
    [self getCppIMutableBoolean]->setBool(value == YES);
}


@end



