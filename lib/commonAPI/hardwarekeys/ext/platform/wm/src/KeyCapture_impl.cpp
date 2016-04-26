#include "../../../shared/generated/cpp/KeyCaptureBase.h"
#include "KeyModule.h"
#include "common/RhodesApp.h"
#include <algorithm>
extern "C" int rho_wmimpl_get_control_keys_count();
extern "C" int* rho_wmimpl_get_control_keys();

namespace rho {

using namespace apiGenerator;
using namespace common;

BOOL IsControl ()
{
	SHORT state;

	state = GetKeyState (VK_CONTROL);
	return (state & 0x1000) != 0;
}

class CKeyCaptureImpl: public CKeyCaptureSingletonBase
{
public:
	CKeyModule* m_pKeyModule;
    CKeyCaptureImpl(): CKeyCaptureSingletonBase()
    {
		m_pKeyModule = new CKeyModule();
		m_pKeyModule->Initialize();
		//Sabir:
		//commented below registration as the class is already registered inside CKeyCaptureSingletonBase::CKeyCaptureSingletonBase() for event notifications
		//this will fix keyEvent getting fired twice
		//RHODESAPP().getExtManager().registerExtension("KeyCaptureObject", this );
    }

	void getHomeKeyValue(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pKeyModule)
			return;

		int homeKey = m_pKeyModule->getHomeKey();
		if (homeKey == 0)
			oResult.set("disabled");
		else
			oResult.set(homeKey);
	}

    void setHomeKeyValue( const rho::String& homeKeyValue, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pKeyModule)
			return;

		rho::String comparitor;
		comparitor.assign(homeKeyValue);
		std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);

		if (comparitor.compare("disabled") == 0 || comparitor.compare("disable") == 0)
			m_pKeyModule->setHomeKey(0);
		else if (comparitor.compare("enabled") == 0 || comparitor.compare("enable") == 0)
			m_pKeyModule->setHomeKey(0x74);
		else
			m_pKeyModule->setHomeKey(rho::common::convertToStringW(homeKeyValue).c_str());		
	}

    void captureKey( bool dispatch,  const rho::String& keyValue, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pKeyModule)
			return;
		int iKeyValue = m_pKeyModule->parseKeyValue(keyValue);
		if ((iKeyValue != INVALID_KEY)&&(iKeyValue != KEY_EMPTYSTRING))
			m_pKeyModule->setKeyCallback(dispatch, iKeyValue, oResult);
	}

    void remapKey( const rho::String& keyValue,  const rho::String& remapTo, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pKeyModule)
			return;

		int iKeyFrom = m_pKeyModule->parseKeyValue(keyValue);
		if (iKeyFrom == ALL_KEYS)
		{
			LOG(WARNING) + "Unable to remap from All Keys";
		}
		else if ((iKeyFrom != INVALID_KEY)&&(iKeyFrom != KEY_EMPTYSTRING))
		{
			int iKeyTo = m_pKeyModule->parseKeyValue(remapTo);
			if (iKeyTo != INVALID_KEY && iKeyTo != ALL_KEYS)
				m_pKeyModule->setRemap(iKeyFrom, iKeyTo);
			else
				LOG(WARNING) + "Unrecognised Key Value in Remap Parameter " + remapTo;
		}
	}

    void captureTrigger(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pKeyModule)
			return;

		m_pKeyModule->setTriggerCallback(oResult);		
	}

	//  RE4 Key presses are received in onWndMsg (per JH)
//	bool onHTMLWndMsg(MSG &oMsg)
//	{
//		if (!m_pKeyModule)
//			return false;
//		return m_pKeyModule->onPrimaryMessage(&oMsg);
//	}

	bool onWndMsg(MSG &oMsg)
	{
		if (oMsg.message == WM_KEYDOWN ||
			oMsg.message == WM_CHAR ||
			oMsg.message == WM_KEYUP)
		{
			return m_pKeyModule->onPrimaryMessage(&oMsg);
		}
		return false;
	}

	void OnAppActivate(bool bActivate, const CRhoExtData& oExtData)
	{
		if (!m_pKeyModule)
			return;
		m_pKeyModule->onRhoAppFocus(bActivate);
	}
};

