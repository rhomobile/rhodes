/*
 *  extensions.c
 *  rhorubylib
 *
 *  Created by Brian Moore on 1/19/10.
 *  Copyright 2010 RhoMobile. All rights reserved.
 *
 */

// This file is processed by the build script to add binary extensions

//EXTERNS
extern void Init_fcntl(void);
extern void Init_digest(void);
extern void Init_openssl(void);
//END

void Init_Extensions(void) {
//CALLS
    Init_fcntl();
    Init_digest();
    Init_openssl();
//END
}
