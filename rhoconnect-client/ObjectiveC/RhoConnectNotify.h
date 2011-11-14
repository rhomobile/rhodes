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

#import <Foundation/Foundation.h>

#include "RhoConnectClient/RhoConnectClient.h"

@interface RhoConnectNotify : NSObject {
}

@property int       total_count;
@property int       processed_count;
@property int       cumulative_count;
@property int       source_id;
@property int       error_code;
@property(readonly) NSString* source_name;
@property(readonly) NSString* status;
@property(readonly) NSString* sync_type;
@property(readonly) NSString* error_message;
@property(readonly) NSString* callback_params;
@property(readonly) RHO_CONNECT_NOTIFY notify_data;

- (id) init: (RHO_CONNECT_NOTIFY*) data;
- (void)dealloc;

- (Boolean) hasCreateErrors;
- (Boolean) hasUpdateErrors;
- (Boolean) hasDeleteErrors;
- (Boolean) isUnknownClientError;

- (RHO_CONNECT_NOTIFY*)getNotifyPtr;

@end
