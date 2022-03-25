
#import "CCocoaServer.h"
#import "RhoHTTPConnection.h"


#import "DDLog.h"
#import "DDTTYLogger.h"


extern int rho_http_get_port();
extern const char* rho_http_direct_request( const char* method, const char* uri, const char* query, const void* headers, const char* body, int bodylen, int* responseLength );
extern void rho_http_free_response( const char* data );

extern void* rho_http_init_headers_list();
extern void rho_http_add_header( void* list, const char* name, const char* value );
extern void rho_http_free_headers_list( void* list );

static const int ddLogLevel = LOG_LEVEL_VERBOSE;


static CCocoaServer* ourSharedInstance = nil;

@implementation CCocoaServer

+ (CCocoaServer*)sharedInstance {
    if (ourSharedInstance == nil) {
        ourSharedInstance = [[CCocoaServer alloc] init];
    }
    return ourSharedInstance;
}

-(id)init {
    [super init];
    httpServer = nil;
    return self;
}

-(void) createServer {
    NSLog(@"$$$$$ HTTP SERVER CREATE()");

    [DDLog addLogger:[DDTTYLogger sharedInstance]];

    httpServer = [[HTTPServer alloc] init];
    [httpServer setType:@"_http._tcp."];
    [httpServer setPort:rho_http_get_port()]; // setup port
    [httpServer setInterface:@"localhost"]; // accept only inner connections
    [httpServer setConnectionClass:[RhoHTTPConnection class]]; // replace connection for replace responce functionlity
}

-(void)start{
    
    if (httpServer == nil) {
        NSLog(@"$$$$$ HTTP SERVER START() - NO SERVER !");
        return;
    }
    NSLog(@"$$$$$ HTTP SERVER START()");

    NSError *error;
    
    if (![httpServer isRunning]) {
        if([httpServer start:&error])
        {
            NSLog(@"Started HTTP Server on port %hu", [httpServer listeningPort]);
        }
        else
        {
            NSLog(@"Error starting HTTP Server: %@", [error localizedDescription]);
        }
    }
}



-(void)stop {
    if (httpServer == nil) {
        NSLog(@"$$$$$ HTTP SERVER STOP() - NO SERVER !");

        return;
    }
    NSLog(@"$$$$$ HTTP SERVER STOP()");

    if ([httpServer isRunning]) {
        [httpServer stop];
    }
}


@end




void rho_cocoa_server_init() {
    CCocoaServer* server = [CCocoaServer sharedInstance];
    [server createServer];
    [server start];
}

void rho_cocoa_server_stop() {
    CCocoaServer* server = [CCocoaServer sharedInstance];
    [server stop];
}

void rho_cocoa_server_start() {
    CCocoaServer* server = [CCocoaServer sharedInstance];
    [server start];
}
