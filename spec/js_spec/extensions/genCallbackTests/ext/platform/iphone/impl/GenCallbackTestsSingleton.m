
#import "GenCallbackTestsSingleton.h"


@implementation GenCallbackTestsSingleton


-(NSString*)getInitialDefaultID {

}

// Should reverse string
-(void) testString:(NSString*)value methodResult:(id<IMethodResult>)methodResult {
    NSMutableString *reversedString = [NSMutableString string];
    NSInteger charIndex = [value length];
    while (charIndex > 0) {
        charIndex--;
        NSRange subStrRange = NSMakeRange(charIndex, 1);
        [reversedString appendString:[value substringWithRange:subStrRange]];
    }
    [methodResult setResult:reversedString];
}

// Should negate boolean value
-(void) testBool:(BOOL)value methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:(!value)]];
}

// Should increment integer by one
-(void) testInt:(int)value methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:(value + 1)]];
}

-(void) testFloat:(float)value methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithFloat:(value - 0.5)]];
}

// Should sort strings in alphabetical order
-(void) testArray:(NSArray*)value methodResult:(id<IMethodResult>)methodResult {
    NSArray *sorted = [value sortedArrayUsingSelector:@selector(localizedCaseInsensitiveCompare:)];
    [methodResult setResult:(sorted)];
}

-(void) testHash:(NSDictionary*)value methodResult:(id<IMethodResult>)methodResult {
    NSCharacterSet *set = [NSCharacterSet characterSetWithCharactersInString:@"aeiouy"];
    NSEnumerator *enumerator = [value keyEnumerator];
    NSMutableDictionary *result = [NSMutableDictionary new];
    
    for(NSString *aKey in enumerator) {
        if ([[aKey lowercaseString] rangeOfCharacterFromSet:set].location != 0)
        {
            [result setObject:[value valueForKey:aKey] forKey:aKey];
        }
    }

    [methodResult setResult:(result)];
}

-(void) testNull:(id<IMethodResult>)methodResult {
   [methodResult setResult:([NSNull null])];
}

@end