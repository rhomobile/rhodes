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

@interface RhoRingtoneManagerPlayStartTask : NSObject<RhoRunnable>
{
    NSString *file;
}

@property (nonatomic,copy) NSString *file;

- (id)initWithFile:(NSString*)f;
- (void)dealloc;
- (void)run;

@end

@implementation RhoRingtoneManagerPlayStartTask

@synthesize file;

- (id)initWithFile:(NSString *)f {
    file = nil;
    self.file = f;
    return self;
}

- (void)dealloc {
    [file release];
    [super dealloc];
}

- (void)run {
    [[Rhodes sharedInstance] playStart:file mediaType:NULL];
}

@end

@interface RhoRingtoneManagerPlayStopTask : NSObject<RhoRunnable> {}

- (void)run;

@end

@implementation RhoRingtoneManagerPlayStopTask

- (void)run {
    [[Rhodes sharedInstance] playStop];
}

@end


@implementation RingtoneManager

+ (void)playStart:(NSString*)fileName {
    id task = [[[RhoRingtoneManagerPlayStartTask alloc] initWithFile:fileName] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
}

+ (void)playStop {
    id task = [[RhoRingtoneManagerPlayStopTask alloc] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
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
