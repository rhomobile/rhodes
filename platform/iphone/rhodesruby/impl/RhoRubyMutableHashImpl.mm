
#import "RhoRubyMutableHashImpl.h"
#import "../api/RhoRubySingletone.h"
#import "RhoRubyImpl.h"

#include "../../../shared/rhoruby/api/RhoRuby.h"


@implementation RhoRubyMutableHashImpl

-(rho::ruby::IHash*) getCppIHash {
    return (rho::ruby::IHash*)[self getCppObject];
}

-(rho::ruby::IMutableHash*) getCppIMutableHash {
    return (rho::ruby::IMutableHash*)[self getCppObject];
}


- (NSString*) getClassName {
    return @"Hash";
}

// return array of IRhoRubyString
-(id<IRhoRubyArray>) getKeys {
    rho::ruby::IArray* cpp_obj = [self getCppIHash]->getKeys();
    RhoRubyImpl* rr = (RhoRubyImpl*)[RhoRubySingletone getRhoRuby];
    return (id<IRhoRubyArray>)[rr makeRhoRubyObjectFromCpp:cpp_obj];
}

-(id<IRhoRubyObject>) getItemByKey:(NSString*)key {
    rho::ruby::IObject* cpp_obj = [self getCppIHash]->getItem([key UTF8String]);
    RhoRubyImpl* rr = (RhoRubyImpl*)[RhoRubySingletone getRhoRuby];
    return [rr makeRhoRubyObjectFromCpp:cpp_obj];
}

-(void)addItemWithKey:(NSString*)key item:(id<IRhoRubyObject>)item {
    RhoRubyObjectImpl* rr_obj = (RhoRubyObjectImpl*)item;
    rho::ruby::IObject* cpp_obj = [rr_obj getCppObject];
    [self getCppIMutableHash]->addItem([key UTF8String], cpp_obj);
}




@end


