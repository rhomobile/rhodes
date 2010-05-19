//
//  DateTime.h
//  rhorunner
//
//  Created by lars on 8/12/09.
//  Copyright 2009 Home. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface DateTime : NSObject {
@private
    NSString* url;
    NSString* title;
    long	  initialTime;
    int		  format;
    NSString* data;
}

@property (nonatomic, retain) NSString* url;
@property (nonatomic, retain) NSString* title;
@property (assign) long initialTime;
@property (assign) int format;
@property (nonatomic, retain) NSString* data;

@end
