//
//  RhoConnectObjectNotify.h
//  RhoConnectClient
//
//  Created by evgeny vovchenko on 4/22/11.
//  Copyright 2011 RhoMobile. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "RhoConnectClient/RhoConnectClient.h"

@interface RhoConnectObjectNotify : NSObject {
    
}
@property(readonly) NSMutableArray* deleted_objects;
@property(readonly) NSMutableArray* updated_objects;
@property(readonly) NSMutableArray* created_objects;

@property(readonly) NSMutableArray* deleted_source_ids;
@property(readonly) NSMutableArray* updated_source_ids;
@property(readonly) NSMutableArray* created_source_ids;

- (id) init: (RHO_CONNECT_OBJECT_NOTIFY*) data;
- (void)dealloc;

@end
