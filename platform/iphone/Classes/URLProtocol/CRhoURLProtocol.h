
#import <Foundation/Foundation.h>



@interface CRhoURLProtocol : NSURLProtocol {}


+ (void) initAndRegister;

- (void)sendResponseWithResponseCode:(NSInteger)statusCode data:(NSData*)data;

@end
