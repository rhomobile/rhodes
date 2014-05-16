
#include <string>
#include <wrl.h>
#include <ppl.h>
#include <ppltasks.h>
#include <concurrent_unordered_map.h>
#include "include\InternalSocketManagement.h"

using namespace std;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Networking;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;
using namespace Microsoft::WRL::Wrappers;


static concurrent_unordered_map<SOCKET, shared_ptr<InternalSocketHandle>> g_sockets;

static SRWLOCK g_socketLock = SRWLOCK_INIT;
static DWORD FLS_SLOT = 0;
static unsigned long long winrt_sock_init_count = 0;
static unsigned long long socket_handle_count = 0;

struct winrt_sock_fls_data
{
    winrt_sock_fls_data() : last_error(0)
    {
        global_hostent.h_addr_list = (char **) malloc(MAX_HOST_ENTRY_LIST_ITEMS * sizeof(char *) );

        for (int i = 0; i < MAX_HOST_ENTRY_LIST_ITEMS; i++)
        {
            global_hostent.h_addr_list[i] = (char *) malloc(sizeof(struct in_addr));
        }

        global_hostent.h_name = nullptr;
    }
    
    ~winrt_sock_fls_data()
    {
        if (global_hostent.h_name)
            delete [] global_hostent.h_name;

        for (int i = 0; i < MAX_HOST_ENTRY_LIST_ITEMS; i++)
        {
            if (global_hostent.h_addr_list[i])
                free(global_hostent.h_addr_list[i]);
        }
        
        free(global_hostent.h_addr_list);
    }

    hostent global_hostent;
    int last_error;
    char inet_ntoa_string[MAX_IP_ADDRESS_CHAR_LEN];
};

void __stdcall FlsDealloc(void * pVoid)
{
    if (!pVoid)
        return;

    auto flsData = static_cast<winrt_sock_fls_data*>(FlsGetValue(FLS_SLOT));
    delete flsData;
}

winrt_sock_fls_data* ensure_fls_data()
{
    auto flsData = FlsGetValue(FLS_SLOT);

    if (!flsData)
    {
        flsData = new winrt_sock_fls_data;
        FlsSetValue(FLS_SLOT, flsData);
    }

    return static_cast<winrt_sock_fls_data*>(flsData);
}

int GlobalSettings::GetLastError()
{
    auto flsData = ensure_fls_data();

    return flsData->last_error;
}

void GlobalSettings::SetLastError(int errono)
{
    auto flsData = ensure_fls_data();

    flsData->last_error = errono;
}

bool GlobalSettings::InitializeLibrary()
{
    auto initCount = InterlockedIncrement(&winrt_sock_init_count);

    //Add thread sync
    if (initCount == 1)
    {
        FLS_SLOT = FlsAlloc(FlsDealloc);
    }

    auto profile = NetworkInformation::GetInternetConnectionProfile();
    auto interfaceType = profile->NetworkAdapter->IanaInterfaceType;
    // 71 is WiFi & 6 is Ethernet
    if (interfaceType != 71 && interfaceType != 6 && interfaceType != 23)
    {
        return false;
    }

    return true;
}

void GlobalSettings::UninitializeLibrary()
{
    auto initCount = InterlockedDecrement(&winrt_sock_init_count);
    
    if (initCount == 0)
    {
        FlsDealloc(FlsDealloc);
    }
    else if (initCount < 0)
    {
        initCount = 0;
    }
}

bool GlobalSettings::IsLibraryInitialized()
{
    return winrt_sock_init_count > 0;
}

char* GlobalSettings::Get_inet_ntoa_String()
{
    auto flsData = ensure_fls_data();

    return flsData->inet_ntoa_string;
}

HOSTENT* GlobalSettings::GetHostEntry()
{
    auto flsData = ensure_fls_data();
    return &flsData->global_hostent;
}

std::shared_ptr<InternalSocketHandle> SocketManagement::CreateSocket(InternalSocketType socketType)
{
    auto socket_handle_number = InterlockedIncrement(&socket_handle_count);

    auto socketHandle = make_shared<InternalSocketHandle>((int)socket_handle_number);
    socketHandle->SetSocketType(socketType);

    SRWLock lock;
    lock.LockExclusive(&g_socketLock);

    g_sockets.insert(make_pair((SOCKET)socket_handle_number, socketHandle));

    return socketHandle;
}

std::shared_ptr<InternalSocketHandle> SocketManagement::GetSocket(SOCKET s)
{
    SRWLock lock;
    auto lck = lock.LockExclusive(&g_socketLock);
   
    if (g_sockets.find(s) == g_sockets.end())
    {
        return nullptr;
    }

    return g_sockets[s];
}

void SocketManagement::CloseSocket(SOCKET s)
{
    if (g_sockets.find(s) == g_sockets.end())
    {
        return;
    }

    auto socketHandle = g_sockets[s];

    auto socket = socketHandle->GetSocket();

    /* prob unneeded */
    socket->Close();
   
    SRWLock lock;
    auto lck = lock.LockExclusive(&g_socketLock);

    g_sockets.unsafe_erase(g_sockets.find(s));
}

std::shared_ptr<InternalSocketHandle> SocketManagement::CreateSocketHandleFromStreamSocket(Windows::Networking::Sockets::StreamSocket^ socket)
{
    auto socket_handle_number = InterlockedIncrement(&socket_handle_count);

    auto socketHandle = make_shared<InternalSocketHandle>((int) socket_handle_number);
    socketHandle->SetSocketType(InternalSocketType::Stream);

    socketHandle->SetSocket(socket);

    SRWLock lock;
    auto lck = lock.LockExclusive(&g_socketLock);

    g_sockets.insert(make_pair((SOCKET) socket_handle_number, socketHandle));

    return socketHandle;
}
