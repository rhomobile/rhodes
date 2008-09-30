/*
 *  defs.h
 *  Browser
 *
 *  Created by adam blum on 9/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#define _DEBUG 1

#ifdef _DEBUG
#define	DBG(x)          printf x ;
#else
#define	DBG(x)
#endif /* DEBUG */

#define	URI_MAX		16384			/* Default max request size	*/


