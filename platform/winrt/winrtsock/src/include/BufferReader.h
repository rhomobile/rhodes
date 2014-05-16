#pragma once
#include "MiscUtils.h"

class BufferReader
{
public:
    BufferReader();
    void Attatch(Windows::Storage::Streams::IBuffer^ buffer);

    int UnconsumedBytes();

    int Read(const unsigned char* pBuf, int length);

    bool HasBuffer();

    void Detatch();
private:
    Windows::Storage::Streams::IBuffer^ m_buffer;
    unsigned int m_currentPosition;
    unsigned char* m_pBuffer;
    unsigned int m_bufferLength;
};


