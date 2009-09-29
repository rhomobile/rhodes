#import <Foundation/Foundation.h>


@interface WebViewUrl : NSObject {
@private
	NSString* url;
	int webViewIndex;	
}

@property(copy) NSString* url;
@property(assign) int webViewIndex;	

@end
