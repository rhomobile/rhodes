
#import <Foundation/Foundation.h>

@protocol IMethodResult <NSObject>

- (void) setResult:(NSObject*)value;

@end


@interface CMethodResult_SimpleHolder : NSObject <IMethodResult> {
    NSObject* resultObject;
}

-(id) init;

+(CMethodResult_SimpleHolder*) makeEmptyHolder;

- (void) setResult:(NSObject*)value;
- (NSObject*) getResult;


@end