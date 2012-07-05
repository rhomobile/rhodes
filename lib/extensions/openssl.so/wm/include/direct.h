#ifndef __direct_h__
#define __direct_h__

#ifdef __cplusplus
extern "C" {
#endif

char *_getcwd(char *buffer, int maxlen);
#define getcwd _getcwd

int _mkdir(char* dirname, int mode);

#define mkdir _mkdir

int _rmdir(const char * dir);
#define rmdir _rmdir

#ifdef __cplusplus
}
#endif

#endif