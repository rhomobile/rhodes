
#import "../api/IRhoRubyObject.h"

#include "../../../shared/rhoruby/api/RhoRuby.h"

@interface RhoRubyObjectImpl : NSObject<IRhoRubyObject> {
    rho::ruby::IObject* mCppObject;
}

- (id)initWithCppObject:(rho::ruby::IObject*)pCppObject;

- (NSString*) getClassName;

- (RHO_RUBY_BASIC_TYPES) getBasicType;

- (rho::ruby::IObject*) getCppObject;

- (void) dealloc;

@end











