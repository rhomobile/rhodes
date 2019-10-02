
#import "RhoRubyMutableArrayImpl.h"
#import "../api/RhoRubySingletone.h"
#import "RhoRubyImpl.h"

#include "../../../shared/rhoruby/api/RhoRuby.h"


@implementation RhoRubyMutableArrayImpl

-(rho::ruby::IArray*) getCppIArray {
    return (rho::ruby::IArray*)[self getCppObject];
}

-(rho::ruby::IMutableArray*) getCppIMutableArray {
    return (rho::ruby::IMutableArray*)[self getCppObject];
}


- (NSString*) getClassName {
    return @"Array";
}

-(int) getItemsCount {
    return [self getCppIArray]->getItemsCount();
}

-(id<IRhoRubyObject>) getItemByIndex:(int)index {
    rho::ruby::IObject* cpp_obj = [self getCppIArray]->getItem(index);
    RhoRubyImpl* rr = (RhoRubyImpl*)[RhoRubySingletone getRhoRuby];
    return [rr makeRhoRubyObjectFromCpp:cpp_obj];
}

-(void)addItem:(id<IRhoRubyObject>)item {
    RhoRubyObjectImpl* rr_obj = (RhoRubyObjectImpl*)item;
    rho::ruby::IObject* cpp_obj = [rr_obj getCppObject];
    [self getCppIMutableArray]->addItem(cpp_obj);
}


@end


