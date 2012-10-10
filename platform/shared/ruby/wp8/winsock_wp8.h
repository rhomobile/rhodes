#ifndef __WINSOCK_WP8_H_
#define __WINSOCK_WP8_H_

#include <windows.h>
#include <winsock2.h>

#include "../../common/RhoDefs.h"

RHO_GLOBAL int WSASendTo(_In_ SOCKET s,
    _In_reads_(dwBufferCount) LPWSABUF lpBuffers,
    _In_ DWORD dwBufferCount,
    _Out_opt_ LPDWORD lpNumberOfBytesSent,
    _In_ DWORD dwFlags,
    _In_reads_bytes_opt_(iTolen) const struct sockaddr FAR * lpTo,
    _In_ int iTolen,
    _Inout_opt_ LPWSAOVERLAPPED lpOverlapped,
    _In_opt_ LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
 
RHO_GLOBAL int listen(SOCKET s, int backlog);

RHO_GLOBAL int ioctlsocket(_In_ SOCKET s, _In_ long cmd, _Inout_ u_long FAR * argp);

#endif