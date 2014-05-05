#include <common/RhodesApp.h>
#include "../../../shared/generated/cpp/AudioCaptureBase.h"
#include "AudioCapture.h"

namespace rho {
	using namespace apiGenerator;
	using namespace common;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "AudioCapture"

class CAudioCaptureImpl: public IRhoExtension, public CAudioCaptureBase
{
	CAudioCapture *pAudio;
public:
    CAudioCaptureImpl(const rho::String& strID): CAudioCaptureBase()
    {
		pAudio = NULL;
		pAudio = new CAudioCapture(true);
    }

	~CAudioCaptureImpl()
	{
		LOG(INFO) + "Shutting down Audio Capture "; 
		if (pAudio){
			delete pAudio;
			pAudio = NULL;	
		}
	}
	
	virtual void start( const rho::Hashtable<rho::String, rho::String>& props, rho::apiGenerator::CMethodResult& oResult){
		if (!pAudio) {return;}
		pAudio->SetCallback(NULL);		
		if (oResult.hasCallback()){
			DEBUGMSG(true, (L"Callback"));
			pAudio->SetCallback(&oResult);
			CMethodResult oRes;
			setProperties(props, oRes);
			pAudio->Start();
		}
		else{
			LOG(INFO) +  L"Could not start Audio Capture. Callback is Mandatory.";
			return;
		}
    }
	
    virtual void stop( rho::apiGenerator::CMethodResult& oResult) {
		if (!pAudio) {return;}
		pAudio->Stop();
    }

	virtual void cancel(rho::apiGenerator::CMethodResult& oResult) {
		if (!pAudio) {return;}
		pAudio->Cancel();
    }
	
    virtual void onBeforeNavigate(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData)
	{
		if (!pAudio){return;}
		pAudio->Cancel();
	}

	virtual void OnAppActivate(bool bActivate, const CRhoExtData& oExtData)
	{
		if (!pAudio) {return;}
		pAudio->ApplicationFocusChange(bActivate);
	}

	virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult)
	{
		if (!pAudio) {return;}
		LOG(INFO) +  L"Setting Property " + propertyName.c_str() + " to " + propertyValue.c_str();		
		BOOL bRet = pAudio->SetPropertyOrMethod(convertToStringW(propertyName).c_str(), convertToStringW(propertyValue).c_str());
		oResult.set(bRet == TRUE);
	}

    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{
		//  Set multiple properties
		if (!pAudio) {return;}
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			LOG(INFO) +  L"Setting Property " + iterator->first.c_str() + " to " + iterator->second.c_str();			
			pAudio->SetPropertyOrMethod(convertToStringW(iterator->first).c_str(), convertToStringW(iterator->second).c_str());
		}
		oResult.set(true);
	}

	virtual void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!pAudio) {return;}
		if (m_hashProps.containsKey(propertyName))
			oResult.set(m_hashProps.get(propertyName));
		else
		{			
			WCHAR szValue[MAX_PATH];
			BOOL bResult = pAudio->RetrieveProperty(convertToStringW(propertyName).c_str(), szValue);
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
		if (!pAudio) {return;}
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
		if (!pAudio) {return;}
		rho::Hashtable<rho::String, rho::String> propsHash;
		CMethodResult oRes;
		rho::Vector<rho::String> arrayofNames;
		arrayofNames.push_back("fileName");
		arrayofNames.push_back("duration");	
		typedef std::vector<rho::String>::const_iterator it_type;
		for (it_type iterator = arrayofNames.begin(); iterator != arrayofNames.end(); iterator++)
		{
			getProperty(*iterator, oRes);			
			propsHash.put(*iterator, convertToStringA(oRes.toString()));
		}
		oResult.set(propsHash);		
	}
};

class CAudioCaptureSingleton: public CAudioCaptureSingletonBase
{
    ~CAudioCaptureSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CAudioCaptureFactory: public CAudioCaptureFactoryBase
{
    ~CAudioCaptureFactory(){}
    virtual IAudioCaptureSingleton* createModuleSingleton();
    virtual IAudioCapture* createModuleByID(const rho::String& strID);
};

extern "C" void Init_AudioCapture_extension()
{
    CAudioCaptureFactory::setInstance( new CAudioCaptureFactory() );
    Init_AudioCapture_API();
}

IAudioCapture* CAudioCaptureFactory::createModuleByID(const rho::String& strID)
{
    return new CAudioCaptureImpl(strID);
}

IAudioCaptureSingleton* CAudioCaptureFactory::createModuleSingleton()
{
    return new CAudioCaptureSingleton();
}

void CAudioCaptureSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CAudioCaptureSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}