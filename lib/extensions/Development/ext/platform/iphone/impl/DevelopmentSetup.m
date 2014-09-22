#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

#import <UIKit/UIKit.h>

#import "Rhodes.h"

extern void Bundle_update_on_triple_tap();
extern void Bundle_update_on_quadro_tap();


@interface RhoSpecialGesturesReceiver : NSObject {
}

- (void)handleTripleTap:(UITapGestureRecognizer *)sender;
- (void)handleQuadroTap:(UITapGestureRecognizer *)sender;

@end

@implementation RhoSpecialGesturesReceiver
- (void)handleTripleTap:(UITapGestureRecognizer *)sender
{
    if (sender.state == UIGestureRecognizerStateEnded)
    {
        // handling code
        AudioServicesPlaySystemSound(1003);
        Bundle_update_on_triple_tap();
    }
}

- (void)handleQuadroTap:(UITapGestureRecognizer *)sender
{
    if (sender.state == UIGestureRecognizerStateEnded)
    {
        // handling code
        Bundle_update_on_quadro_tap();
    }
}

@end

static RhoSpecialGesturesReceiver* ourRhoSpecialGesturesReceiver = nil;

extern void Development_Init();

void Init_Development_extension()
{
    ourRhoSpecialGesturesReceiver = [[RhoSpecialGesturesReceiver alloc] init];
    
    UITapGestureRecognizer* triple = [[UITapGestureRecognizer alloc] initWithTarget:ourRhoSpecialGesturesReceiver action:@selector(handleTripleTap:)];
    triple.numberOfTapsRequired = 1;
    triple.numberOfTouchesRequired = 3;
    UITapGestureRecognizer* quadro = [[UITapGestureRecognizer alloc] initWithTarget:ourRhoSpecialGesturesReceiver action:@selector(handleQuadroTap:)];
    quadro.numberOfTapsRequired = 1;
    quadro.numberOfTouchesRequired = 4;
    
    //[UIApplication sharedApplication]
    [[Rhodes sharedInstance].window addGestureRecognizer:triple];
    [[Rhodes sharedInstance].window addGestureRecognizer:quadro];
    
    
    Development_Init();
}