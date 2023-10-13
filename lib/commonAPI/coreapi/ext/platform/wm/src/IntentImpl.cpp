#include "../../../shared/IntentImplBase.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"
#include "common/RhoFilePath.h"
#include "Intents.h"
#include "SystemImpl.h"

extern UINT WM_INTENTMSG;

namespace rho {

	using namespace apiGenerator;
	using namespace common;

	class CIntentSingletonImpl: public CIntentImplBase
	{
	public:

		CIntentSingletonImpl(): CIntentImplBase(){}

		virtual void send( const Hashtable<String, String>& params, apiGenerator::CMethodResult& oResult) {
			String appName = params.get("appName");
			String data = params.get("data");
			String uri = params.get("uri");
			if (appName.length() > 0)
				sendApplicationMessage(appName, data, oResult);
			else if (uri.length() > 0)
				CSystemFactory::getSystemSingletonS()->openUrl(uri, oResult);
			else
				oResult.setError("Intent sending error: no 'appName' or 'uri' specified");
		} 

		virtual void startListening(apiGenerator::CMethodResult& oResult) {
			startApplicationMessageNotifications(oResult);
		} 

		virtual void stopListening(apiGenerator::CMethodResult& oResult) {
			stopApplicationMessageNotifications(oResult);
		} 
		virtual void cleanStartParameters(rho::apiGenerator::CMethodResult& oResult){
			
		}

	private:
		void sendApplicationMessage(const String& appName, const String& params, apiGenerator::CMethodResult& oResult) {
			CFilePath oPath(appName);    
			String appNamePath = oPath.getBaseName();
			appNamePath.resize(appNamePath.length() - 4);

			COPYDATASTRUCT cds;

			String strAppName = appNamePath + ".MainWindow";
			StringW strAppNameW = convertToStringW(strAppName);

			HWND appWindow = FindWindow(strAppNameW.c_str(), NULL);

			if (appWindow == NULL) {
				apiGenerator::CMethodResult runResult;
				static_cast<CSystemImpl*>(CSystemFactory::getSystemSingletonS())->runApplicationShowWindow(appName, "", false, false, runResult);

				if (runResult.isError()) {
					oResult.setError(runResult.getErrorString());
					return;
				}

				int maxTimeout = 5000;
				int currTime = 0;

				for (currTime = 0; currTime < maxTimeout; currTime += 100) {
					Sleep(100);
					appWindow = FindWindow(strAppNameW.c_str(), NULL);
					if (appWindow != NULL)
						break;
				}

				waitIntentEvent(appName);

				appWindow = FindWindow(strAppNameW.c_str(), NULL);

				if (appWindow == NULL) {
					oResult.setError("application is not running");
					return;
				}
			}

			InterprocessMessage msg;
			strcpy(msg.params, params.c_str());
			strcpy(msg.appName, appName.c_str());

			cds.dwData = COPYDATA_INTERPROCESSMESSAGE;
			cds.cbData = sizeof(InterprocessMessage);
			cds.lpData = &msg;

			SendMessage(appWindow, WM_INTENTMSG, (WPARAM)(HWND)0, (LPARAM) (LPVOID) &cds);
		}

	};

	class CIntentImpl : public CIntentBase
	{
	public:
		virtual ~CIntentImpl() {}
	};

	////////////////////////////////////////////////////////////////////////

	class CIntentFactory: public CIntentFactoryBase    {
	public:
		CIntentFactory(){}

		IIntentSingleton* createModuleSingleton()
		{ 
			return new CIntentSingletonImpl();
		}

		virtual IIntent* createModuleByID(const String& strID){ return new CIntentImpl(); };

	};

}

extern "C" void Init_Intent()
{
    rho::CIntentFactory::setInstance( new rho::CIntentFactory() );
    rho::Init_Intent_API();
    
}