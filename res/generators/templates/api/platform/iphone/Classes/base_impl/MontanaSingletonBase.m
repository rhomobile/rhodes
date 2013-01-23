
#import "Barcode1SingletonBase.h"

@implementation Barcode1SingletonBase

-(id)init {
    self = [super init];
    mDefaultID= nil;
    return self;
}

-(NSString*) getDefaultID {
    if (mDefaultID == nil) {
        mDefaultID = [[self getInitialDefaultID] retain];
    }
    return mDefaultID;
}

-(void) setDefaultID:(NSString*)defaultID {
    mDefaultID = [defaultID retain];
}

-(NSString*)getInitialDefaultID {
    return nil;
}

-(void)dealloc {
    [mDefaultID release];
    [super dealloc];
}

@end