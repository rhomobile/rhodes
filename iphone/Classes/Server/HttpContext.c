
#include <unistd.h>

#pragma mark Includes
#if TARGET_IPHONE_SIMULATOR
#import <CoreServices/CoreServices.h>
#else
#import <CFNetwork/CFNetwork.h>
#endif

#include "defs.h"
#include "HttpContext.h"
#include "HttpMessage.h"
#include "Dispatcher.h"
#include "AppManagerI.h"

#pragma mark -
#pragma mark Type Declarations



#pragma mark -
#pragma mark Constant Definitions

static const CFTimeInterval kTimeOutInSeconds = 60;

static const CFOptionFlags kReadEvents = kCFStreamEventHasBytesAvailable |
                                         kCFStreamEventErrorOccurred |
                                         kCFStreamEventEndEncountered;

static const CFOptionFlags kWriteEvents = kCFStreamEventCanAcceptBytes |
										  kCFStreamEventErrorOccurred;


#pragma mark -
#pragma mark Static Function Declarations

static int _HttpGetHeadersLen(const char *buf, size_t buflen);

static void _HttpContextHandleHasBytesAvailable(HttpContextRef context);
static void _HttpContextHandleEndEncountered(HttpContextRef context);
static void _HttpContextHandleCanAcceptBytes(HttpContextRef context);
static void _HttpContextHandleErrorOccurred(HttpContextRef context);
static void _HttpContextHandleTimeOut(HttpContextRef context);

static void _ReadStreamCallBack(CFReadStreamRef inStream, CFStreamEventType type, HttpContextRef context);
static void _WriteStreamCallBack(CFWriteStreamRef outStream, CFStreamEventType type, HttpContextRef context);
static void _TimerCallBack(CFRunLoopTimerRef timer, HttpContextRef context);


#pragma mark -
#pragma mark Extern Function Definitions (API)

/* extern */ HttpContextRef
HttpContextCreate(CFAllocatorRef alloc, CFSocketNativeHandle nativeSocket) {

	HttpContextRef context = NULL;

	do {
		// Allocate the buffer for the context.
		context = CFAllocatorAllocate(alloc, sizeof(context[0]), 0);
		
		// Fail if unable to create the context
		if (context == NULL)
			break;
		
		memset(context, 0, sizeof(context[0]));
		
		// Save the allocator for deallocating later.
		if (alloc)
			context->_alloc = CFRetain(alloc);
		
		// Bump the retain count.
		HttpContextRetain(context);
		
		// Create the streams for the incoming socket connection.
		CFStreamCreatePairWithSocket(alloc, nativeSocket, &(context->_inStream), &(context->_outStream));

		// Require both streams in order to create.
		if ((context->_inStream == NULL) || (context->_outStream == NULL))
			break;
				
		// Give up ownership of the native socket.
		CFReadStreamSetProperty(context->_inStream, kCFStreamPropertyShouldCloseNativeSocket, kCFBooleanTrue);
			
		// Create the receive buffer of no fixed size.
		context->_rcvdBytes = CFDataCreateMutable(alloc, 0);
		
		// Fail if unable to create receive buffer.
		if (context->_rcvdBytes == NULL)
			break;

        // Create the outbound buffer of no fixed size.
		context->_sendBytes = CFDataCreateMutable(alloc, 0);
		
		// Fail if unable to create send buffer.
		if (context->_sendBytes == NULL)
			break;
        
        context->_request = CFAllocatorAllocate(alloc, sizeof(context->_request[0]), 0);

        // Fail if unable to create request.
		if (context->_request == NULL)
			break;
        
        memset(context->_request, 0, sizeof(context->_request[0]));
                
		return context;
		
	} while (0);
	
	// Something failed, so clean up.
	HttpContextRelease(context);
	
	return NULL;
}


/* extern */ HttpContextRef
HttpContextRetain(HttpContextRef context) {
	
	// Bump the retain count if context is good.
	if (context != NULL)
		context->_retainCount++;
		
	return context;
}


