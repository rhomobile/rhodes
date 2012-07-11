#ifndef __utime_h__
#define __utime_h__

#ifdef __cplusplus
extern "C" {
#endif

	struct utimbuf {
	time_t actime;          /* access time */
	time_t modtime;         /* modification time */
};

int _utime(const char *f, struct utimbuf *t);
#define utime _utime

#ifdef __cplusplus
}
#endif


#endif
