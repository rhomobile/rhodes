/*
 *  defs.h
 *  Browser
 *
 *  Created by adam blum on 9/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifdef DEBUG
#define _DEBUG 1
#endif

#ifdef _DEBUG
#define	DBG(x)          printf x ;
#else
#define	DBG(x)
#endif /* _DEBUG */

#define	URI_MAX		16384			/* Default max request size	*/


