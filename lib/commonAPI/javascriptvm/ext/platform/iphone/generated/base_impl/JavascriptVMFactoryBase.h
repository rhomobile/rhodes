
#import "IJavascriptVM.h"

@interface JavascriptVMFactoryBase : NSObject<IJavascriptVMFactory> {
    NSMutableDictionary* mJavascriptVMInstances;
    id<IJavascriptVMSingleton> mJavascriptVMSingleton;
}

-(id)init;

-(id<IJavascriptVMSingleton>) getJavascriptVMSingleton;
-(id<IJavascriptVM>) getJavascriptVMByID:(NSString*)ID;

-(void) destroyObjectByID:(NSString*)ID;

-(id<IJavascriptVM>)createJavascriptVMByID:(NSString*)ID;

-(void)dealloc;

@end