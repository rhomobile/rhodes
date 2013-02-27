#pragma once

#include "common/ThreadQueue.h"
#include "common/AutoPointer.h"
#include "MethodResult.h"

namespace rho
{
namespace apiGenerator
{

class CGeneratorQueue : public rho::common::CThreadQueue
{
public:

    class CGeneratorQueueCommand: public rho::common::CThreadQueue::IQueueCommand
    {
        rho::common::CInstanceClassFunctorBase<CMethodResult>* m_pFunctor;
    public:
        CGeneratorQueueCommand(rho::common::CInstanceClassFunctorBase<CMethodResult>* pFunctor) : m_pFunctor(pFunctor){}
        virtual ~CGeneratorQueueCommand(){};
        virtual bool equals(const IQueueCommand& cmd)
        {
            return false;
        }
        virtual rho::String toString()
        {
            //TODO: toString
            return "";
        }
        virtual void cancel(){}

        virtual void execute()
        {
            m_pFunctor->runObject();
        }
    };

    CGeneratorQueue() : CThreadQueue()
    {
        setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);
    }
};

}
}