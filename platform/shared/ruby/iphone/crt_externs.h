/*
 *  crt_externs.h
 *  rhorubylib
 *
 *  Created by evgeny vovchenko on 10/2/08.
 *  Copyright 2008 RhoMobile. All rights reserved.
 *
 */

#include <sys/cdefs.h>
//RHO
#if 0
__BEGIN_DECLS
extern char ***_NSGetArgv(void);
extern int *_NSGetArgc(void);
extern char ***_NSGetEnviron(void);
extern char **_NSGetProgname(void);
#ifdef __LP64__
extern struct mach_header_64 *
#else /* !__LP64__ */
extern struct mach_header *
#endif /* __LP64__ */
_NSGetMachExecuteHeader(void);
__END_DECLS
#endif //0