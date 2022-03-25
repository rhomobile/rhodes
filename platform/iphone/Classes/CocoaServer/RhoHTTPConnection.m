#import "RhoHTTPConnection.h"
#import "GCDAsyncSocket.h"
#import "HTTPServer.h"
#import "HTTPMessage.h"
#import "HTTPResponse.h"
#import "HTTPAuthenticationRequest.h"
#import "DDNumber.h"
#import "DDRange.h"
#import "DDData.h"
#import "HTTPFileResponse.h"
#import "HTTPAsyncFileResponse.h"
#import "WebSocket.h"
#import "HTTPLogging.h"


extern const char* rho_http_direct_request( const char* method, const char* uri, const char* query, const void* headers, const char* body, int bodylen, int* responseLength );
extern void rho_http_free_response( const char* data );

extern void* rho_http_init_headers_list();
extern void rho_http_add_header( void* list, const char* name, const char* value );
extern void rho_http_free_headers_list( void* list );

#define HTTP_RESPONSE                      90
#define HTTP_FINAL_RESPONSE                91
#define TIMEOUT_WRITE_BODY                   -1

static const int ddLogLevel = LOG_LEVEL_VERBOSE;

@implementation RhoHTTPConnection


- (BOOL)supportsMethod:(NSString *)method atPath:(NSString *)path
{

    if ([method isEqualToString:@"GET"])
        return YES;
    
    if ([method isEqualToString:@"POST"])
        return YES;
    
    if ([method isEqualToString:@"PUT"])
        return YES;

    if ([method isEqualToString:@"HEAD"])
        return YES;
        
    return NO;
}

/*
 - (BOOL)expectsRequestBodyFromMethod:(NSString *)method atPath:(NSString *)path {
    return YES;
}
*/

- (void)processBodyData:(NSData *)postDataChunk {
    // only up to 256 Kb !!! - if larger we must accumulate data !
    [request setBody:postDataChunk];
}

- (void)replyToHTTPRequest {
    
    NSString *requestUri = [self requestURI];
    
    NSURL* nsurl = [NSURL URLWithString:requestUri];
    NSString *requestMethod = [request method];
    NSDictionary* requestHeaders = [request allHeaderFields];
    NSData* requestBody = [request body];
    
    
    
    
    
    const char* uri = [[nsurl path] UTF8String];
    const char* method = [requestMethod UTF8String];
    const char* body = [requestBody bytes];
    int bodylen = (int)requestBody.length;
    const char* query = [[nsurl query] UTF8String];
    
    void* cHeaders = rho_http_init_headers_list();
    
    for (NSString* key in requestHeaders) {
      NSString* value = [requestHeaders objectForKey:key];
      
      rho_http_add_header(cHeaders, [key UTF8String], [value UTF8String]);
      
    }
    
    int len = 0;
    
    //if ([requestUri isEqualToString:@"/system/js_api_entrypoint"]) {
    //    int o= 9;
    //    o = 0;
    //}
    
    
    
    const char* response = rho_http_direct_request(method, uri, query, cHeaders, body, bodylen, &len);
    
    rho_http_free_headers_list(cHeaders);
    
    //CRhoURLResponse* resp = nil;
    
    if ( response != 0 ) {
      
      /*
       // parse http responce for prepare detailed responce instead of raw data
       // not used currently - just send raw data
       
      self.httpStatusCode = 0;
      self.httpBody = nil;
      self.httpHeaderName = nil;
      self.httpHeaders = nil;
      
      http_parser_settings settings;
      settings.on_header_field = on_http_header;
      settings.on_header_value = on_http_header_value;
      settings.on_body = on_http_body;
      settings.on_status = on_http_status;
      
      settings.on_headers_complete = on_http_cb;
      settings.on_message_begin = on_http_cb;
      settings.on_message_complete = on_http_cb;
      settings.on_url = on_http_data_cb;
      http_parser *parser = malloc(sizeof(http_parser));
      parser->data = self;
      http_parser_init(parser, HTTP_RESPONSE);
      http_parser_execute(parser, &settings, response, len);
      
      NSString* strHttpVer = [NSString stringWithFormat:@"%d.%d",parser->http_major,parser->http_minor];
      
      self.httpStatusCode = parser->status_code;
      
      free(parser);
       */
        
        NSData* responseData = [[NSData dataWithBytes:response length:len] copy];
        rho_http_free_response(response);
        
        char a[1];
        a[0] = 0;
        NSData* zz = [NSData dataWithBytes:a length:1];

        [asyncSocket writeData:responseData withTimeout:TIMEOUT_WRITE_BODY tag:HTTP_FINAL_RESPONSE];

        [self finishResponse];
    }
    else {
        [self handleResourceNotFound];
    }
    
    // totally replace responce functionality
    //[super replyToHTTPRequest];
}


@end
