#pragma once

#include "common/ThreadQueue.h"

class CGeneratorQueue : public rho::common::CThreadQueue
{
public:

    class CGeneratorQueueCommand: public rho::common::CThreadQueue::IQueueCommand
    {
        rho::common::IRhoRunnable* m_pFunctor;
    public:
        CGeneratorQueueCommand(rho::common::IRhoRunnable* pFunctor) : m_pFunctor(pFunctor){}
        virtual ~CGeneratorQueueCommand(){};
        virtual bool equals(const IQueueCommand& cmd)
        {
            return false;
        }
        virtual rho::String toString()
        {
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
        //TODO: use log category from CBarcode1
        //CThreadQueue::setLogCategory(getLogCategory());

        setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);
    }
};

