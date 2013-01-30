
#import "CMethodResult.h"

#import "CRubyConverter.h"
#import "CJSConverter.h"
#import "RubyCallbackHelper.h"

@implementation CMethodResult

-(id) init {
    self = [super init];
    mValue = nil;
    mRubyCallbackURL = nil;
    mJSCallbackUID = nil;
    mCallbackParam = nil;
    mRubyFactory = nil;
    return self;
}

- (void) setResult:(NSObject*)value {
    mValue = [value retain];
    [self callCallback];
}

-(NSObject*) getResult {
    return mValue;
}


-(VALUE) toRuby {
    if (mRubyFactory != nil) {
        return [mRubyFactory makeRubyValue:mValue];
    }
    return [CRubyConverter convertToRuby:mValue];
}

-(NSString*) toJSON {
    return [CJSConverter convertToJS:mValue];
}


-(void) setRubyCallback:(NSString*)url {
    mRubyCallbackURL = [url retain];
}

-(void) setJSCallback:(NSString*)uid {
    mJSCallbackUID = [uid retain];
}

-(void) setCallbackParam:(NSString*)param {
    mCallbackParam = [param retain];
}

-(void) callRubyCallback:(NSString*)url {
    RubyCallbackHelper_callCallback(mRubyCallbackURL, self, mCallbackParam);
}

-(void) setRubyFactory:(id<IMethodResult_RubyObjectFactory>)factory {
    mRubyFactory = factory;
}


-(void) callJSCallback:(NSString*)uid {
    //TODO:
}

-(void) callCallback {
    if (mRubyCallbackURL != nil) {
        [self callRubyCallback:mRubyCallbackURL];
    }
    else if (mJSCallbackUID != nil) {
        [self callJSCallback:mJSCallbackUID];
    }
}

+ (NSObject*) getObjectiveCNULL {
    return [NSNull null];
}


-(void) dealloc {
    [mRubyCallbackURL release];
    [mJSCallbackUID release];
    [mCallbackParam release];
    [mValue release];
    [super dealloc];
}


@end
