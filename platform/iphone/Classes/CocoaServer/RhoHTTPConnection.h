#import <Foundation/Foundation.h>
#import "HTTPConnection.h"



@interface RhoHTTPConnection : HTTPConnection
{
    UInt64         mBodyLen;
    NSMutableData* mBody;
    NSData* mResponseData;
}

- (void)replyToHTTPRequest;

@end