/* extern */ void
HttpContextRelease(HttpContextRef context) {

	if (context != NULL) {
		
		// Decrease the retain count.
		context->_retainCount--;
		
		// Don't dispose until the count goes to zero.
		if (context->_retainCount > 0)
			return;

        // The streams and timers retain the context.  If any of these are still set, it means
        // that somehow we hit a retain count of zero, but are still held by them!  This would
        // happen if the context was over-released.
        assert(!context->_inStream && !context->_outStream && !context->_timer);

        // Hold locally so deallocation can happen and then safely release.
		CFAllocatorRef alloc = context->_alloc;
		
        // Release request
        if (context->_request != NULL) {
            
            // Release uri
            if (context->_request->_uri != NULL )
                CFAllocatorDeallocate(alloc, context->_request->_uri);
            
            // Free memory allocated for headers
            if (context->_request->_headers != NULL)
                CFAllocatorDeallocate(alloc, context->_request->_headers);
                
            // Free memory in use by the request
            CFAllocatorDeallocate(alloc, context->_request);
        }
        
		// Release the recieve buffer if there is one.
		if (context->_rcvdBytes != NULL)
			CFRelease(context->_rcvdBytes);
		
		// Release the send buffer if there is one.
		if (context->_sendBytes != NULL)
			CFRelease(context->_sendBytes);
        
		// Free the memory in use by the context.
		CFAllocatorDeallocate(alloc, context);
		
		// Release the allocator.
		if (alloc)
			CFRelease(alloc);
	}
}


/* extern */ Boolean
HttpContextOpen(HttpContextRef context) {

	do {
		Boolean didSet;
		CFRunLoopRef runLoop = CFRunLoopGetCurrent();
		
		CFStreamClientContext streamCtxt = {0, context, (void*(*)(void*))&HttpContextRetain, (void(*)(void*))&HttpContextRelease, NULL};
		CFRunLoopTimerContext timerCtxt = {0, context, (const void*(*)(const void*))&HttpContextRetain, (void(*)(const void*))&HttpContextRelease, NULL};
		
		// Set the client on the read stream.
		didSet = CFReadStreamSetClient(context->_inStream,
									   kReadEvents,
									   (CFReadStreamClientCallBack)&_ReadStreamCallBack,
									   &streamCtxt);
									   
		// Fail if unable to set the client.
		if (!didSet)
			break;
		
		// Set the client on the write stream.
		didSet = CFWriteStreamSetClient(context->_outStream,
										kWriteEvents,
										(CFWriteStreamClientCallBack)&_WriteStreamCallBack,
										&streamCtxt);
		
		// Fail if unable to set the client.
		if (!didSet)
			break;
			
		// Schedule the streams on the current run loop and default mode.
		CFReadStreamScheduleWithRunLoop(context->_inStream, runLoop, kCFRunLoopCommonModes);
		CFWriteStreamScheduleWithRunLoop(context->_outStream, runLoop, kCFRunLoopCommonModes);
		
		// Open the stream for reading.
		if (!CFReadStreamOpen(context->_inStream))
			break;
			
		// Open the stream for writing.
		if (!CFWriteStreamOpen(context->_outStream))
			break;
		
		// Create the timeout timer
		context->_timer = CFRunLoopTimerCreate(CFGetAllocator(context->_inStream),
											   CFAbsoluteTimeGetCurrent() + kTimeOutInSeconds,
											   0,		// interval
											   0,		// flags
											   0,		// order
											   (CFRunLoopTimerCallBack)_TimerCallBack,
											   &timerCtxt);
		
		// Fail if unable to create the timer.
		if (context->_timer == NULL)
			break;
			
        CFRunLoopAddTimer(runLoop, context->_timer, kCFRunLoopCommonModes);
            
		return TRUE;
		
	} while (0);
	
	// Something failed, so clean up.
	HttpContextClose(context);
	
	return FALSE;
}


/* extern */ void
HttpContextClose(HttpContextRef context) {

	CFRunLoopRef runLoop = CFRunLoopGetCurrent();

	// Check if the read stream exists.
	if (context->_inStream) {
		
		// Unschedule, close, and release it.
		CFReadStreamSetClient(context->_inStream, 0, NULL, NULL);
		CFReadStreamUnscheduleFromRunLoop(context->_inStream, runLoop, kCFRunLoopCommonModes);
		CFReadStreamClose(context->_inStream);
		CFRelease(context->_inStream);
		
		// Remove the reference.
		context->_inStream = NULL;
	}

	// Check if the write stream exists.
	if (context->_outStream) {
		
		// Unschedule, close, and release it.
		CFWriteStreamSetClient(context->_outStream, 0, NULL, NULL);
		CFWriteStreamUnscheduleFromRunLoop(context->_outStream, runLoop, kCFRunLoopCommonModes);
		CFWriteStreamClose(context->_outStream);
		CFRelease(context->_outStream);
		
		// Remove the reference.
		context->_outStream = NULL;
	}

    // Get rid of the timer, if it still exists
    if (context->_timer != NULL) {

        CFRunLoopTimerInvalidate(context->_timer);
        CFRelease(context->_timer);
        context->_timer = NULL;
    }
}

const char* 
HttpGetSiteRoot() {
	return GetApplicationsRootPath();
}

#pragma mark -
#pragma mark Static Function Definitions

