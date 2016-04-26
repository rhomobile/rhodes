#include <common/RhodesApp.h>
#include "../../../shared/generated/cpp/CardreaderBase.h"
#include "CardReader.h"

namespace rho {
	using namespace apiGenerator;
	using namespace common;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "CardReader"

class CCardReaderImpl: public IRhoExtension, public CCardReaderBase
{
public:
	CCardReader *m_pCardReader;

    CCardReaderImpl(const rho::String& strID): CCardReaderBase()
    {
		//Is this the Card reader instance ID?
		m_hashProps.put( "ID", strID);
		
		LOG(INFO) + "Initialising interface for Card reader " + strID; 
		RHODESAPP().getExtManager().registerExtension(/*convertToStringA*/(strID), this );

		int iInstance = 0;
		if (strID.length() > 3)
			iInstance = atoi(strID.c_str() + 3);
		m_pCardReader = new CCardReader(iInstance);
		if (m_pCardReader)
		{
			if (m_pCardReader->m_bDllPresent == FALSE)
			{
				LOG(WARNING) + "Failed to initialise interface for Card reader ";
				delete m_pCardReader;
				m_pCardReader = NULL;
			}
			else
			{
				m_pCardReader->Initialise(true, rho::common::convertToStringW(strID).c_str());	
				LOG(INFO) + "Initialised interface for Card reader " + strID; 
			}			
		}

    }
    
	~CCardReaderImpl()
	{
		LOG(INFO) + "Shutting down Card reader "; 
		if (m_pCardReader)
			delete m_pCardReader;
		
	}

    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult)
	{
		if (!m_pCardReader) {return;}
		LOG(INFO) +  L"Setting Property " + propertyName.c_str() + " to " + propertyValue.c_str();		
		BOOL bRet = m_pCardReader->SetPropertyOrMethod(convertToStringW(propertyName).c_str(), convertToStringW(propertyValue).c_str());
		oResult.set(bRet == TRUE);
	}

    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{
		//  Set multiple properties
		if (!m_pCardReader) {return;}
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			LOG(INFO) +  L"Setting Property " + iterator->first.c_str() + " to " + iterator->second.c_str();			
			m_pCardReader->SetPropertyOrMethod(convertToStringW(iterator->first).c_str(), convertToStringW(iterator->second).c_str());
		}
		oResult.set(true);
	}

	virtual void clearAllProperties(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pCardReader) {return;}		
		//  Clearing all Card Reader properties will not disable the Card Reader		
		m_pCardReader->ClearAllProperties();
	}

    virtual void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pCardReader) {return;}
		if (m_hashProps.containsKey(propertyName))
			oResult.set(m_hashProps.get(propertyName));
		else
		{			
			WCHAR szValue[MAX_PATH];
			BOOL bResult = m_pCardReader->RetrieveProperty(convertToStringW(propertyName).c_str(), szValue);
			LOG(INFO) +  L"Getting Property " + convertToStringW(propertyName).c_str() + " as " + szValue;
			if (szValue && (bResult == TRUE))
			{
				rho::StringW szStringValue;
				szStringValue.insert(0, szValue);
				oResult.set(szStringValue);
			}			
			else
			{
				oResult.set(L"Unavailable");
			}
		}
	}

    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pCardReader) {return;}
		rho::Hashtable<rho::String, rho::String> propsHash;
		CMethodResult oRes;
		typedef std::vector<rho::String>::const_iterator it_type;
		for (it_type iterator = arrayofNames.begin(); iterator != arrayofNames.end(); iterator++)
		{
			getProperty(*iterator, oRes);
			propsHash.put(*iterator, convertToStringA(oRes.toString()));
		}
		oResult.set(propsHash);
	}

    virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pCardReader) {return;}
		rho::Hashtable<rho::String, rho::String> propsHash;
		CMethodResult oRes;
		rho::Vector<rho::String> arrayofNames;
		arrayofNames.push_back("pinTimeout");
		arrayofNames.push_back("pinEntry");
		arrayofNames.push_back("panData");
		arrayofNames.push_back("autoTab");
		arrayofNames.push_back("autoEnter");
		arrayofNames.push_back("moduleName");		
		typedef std::vector<rho::String>::const_iterator it_type;
		for (it_type iterator = arrayofNames.begin(); iterator != arrayofNames.end(); iterator++)
		{
			getProperty(*iterator, oRes);			
			propsHash.put(*iterator, convertToStringA(oRes.toString()));
		}
		oResult.set(propsHash);		
	}

    virtual void open(CMethodResult& oResult)
	{
		if (!m_pCardReader) {return;}
		if (!oResult.hasCallback())
		{
			m_pCardReader->SetCallback(NULL);
		}
		else
		{
			m_pCardReader->SetCallback(&oResult);
		}
		m_pCardReader->SetPropertyOrMethod(L"open", L"");
	}

    virtual void close(CMethodResult& oResult)
	{
		if (!m_pCardReader) {return;}
		m_pCardReader->SetPropertyOrMethod(L"close", L"");
	}   

	//  Virtual Methods overridden from IRhoExtension
    virtual void onBeforeNavigate(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData)
	{
		if (!m_pCardReader) {return;}
		m_pCardReader->BeforeNavigate();
	}

    virtual void onDocumentComplete(const wchar_t* szUrlOfDocument, const CRhoExtData& oExtData)
	{
		if (!m_pCardReader) {return;}
		m_pCardReader->DocumentComplete();
	}

	virtual void OnAppActivate(bool bActivate, const CRhoExtData& oExtData)
	{
		if (!m_pCardReader) {return;}
		m_pCardReader->ApplicationFocusChange(bActivate);
	}


};

