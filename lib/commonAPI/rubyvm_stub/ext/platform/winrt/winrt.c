#include <stdlib.h>
#include "ruby.h"
#include "ruby\win32\dir.h"

int WSAAPI
rb_w32_recv(int fd, char *buf, int len, int flags)
{
    return 0;
}

int
rb_w32_is_socket(int fd)
{
    return 0;
}

int WSAAPI
rb_w32_send(int fd, const char *buf, int len, int flags)
{
    return 0;
}

void rho_file_impl_delete_files_in_folder(const char *szFolderPath)
{
    // TODO: implement
}

DIR* rb_w32_opendir(const char* filename)
{
    // TODO: implement
    return NULL;
}

void rb_w32_closedir(DIR *dirp)
{
    // TODO: implement
}

struct direct* rb_w32_readdir(DIR *dirp)
{
    // TODO: implement
    return NULL;
}  
