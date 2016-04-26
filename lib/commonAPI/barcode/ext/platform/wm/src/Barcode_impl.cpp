#include <common/RhodesApp.h>
#include "../../../shared/generated/cpp/BarcodeBase.h"
#include "Scanner.h"
namespace rho {
	using namespace apiGenerator;
	using namespace common;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode"

enum deviceManufacturer_T {SCANNER_SYMBOL, SCANNER_INTERMEC, SCANNER_HONEYWELL, SCANNER_OTHER};
deviceManufacturer_T g_deviceManufacturer;

class CBarcodeImpl: public IRhoExtension, public CBarcodeBase
{
public: 
	CScanner *m_pScanner;
	CBarcodeImpl(const rho::String& strID): CBarcodeBase()
    {
		m_hashProps.put( "ID", strID);
		if (g_deviceManufacturer == SCANNER_SYMBOL)
		{
			//  For Symbol Scanners the ID will be in the form "SCNX" where
			//  X is a number
			int iInstance = 0;
			if (strID.length() > 3)
				iInstance = atoi(strID.c_str() + 3);
			LOG(INFO) + "Initialising Scanner Interface for Scanner " + strID; 
			RHODESAPP().getExtManager().registerExtension(/*convertToStringA*/(strID), this );
			m_pScanner = new CScanner(iInstance);
			if (m_pScanner){
				m_pScanner->Initialise(true, rho::common::convertToStringW(strID).c_str());
				// Modified by Abhineet Agarwal
				// Invoking the below method to check whether battery is low or critical
				// and whether or not, the scanner should be disabled.
				m_pScanner->StartOrDisableCheckingLowBattery();
			}
		}
		else
		{
			//  Unsupported Scanner implementation
			m_pScanner = NULL;
		}
    }
	   
