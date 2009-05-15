#ifndef _RHOINPUTSTREAM_H_
#define _RHOINPUTSTREAM_H_

namespace rho{
namespace common{

struct InputStream
{
    virtual ~InputStream(void){}

    virtual  int available() = 0;

    virtual  int read() = 0;
    virtual  int read(unsigned char* buffer, int bufOffset, int bytesToRead) = 0;

    virtual  void reset() = 0;
};

}
}

#endif //_RHOINPUTSTREAM_H_
