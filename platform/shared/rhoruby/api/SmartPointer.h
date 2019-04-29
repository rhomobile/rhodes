#pragma once

namespace rho {
namespace ruby {


    
// autorelease IObject* pointers
    
template <typename T>
class SmartPointer {
    T *m_obj;
public:
    SmartPointer(T *obj)
        : m_obj(obj)
    { }
    ~SmartPointer() {
        m_obj->release();
    }
    T* getPointer() { return m_obj; }
    T* operator->() { return m_obj; }
    T& operator* () { return *m_obj; }
};


}
}
