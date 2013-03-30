
#import "Megamodule.h"
#import <UIKit/UIKit.h>

@implementation Megamodule

-(id)init {
    self = [super init];
    
    intProp = 0;
    
    
    return self;
}


/*
-(void) getIntegerProperty:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:intProp]];
}
     
-(void) setIntegerProperty:(int)value {
    intProp = value;
}
 */
     
-(void) typesTest:(NSString*)paramStr paramBool:(BOOL)paramBool paramInt:(int)paramInt paramFloat:(float)paramFloat paramArray:(NSArray*)paramArray paramHash:(NSDictionary*)paramHash methodResult:(id<IMethodResult>)methodResult {
    
    BOOL checked = YES;
    
    //(paramStr == "ABC") AND (paramBool == true) AND (paramInt == 555123) AND (paramFloat == 3.14) AND (paramArray == [1,2,3]) AND (paramHash == {:p1 => "abc", :p2 => "qwe"}
    
    if ([@"ABC" compare:paramStr] != NSOrderedSame) {
        checked = NO;
    }
    
    if (paramBool != YES) {
        checked = NO;
    }
    
    if (paramInt != 555123) {
        checked = NO;
    }
    
    if (paramFloat != 3.14f) {
        checked = NO;
    }
    
    if ([paramArray count] != 3) {
        checked = NO;
    }
    else {
        if (!([[paramArray objectAtIndex:0] isKindOfClass:[NSNumber class]]) ||
            !([[paramArray objectAtIndex:1] isKindOfClass:[NSNumber class]]) ||
            !([[paramArray objectAtIndex:2] isKindOfClass:[NSNumber class]]) ) {
            checked = NO;
        }
        else {
            if (( [((NSNumber*)[paramArray objectAtIndex:0]) intValue] != 1 ) ||
                ( [((NSNumber*)[paramArray objectAtIndex:1]) intValue] != 2 ) ||
                ( [((NSNumber*)[paramArray objectAtIndex:2]) intValue] != 3 ) ) {
                checked = NO;
            }
        }
    }
    
    NSArray* keys = [paramHash allKeys];
    if ([keys count] != 2) {
        checked = NO;
    }
    else {
        NSObject* value1 = [paramHash objectForKey:@"p1"];
        NSObject* value2 = [paramHash objectForKey:@"p2"];
        if (!([value1 isKindOfClass:[NSString class]]) || !([value2 isKindOfClass:[NSString class]])) {
            checked = NO;
        }
        else {
            if (([@"abc" compare:(NSString*)value1] != NSOrderedSame) ||
                ([@"qwe" compare:(NSString*)value2] != NSOrderedSame)) {
                checked = NO;
            }
        }
    }
    [methodResult setResult:[NSNumber numberWithBool:checked]];
}
     
-(void) produceArray:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:10];
    int i;
    for (i = 1; i <= 10; i++) {
        [ar addObject:[NSNumber numberWithInt:i]];
    }
    [methodResult setResult:ar];
}

-(void) produceHash:(id<IMethodResult>)methodResult {
   // { :parame1 => "value1", :parama2 = > 55 }
    NSMutableDictionary* hash = [NSMutableDictionary dictionaryWithCapacity:2];
    [hash setObject:@"value1" forKey:@"parame1"];
    [hash setObject:[NSNumber numberWithInt:55] forKey:@"parama2"];
    [methodResult setResult:hash];
}

-(void) produceComplicatedResult:(id<IMethodResult>)methodResult {
    //[{ :parame1 => "value1", :parama2 = > 55 }, { :paramu3 => [1,2,3] } ]
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:10];
    
    // 1
    NSMutableDictionary* hash1 = [NSMutableDictionary dictionaryWithCapacity:2];
    [hash1 setObject:@"value1" forKey:@"parame1"];
    [hash1 setObject:[NSNumber numberWithInt:55] forKey:@"parama2"];
    [ar addObject:hash1];

    //2
    NSMutableDictionary* hash2 = [NSMutableDictionary dictionaryWithCapacity:2];
    NSMutableArray* array2 = [NSMutableArray arrayWithCapacity:1];
    [array2 addObject:[NSNumber numberWithInt:1]];
    [array2 addObject:[NSNumber numberWithInt:2]];
    [array2 addObject:[NSNumber numberWithInt:3]];
    [hash2 setObject:array2 forKey:@"paramu3"];
    [ar addObject:hash2];
    
    [methodResult setResult:ar];
}
     
-(void) showAlertFromUIThread:(NSString*)message {
    // this code crashed if run from non UI thread !
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Alert from UI Thread"
                                                    message:message
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles: nil];
    [alert show];
    [alert release];
}
     
-(void) setPeriodicallyCallback:(int)periodInMilliseconds methodResult:(id<IMethodResult>)methodResult {

}
     
-(void) isPeriodicallyCallbackSetted:(id<IMethodResult>)methodResult {

}
     
-(void) stopPeriodicallyCallback {

}

-(void) complicatedTypesTest1:(NSArray*)paramArray methodResult:(id<IMethodResult>)methodResult {

}

- (void)dealloc {
    
    
    [super dealloc];
}

@end
