
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"
#include "include\IRWSocket.h"

using namespace std;

unsigned long int timevalTomiliseconds(const timeval* time)
{
    return ((time->tv_sec * 1000) + (time->tv_usec / 1000));
}

int __stdcall select(int /*nfds*/, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout)
{
    if (!GlobalSettings::IsLibraryInitialized())
    {
        WSASetLastError(WSANOTINITIALISED);
        return SOCKET_ERROR;
    }

    int socketStatusFoundCount = 0;
    int totalMsSlept = 0;
    int timeoutMs = timevalTomiliseconds(timeout);

    bool finished = false;

    //Probably should memcpy these based on fd_count
    //just in case FD_SETSIZE is defined different from when
    //we compile it
    fd_set local_readfds = *readfds;
    fd_set local_writefds = *writefds;
    fd_set local_exceptfds = *exceptfds;

    while (!finished)
    {
        int fdCount = local_readfds.fd_count;

        for (int i = 0; i < fdCount; i++)
        {
            if (local_readfds.fd_array[i] != 0)
            {
                auto socketHandle = SocketManagement::GetSocket(local_readfds.fd_array[i]);

                if (socketHandle != nullptr)
                {
                    auto socket = socketHandle->GetSocketAsNoRef<IRWSocket>();

                    if (socket != nullptr)
                    {
                        if (socket->GetLoadedBytes() == 0)
                        {
                            readfds->fd_array[i] = 0;
                        }
                        else
                        {
                            readfds->fd_array[i] = local_readfds.fd_array[i];
                            socketStatusFoundCount++;
                        }
                    }
                }
            }
        }

        fdCount = 0;
        fdCount = local_writefds.fd_count;

        for (int i = 0; i < fdCount; i++)
        {
            if (local_writefds.fd_array[i] != 0)
            {
                auto socketHandle = SocketManagement::GetSocket(local_writefds.fd_array[i]);
                if (socketHandle != nullptr)
                {
                    auto socket = socketHandle->GetSocketAsNoRef<IRWSocket>();

                    if (socket != nullptr)
                    {
                        if (socket->GetConnectionStatus() != InternalSocketConnectionStatus::Connected)
                        {
                            writefds->fd_array[i] = 0;
                        }
                        else
                        {
                            writefds->fd_array[i] = local_writefds.fd_array[i];
                            socketStatusFoundCount++;
                        }
                    }
                }
            }
        }

        fdCount = 0;
        fdCount = local_exceptfds.fd_count;

        for (int i = 0; i < fdCount; i++)
        {
            if (local_exceptfds.fd_array[i] != 0)
            {
                auto socketHandle = SocketManagement::GetSocket(local_exceptfds.fd_array[i]);
                if (socketHandle != nullptr)
                {
                    auto socket = socketHandle->GetSocketAsNoRef<IRWSocket>();

                    if (socket != nullptr)
                    {
                        InternalSocketConnectionStatus status = socket->GetConnectionStatus();

                        if (status != InternalSocketConnectionStatus::ForcablyClosed && 
                            status != InternalSocketConnectionStatus::Refused        && 
                            status != InternalSocketConnectionStatus::TimedOut       && 
                            status != InternalSocketConnectionStatus::Unreachable)
                        {
                            exceptfds->fd_array[i] = 0;
                        }
                        else
                        {
                            exceptfds->fd_array[i] = local_exceptfds.fd_array[i];
                            socketStatusFoundCount++;
                        }
                    }
                }
            }
        }

        if (socketStatusFoundCount || totalMsSlept >= timeoutMs)
            finished = true;
        else
        {
            DWORD sleepTime = min(timeoutMs, 16);
            Sleep(sleepTime);
            totalMsSlept += sleepTime;
        }
    }

    WSASetLastError(0);
    return socketStatusFoundCount;
}