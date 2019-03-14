#pragma once


namespace rho {
namespace ruby {


class IRefCountedObject {

public:
    virtual ~IRefCountedObject() {}

    virtual void addRef() = 0;

    virtual void release() = 0;

};

}
}
