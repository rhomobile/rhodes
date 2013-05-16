/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/


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

// Should remove all hashes starting with vowels and reverse value characters
-(void) testHash:(NSDictionary*)value methodResult:(id<IMethodResult>)methodResult {
    NSCharacterSet *set = [NSCharacterSet characterSetWithCharactersInString:@"aeiouy"];
    NSEnumerator *enumerator = [value keyEnumerator];
    NSMutableDictionary *result = [NSMutableDictionary new];
    
    for(NSString *aKey in enumerator) {
        if ([[aKey lowercaseString] rangeOfCharacterFromSet:set].location != 0)
        {
            NSString *val = [value valueForKey:aKey];
            
            NSMutableString *reversedString = [NSMutableString string];
            NSInteger charIndex = [val length];
            while (charIndex > 0) {
                charIndex--;
                NSRange subStrRange = NSMakeRange(charIndex, 1);
                [reversedString appendString:[val substringWithRange:subStrRange]];
            }
            
            [result setObject:reversedString forKey:aKey];
        }
    }

    [methodResult setResult:(result)];
}

-(void) testNull:(id<IMethodResult>)methodResult {
   [methodResult setResult:([NSNull null])];
}

@end