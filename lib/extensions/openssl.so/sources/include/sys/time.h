#ifndef __systime_h__
#define __systime_h__

#ifdef __cplusplus
extern "C" {
#endif

struct timezone 
{
	int  tz_minuteswest; /* minutes W of Greenwich */
	int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz);

#ifdef __cplusplus
}
#endif

#endif /// #ifndef __systime_h__