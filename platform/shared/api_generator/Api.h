#pragma once

#include <string>
#include <memory>
#if __cplusplus == 201103L
#include <unordered_map>
#define MAP_TEMPLATE std::unordered_map
#else
#include <hash_map>
#define MAP_TEMPLATE stdext::hash_map
#endif



namespace rho {
namespace apiGenerator {

template <typename FUNCTOR>
class ApiHandler
{
public:
    typedef FUNCTOR FunctorType;
public:
    ApiHandler(const char* pcszModuleId): m_pcszModule(pcszModuleId) {}
    virtual ~ApiHandler(){}

    virtual void initialize() = 0;


    const char* getModuleId() { return m_pcszModule; }

    FunctorType getStaticMethod(const std::string& methodId)
    {
        if(!m_pStaticMethods.get())
        {
            initialize();
        }
        return (*m_pStaticMethods)[methodId];
    }

    FunctorType getInstanceMethod(const std::string& methodId)
    {
        if(!m_pStaticMethods.get())
        {
            initialize();
        }
        return (*m_pInstanceMethods)[methodId];
    }

protected:
    void defineStaticMethod(const char* strMethodId, FunctorType functor)
    {
        (*m_pStaticMethods)[strMethodId] = functor;
    }
    void defineInstanceMethod(const char* strMethodId, FunctorType functor)
    {
        (*m_pInstanceMethods)[strMethodId] = functor;
    }

    typedef MAP_TEMPLATE<std::string, FunctorType> MethodMap;

    const char* m_pcszModule;
    std::auto_ptr<MethodMap> m_pStaticMethods;
    std::auto_ptr<MethodMap> m_pInstanceMethods;

};


}}
