
#pragma mark Includes

#if TARGET_IPHONE_SIMULATOR
#import <CoreServices/CoreServices.h>
#else
#import <CFNetwork/CFNetwork.h>
#endif

#include <assert.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "defs.h"
#include "Server.h"

#pragma mark -
#pragma mark Type Declarations

struct __Server {
	CFAllocatorRef		_alloc;			// Allocator used to allocate this
	UInt32				_rc;			// Number of times retained.
	
	CFSocketRef			_sockets[1];	// Server sockets listening for connections
	
	CFStringRef			_name;			// Name that is being registered
	CFStringRef			_type;			// Service type that is being registered
    UInt32				_port;			// Port being serviced

	CFNetServiceRef		_service;		// Registered service on the network

	ServerCallBack		_callback;		// User's callback function
	ServerContext		_ctxt;			// User's context info
};


#pragma mark -
#pragma mark Constant Definitions


#pragma mark -
#pragma mark Static Function Declarations

static CFStringRef _ServerCopyDescription(ServerRef server);
static void _ServerReleaseSocket(ServerRef server);
static void _ServerHandleAccept(ServerRef server, CFSocketNativeHandle nativeSocket);
static void _SocketCallBack(CFSocketRef sock, CFSocketCallBackType type, CFDataRef address, const void *data, ServerRef server);

static Boolean _ServerCreateAndRegisterNetService(ServerRef server);
static void _ServerReleaseNetService(ServerRef server);
static void _ServerHandleNetServiceError(ServerRef server, CFStreamError* error);
static void _NetServiceCallBack(CFNetServiceRef service, CFStreamError* error, ServerRef server);


#pragma mark -
#pragma mark Extern Function Definitions (API)

/* extern */ ServerRef
ServerCreate(CFAllocatorRef alloc, ServerCallBack callback, ServerContext* context) {
    
    ServerRef server = NULL;
	    
	do {
		int yes = 1;
		CFSocketContext socketCtxt = {0,
									  NULL,
									  (const void*(*)(const void*))&ServerRetain,
									  (void(*)(const void*))&ServerRelease,
									  (CFStringRef(*)(const void *))&_ServerCopyDescription};
			
		DBG(("Allocate the buffer for the server.\n"));
		server = CFAllocatorAllocate(alloc, sizeof(server[0]), 0);
		
		// Fail if unable to create the server
		if (server == NULL)
			break;
		
		memset(server, 0, sizeof(server[0]));
		
		DBG(("Save the allocator for deallocating later.\n"));
		server->_alloc = alloc ? CFRetain(alloc) : NULL;
		
        // Bump the retain count.
        ServerRetain((ServerRef)server);
        
		// Make sure the server is saved for the callback.
		socketCtxt.info = server;
		
		DBG(("Create the IPv4 server socket.\n"));
		server->_sockets[0] = CFSocketCreate(alloc,
										 PF_INET,
										 SOCK_STREAM,
										 IPPROTO_TCP,
										 kCFSocketAcceptCallBack,
										 (CFSocketCallBack)&_SocketCallBack,
										 &socketCtxt);
		
		// If the socket couldn't create, bail.
		if (server->_sockets[0] == NULL)
			break;

		/*DBG(("Create the IPv6 server socket.\n"));
		server->_sockets[1] = CFSocketCreate(alloc,
										 PF_INET6,
										 SOCK_STREAM,
										 IPPROTO_TCP,
										 kCFSocketAcceptCallBack,
										 (CFSocketCallBack)&_SocketCallBack,
										 &socketCtxt);
		
		// If the socket couldn't create, bail.
		if (server->_sockets[1] == NULL)
			break;*/
		
		// In order to accomadate stopping and starting the process without closing the socket,
		// set the addr for resuse on the native socket.  This is not required if the port is
		// being supplied by the OS opposed to being specified by the user.
		DBG(("Set socket options as SO_REUSEADDR\n"));
		setsockopt(CFSocketGetNative(server->_sockets[0]), SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes));
		//setsockopt(CFSocketGetNative(server->_sockets[1]), SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes));
        
		DBG(("Save the user's callback in context.\n"));
		server->_callback = callback;
		memcpy(&(server->_ctxt), context, sizeof(server->_ctxt));
		
		DBG(("If there is info and a retain function, retain the info.\n"));
		if (server->_ctxt.info && server->_ctxt.retain)
			server->_ctxt.info = server->_ctxt.retain(server->_ctxt.info);
		
		DBG(("Server created\n"));
		return server;
			
	} while (0);
	
	DBG(("Something failed in ServerCreate, so clean up.\n"));
	if (server) {
		ServerInvalidate((ServerRef)server);
		ServerRelease((ServerRef)server);
	}

    return NULL;
}


