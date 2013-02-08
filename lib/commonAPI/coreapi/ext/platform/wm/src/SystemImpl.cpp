#include "../../../shared/SystemImplBase.h"

#include "common/RhoConf.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"

#include <windows.h>

namespace rho {

using namespace apiGenerator;

class CSystemImpl: public CSystemImplBase
{
public:
    CSystemImpl(const rho::StringW& strID): CSystemImplBase(strID){}

    void getOsVersion(CMethodResult& oResult);
    void getIsEmulator(CMethodResult& oResult);
};

void CSystemImpl::getOsVersion(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    CSystemImplBase::getOsVersion(oResult)
#else
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(osv);
	if (!GetVersionEx(&osv))
    {
        LOG(ERROR) + "GetVersionEx failed.";
		oResult.set(L"0");
    }
    else
    {
	    wchar_t buf[50];
        memset(buf, 0, sizeof(buf));
	    _snwprintf(buf, sizeof(buf) - 1, L"%u.%u.%u", (unsigned)osv.dwMajorVersion,
		    (unsigned)osv.dwMinorVersion, (unsigned)osv.dwBuildNumber);

        oResult.set(StringW(buf));
    }
#endif
}

void CSystemImpl::getIsEmulator(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    CSystemImplBase::getIsEmulator(oResult);
#else

#ifdef OS_WINDOWS_DESKTOP
    oResult.set(true);
#else

	TCHAR buf[255];
	SystemParametersInfo(SPI_GETOEMINFO, 255, &buf, NULL);
	bool bEmulator = (wcsnicmp(buf, _T("Microsoft DeviceEmulator"), 24)==0);

    oResult.set(bEmulator);
#endif

#endif
}

////////////////////////////////////////////////////////////////////////
class CSystemSingleton: public CModuleSingletonBase<ISystemSingleton>
{
public:
    ~CSystemSingleton(){}
    rho::StringW getInitialDefaultID(){return L"1";}
};

class CSystemFactory: public CSystemFactoryBase
{
public:
    ~CSystemFactory(){}

    ISystemSingleton* createModuleSingleton(){ return new CSystemSingleton(); }
    ISystem* createModuleByID(const rho::StringW& strID){ return new CSystemImpl(strID); }
};

extern "C" void Init_System_extension()
{
    CSystemFactory::setInstance( new CSystemFactory() );
    Init_System_API();
}

}