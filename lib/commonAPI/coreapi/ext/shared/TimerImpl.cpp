//
//  TimerImpl.cpp
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoTime.h"
#include "generated/cpp/TimerBase.h"
#include "logging/RhoLog.h"
#include <stdlib.h>

extern "C" void rho_ios_log_console_output(const char* message);



#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "TimerImpl"


namespace rho
{
	class CTimerFactory;

    using namespace apiGenerator;
    using namespace common;

    class CTimerSingletonImpl: public CTimerSingletonBase
    {
		static unsigned int createdTimers;

    public:

        CTimerSingletonImpl(): CTimerSingletonBase(){}

        //methods
        // create Create a timers objects.
        virtual void create(rho::apiGenerator::CMethodResult& oResult)
		{
			++createdTimers;

			char buf[10];
            sprintf(buf, "%d", createdTimers);
			oResult.set(buf);
        }
        // enumerate This is documentation
        virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) { }
    };

	unsigned int  CTimerSingletonImpl::createdTimers = 0;

    class CTimerImpl : public CTimerBase, public common::CRhoTimer::ICallback
    {
	private:
		class TimerResultThread : public CRhoThread
		{
		private:
			rho::apiGenerator::CMethodResult m_oResult;

		public:
			TimerResultThread(const rho::apiGenerator::CMethodResult& oResult) : m_oResult(oResult) {}

			virtual void run()
			{

				while ( !RHODESAPP().isApplicationActive() ) {
					wait(10);
				}

				m_oResult.set("");
			}
		};

	private:
		rho::String                      m_timerID;
		rho::apiGenerator::CMethodResult m_oResult;
		TimerResultThread*               m_answerThread;

    public:
		CTimerImpl(const rho::String& timerID) : m_timerID(timerID), m_answerThread(0) {}
		virtual ~CTimerImpl()
		{
			if (!m_answerThread)
				delete m_answerThread;
		}

        //methods
        virtual void start( int interval, rho::apiGenerator::CMethodResult& oResult)
		{
			if (interval <= 0)
			{
                LOG(ERROR) + "$NetRequestProcess$ PRE SET ERROR CTimerImpl::start";
				oResult.setError("invalid interval");
                LOG(ERROR) + "$NetRequestProcess$ POST SET ERROR CTimerImpl::start";

                return;
			}

			m_oResult = oResult;
			common::CRhoTimer& timerManager = RHODESAPP().getTimer();
			timerManager.addNativeTimer(interval, this);
        }

        virtual void stop(rho::apiGenerator::CMethodResult& oResult)
		{
			common::CRhoTimer& timerManager = RHODESAPP().getTimer();
			timerManager.stopNativeTimer(this);
        }

        virtual void isAlive(rho::apiGenerator::CMethodResult& oResult)
		{
			common::CRhoTimer& timerManager = RHODESAPP().getTimer();
			oResult.set(timerManager.isNativeTimerExist(this));
        }

		virtual bool onTimer()
        {
            if (!m_answerThread) {
                m_answerThread = new TimerResultThread(m_oResult);
			}

			m_answerThread->start(IRhoRunnable::epNormal);

			return true;
		}
    };

    ////////////////////////////////////////////////////////////////////////

    class CTimerFactory: public CTimerFactoryBase
	{
    public:
        CTimerFactory(){}

        ITimerSingleton* createModuleSingleton()
        {
            return new CTimerSingletonImpl();
        }

        virtual ITimer* createModuleByID(const rho::String& strID){
			return new CTimerImpl(strID);
		};

		unsigned int getAliveTimers() const
		{
			return m_hashModules.size();
		}
    };

}

extern "C" void Init_Timer_extension()
{
    rho::CTimerFactory::setInstance( new rho::CTimerFactory() );
    rho::Init_Timer_API();
#ifndef RHO_NO_RUBY_API
    RHODESAPP().getExtManager().requireRubyFile("RhoTimerApi");
#endif
}
