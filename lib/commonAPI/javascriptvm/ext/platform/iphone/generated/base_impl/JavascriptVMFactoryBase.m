
#import "JavascriptVMFactoryBase.h"
#import "JavascriptVM.h"
#import "JavascriptVMSingleton.h"

@implementation JavascriptVMFactoryBase

-(id)init {
    self = [super init];
    mJavascriptVMInstances = [NSMutableDictionary dictionary];
    mJavascriptVMSingleton = nil;
    return self;
}

-(id<IJavascriptVM>) getJavascriptVMByID:(NSString*)ID {
    JavascriptVM* obj = [mJavascriptVMInstances objectForKey:ID];
    if (obj == nil) {
        obj = [self createJavascriptVMByID:ID];
        [mJavascriptVMInstances setObject:obj forKey:ID];
    }
    return obj;
}

-(void) destroyObjectByID:(NSString*)ID {
    JavascriptVM* obj = [mJavascriptVMInstances objectForKey:ID];
    if (obj != nil) {
        [mJavascriptVMInstances removeObjectForKey:ID];
    }
}


-(id<IJavascriptVM>)createJavascriptVMByID:(NSString*)ID {
    JavascriptVM* obj = [[JavascriptVM alloc] init];
    [obj setProperty:@"ID" propertyValue:ID];
    return obj;
}


-(id<IJavascriptVMSingleton>) getJavascriptVMSingleton {
    if (mJavascriptVMSingleton == nil) {
        mJavascriptVMSingleton = [[JavascriptVMSingleton alloc] init];
    }
    return mJavascriptVMSingleton;
}

-(void)dealloc {
    [mJavascriptVMInstances release];
    [super dealloc];
}


@end