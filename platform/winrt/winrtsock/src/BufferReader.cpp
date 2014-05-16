
#include "include\BufferReader.h"

using namespace Windows::Storage::Streams;

BufferReader::BufferReader() : m_currentPosition(0), m_pBuffer(nullptr), m_bufferLength(0)
{

}

void BufferReader::Attatch(IBuffer^ buffer)
{
    m_buffer = buffer;
    m_bufferLength = buffer->Length;
    m_currentPosition = 0;
    GetRawBufferFromIBuffer(buffer, &m_pBuffer);
}

int BufferReader::UnconsumedBytes()
{
    return m_bufferLength - m_currentPosition;
}

int BufferReader::Read(const unsigned char* pBuf, int length)
{
    int bytesLeft = UnconsumedBytes();

    if (bytesLeft == 0 || length == 0)
        return 0;

    length = min(length, bytesLeft);

    memcpy((void*) pBuf, m_pBuffer + m_currentPosition, length);

    m_currentPosition += length;

    return length;
}

bool BufferReader::HasBuffer()
{
    return m_buffer != nullptr;
}

void BufferReader::Detatch()
{
    m_bufferLength = 0;
    m_currentPosition = 0;
    m_buffer = nullptr;
    m_pBuffer = nullptr;
}
