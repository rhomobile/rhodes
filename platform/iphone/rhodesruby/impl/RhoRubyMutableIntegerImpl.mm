
#import "RhoRubyMutableIntegerImpl.h"
#import "../api/RhoRubySingletone.h"
#import "RhoRubyImpl.h"

#include "../../../shared/rhoruby/api/RhoRuby.h"


@implementation RhoRubyMutableIntegerImpl

-(rho::ruby::IInteger*) getCppIInteger {
    return (rho::ruby::IInteger*)[self getCppObject];
}

-(rho::ruby::IMutableInteger*) getCppIMutableInteger {
    return (rho::ruby::IMutableInteger*)[self getCppObject];
}


- (NSString*) getClassName {
    return @"Fixnum";}

-(long int) getLong {
    return [self getCppIInteger]->getLong();
}

-(void) setLong:(long int)value {
    [self getCppIMutableInteger]->setLong(value);
}


@end



