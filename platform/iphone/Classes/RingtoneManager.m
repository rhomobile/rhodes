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

#import "RingtoneManager.h"
#import "Rhodes.h"

#include "ruby/ext/rho/rhoruby.h"

#import "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RingtoneManager"

@interface RhoRingtoneManagerPlayStartTask : NSObject {}
+ (void)run:(NSString*)file;
@end

@implementation RhoRingtoneManagerPlayStartTask
+ (void)run:(NSString*)file {
    [[Rhodes sharedInstance] playStart:file mediaType:NULL];
}
@end

@interface RhoRingtoneManagerPlayStopTask : NSObject {}
+ (void)run;
@end

@implementation RhoRingtoneManagerPlayStopTask
+ (void)run {
    [[Rhodes sharedInstance] playStop];
}
@end


@implementation RingtoneManager

+ (void)playStart:(NSString*)fileName {
    id runnable = [RhoRingtoneManagerPlayStartTask class];
    [Rhodes performOnUiThread:runnable arg:fileName wait:NO];
}

+ (void)playStop {
    id runnable = [RhoRingtoneManagerPlayStopTask class];
    [Rhodes performOnUiThread:runnable wait:NO];
}

@end

VALUE rho_ringtone_manager_get_all()
{
    // No API to get ringtones
    return rho_ruby_get_NIL();
}

void rho_ringtone_manager_play(char *file_name)
{
    if (file_name == NULL) {
        RAWLOG_ERROR("RingtoneManager.play - please specify file name to play");
        return;
    }
    
    [RingtoneManager playStart:[NSString stringWithUTF8String:file_name]];
}

void rho_ringtone_manager_stop()
{
    [RingtoneManager playStop];
}
