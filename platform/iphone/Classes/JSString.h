#import <Foundation/Foundation.h>

@interface JSString : NSObject {
	NSString *inputJs;
	NSString *outputJs;
}

@property(copy) NSString *inputJs;
@property(copy) NSString *outputJs;	
@end
