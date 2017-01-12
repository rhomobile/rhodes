
#import <Foundation/Foundation.h>



@interface CRhoURLProtocol : NSURLProtocol {}


+ (void) initAndRegister;

- (void)sendResponseWithResponseCode:(NSInteger)statusCode data:(NSData*)data;

@property (nonatomic) NSInteger httpStatusCode;
@property (retain) NSString* httpHeaderName;
@property (retain) NSMutableDictionary* httpHeaders;
@property (retain) NSData* httpBody;

@property (nonatomic) BOOL isStopped;

@end
