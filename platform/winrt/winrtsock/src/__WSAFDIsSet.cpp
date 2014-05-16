
#include "..\include\winrtsock.h"

int __stdcall __WSAFDIsSet(SOCKET fd, fd_set * set)
{
    int i = set->fd_count;

    while (i--)
        if (set->fd_array[i] == fd)
            return 1;

    return 0;
}