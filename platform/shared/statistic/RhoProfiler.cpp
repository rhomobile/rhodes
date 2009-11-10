#include "RhoProfiler.h"
#include "logging/RhoLog.h"
#include "common/RhoTime.h"
#include <map>

namespace rho {
class CProfiler
{
    DEFINE_LOGCLASS;

    class CCounter
    {
        common::CTimeInterval m_startTime;
        bool m_bWasStarted;
    public:
        CCounter(bool bStart=true) : m_bWasStarted(false){ 
            if ( bStart )
                start();     
        }

        virtual bool isGlobal()const{ return false;}

        virtual void start(){ m_startTime = common::CTimeInterval::getCurrentTime(); m_bWasStarted = true; }
        virtual common::CTimeInterval stop(){
            if ( m_startTime.isEmpty() )
                return m_startTime;

            common::CTimeInterval res = common::CTimeInterval::getCurrentTime()-m_startTime;
            m_startTime = common::CTimeInterval();
            return res;
        }
        virtual common::CTimeInterval flush(){ return stop(); }

        bool isWasStarted()const{ return m_bWasStarted;}
    };

    class CGlobalCounter : public CCounter
    {
        common::CTimeInterval m_sumGlobal;
    public:
        CGlobalCounter() : CCounter(false){ }
        virtual bool isGlobal()const{ return true;}

        virtual common::CTimeInterval stop(){
            m_sumGlobal += CCounter::stop();

            return m_sumGlobal;
        }
        virtual common::CTimeInterval flush(){ 
            common::CTimeInterval res = stop(); 
            m_sumGlobal = common::CTimeInterval();
            return res;
        }

    };

    std::map<String,CCounter*> m_mapCounters;
public:
    CProfiler(){}
    ~CProfiler(){}

    void startCounter(const char* szCounterName);
    void stopCounter(const char* szCounterName, bool bDestroy =false);
    void createCounter(const char* szCounterName);
    void destroyCounter(const char* szCounterName);
    void flushCounter(const char* szCounterName, const char* msg);

};
IMPLEMENT_LOGCLASS(CProfiler,"PROFILER");

void CProfiler::startCounter(const char* szCounterName)
{
    CCounter* pCounter = m_mapCounters[szCounterName];
    if ( !pCounter || !pCounter->isWasStarted() )
        LOG(INFO) + szCounterName + " : START";

    if ( !pCounter )
        m_mapCounters[szCounterName] = new CCounter(); 
    else
        pCounter->start();
}

void CProfiler::stopCounter(const char* szCounterName, bool bDestroy /*=false*/)
{
    CCounter* pCounter = m_mapCounters[szCounterName];
    if ( !pCounter ){
        LOG(ERROR) + szCounterName + " : Cannot find counter.";
        return;
    }

    if ( bDestroy || !pCounter->isGlobal() )
    {
        common::CTimeInterval oInterval = pCounter->stop();
        LOG(INFO) + szCounterName + " (" + oInterval.toString() + ") : STOP";

        m_mapCounters.erase(szCounterName);
    }else
        pCounter->stop();

}

void CProfiler::flushCounter(const char* szCounterName, const char* msg)
{
    CCounter* pCounter = m_mapCounters[szCounterName];
    if ( !pCounter ){
        LOG(ERROR) + szCounterName + " : Cannot find counter.";
        return;
    }

    common::CTimeInterval oInterval = pCounter->flush();
    LOG(INFO) + szCounterName + (msg && *msg ? " - " : "" ) + (msg && *msg ? msg : "" ) +
        " (" + oInterval.toString() + ") : STOP";
}

void CProfiler::createCounter(const char* szCounterName)
{
    m_mapCounters[szCounterName] = new CGlobalCounter(); 
}

void CProfiler::destroyCounter(const char* szCounterName)
{
    stopCounter( szCounterName, true );
}

}

static rho::CProfiler g_oProfiler;
extern "C"{

void rhoStartProfilerCounter(const char* file, int line, const char* szCounterName )
{
    g_oProfiler.startCounter(szCounterName);
}

void rhoStopProfilerCounter(const char* file, int line, const char* szCounterName )
{
    g_oProfiler.stopCounter(szCounterName);
}

void rhoCreateProfilerCounter(const char* file, int line, const char* szCounterName )
{
    g_oProfiler.createCounter(szCounterName);
}

void rhoDestroyProfilerCounter(const char* file, int line, const char* szCounterName )
{
    g_oProfiler.destroyCounter(szCounterName);
}

void rhoFlushProfilerCounter(const char* file, int line, const char* szCounterName, const char* msg )
{
    g_oProfiler.flushCounter(szCounterName, msg );
}

}