class CCardReaderSingleton: public CCardReaderSingletonBase
{
public:
	CCardReaderSingleton(): CCardReaderSingletonBase() {}
    ~CCardReaderSingleton()
	{
		if ( m_hCardReaderLibrary != NULL )
		{			
			FreeLibrary((HINSTANCE)m_hCardReaderLibrary);			
		}
	}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
	void initialise();

	HINSTANCE			m_hCardReaderLibrary;		///< Handle to card reader DLL being used	
	LPFINDFIRST			m_lpCardReaderFindFirst;	///< Pointer to the FindFirst function in the card reader DLL
	LPFINDNEXT			m_lpCardReaderFindNext;		///< Pointer to the FindNext function in the card reader DLL
	LPFINDCLOSE			m_lpCardReaderFindClose;	///< Pointer to the FindClose function in the card reader DLL	
};

class CCardReaderFactory: public CCardReaderFactoryBase
{
    ~CCardReaderFactory(){}
    virtual ICardReaderSingleton* createModuleSingleton();
    virtual ICardReader* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Cardreader_extension()
{
    CCardReaderFactory::setInstance( new CCardReaderFactory() );
    Init_CardReader_API();
}

ICardReader* CCardReaderFactory::createModuleByID(const rho::String& strID)
{
    return new CCardReaderImpl(strID);
}

ICardReaderSingleton* CCardReaderFactory::createModuleSingleton()
{
    CCardReaderSingleton* pCardReaderSingleton = new CCardReaderSingleton();
	pCardReaderSingleton->initialise();
	return pCardReaderSingleton;
}

void CCardReaderSingleton::initialise()
{
	// Load the DLL
	m_hCardReaderLibrary = LoadLibrary(DEFAULT_DLL_NAME);

	// If we loaded it, get function pointers
	if ( m_hCardReaderLibrary != NULL )
	{		
		m_lpCardReaderFindFirst = (LPFINDFIRST) GetProcAddress((HINSTANCE)m_hCardReaderLibrary, FUNC_NAME_FINDFIRST);
		m_lpCardReaderFindNext = (LPFINDNEXT) GetProcAddress((HINSTANCE)m_hCardReaderLibrary, FUNC_NAME_FINDNEXT);
		m_lpCardReaderFindClose = (LPFINDCLOSE) GetProcAddress((HINSTANCE)m_hCardReaderLibrary, FUNC_NAME_FINDCLOSE);
	}
	else
	{
		LOG(ERROR) +  L"Error : Could not load the CardReader DLL";
	}	
}

void CCardReaderSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
	HANDLE hFindHandle;
	DWORD dwStatus;
	int iCardReaderCount=0;
	TCHAR tcLogText[1024];
	MSR_FINDINFO		m_CardReaderFindInfo[MAX_DLLS];	///< A list of the unique card reader drivers found in the card reader DLL.

	if ( m_hCardReaderLibrary != NULL )
	{
		// Find first available MSR 3000 API compliant driver info
		dwStatus = m_lpCardReaderFindFirst(&m_CardReaderFindInfo[0], &hFindHandle);

		if(MSR_SUCCESS != dwStatus)
		{
			LOG(WARNING) +  L"No card reader drivers found on this device, please install drivers";		
		}
		else
		{
			wsprintf(tcLogText, L"Found Card Reader Driver: %s (%s)", m_CardReaderFindInfo[0].szFriendlyName, m_CardReaderFindInfo[0].szDeviceName);
			//  Remove Comma from end of Device Name
			TCHAR szModifiedDeviceName[MAX_PATH + 1];
			wcscpy(szModifiedDeviceName, m_CardReaderFindInfo[0].szDeviceName);
			if (wcslen(szModifiedDeviceName) > 0 &&
				szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] == L':')
				szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] = L'\0';
			arIDs.addElement(convertToStringA(szModifiedDeviceName));
			LOG(INFO) +  tcLogText;
		}

		// Find all additional drivers
		for(iCardReaderCount=1; iCardReaderCount<MAX_DLLS; iCardReaderCount++)
		{
			dwStatus = m_lpCardReaderFindNext(&m_CardReaderFindInfo[iCardReaderCount], hFindHandle);

			if(MSR_SUCCESS != dwStatus)
				break;
			else
			{
				wsprintf(tcLogText, L"Found Card Reader Driver: %s (%s)", m_CardReaderFindInfo[iCardReaderCount].szFriendlyName, m_CardReaderFindInfo[iCardReaderCount].szDeviceName);
				//  Remove Comma from end of Device Name
				TCHAR szModifiedDeviceName[MAX_PATH + 1];
				wcscpy(szModifiedDeviceName, m_CardReaderFindInfo[iCardReaderCount].szDeviceName);
				if (wcslen(szModifiedDeviceName) > 0 &&
					szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] == L':')
					szModifiedDeviceName[wcslen(szModifiedDeviceName) - 1] = L'\0';
				arIDs.addElement(convertToStringA(szModifiedDeviceName));
				LOG(INFO) +  tcLogText;
			}
		}

		// Close FindHandle
		m_lpCardReaderFindClose(hFindHandle);
		oResult.set(arIDs);
	}
	else
	{
		oResult.set("");
	}    
}

rho::String CCardReaderSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();

	if (arIDs.isEmpty())
		return "";
	else        
		return arIDs[0];
}

}