/* extern */ ServerRef
ServerRetain(ServerRef s) {

    // Bump the retain count if server is good.
	s->_rc++;
		
	return s;
}


/* extern */ void
ServerRelease(ServerRef s) {
	
	// Decrease the retain count.
	s->_rc--;
	
	// Destroy the object if not being held.
	if (s->_rc == 0) {
		
		// Hold locally so deallocation can happen and then safely release.
		CFAllocatorRef alloc = s->_alloc;
		
		// Invalidate the server which will release the socket and service.
		ServerInvalidate(s);
			
		// Free the memory in use by the server.
		CFAllocatorDeallocate(alloc, s);
		
		// Release the allocator.
		if (alloc)
			CFRelease(alloc);
	}
}


/* extern */ void
ServerShow(ServerRef server) {

	// Get the description
    CFStringRef debug = _ServerCopyDescription(server);
						
	// Print it out
	CFShow(debug);
	
	// Toss it
	CFRelease(debug);
}


/* extern */ Boolean
ServerConnect(ServerRef s, CFStringRef name, CFStringRef type, UInt32 port) {
	
	CFDataRef address = NULL;

    do {
        unsigned i;
        CFRunLoopRef rl = CFRunLoopGetCurrent();
        CFAllocatorRef alloc = s->_alloc;

        struct sockaddr_in addr4;
        //struct sockaddr_in6 addr6;

        // Make sure the port is valid (0 - 65535).
        if ((port & 0xFFFF0000U) != 0)
            break;

        // NULL means to use the machine name.
        if (name == NULL)
            name = CFSTR("");

        for (i = 0; i < (sizeof(s->_sockets) / sizeof(s->_sockets[0])); i++) {

            // Create the run loop source for putting on the run loop.
            CFRunLoopSourceRef src = CFSocketCreateRunLoopSource(alloc, s->_sockets[i], 0);
            if (src == NULL)
                break;

            // Add the run loop source to the current run loop and default mode.
            CFRunLoopAddSource(rl, src, kCFRunLoopCommonModes);
            CFRelease(src);
        }

        memset(&addr4, 0, sizeof(addr4));

        // Put the local port and address into the native address.
        addr4.sin_len = sizeof(addr4);
        addr4.sin_family = AF_INET;
        addr4.sin_port = htons((UInt16)port);
        addr4.sin_addr.s_addr = htonl(INADDR_ANY);

        // Wrap the native address structure for CFSocketCreate.
        address = CFDataCreateWithBytesNoCopy(alloc, (const UInt8*)&addr4, sizeof(addr4), kCFAllocatorNull);

        // If it failed to create the address data, bail.
        if (address == NULL)
            break;

        // Set the local binding which causes the socket to start listening.
        if (CFSocketSetAddress(s->_sockets[0], address) != kCFSocketSuccess)
            break;

        CFRelease(address);

        address = CFSocketCopyAddress(s->_sockets[0]);
        memcpy(&addr4, CFDataGetBytePtr(address), CFDataGetLength(address));

        port = ntohs(addr4.sin_port);

        CFRelease(address);

        /*memset(&addr6, 0, sizeof(addr6));

        // Put the local port and address into the native address.
        addr6.sin6_family = AF_INET6;
        addr6.sin6_port = htons((UInt16)port);
        addr6.sin6_len = sizeof(addr6);
        memcpy(&(addr6.sin6_addr), &in6addr_any, sizeof(addr6.sin6_addr));

        // Wrap the native address structure for CFSocketCreate.
        address = CFDataCreateWithBytesNoCopy(alloc, (const UInt8*)&addr6, sizeof(addr6), kCFAllocatorNull);

        // Set the local binding which causes the socket to start listening.
        if (CFSocketSetAddress(s->_sockets[1], address) != kCFSocketSuccess)
            break;
		*/
        // Save the name, service type and port.
        s->_name = CFRetain(name);
        s->_type = type ? CFRetain(type) : NULL;
        s->_port = port;

        // Attempt to register the service on the network.
        if (type && !_ServerCreateAndRegisterNetService(s))
            break;
        
        // Release this since it's not needed any longer.
        CFRelease(address);

        return TRUE;

    } while (0);
	
	// Handle the error cleanup.
	
	// Release the address data if it was created.
	if (address)
		CFRelease(address);
		
	// Kill the socket if it was created.
	_ServerReleaseSocket(s);
		
	return FALSE;
}


