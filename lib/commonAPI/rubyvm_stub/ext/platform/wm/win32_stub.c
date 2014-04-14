
#include <Windows.h>

#include <ruby/wince/sys/types.h>
#include <ruby/win32/dir.h>

//TODO: implement this functions
int rb_w32_rename(const char *oldname, const char *newname)
{
    return 0;
}

int rb_w32_unlink(const char *path)
{
    return 0;
}

int
rb_w32_map_errno(unsigned long winerr)
{
    return 0;
}

int 
rb_w32_recv(int fd, char *buf, int len, int flags)
{
    return 0;
}

int
rb_w32_is_socket(int fd)
{
    return 0;
}

int 
rb_w32_send(int fd, const char *buf, int len, int flags)
{
    return 0;
}

char *
rb_w32_getenv(const char *name)
{
    return "";
}


DIR*           
rb_w32_opendir(const char* dirName)
{
    return 0;
}

void           
rb_w32_closedir(DIR *dirObj)
{
}

struct direct* 
rb_w32_readdir(DIR *dirObj)
{
    return 0;
}