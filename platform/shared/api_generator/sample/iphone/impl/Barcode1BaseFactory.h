
#import "../api/IBarcode1.h"

@interface Barcode1BaseFactory : NSObject {
    NSMutableDictionary* mBarcode1Instances;
    NSString* mDefaultID;
}

-(id)init;

-(id<IBarcode1>) getBarcode1ByID:(NSString*)ID;
-(NSString*) getDefaultID;
-(void) setDefaultID:(NSString*)defaultID;
-(NSString*)getInitialDefaultID;
-(Class) getBarcode1ImplementationClass;

-(void)dealloc;

@end