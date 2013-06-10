#include <common/RhodesApp.h>
#include "../../../shared/generated/cpp/LedBase.h"
namespace rho {
	using namespace apiGenerator;
	using namespace common;
	using namespace notification;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Led"


class CLedImpl: public IRhoExtension, public CLedBase
{
public: 
	CLedImpl(const rho::String& strID): CLedBase()
    {
    }
	   
	~CLedImpl()
	{
	}

	void getName(rho::apiGenerator::CMethodResult& oResult)
	{}

    void illuminate(rho::apiGenerator::CMethodResult& oResult)
	{}

    void extinguish(rho::apiGenerator::CMethodResult& oResult)
	{}

    void flash( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{}

};

class CLedSingleton: public CLedSingletonBase
{
public:
	CLedSingleton(): CLedSingletonBase(){}
	~CLedSingleton()
	{
	}
    void enumerate(rho::apiGenerator::CMethodResult& oResult);

};

class CLedFactory: public CLedFactoryBase
{
	~CLedFactory(){}
    virtual ILedSingleton* createModuleSingleton();
    virtual ILed* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Led_extension()
{
    CLedFactory::setInstance( new CLedFactory() );
    Init_Led_API();
}

ILed* CLedFactory::createModuleByID(const rho::String& strID)
{
    return new CLedImpl(strID);
}

ILedSingleton* CLedFactory::createModuleSingleton()
{
    CLedSingleton* pLedSingleton = new CLedSingleton();
	return pLedSingleton;
}

void CLedSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
/*	if (g_deviceManufacturer == SCANNER_MOTOROLA)
	{
		SCAN_FINDINFO scanFindInfo;
		memset(&scanFindInfo, 0, sizeof(scanFindInfo));
		scanFindInfo.StructInfo.dwAllocated = sizeof(scanFindInfo);
		HANDLE scanFindHandle = NULL;
		DWORD dwResult = 
			lpfn_SCAN_FindFirst(&scanFindInfo, &scanFindHandle);
		while(dwResult == E_SCN_SUCCESS)
		{
			//  Remove Comma from end of Device Name
			TCHAR szModifiedDeviceName[MAX_DEVICE_NAME + 1];
			wcscpy(szModifiedDeviceName, scanFindInfo.szDeviceName);
			if (wcslen(szModifiedDeviceName) > 0 &&
				szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] == L':')
				szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] = L'\0';
			arIDs.addElement(convertToStringA(szModifiedDeviceName));
			dwResult = lpfn_SCAN_FindNext(&scanFindInfo, scanFindHandle);
		}
		lpfn_SCAN_FindClose(scanFindHandle);	
	}
  */  //oResult.setParamName("scannerArray");
	oResult.set(arIDs);
}





}