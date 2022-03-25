
#import <Foundation/Foundation.h>
#import "HTTPServer.h"




@interface CCocoaServer : NSObject {

    HTTPServer *httpServer;
    
}

+ (CCocoaServer*)sharedInstance;


-(id)init;
-(void)start;
-(void)stop;



-(void) createServer;

@end


