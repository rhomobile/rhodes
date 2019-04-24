
#import "RhoRubyMutableStringImpl.h"
#import "../api/RhoRubySingletone.h"
#import "RhoRubyImpl.h"

#include "../../../shared/rhoruby/api/RhoRuby.h"


@implementation RhoRubyMutableStringImpl

-(rho::ruby::IString*) getCppIString {
    return (rho::ruby::IString*)[self getCppObject];
}

-(rho::ruby::IMutableString*) getCppIMutableString {
    return (rho::ruby::IMutableString*)[self getCppObject];
}


- (NSString*) getClassName {
    return @"String";}

-(NSString*) getString {
    return [NSString stringWithUTF8String:[self getCppIString]->getUTF8()];
}

-(void) setString:(NSString*)value {
    [self getCppIMutableString]->setUTF8([value UTF8String]);
}


@end



