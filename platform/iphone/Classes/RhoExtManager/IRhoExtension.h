//
//  IRhoExtension.h
//  RhoAppBaseLib

//
//

#import <Foundation/Foundation.h>


@protocol IRhoExtension <NSObject>

-(BOOL) onBeforeNavigate:(NSString*)url tabIndex:(int)tabIndex;

@end
