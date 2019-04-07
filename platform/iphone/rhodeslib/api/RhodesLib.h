
#import <Foundation/Foundation.h>


@interface RhodesLib : NSObject {}

+(RhodesLib*)getSharedInstance;

//-(BOOL)didFinishLaunchingWithOptions:(NSDictionary *)launchOptions application:(UIApplication *)application

-(void) startRhodes;

-(void) onActivate;

-(void) onDeactivate;


@end