void 
_dbg_print_data(UInt8* buffer, CFIndex size) {
    
    for(CFIndex i = 0; i < size; i++) {
        DBG(("%c",buffer[i]));
    }
    
}

/* static */ void
_HttpContextHandleHasBytesAvailable(HttpContextRef context) {

	UInt8 buffer[2048];
	
	// Try reading the bytes into the buffer.
	CFIndex bytesRead = CFReadStreamRead(context->_inStream, buffer, sizeof(buffer));
	
	// Reset the timeout.
	CFRunLoopTimerSetNextFireDate(context->_timer, CFAbsoluteTimeGetCurrent() + kTimeOutInSeconds);
	
	// If there wasn't an error (-1) and not end (0), process the data.
	if (bytesRead > 0) {
		
		// Add the bytes of data to the receive buffer.
		CFDataAppendBytes(context->_rcvdBytes, buffer, bytesRead);

		// Parse recieved data
        int result = HTTPParseRequest(context);      
        
        if ( result == 1 ) {
            // HTTP message is fully loaded, process it
            HTTPProcessMessage(context);
			
			// Reset the timeout.
			CFRunLoopTimerSetNextFireDate(context->_timer, CFAbsoluteTimeGetCurrent() + kTimeOutInSeconds);
        }
        
        // If the ouput stream can write, try sending the bytes.
		if (result != 0 && CFWriteStreamCanAcceptBytes(context->_outStream))
			_HttpContextHandleCanAcceptBytes(context);
	}
}


/* static */ void
_HttpContextHandleEndEncountered(HttpContextRef context) {
    DBG(("Handle End Encountered 0x%X\n",context));
	// End was hit, so destroy the context.
    HttpContextClose(context);
    HttpContextRelease(context);
}

/* static */ void
_HttpContextHandleCanAcceptBytes(HttpContextRef context) {
	DBG(("Sending data to the view\n"));
	
	// Get the start of the buffer to send.
	const UInt8* start = CFDataGetBytePtr(context->_sendBytes);
	
    // Get number of bytes ready to be send
    int size = CFDataGetLength(context->_sendBytes);
    
	// Writing resets the timer.
	CFRunLoopTimerSetNextFireDate(context->_timer, CFAbsoluteTimeGetCurrent() + kTimeOutInSeconds);
	
	// If there data in the buffer to send, take care of sending the data.
	if (size != 0) {
		
		// Write all of the bytes redy to be sent
		CFIndex bytesWritten = CFWriteStreamWrite(context->_outStream, start, size);
		DBG(("%d bytes sent\n", bytesWritten));
			 
		// If successfully sent the data, remove the bytes from the buffer.
		if (bytesWritten > 0)
			CFDataDeleteBytes(context->_sendBytes, CFRangeMake(0, bytesWritten));
	}
}


/* static */ void
_HttpContextHandleErrorOccurred(HttpContextRef context) {
    DBG(("Handle Error Occurred 0x%X\n",context));
	// Hit an error, so destroy the context which will close the streams.
    HttpContextClose(context);
	HttpContextRelease(context);
}


/* static */ void
_HttpContextHandleTimeOut(HttpContextRef context) {
    DBG(("Handle Timeout 0x%X\n",context));
	// Haven't heard from the client so kill everything.
    HttpContextClose(context);
    HttpContextRelease(context);
}


/* static */ void
_ReadStreamCallBack(CFReadStreamRef inStream, CFStreamEventType type, HttpContextRef context) {

	assert(inStream == context->_inStream);
	
	// Dispatch the event properly.
    switch (type) {
        case kCFStreamEventHasBytesAvailable:
            _HttpContextHandleHasBytesAvailable(context);
            break;
			
        case kCFStreamEventEndEncountered:
			_HttpContextHandleEndEncountered(context);
            break;
       
        case kCFStreamEventErrorOccurred:
			_HttpContextHandleErrorOccurred(context);
			break;
            
        default:
            break;
    }
}


/* static */ void
_WriteStreamCallBack(CFWriteStreamRef outStream, CFStreamEventType type, HttpContextRef context) {

	assert(outStream == context->_outStream);

	// Dispatch the event properly.
    switch (type) {
		case kCFStreamEventCanAcceptBytes:
			_HttpContextHandleCanAcceptBytes(context);
			break;
			
        case kCFStreamEventErrorOccurred:
			_HttpContextHandleErrorOccurred(context);
			break;
            
        default:
            break;
    }
}


/* static */ void
_TimerCallBack(CFRunLoopTimerRef timer, HttpContextRef context) {

	assert(timer == context->_timer);

	// Dispatch the timer event.
	_HttpContextHandleTimeOut(context);
}
