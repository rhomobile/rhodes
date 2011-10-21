We are very sorry, but we removed ZBar source code from our project by ZBar license requirements.
For build Barcode extension for WM, you should download ZBar sources from
http://zbar.sourceforge.net/
and copy to this folder(rhodes\lib\extensions\barcode\ext\barcode\shared\zbar\zbar\) "include" and "zbar" folders from ZBar sources.

After it you should fix ZBar surces for compile by Microsoft Visual Studio (description for 0.10 version of ZBar) :


- fix includes for found headers (add ../ etc if it needed)

- move declaration of all local variables in functions to begin of scope (pure C style)

- add   #include "../../config.h"   to   begin of include/zbar.h

- memset and some pointer operation in convert.c and scanner.c not get void* -> replace to uint8_t*

- remove include of unistd.h from img_scanner.c, processor.h

- replace <config.h> to our local config.h  (rhodes\lib\extensions\barcode\ext\barcode\shared\zbar\config.h)

- replace <inttypes.h> to our local inttypes.h  (rhodes\lib\extensions\barcode\ext\barcode\shared\zbar\inttypes.h)

- in image.c :
int len = ...
char filename[len]
is impossible - use:   filename = malloc(len+1);

- remove including of <sys/time.h> from img_scanner.c and  timer.h

- in img_scanner.c comment <sys/time.h> related code :
replace
    struct timeval abstime;
    gettimeofday(&abstime, NULL);
    iscn->time = (abstime.tv_sec * 1000) + ((abstime.tv_usec / 500) + 1) / 2;
to:
    //struct timeval abstime;
    //gettimeofday(&abstime, NULL);
    //iscn->time = (abstime.tv_sec * 1000) + ((abstime.tv_usec / 500) + 1) / 2;
    iscn->time = 0;

- replace intptr_t to uintptr_t in img_scanner.c

- in thread.h :
replace
#if defined(_WIN32)
into:
#if 0//defined(_WIN32)

- add #include <stddef.h> to scanner.c

- in scanner.c modify code for calculate offset of value in structure :
replace
    memset(&scn->x, 0, sizeof(zbar_scanner_t) + (void*)scn - (void*)&scn->x);
to:
    memset(&scn->x, 0, sizeof(zbar_scanner_t) - offsetof(zbar_scanner_t, x));



Also may be you will need make few changes in rhodes code (already exist in Master version, but not exist in latest release) :


- remove strdup() from rhodes\lib\extensions\barcode\ext\barcode\shared\zbar\rho_bridge.c

- add
#define zbar_fourcc(a, b, c, d)                 \
        ((unsigned long)(a) |                   \
         ((unsigned long)(b) << 8) |            \
         ((unsigned long)(c) << 16) |           \
         ((unsigned long)(d) << 24))

to rhodes\lib\extensions\barcode\ext\barcode\shared\src\zbar.c





