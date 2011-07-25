/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include <CoreFoundation/CoreFoundation.h>
#include <CFNetwork/CFNetwork.h>

#include "sslimpl.h"

#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SSL"

namespace rho
{
namespace net
{

struct ssl_data_t {
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
};

static void report_error(const char *s, CFStreamError *e)
{
    if (e->domain == kCFStreamErrorDomainPOSIX)
        RAWLOG_ERROR2("%s, errno: %d", s, e->error);
    else if (e->domain == kCFStreamErrorDomainMacOSStatus)
        RAWLOG_ERROR2("%s, Mac OS status: %d", s, e->error);
    else
        RAWLOG_ERROR1("%s, unknown error", s);
}
    

void * SSLImpl::createStorage()
{
    return calloc(1, sizeof(ssl_data_t));
}

void SSLImpl::freeStorage(void *ptr)
{
    if (ptr)
        free(ptr);
}
    
CURLcode SSLImpl::connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage)
{
    ssl_data_t *data = (ssl_data_t*)storage;
    
    CFStreamCreatePairWithSocket(kCFAllocatorDefault, sockfd, &data->readStream, &data->writeStream);
    // Indicate that the connection needs to be done in secure manner
    CFReadStreamSetProperty(data->readStream, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelSSLv3);
    CFWriteStreamSetProperty(data->writeStream, kCFStreamPropertySocketSecurityLevel, kCFStreamSocketSecurityLevelSSLv3);
    
    CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    if (!dict) {
        RAWLOG_ERROR("Can not allocate CFMutableDictionaryRef");
        return CURLE_SSL_CONNECT_ERROR;
    }
    
    CFDictionarySetValue(dict, kCFStreamSSLValidatesCertificateChain, ssl_verify_peer ? kCFBooleanTrue : kCFBooleanFalse);
    CFReadStreamSetProperty(data->readStream, kCFStreamPropertySSLSettings, dict);
    CFWriteStreamSetProperty(data->writeStream, kCFStreamPropertySSLSettings, dict);
    CFRelease(dict);
    
    if (!CFReadStreamOpen(data->readStream) || !CFWriteStreamOpen(data->writeStream)) {
        RAWLOG_ERROR("SSL connection error");
        return CURLE_SSL_CONNECT_ERROR;
    }
    CFReadStreamScheduleWithRunLoop(    data->readStream,
                                        CFRunLoopGetCurrent(),
                                        kCFRunLoopCommonModes);
    CFWriteStreamScheduleWithRunLoop(    data->writeStream,
                                    CFRunLoopGetCurrent(),
                                    kCFRunLoopCommonModes);
    *done = 1;
    return CURLE_OK;
}

void SSLImpl::shutdown(void *storage)
{
    ssl_data_t *data = (ssl_data_t*)storage;
    if (!data)
        return;
    CFReadStreamScheduleWithRunLoop(    data->readStream,
                                    CFRunLoopGetCurrent(),
                                    kCFRunLoopCommonModes);
    CFWriteStreamScheduleWithRunLoop(    data->writeStream,
                                     CFRunLoopGetCurrent(),
                                     kCFRunLoopCommonModes);
    if (data->readStream) {
        CFReadStreamRef readStream = (CFReadStreamRef)data->readStream;
        CFReadStreamClose(readStream);
        CFRelease(readStream);
        data->readStream = NULL;
    }
    if (data->writeStream) {
        CFWriteStreamRef writeStream = (CFWriteStreamRef)data->writeStream;
        CFWriteStreamClose(writeStream);
        CFRelease(writeStream);
        data->writeStream = NULL;
    }
}

ssize_t SSLImpl::send(const void *mem, size_t len, void *storage)
{
    ssl_data_t *data = (ssl_data_t*)storage;
    
    if (!CFWriteStreamCanAcceptBytes(data->writeStream))
        return 0;
    
    int rc = CFWriteStreamWrite(data->writeStream, (const UInt8 *)mem, len);
    if (rc < 0) {
        CFStreamError err = CFWriteStreamGetError(data->writeStream);
        report_error("SSL send failed", &err);
        return -1;
    }
    
    return rc;
}

ssize_t SSLImpl::recv(char *buf, size_t size, int *wouldblock, void *storage)
{
    ssl_data_t *data = (ssl_data_t*)storage;
    
    if (!CFReadStreamHasBytesAvailable(data->readStream)) {
        *wouldblock = 1;
        return -1;
    }
    
    *wouldblock = 0;
    
    CFIndex rc = CFReadStreamRead(data->readStream, (UInt8*)buf, size);
    if (rc < 0) {
        CFStreamError err = CFReadStreamGetError(data->readStream);
        report_error("SSL recv failed", &err);
        return -1;
    }
    
    return rc;
    
}

} // namespace net
} // namespace rho
