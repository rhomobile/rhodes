
#import "RhoRubyObjectImpl.h"



#import "../api/RhoRubySingletone.h"
#import "RhoRubyImpl.h"

@implementation RhoRubyObjectImpl


-(rho::ruby::IObject*) getCppIObject {
    return mCppObject;
}

- (id)initWithCppObject:(rho::ruby::IObject*)pCppObject {
    mCppObject = pCppObject;
    [self getCppIObject]->addRef();
    return self;
}

- (NSString*) getClassName {
    return [NSString stringWithUTF8String:[self getCppIObject]->getClassName()];
}

- (RHO_RUBY_BASIC_TYPES) getBasicType {
    return [((RhoRubyImpl*)[RhoRubySingletone getRhoRuby]) convertBasicTypeFromCpp:[self getCppIObject]->getBasicType()];
}

- (rho::ruby::IObject*) getCppObject {
    return mCppObject;
}

- (void) dealloc {
    if (mCppObject != NULL) {
        [self getCppIObject]->release();
    }
    mCppObject = NULL;
    [super dealloc];
}

@end