class CKeyCaptureSingleton: public CKeyCaptureSingletonBase
{
    ~CKeyCaptureSingleton(){}
};

class CKeyCaptureFactory: public IRhoExtension, public CKeyCaptureFactoryBase
{
public:
	CKeyCaptureFactory()
	{
		m_hDisplayClass = NULL;
		m_bAppActivated = true;
		RHODESAPP().getExtManager().registerExtension("KeyCaptureFactory", this );
	}

    ~CKeyCaptureFactory(){}
	virtual IKeyCaptureSingleton* createModuleSingleton()
	{
		return new CKeyCaptureImpl();
	}

	void OnAppActivate(bool bActivate, const CRhoExtData& oExtData)
	{
		m_bAppActivated = bActivate;
	}

	bool onWndMsg(MSG &oMsg)
	{
		//  Logic to block the display of the today screen when the user presses the red phone key should be 
		//  in force whenever this extension is loaded
		switch (oMsg.message)
		{
			//  If the user has pressed the Red Phone key give focus back to 
			//  RhoElements rather than the today screen
			case WM_APP + 16388:
			case WM_APP + 16387:
			{
				//  On WM Simplified Chinese devices WM_APP + 16388 corresponds to 
				//  clicking in a text field and does not behave the same as other WM
				//  devices (localized or otherwise)
				if (oMsg.wParam != 305)
				{
					if (m_bAppActivated)
					{
						//  Previously the logic below was undertaken in before navigate,
						//  moving it here since it is not necessary to do this repeatedly.
						if (m_hDisplayClass == NULL)
						{
							m_hDisplayClass = FindWindow(NULL, convertToStringW(RHODESAPP().getAppTitle()).c_str());
						}
						SetForegroundWindow
							(m_hDisplayClass != NULL ? m_hDisplayClass : oMsg.hwnd);
						SetFocus
							(m_hDisplayClass != NULL ? m_hDisplayClass : oMsg.hwnd);
					}
				}
				return false;
			}
			case WM_KEYDOWN:
				{
					//  Intercept Control keystrokes if applicable.  This logic is here as
					//  we want to block CTRL keys even if the user has not instantiated the 
					//  KeyCapture module.
					int code = (int) oMsg.wParam;
					int n;
					int nControlKeys = rho_wmimpl_get_control_keys_count();
					int* pControlKeys = rho_wmimpl_get_control_keys();

					// Allow the CTRL key itself but intercept CTRL + key
					if (code != VK_CONTROL && IsControl ())
					{
						// Check for key in allowed control key list
						for (n = 0; n < nControlKeys; n++)
							if (pControlKeys [n] == code)
								return FALSE;

						// Not in list so mark as handled
						return TRUE;
					}
				}
		}
		return false;
	}
	HWND m_hDisplayClass;
	bool m_bAppActivated;
};

extern "C" void Init_KeyCapture_extension()
{ 	
	CKeyCaptureFactory::setInstance( new CKeyCaptureFactory() );
	Init_KeyCapture_API();
	CRhoExtData rhodesData = RHODESAPP().getExtManager().makeExtData();
	typedef BOOL (*BLOCK_HOT_KEYS)(HWND);
	CStaticClassFunctor<BLOCK_HOT_KEYS, HWND>* FunPtr = new CStaticClassFunctor<BLOCK_HOT_KEYS, HWND>(&CKeyModule::BlockHotKeys, rhodesData.m_hBrowserWnd);
	// Key blocking only has to be done once
	if(FunPtr != NULL)
	{
		//let us perform keycapture module initialization in a ui thread
		rho_os_impl_performOnUiThread( FunPtr );
	}
	else
	{
		LOG(ERROR) + "KeyCapture module initialization failed";
	}
}


}
