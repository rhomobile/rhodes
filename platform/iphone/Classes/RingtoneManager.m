//
//  RingtoneManager.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 29.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

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
