//
//  IAppMessageReceiver.h
//  rhorunner
//
//  Created by Alex Epifanoff on 13.12.13.
//
//

#ifndef rhorunner_IAppMessageReceiver_h
#define rhorunner_IAppMessageReceiver_h

#import <Foundation/Foundation.h>

@protocol IAppMessageReceiver <NSObject>

- (void) onAppMessageReceived:(NSString *)message app:(NSString*)app;

@end


#endif
