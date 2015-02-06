
#import "Camera.h"
#import "api_generator/iphone/CMethodResult.h"


void camera_take_picture(NSDictionary* options, id<IMethodResult> callback_api);


@implementation Camera


-(NSDictionary*) constructProperties:(NSDictionary*)options {
    CMethodResult_SimpleHolder* holder = [CMethodResult_SimpleHolder makeEmptyHolder];
    [self getAllProperties:holder];
    NSMutableDictionary* props = [NSMutableDictionary dictionaryWithCapacity:16];
    [props addEntriesFromDictionary:(NSDictionary*)[holder getResult]];
    [props addEntriesFromDictionary:[CameraBase applyAliasesToDictionary:[Camera convertValuesToString:options]]];
    return props;
}

-(void) takePicture:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    camera_take_picture([self constructProperties:propertyMap], methodResult);
}

+(NSDictionary*)convertValuesToString:(NSDictionary*)dict {
    if ((dict == nil) || ([dict isKindOfClass:[NSNull class]])) {
        return [NSMutableDictionary dictionaryWithCapacity:1];
    }
    NSMutableDictionary* res_dict = [NSMutableDictionary dictionaryWithCapacity:[dict count]];
    NSEnumerator* enumerator = [dict keyEnumerator];
    NSObject* obj = nil;
    while ((obj = [enumerator nextObject]) != nil) {
        NSObject* objKey = obj;
        NSObject* objValue = [dict objectForKey:objKey];
        NSString* str = @"";

        if ([objValue isKindOfClass:[NSString class]]) {
            // string
            NSString* objString = (NSString*)objValue;
            str = [objString copy];
        }
        else if ([objValue isKindOfClass:[NSNumber class]]) {
            // int, bool or float
            NSNumber* objNumber = (NSNumber*)objValue;
            if ([CMethodResult isBoolInsideNumber:objNumber]) {
                str = [objNumber boolValue]?@"true":@"false";
            }
            else if ([CMethodResult isFloatInsideNumber:objNumber]) {
                str = [NSString stringWithFormat:@"%@", objNumber];
            }
            else {
                str = [NSString stringWithFormat:@"%@", objNumber];
            }
        }
        if ([str length] > 0) {
            [res_dict setObject:str forKey:objKey];
        }
    }
    return res_dict;
}

-(void) getSupportedSizeList:(id<IMethodResult>)methodResult {
    // unsupported on iOS
}


/* Shows the preview in user configurable viewer window. */
-(void) showPreview:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    // unsupported on iOS
}

/* Hides the preview opened by using showPreview. */
-(void) hidePreview:(id<IMethodResult>)methodResult {
    // unsupported on iOS
}

/* Capture the image and save it to a file. */
-(void) capture:(id<IMethodResult>)methodResult {
    // unsupported on iOS
}



@end