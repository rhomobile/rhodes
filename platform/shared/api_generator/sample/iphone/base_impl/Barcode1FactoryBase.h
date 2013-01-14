
#import "../api/IBarcode1.h"

@interface Barcode1FactoryBase : NSObject<IBarcode1Factory> {
    NSMutableDictionary* mBarcode1Instances;
    id<IBarcode1Singleton> mBarcode1Singleton;
}

-(id)init;

-(id<IBarcode1Singleton>) getBarcode1Singleton;
-(id<IBarcode1>) getBarcode1ByID:(NSString*)ID;

-(id<IBarcode1>)createBarcode1ByID:(NSString*)ID;

-(void)dealloc;

@end