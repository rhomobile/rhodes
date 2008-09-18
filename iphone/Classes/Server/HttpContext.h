
#ifndef __HTTPCONTEXT__
#define __HTTPCONTEXT__

#include <CoreFoundation/CoreFoundation.h>


#if defined(__cplusplus)
extern "C" {
#endif

/*
 * For parsing. This guy represents a substring.
 */
struct vec {
    const char *ptr;
    int         len;
};

/*
 * This thing is aimed to hold values of any type.
 * Used to store parsed headers' values.
 */
typedef unsigned long big_int_t;	/* Type for Content-Length	*/
union variant {
    char		*v_str;
    int         v_int;
    big_int_t	v_big_int;
    time_t		v_time;
    void		(*v_func)(void);
    void		*v_void;
    struct vec	v_vec;
};

struct http_header {
    int         len;		/* Header name length	*/
    int         type;		/* Header type			*/
    size_t		offset;		/* Value placeholder	*/
    const char	*name;		/* Header name			*/
};

/*
 * This guy holds parsed HTTP headers
 */
struct headers {
    union variant	cl;		/* Content-Length:		*/
    union variant	ct;		/* Content-Type:		*/
    union variant	connection;	/* Connection:			*/
    union variant	ims;		/* If-Modified-Since:		*/
    union variant	user;		/* Remote user name		*/
    union variant	auth;		/* Authorization		*/
    union variant	useragent;	/* User-Agent:			*/
    union variant	referer;	/* Referer:			*/
    union variant	cookie;		/* Cookie:			*/
    union variant	location;	/* Location:			*/
    union variant	range;		/* Range:			*/
    union variant	status;		/* Status:			*/
    union variant	transenc;	/* Transfer-Encoding:		*/
};
    
    
#define _FLAG_HEADERS_PARSED 0x01
struct __HttpRequest {
    int                 _flags;
	unsigned long       _major_version;	/* Major HTTP version number    */
	unsigned long       _minor_version;	/* Minor HTTP version number    */
    int                 _method;
    char*               _uri;
    char*               _headers;
    struct headers      _cheaders;		/* Parsed client headers	*/;
};
typedef struct __HttpRequest* HttpRequestRef;
    
struct __HttpContext {
    CFAllocatorRef		_alloc;			// Allocator used to allocate this
    UInt32				_retainCount;	// Number of times retained.
        
    CFRunLoopTimerRef	_timer;			// Timer for controlling timeouts
        
    CFReadStreamRef		_inStream;		// Incoming data stream
    CFWriteStreamRef	_outStream;		// Outgoing data stream
        
    CFMutableDataRef	_rcvdBytes;		// Buffer of received bytes
    CFMutableDataRef	_sendBytes;		// Buffer of bytes to send
    
    HttpRequestRef      _request;
};
typedef struct __HttpContext* HttpContextRef;

void HttpSendErrorToTheServer(HttpContextRef context, int status, const char *reason);
    
HttpContextRef HttpContextCreate(CFAllocatorRef alloc, CFSocketNativeHandle nativeSocket);

HttpContextRef HttpContextRetain(HttpContextRef context);
    
void HttpContextRelease(HttpContextRef context);

Boolean HttpContextOpen(HttpContextRef context);

void HttpContextClose(HttpContextRef context);


#if defined(__cplusplus)
}
#endif

#endif	/* __HTTPCONTEXT__ */