/* extern */ void
ServerInvalidate(ServerRef s) {
	
	// Release the user's context info pointer.
	if (s->_ctxt.info && s->_ctxt.release)
		s->_ctxt.release(s->_ctxt.info);
		
	// Clear out the context, so nothing can be called.
	memset(&(s->_ctxt), 0, sizeof(s->_ctxt));
	
	// Guarantee that there will be no user callback.
	s->_callback = NULL;

    // Release the net service.
    _ServerReleaseNetService(s);
    
    if (s->_name) {
        CFRelease(s->_name);
        s->_name = NULL;
    }

    if (s->_type) {
        CFRelease(s->_type);
        s->_type = NULL;
    }

    // Release the socket.
    _ServerReleaseSocket(s);
    
    DBG(("Server Invalidated\n"));
}


#pragma mark -
#pragma mark Static Function Definitions

/* static */ CFStringRef
_ServerCopyDescription(ServerRef server) {
	
	CFTypeRef socket4, socket6, service;
	CFStringRef info, result;
	
	// Start with everything being "NULL"
	socket4 = socket6 = service = CFSTR("0x0");
	
	// Set socket4 to it's value
	if (server->_sockets[0] != NULL)
		socket4 = (CFTypeRef)(server->_sockets[0]);

	// Set socket6 to it's value
	if (server->_sockets[1] != NULL)
		socket6 = (CFTypeRef)(server->_sockets[1]);

	// Set service to it's value
	if (server->_service != NULL)
		service = (CFTypeRef)(server->_service);
    
	// Set the user's context based upon supplied "copyDescription"
	if (server->_ctxt.copyDescription)
		info = server->_ctxt.copyDescription(server->_ctxt.info);
	else
		info = CFStringCreateWithFormat(server->_alloc, NULL, CFSTR("<0x%x>"), (UInt32)(server->_ctxt.info));
	
	// Create the debug string
    result = CFStringCreateWithFormat(server->_alloc,
									  NULL,
									  CFSTR("<Server 0x%x>{sockets=[%@, %@] service=%@, info=%@}"),
									  (UInt32)server,
									  socket4,
                                      socket6,
									  service,
									  info);
	
	// Release the user's string
	CFRelease(info);
	
	return result;
}


