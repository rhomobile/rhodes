
#ifndef __SERVERCONTEXT__
#define __SERVERCONTEXT__

#include <CoreFoundation/CoreFoundation.h>


#if defined(__cplusplus)
extern "C" {
#endif


typedef struct __Server* ServerRef;

typedef struct {
    void *info;
    void *(*retain)(void *info);
    void (*release)(void *info);
    CFStringRef (*copyDescription)(void *info);
} ServerContext;
typedef void (*ServerCallBack)(ServerRef server, CFSocketNativeHandle sock, CFStreamError* error, void* info);


/*
** ServerCreate
**
** Create a new instance of a server object.  This will allocate
** only the underlying structures and set aside the resources.
** Use ServerConnect to actually start the server listening.
**
** alloc	Allocator to use for allocating.  NULL indicates
**			the default allocator.
**
** callback	Function to call as incoming connections are accepted
**
** context	Reference to a context block which will be copied into
**			the server context for the callbacks.
*/
ServerRef ServerCreate(CFAllocatorRef alloc, ServerCallBack callback, ServerContext* context);


/*
** ServerRetain
**
** Increment the retain count on the given server reference.
** This is comparable to CFRetain on a CFTypeRef.  It only
** works on server references.
**
** server Reference to the server.  Must be non-NULL.
*/
ServerRef ServerRetain(ServerRef server);


/*
** ServerRelease
**
** Decrements the retain count on the give server reference.
** If all retained references to this object are released, the
** object with be destroyed.  This is comparable to CFRelease
** on a CFTypeRef.  This only works on server references.
**
** server Reference to the server.  Must be non-NULL.
*/
void ServerRelease(ServerRef server);


/*
** ServerShow
**
** Debug utility for examining a server reference.  Comparable to
** CFShow on a CFTypeRef.  It only works on server references.
**
** server Reference to the server.  Must be non-NULL.
*/
void ServerShow(ServerRef server);


/*
** ServerConnect
**
** Starts the socket listening on the given port.  Registers
** the service by name and type on the local network.  The
** socket and service will be registered on the current
** run loop in the common modes.
**
** server Reference to the server.  Must be non-NULL.
**
** name		Unique name to register on the network.  NULL indicates
**			to use the computer's name.  The name must be unique in
**			the local domain and for the given service type.
**
** type 	Service type being registered.  Service types can be
**			retrieved from IANA (www.iana.org).  Examples include
**			_http._tcp. and _echo._tcp.
**
** port		TCP port on which to listen.  If a well-known port is
**			not required, set to zero and one will be assigned.
*/
Boolean ServerConnect(ServerRef server, CFStringRef name, CFStringRef type, UInt32 port);


/*
** ServerInvalidate
**
** Removes the client and its associated data from the server reference.
** This ensures that the client will no longer get callbacks associated
** with this instance of the object.
**
** server Reference to the server.  Must be non-NULL.
*/
void ServerInvalidate(ServerRef server);


#if defined(__cplusplus)
}
#endif

#endif	/* __SERVERCONTEXT__ */
