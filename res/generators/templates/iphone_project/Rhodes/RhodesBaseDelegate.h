//
//  RhodesBaseDelegate.h
//  Rhodes
//
//  Created by Dmitry Soldatenkov on 20.11.13.
//  Copyright (c) 2013 Rhomobile. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "<%= rhodes_root_path %>/platform/iphone/Classes/Rhodes.h"

@interface RhodesBaseDelegate : Rhodes <UIApplicationDelegate,
UITabBarControllerDelegate, AVAudioPlayerDelegate, UIAlertViewDelegate>
{
    
}

@end
