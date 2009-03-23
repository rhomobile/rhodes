
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

enum {HDR_DATE, HDR_INT, HDR_STRING};	/* HTTP header types */
struct http_header {
    int         len;		/* Header name length	*/
    int         type;		/* Header type			*/
    size_t		offset;		/* Value placeholder	*/
    const char	*name;		/* Header name			*/
};

struct parsed_header {
	const char* _name;
	int         _type;
	union  variant _v;
};

/*
 * This guy holds parsed HTTP headers
 */
struct headers {
    struct parsed_header	cl;		/* Content-Length:		*/
    struct parsed_header	ct;		/* Content-Type:		*/
    struct parsed_header	connection;	/* Connection:			*/
    struct parsed_header	ims;		/* If-Modified-Since:		*/
    struct parsed_header	user;		/* Remote user name		*/
    struct parsed_header	auth;		/* Authorization		*/
    struct parsed_header	useragent;	/* User-Agent:			*/
    struct parsed_header	referer;	/* Referer:			*/
    struct parsed_header	cookie;		/* Cookie:			*/
    struct parsed_header	location;	/* Location:			*/
    struct parsed_header	range;		/* Range:			*/
    struct parsed_header	status;		/* Status:			*/
    struct parsed_header	transenc;	/* Transfer-Encoding:		*/
	struct parsed_header	accept;		/* Accept */
	struct parsed_header	accept_encoding; /* Accept-Encoding */
	struct parsed_header	accept_language; /* Accept-Language */
	struct parsed_header	host;		/* Host */
	struct parsed_header	x_requested_with;	/* X-Requested-With */
};
    
    
#define _FLAG_HEADERS_PARSED 0x01
struct __HttpRequest {
    int                 _flags;
	unsigned long       _major_version;	/* Major HTTP version number */
	unsigned long       _minor_version;	/* Minor HTTP version number */
    int                 _method;
    char*               _uri;
	char*				_query;
    char*               _headers;
	struct vec			_mime_type;		/* Mime type */
    struct headers      _cheaders;		/* Parsed client headers */;
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
	
const char* HttpGetSiteRoot();	

HttpContextRef HttpContextCreate(CFAllocatorRef alloc, CFSocketNativeHandle nativeSocket);

HttpContextRef HttpContextRetain(HttpContextRef context);
    
void HttpContextRelease(HttpContextRef context);

Boolean HttpContextOpen(HttpContextRef context);

void HttpContextClose(HttpContextRef context);

void _dbg_print_data(UInt8* buffer, CFIndex size);
	
#if defined(__cplusplus)
}
#endif

#endif	/* __HTTPCONTEXT__ */
