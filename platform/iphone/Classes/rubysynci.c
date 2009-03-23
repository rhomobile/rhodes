/*
 *  rubysynci.c
 *  rhorunner
 *
 *  Created by vlad on 10/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "defs.h"
#include "SyncEngine.h"
#include "rubysynci.h"

void dosync() {
	wake_up_sync_engine();
}