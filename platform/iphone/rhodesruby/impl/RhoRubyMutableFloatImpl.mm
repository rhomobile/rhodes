
#import "RhoRubyMutableFloatImpl.h"
#import "../api/RhoRubySingletone.h"
#import "RhoRubyImpl.h"

#include "../../../shared/rhoruby/api/RhoRuby.h"


@implementation RhoRubyMutableFloatImpl

-(rho::ruby::IFloat*) getCppIFloat {
    return (rho::ruby::IFloat*)[self getCppObject];
}

-(rho::ruby::IMutableFloat*) getCppIMutableFloat {
    return (rho::ruby::IMutableFloat*)[self getCppObject];
}


- (NSString*) getClassName {
    return @"Float";}

-(double) getDouble {
    return [self getCppIFloat]->getDouble();
}

-(void) setDouble:(double)value {
    [self getCppIMutableFloat]->setDouble(value);
}


@end