/* static */ void
_ServerReleaseNetService(ServerRef server) {
	
	// Unschedule, cancel, and release the net service if there is one.
	if (server->_service != NULL) {
		CFNetServiceUnscheduleFromRunLoop(server->_service, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
		CFNetServiceSetClient(server->_service, NULL, NULL);
		CFNetServiceCancel(server->_service);
		CFRelease(server->_service);
		server->_service = NULL;
	}
}


/* static */ void
_ServerReleaseSocket(ServerRef server) {

    unsigned i;

    for (i = 0; i < (sizeof(server->_sockets) / sizeof(server->_sockets[0])); i++) {

        // Invalidate and release the socket if there is one.
        if (server->_sockets[i] != NULL) {
            CFSocketInvalidate(server->_sockets[i]);
            CFRelease(server->_sockets[i]);
            server->_sockets[i] = NULL;
        }
    }
}


/* static */ Boolean
_ServerCreateAndRegisterNetService(ServerRef server) {

	do {
        UInt32 port = server->_port;
		Boolean didSet, didRegister;
		CFNetServiceClientContext netSvcCtxt = {0,
												server,
												(const void*(*)(const void*))&ServerRetain,
												(void(*)(const void*))&ServerRelease,
												(CFStringRef(*)(const void*))&_ServerCopyDescription};
        
        // If the port was unspecified, get the port from the socket.
        if (port == 0) {
            
            // Get the local address
            CFDataRef addr = CFSocketCopyAddress(server->_sockets[0]);
            struct sockaddr_in* nativeAddr = (struct sockaddr_in*)CFDataGetBytePtr(addr);
            
            CFRelease(addr);
            
            port = ntohs(nativeAddr->sin_port);
        }
        
        // Create the service for registration.
        server->_service = CFNetServiceCreate(server->_alloc,
                                              CFSTR(""),
                                              server->_type,
                                              server->_name ? server->_name : CFSTR(""),
                                              port);
        
		// Require the service for the socket.
		if (server->_service == NULL)
			break;
					
		// Try setting the client on the service.
		didSet = CFNetServiceSetClient(server->_service,
									   (CFNetServiceClientCallBack)&_NetServiceCallBack,
									   &netSvcCtxt);
	
		// Check to make sure it set before registering.
		if (!didSet)
			break;
	
		// Schedule the service on the run loop.
		CFNetServiceScheduleWithRunLoop(server->_service, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
			
		// Start the registration.
		didRegister = CFNetServiceRegisterWithOptions(server->_service, 0, NULL);
		
		// If registration failed, die.
		if (!didRegister)
			break;
			
		return TRUE;
	
    } while (0);

	// Failed to set up the service, so clean up anything that succeeded.
	_ServerReleaseNetService(server);
    
    return FALSE;
}


/* static */ void
_ServerHandleAccept(ServerRef server, CFSocketNativeHandle nativeSocket) {
	
	// Inform the user of an incoming connection.
	if (server->_callback != NULL) {
		CFStreamError error = {0, 0};
		server->_callback((ServerRef)server, nativeSocket, &error, server->_ctxt.info);
	}
}


/* static */ void
_ServerHandleNetServiceError(ServerRef server, CFStreamError* error) {

	// Only fail if there is an error.  A zero indicates that the
	// service has successfully registered on the network.
	if (error->error) {
		
		// No matter what happened, tear down the service.
		_ServerReleaseNetService(server);

		// Handle the error.
		if (error->error != 0) {
		
			// Kill the underlying socket to prevent callbacks.
			_ServerReleaseSocket(server);
		
			// Inform the user of the error.
			if (server->_callback != NULL)
				server->_callback((ServerRef)server, (CFSocketNativeHandle)(-1), error, server->_ctxt.info);
		}
	}
}


/* static */ void
_SocketCallBack(CFSocketRef sock, CFSocketCallBackType type, CFDataRef address, const void *data, ServerRef server) {

    assert((sock == server->_sockets[0]) || (sock == server->_sockets[1]));

	// Only care about accept callbacks.
    if (type == kCFSocketAcceptCallBack) {
    
		assert((data != NULL) && (*((CFSocketNativeHandle*)data) != -1));
		
		// Dispatch the accept event.
		_ServerHandleAccept(server, *((CFSocketNativeHandle*)data));
	}
}


/* static */ void
_NetServiceCallBack(CFNetServiceRef service, CFStreamError* error, ServerRef server) {
    
    assert(service == server->_service);
    
	// Dispatch the registration error.
    _ServerHandleNetServiceError(server, error);
}
