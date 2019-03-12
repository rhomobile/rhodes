#pragma once


namespace rho {
namespace ruby {


class IRefCountedObject {

public:
    virtual ~IRefCountedObject() = 0;

    virtual void addRef() = 0;

    virtual void release() = 0;

};

}
}
