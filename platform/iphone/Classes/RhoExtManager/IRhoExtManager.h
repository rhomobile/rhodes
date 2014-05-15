//
//  IRhoExtManager.h
//  RhoAppBaseLib

//
//

#import <Foundation/Foundation.h>
#import "IRhoExtension.h"


@protocol IRhoExtManager <NSObject>

-(void) registerExtension:(id<IRhoExtension>)extension;
-(void) unregisterExtension:(id<IRhoExtension>)extension;

-(BOOL) onBeforeNavigate:(NSString*)url tabIndex:(int)tabIndex;


@end
