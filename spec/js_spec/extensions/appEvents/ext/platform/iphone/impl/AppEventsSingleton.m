
#import "AppEventsSingleton.h"


extern void rho_app_events_simulate_event(const char* applicationEvent);
extern void rho_app_add_conflict_int(const char* name, int savedValue, int newValue);
extern void rho_app_add_conflict_str(const char* name, const char* savedValue, const char* newValue);
extern void rho_app_events_simulate_conflict();

@implementation AppEventsSingleton

-(void) simulateEvent:(NSString*)applicationEvent {
    rho_app_events_simulate_event([applicationEvent UTF8String]);
}

-(void) addConflictInt:(NSString*)valueName savedValue:(int)savedValue newValue:(int)newValue {
    rho_app_add_conflict_int([valueName UTF8String], savedValue, newValue);
}

-(void) addConflictString:(NSString*)valueName savedValue:(NSString*)savedValue newValue:(NSString*)newValue
{
    rho_app_add_conflict_str([valueName UTF8String], [savedValue UTF8String], [newValue UTF8String]);
}

/* Simulate Conflicts event */
-(void) simulateConflicts {
    rho_app_events_simulate_conflict();
}

@end