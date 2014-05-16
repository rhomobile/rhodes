#pragma once

#include <string>
#include <wrl.h>
#include <Robuffer.h>
#include <assert.h>
#include "..\include\winrtsock.h"
#include "inet_aton.h"


inline std::wstring SlimToWideString(unsigned char* charString)
{
    auto slimString = std::string(reinterpret_cast<char*>(charString));
    return std::wstring(begin(slimString), end(slimString));
}

inline Platform::String^ CharToPlatformString(unsigned char * const charString)
{
    auto wideString = SlimToWideString(charString);

    const wchar_t* w_char = wideString.c_str();
    return ref new Platform::String(w_char);
}

inline Microsoft::WRL::ComPtr<Windows::Storage::Streams::IBufferByteAccess> GetBufferByteAccess(Windows::Storage::Streams::IBuffer^ buffer)
{
    auto pUnk = reinterpret_cast<IUnknown*>(buffer);

    Microsoft::WRL::ComPtr<Windows::Storage::Streams::IBufferByteAccess> comBuff;
    pUnk->QueryInterface(__uuidof(Windows::Storage::Streams::IBufferByteAccess), (void**)comBuff.ReleaseAndGetAddressOf());

    return comBuff;
}


inline void GetRawBufferFromIBuffer(Windows::Storage::Streams::IBuffer^ buffer, unsigned char** pbuffer)
{
    Platform::Object^ obj = buffer;
    Microsoft::WRL::ComPtr<IInspectable> insp(reinterpret_cast<IInspectable*>(obj));
    Microsoft::WRL::ComPtr<Windows::Storage::Streams::IBufferByteAccess> bufferByteAccess;
    insp.As(&bufferByteAccess);
    bufferByteAccess->Buffer(pbuffer);
}

inline void CopyToIBuffer(unsigned char* buffSource, unsigned int copyByteCount, Windows::Storage::Streams::IBuffer^ buffer, unsigned int writeStartPos = 0)
{
    auto bufferLen = buffer->Capacity;
    assert(copyByteCount <= bufferLen);

    unsigned char* pBuffer;

    GetRawBufferFromIBuffer(buffer, &pBuffer);

    memcpy(pBuffer + writeStartPos, buffSource, copyByteCount);
}

inline void CopyFromIBuffer(unsigned char* buffDestination, unsigned int copyByteCount, Windows::Storage::Streams::IBuffer^ buffer, unsigned int readStartPos = 0)
{
    assert(copyByteCount <= buffer->Capacity);

    unsigned char* pBuffer;

    GetRawBufferFromIBuffer(buffer, &pBuffer);

    memcpy(buffDestination, pBuffer + readStartPos, copyByteCount);
}

inline std::wstring GetWideAddressStringFromSocketAddress(sockaddr_in const * socketIn)
{
    auto charAddress = inet_ntoa(socketIn->sin_addr);
    std::wstring address = SlimToWideString((unsigned char*)charAddress);
    return address;
}

inline void FillSockAddrIn(sockaddr_in* sockAddrIn, std::wstring const& address, int port)
{
    char ipChar[16];
    const wchar_t * ipWide = address.c_str();
    int stringIpCharLen = address.length();

    wcstombs(ipChar, ipWide, stringIpCharLen);
    ipChar[stringIpCharLen] = 0;

    sockAddrIn->sin_port = htons((u_short) port);
    sockAddrIn->sin_addr.S_un.S_addr = inet_addr(ipChar);
}

   