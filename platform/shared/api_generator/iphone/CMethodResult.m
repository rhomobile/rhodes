
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
    mRubyModulePath = nil;
    return self;
}

- (void) setResult:(NSObject*)value {
    if (mRubyModulePath != nil) {
        // convert all strings into CRubyModule !
        if ([value isKindOfClass:[NSString class]]) {
            mValue = [CRubyModule rubyModuleByName:mRubyModulePath instanceID:((NSString*)value)];
        }
        else if ([value isKindOfClass:[NSArray class]]) {
            NSArray* value_ar = (NSArray*)value;
            NSMutableArray* aValue = [NSMutableArray arrayWithCapacity:[value_ar count]];
            int i;
            for (i = 0; i < [value_ar count]; i++) {
                NSObject* item = [value_ar objectAtIndex:i];
                if ([item isKindOfClass:[NSString class]]) {
                    item = [CRubyModule rubyModuleByName:mRubyModulePath instanceID:((NSString*)item)];
                }
                [aValue addObject:item];
            }
            mValue = aValue;
        }
        else {
            mValue = [value retain];
        }
    }
    else {
        mValue = [value retain];
    }
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

-(void) enableObjectCreationModeWithRubyClassPath:(NSString*)classPath {
    mRubyModulePath = [classPath stringByReplacingOccurrencesOfString:@"." withString:@":"];
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


+ (BOOL)isBoolInsideNumber:(NSNumber*)number {
    return [number objCType][0] == 'c';
}

+ (BOOL)isFloatInsideNumber:(NSNumber*)number {
    return ([number objCType][0] == 'f') || ([number objCType][0] == 'd');
}

+ (BOOL)isIntInsideNumber:(NSNumber*)number {
    return [number objCType][0] == 'i';
}




-(void) dealloc {
    [mRubyCallbackURL release];
    [mJSCallbackUID release];
    [mCallbackParam release];
    [mValue release];
    [mRubyModulePath release];
    [super dealloc];
}


@end
