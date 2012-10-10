#include "winsock_wp8.h"

/*---------------- winsock functions. ---------------------------- */

RHO_GLOBAL int WSASendTo(_In_ SOCKET s,
    _In_reads_(dwBufferCount) LPWSABUF lpBuffers,
    _In_ DWORD dwBufferCount,
    _Out_opt_ LPDWORD lpNumberOfBytesSent,
    _In_ DWORD dwFlags,
    _In_reads_bytes_opt_(iTolen) const struct sockaddr FAR * lpTo,
    _In_ int iTolen,
    _Inout_opt_ LPWSAOVERLAPPED lpOverlapped,
    _In_opt_ LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	return 0;
}
 
RHO_GLOBAL int listen(SOCKET s, int backlog)
{
	return 0;
}

RHO_GLOBAL int ioctlsocket(_In_ SOCKET s, _In_ long cmd, _Inout_ u_long FAR * argp)
{
	return 0;
}