	~CBarcodeImpl()
	{
		DEBUGMSG(true, (L"\nShutting down Scanner\n"));
		if (m_pScanner)
		{
			delete m_pScanner;
			m_pScanner = NULL;
		}
		DEBUGMSG(true, (L"\nScanner is shut down\n"));
	}

    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		DEBUGMSG(TRUE, (L"Setting Property %s to %s", convertToStringW(propertyName).c_str(), convertToStringW(propertyValue).c_str()));
		BOOL bRet = m_pScanner->SetPropertyOrMethod(convertToStringW(propertyName).c_str(), convertToStringW(propertyValue).c_str());
		oResult.set(bRet == TRUE);
	}

    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{
		//  Set multiple properties
		if (!m_pScanner) {return;}
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			DEBUGMSG(TRUE, (L"Setting Property %s value %s", convertToStringW(iterator->first).c_str(), convertToStringW(iterator->second).c_str()));
			m_pScanner->SetPropertyOrMethod(convertToStringW(iterator->first).c_str(), convertToStringW(iterator->second).c_str());
		}
		oResult.set(true);
	}

	virtual void clearAllProperties(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		//  Clearing all Scanner properties will disable the Scanner
		m_pScanner->DisableScannerAndResetToDefault();
	}

    virtual void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		if (m_hashProps.containsKey(propertyName))
			oResult.set(m_hashProps.get(propertyName));
		else
		{
			int iLen = m_pScanner->RetrieveProperty(convertToStringW(propertyName).c_str(), NULL);
			if (iLen > -1)
			{
				WCHAR szValue[MAX_PATH];
				if (szValue)
				{
					m_pScanner->RetrieveProperty(convertToStringW(propertyName).c_str(), szValue);
					rho::StringW szStringValue;
					szStringValue.insert(0, szValue);
					oResult.set(szStringValue);
				}
			}
			else
				oResult.set(L"Unavailable");
		}
	}

    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		rho::Hashtable<rho::String, rho::String> propsHash;
		rho::StringW szProperty;
		typedef std::vector<rho::String>::const_iterator it_type;
		for (it_type iterator = arrayofNames.begin(); iterator != arrayofNames.end(); iterator++)
		{
			szProperty = L"";
			if (m_hashProps.containsKey(*iterator))
				szProperty = convertToStringW(m_hashProps.get(*iterator));
			else
			{
				int iLen = m_pScanner->RetrieveProperty(convertToStringW(*iterator).c_str(), NULL);
				if (iLen > -1)
				{
					WCHAR szValue[MAX_PATH];
					if (szValue)
					{
						m_pScanner->RetrieveProperty(convertToStringW(*iterator).c_str(), szValue);
						szProperty.insert(0, szValue);
					}
				}
				else
					szProperty = L"Unavailable";
			}			
			propsHash.put(*iterator, convertToStringA(szProperty));
		}
		oResult.set(propsHash);
	}

    virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		m_pScanner->RetrieveAllProperties(&oResult);
	}

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		if (!oResult.hasCallback())
		{
			m_pScanner->SetCallback(0, NULL);
			DEBUGMSG(true, (L"No Callback"));
		}
		else
		{
			DEBUGMSG(true, (L"Callback"));
			m_pScanner->SetCallback(0, oResult);
		}
	    CMethodResult oRes;
		setProperties(propertyMap, oRes);
		m_pScanner->SetPropertyOrMethod(L"enable", L"");
	}

    virtual void disable(CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		m_pScanner->SetPropertyOrMethod(L"disable", L"");
	}

	virtual void start(CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		m_pScanner->SetPropertyOrMethod(L"start", L"");
	}

    virtual void stop(CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		m_pScanner->SetPropertyOrMethod(L"stop", L"");
	}

    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
	{
		//  For Symbol Scanners Take will enable the Scanner and perform a soft start.
		//  If a barcode is scanned then the data will be returned, if no barcode is scanned
		//  then 'cancel' is returned.  The time we wait for is defined by scanTimeout
		if (!m_pScanner) {return;}
		ResetEvent(m_pScanner->getEnableForTakeComplete());
	    CMethodResult oRes;
		setProperties(propertyMap, oRes);
		m_pScanner->SetPropertyOrMethod(L"enable", L"");
		DWORD dwWaitForEnable = WaitForSingleObject(
			m_pScanner->getEnableForTakeComplete(), 45000);
		if (dwWaitForEnable == WAIT_TIMEOUT)
		{
			LOG(WARNING) + "Scanner did not enable in time for Take Method";
			rho::Hashtable<rho::String, rho::String> scannedData;
			scannedData.put( convertToStringA(L"barcode"), convertToStringA(L"") );
			scannedData.put( convertToStringA(L"status"), convertToStringA(L"cancel") );
			oResult.set(scannedData);
			return;
		}
		else if (dwWaitForEnable == WAIT_FAILED)
			LOG(WARNING) + "Error waiting for Take Barcode "; 
		m_pScanner->SetCallback(4, oResult);
		WCHAR szTimeout[10];	//  Something's gone terribly wrong if the scan timeout is more than a milliard!
		m_pScanner->RetrieveProperty(L"scanTimeout", szTimeout);
		DWORD dwTimeout = _wtoi(szTimeout);
		m_pScanner->SetPropertyOrMethod(L"start", L"");
		//  Wait for the Scan Timeout duration
		DWORD dwWaitRes = WaitForSingleObject(m_pScanner->getReadComplete(), dwTimeout);
		if (dwWaitRes == WAIT_TIMEOUT)
		{
			//  Scanner has timed out trying to scan a barcode, return cancelled to user
			LOG(INFO) + "Barcode Take method did not detect a barcode";
			rho::Hashtable<rho::String, rho::String> scannedData;
			scannedData.put( convertToStringA(L"barcode"), convertToStringA(L"") );
			scannedData.put( convertToStringA(L"status"), convertToStringA(L"cancel") );
			oResult.set(scannedData);
		}
		else if (dwWaitRes == WAIT_FAILED)
			LOG(WARNING) + "Error waiting for Take Barcode "; 
		m_pScanner->SetPropertyOrMethod(L"stop", L"");
		m_pScanner->SetPropertyOrMethod(L"disable", L"");
		m_pScanner->SetCallback(4, NULL);
	}

	virtual void registerBluetoothStatus(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pScanner) {return;}
		m_pScanner->SetCallback(1, oResult);
	}

	virtual void commandRemoteScanner( const rho::String& command, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pScanner) {return;}
		m_pScanner->CommandRemoteScanner(convertToStringW(command).c_str());
	}

    virtual void take_barcode( const rho::String& rubyCallbackURL,  const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{
		oResult.setError("Not Supported on Windows Mobile / CE / Embedded");
	}

    virtual void barcode_recognize( const rho::String& imageFilePath, rho::apiGenerator::CMethodResult& oResult)
	{
		oResult.setError("Not Supported on Windows Mobile / CE / Embedded");
	}

    virtual void getSupportedProperties(rho::apiGenerator::CMethodResult& oResult)
	{
		if(!m_pScanner) {return;}
		m_pScanner->GetSupportedProperties(&oResult);
	}

	//  Virtual Methods overridden from IRhoExtension
	virtual void OnAppActivate(bool bActivate, const CRhoExtData& oExtData)
	{
		if (!m_pScanner) {return;}
		m_pScanner->ApplicationFocusChange(bActivate);
	}

};

