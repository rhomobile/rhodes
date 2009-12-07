#import <Foundation/Foundation.h>

@interface JSString : NSObject {
	NSString *inputJs;
	NSString *outputJs;
@public
    int index;
}

@property(copy) NSString *inputJs;
@property(copy) NSString *outputJs;	
@end
