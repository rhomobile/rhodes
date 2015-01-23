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

#import "DateTime.h"
#import "Rhodes.h"
#import "DateTimePickerDelegate.h"

int rho_rhodesapp_check_mode();

void choose_datetime_with_range(char* callback, char* title, long initial_time, int format, char* data, long min_time, long max_time) {
    if (!rho_rhodesapp_check_mode())
        return;
    DateTime* dateTime = [[DateTime alloc] init];
    dateTime.url = [NSString stringWithUTF8String:callback];
    dateTime.title = [NSString stringWithUTF8String:title];
    dateTime.initialTime = initial_time;
    dateTime.format = format;
    dateTime.data = [NSString stringWithUTF8String:data];
	dateTime.minTime = min_time;
	dateTime.maxTime = max_time;
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(chooseDateTime:) withObject:dateTime waitUntilDone:YES];
    [dateTime release];
}

void choose_datetime(char* callback, char* title, long initial_time, int format, char* data) {
    choose_datetime_with_range(callback, title, initial_time, format, data, 0, 0 );
}

void set_change_value_callback_datetime(char* callback) {
    NSString* ns_callback = nil;
    if (callback != NULL) {
        ns_callback = [NSString stringWithUTF8String:callback];
        if ([ns_callback length] <= 0) {
            ns_callback = nil;
        }
    }
    [DateTimePickerDelegate setChangeValueCallback:ns_callback];
}