//CBarcodeSingletonBase::CBarcodeSingletonBase() {}

class CBarcodeSingleton: public CBarcodeSingletonBase
{
public:
	CBarcodeSingleton(): CBarcodeSingletonBase(){}
	~CBarcodeSingleton()
	{
		if (m_hEMDKLibrary)
			FreeLibrary(m_hEMDKLibrary);
	}
	virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
	HMODULE m_hEMDKLibrary;
	LPFN_SCAN_FINDFIRST_T	lpfn_SCAN_FindFirst;
	LPFN_SCAN_FINDNEXT_T	lpfn_SCAN_FindNext;	
	LPFN_SCAN_FINDCLOSE_T	lpfn_SCAN_FindClose;

	void initialise();
};

class CBarcodeFactory: public CBarcodeFactoryBase
{
	~CBarcodeFactory(){}
    virtual IBarcodeSingleton* createModuleSingleton();
    virtual IBarcode* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Barcode_extension()
{
    CBarcodeFactory::setInstance( new CBarcodeFactory() );
    Init_Barcode_API();
}

IBarcode* CBarcodeFactory::createModuleByID(const rho::String& strID)
{
    return new CBarcodeImpl(strID);
}

IBarcodeSingleton* CBarcodeFactory::createModuleSingleton()
{
    CBarcodeSingleton* pBarcodeSingleton = new CBarcodeSingleton();
	pBarcodeSingleton->initialise();
	return pBarcodeSingleton;
}

void CBarcodeSingleton::initialise()
{
	//  Determine the type of device we are running on and therefore which
	//  Scanner API to load
	m_hEMDKLibrary = LoadLibrary(L"SCNAPI32.DLL");
	if (m_hEMDKLibrary)
	{
		g_deviceManufacturer = SCANNER_SYMBOL;
		//  To enumerate scanners we just need find first, find next and find close
		lpfn_SCAN_FindFirst	= (LPFN_SCAN_FINDFIRST_T)GetProcAddress
			(m_hEMDKLibrary, _T(SCAN_FindFirst_EXPORT));
		lpfn_SCAN_FindNext = (LPFN_SCAN_FINDNEXT_T)GetProcAddress
			(m_hEMDKLibrary, _T(SCAN_FindNext_EXPORT));
		lpfn_SCAN_FindClose = (LPFN_SCAN_FINDCLOSE_T)GetProcAddress
			(m_hEMDKLibrary, _T(SCAN_FindClose_EXPORT));
	}
	else
	{
		//  EMDK Library failed to load, logically we are not on a Symbol Device
		g_deviceManufacturer = SCANNER_OTHER;
	}
}

void CBarcodeSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
	if (g_deviceManufacturer == SCANNER_SYMBOL)
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
    //oResult.setParamName("scannerArray");
	oResult.set(arIDs);
}

rho::String CBarcodeSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}





}