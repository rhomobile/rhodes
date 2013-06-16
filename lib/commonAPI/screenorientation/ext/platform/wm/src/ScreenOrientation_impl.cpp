#include "../../../shared/generated/cpp/ScreenOrientationBase.h"
#include "common/RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "IstDll.h"
#include "Sensor.h"
#include "OrientationSettings.h"

namespace rho {

using namespace apiGenerator;

// TODO: this strings needs to come form the api xml
// so that the strings can be standard across the all platforms (andoid, wm, ios, wp8 etc)
static const rho::String g_normal = "normal";
static const rho::String g_rightHanded = "righthanded";
static const rho::String g_leftHanded = "lefthanded";
static const rho::String g_upsideDown = "upsidedown";
static const rho::String g_invalid = "invalid";

/**
 * 
 * 
 * @author GXV738 (6/15/2013)
 */
class CScreenOrientationSingleton: public CScreenOrientationSingletonBase
{
public:
	CScreenOrientationSingleton();
	virtual ~CScreenOrientationSingleton();

	virtual void getAutoRotate(rho::apiGenerator::CMethodResult& oResult);
    virtual void setAutoRotate( bool autoRotate, rho::apiGenerator::CMethodResult& oResult);
    virtual void normal(rho::apiGenerator::CMethodResult& oResult);
    virtual void rightHanded(rho::apiGenerator::CMethodResult& oResult);
    virtual void leftHanded(rho::apiGenerator::CMethodResult& oResult);
    virtual void upsideDown(rho::apiGenerator::CMethodResult& oResult);
    virtual void setScreenOrientationEvent(rho::apiGenerator::CMethodResult& oResult);
	virtual bool onWndMsg(MSG& oMsg);
	virtual void OnAppActivate(bool bActivate, const common::CRhoExtData& oExtData);
    
protected:
	void setDefaults();
	rho::String modesToString(screenorientation::ScreenOrientationModes& modes);
private:	
	bool										m_isISTEnabled;					// state of availability Motorola Solutions IST api's
	bool										m_isSensorEnabled;				// state of availability Motorola Solutions Sensor api's
	bool										m_supportsScreenOrientation;	// do device support screen orientation
	bool										m_hasFocus;						// whether the app has focus
	bool										m_autoRotate;					// current auto rate state
	screenorientation::ScreenOrientationModes	m_currentOrientation;			// the current device orientation
	screenorientation::ScreenOrientationModes	m_defaultOrientation;			// the default orientation during first access, when destryed destroys the device to this state
	common::CMutex*								m_syncLock;						// a lock to protect the cached callback object, from the javascript thread and ui thread. subject to debate?
	CMethodResult*								m_methodResult;					// synchronized (not sure if this is necessary). Does this object need to be cleaned up?
	DWORD										m_dwLastSettingChangeTime;		// the time when the window received the last screen orientation change event.
};

class CScreenOrientationFactory: public CScreenOrientationFactoryBase
{
    ~CScreenOrientationFactory(){}
    virtual IScreenOrientationSingleton* createModuleSingleton();    
};

extern "C" void Init_ScreenOrientation_extension()
{
    CScreenOrientationFactory::setInstance( new CScreenOrientationFactory() );
    Init_ScreenOrientation_API();
}

IScreenOrientationSingleton* CScreenOrientationFactory::createModuleSingleton()
{
    return new CScreenOrientationSingleton();
}

/**
 * 
 * 
 * @author GXV738 (6/15/2013)
 */
CScreenOrientationSingleton::CScreenOrientationSingleton() : 
									m_isISTEnabled(screenorientation::CIstDll::IsPresent()),
									m_isSensorEnabled(screenorientation::CSensor::IsSupported()),
									m_supportsScreenOrientation(screenorientation::COrientationSettings::IsSupported()),
									m_hasFocus(true),
									m_autoRotate(false),
									m_currentOrientation(screenorientation::COrientationSettings::GetOrientation()),
									m_defaultOrientation(screenorientation::SOM_BAD_ORIENTATION),
									m_syncLock(new common::CMutex()),
									m_methodResult(NULL),
									m_dwLastSettingChangeTime(0)

{
	//save the orientation during startup for restore
	// during exit
	VERIFY(NULL != m_syncLock);
	if (m_supportsScreenOrientation)
	{
		m_defaultOrientation = screenorientation::COrientationSettings::GetOrientation();
	}
	// set the default state to autoroate on supported motorola devices
	setDefaults();
}

/**
 * 
 * 
 * @author GXV738 (6/15/2013)
 */
CScreenOrientationSingleton::~CScreenOrientationSingleton()
{
	//when exiting rho app set the screen orientation to how it was during the startup
	if (m_supportsScreenOrientation && (m_defaultOrientation != m_currentOrientation))
	{
		screenorientation::COrientationSettings::SetOrientation(m_defaultOrientation);
	}

	if (NULL != m_syncLock)
	{
		delete m_syncLock;
		m_syncLock = NULL;
	}
}

/**
 * Ruby/Javascript backend to get the current autorotate state
 * 
 * @author GXV738 (6/15/2013)
 * 
 * @param oResult 
 */
void CScreenOrientationSingleton::getAutoRotate(rho::apiGenerator::CMethodResult& oResult)
{
	// Motorola Solutions exposes turning on/off the auto rotate state
	// via the properitory IST/Sensor interfaces, applicable for MotorolaSolutions
	// device alone
	bool autoRotateState = false;
	
	if (m_isISTEnabled)
	{
		// when the license page is active a call to the 
		// IST api always returns a disabled
		if (!this->m_hasFocus)
		{
			autoRotateState = this->m_autoRotate;
		}
		else
		{
			screenorientation::CIstDll ist;
			if(ist.Open())
			{
				autoRotateState = ist.IsAutoRotateEnabled();
				ist.Close();
			}
		}		
	}
	else if (m_isSensorEnabled)
	{
		// when the license page is active a call to the 
		// IST api always returns a disabled
		if (!this->m_hasFocus)
		{
			autoRotateState = this->m_autoRotate;
		}
		else
		{
			autoRotateState = screenorientation::CSensor::IsAutoRotateEnabled();
		}
	}
	else
	{
		autoRotateState = false;
		LOG(WARNING) + "Get: Autorotate feature is currently enabled only on Motorola devices with IST/Sensor support"; 
	}
	oResult.set(autoRotateState);
}

/**
 * 
 * 
 * @author GXV738 (6/15/2013)
 * 
 * @param autoRotate 
 * @param oResult 
 */
void CScreenOrientationSingleton::setAutoRotate( bool autoRotate, rho::apiGenerator::CMethodResult& oResult)
{
	// Motorola Solutions exposes turning on/off the auto rotate state
	// via the properitory IST interfaces, applicable for MotorolaSolutions
	// device alone
	bool bDone = false;
	if (this->m_isISTEnabled)
	{
		screenorientation::CIstDll ist;
		if(ist.Open())
		{
			ist.EnableAutoRotate(autoRotate);
			this->m_autoRotate = autoRotate;
			ist.Close();
			bDone = true;
		}
	}
	else if (this->m_isSensorEnabled)
	{
		bDone = screenorientation::CSensor::EnableAutoRotate(autoRotate);
		this->m_autoRotate = autoRotate;
	}
	else
	{
		this->m_autoRotate = false;
		LOG(WARNING) + "Set: Autorotate feature is currently enabled only on Motorola devices with IST support"; 
	}
	oResult.set(bDone);
}

/**
 * 
 * 
 * @author GXV738 (6/15/2013)
 * 
 * @param oResult 
 */
void CScreenOrientationSingleton::normal(rho::apiGenerator::CMethodResult& oResult)
{
	bool bDone = false;
	if (this->m_supportsScreenOrientation)
	{
		bDone = screenorientation::COrientationSettings::SetOrientation(screenorientation::SOM_NORMAL);
		if (bDone)		{
			LOG(INFO) + " Screen orientation changed to normal"; 			
		}
	}
	else
	{
		LOG(WARNING) + " Screen orientation change is unsupported:normal"; 
	}
	oResult.set(bDone);

}

/**
 * 
 * 
 * @author GXV738 (6/15/2013)
 * 
 * @param oResult 
 */
void CScreenOrientationSingleton::rightHanded(rho::apiGenerator::CMethodResult& oResult)
{
	bool bDone = false;
	if (this->m_supportsScreenOrientation)
	{
		bDone = screenorientation::COrientationSettings::SetOrientation(screenorientation::SOM_RIGHT_HANDED);
		if (bDone)
		{
			LOG(INFO) + " Screen orientation changed to right handed";
		}
	}
	else
	{
		LOG(WARNING) + " Screen orientation change is unsupported:rh"; 
	}
	oResult.set(bDone);
}

/**
 * 
 * 
 * @author GXV738 (6/15/2013)
 * 
 * @param oResult 
 */
void CScreenOrientationSingleton::leftHanded(rho::apiGenerator::CMethodResult& oResult)
{
	bool bDone = false;
	if (this->m_supportsScreenOrientation)
	{
		bDone = screenorientation::COrientationSettings::SetOrientation(screenorientation::SOM_LEFT_HANDED);
		if (bDone)
		{
			LOG(INFO) + " Screen orientation changed to left handed";
		}
	}
	else
	{
		LOG(WARNING) + " Screen orientation change is unsupported:lh"; 
	}
	oResult.set(bDone);
}

/**
 * 
 * 
 * @author GXV738 (6/15/2013)
 * 
 * @param oResult 
 */
void CScreenOrientationSingleton::upsideDown(rho::apiGenerator::CMethodResult& oResult)
{
	bool bDone = false;
	if (this->m_supportsScreenOrientation)
	{
		bDone = screenorientation::COrientationSettings::SetOrientation(screenorientation::SOM_UPSIDE_DOWN);
		if (bDone)
		{
			LOG(INFO) + " Screen orientation changed to upside down";
		}
	}
	else
	{
		LOG(WARNING) + " Screen orientation change is unsupported:ud"; 
	}
	oResult.set(bDone);;
}

/**
 * 
 * 
 * @author GXV738 (6/15/2013)
 * 
 * @param oResult 
 */
void CScreenOrientationSingleton::setScreenOrientationEvent(rho::apiGenerator::CMethodResult& oResult)
{
	// save the script callback
	// the saved call back is accessed from the ui thread as well
	// hence synchronizing then makes sense. The synchronization perhaps may not be 
	// required if the assignment is probably atomic in nature?
	if (oResult.hasCallback()){
		m_syncLock->Lock();
		m_methodResult = &oResult;
		m_syncLock->Unlock();
		LOG(INFO) + " Added callback to chain"; 
	}
	else
	{
		m_syncLock->Lock();
		m_methodResult = NULL;
		m_syncLock->Unlock();
		LOG(INFO) + " No callback found in registration"; 
	}
}
/**
 * Processing window message only the WM_SETTINGSCHANGE to 
 * receive screen orientation change event from the top level 
 * window. 
 *  
 * The function contains code borrowed from the earlier scan 
 * orientation plugin to filter out filter repeated screen 
 * orientation change messages for a single screenorientation 
 * set call, using a hardcoded timeout value. 
 *  
 * QUESTIONS: 
 *  Do all the windows in RhoElements respond to screen
 *  orientation change. Is there a need to propagate this event
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @param msg 
 * 
 * @return bool 
 */
bool CScreenOrientationSingleton::onWndMsg(MSG& msg)
{
	bool bHandled = false;
	int iDebounceDuration = 20;

	// only WM_SETTINGCHANGE aad SETTINGCHANGE_RESET for screen orientation change
	if ((WM_SETTINGCHANGE == msg.message) && (SETTINGCHANGE_RESET == msg.wParam))
	{
		//  The ScreenOrientation has Changed
		//  Need to use a Debounce duration here as we don't get 
		//  WM_SETTINGCHANGE for our parent's window unless we put a break
		//  point in the code.  I know this is very strange but this is 
		//  confirmed by debug output in the message pump.
		if (this->m_dwLastSettingChangeTime + iDebounceDuration > msg.time)
		{
			//  Message received too close to the previous message
			this->m_dwLastSettingChangeTime = msg.time;
			bHandled = false;
			LOG(INFO) +  " orientation changed event received too close to previous report. Ignoring."; 
		}
		else
		{
			this->m_dwLastSettingChangeTime = msg.time;
			LOG(INFO) +  " orientation changed event received."; 
			screenorientation::ScreenOrientationModes modes = screenorientation::COrientationSettings::GetOrientation();
			rho::String state = this->modesToString(modes);
			if ((modes != screenorientation::SOM_BAD_ORIENTATION) && (this->m_currentOrientation != modes))
			{
				LOG(INFO) + "Orientation changed to " + state;
				this->m_currentOrientation = modes;

				// the saved call back is accessed from the ui thread as well
				// hence synchronizing then makes sense. The synchronization perhaps may not be 
				// required if the assignment is probably atomic in nature?
				m_syncLock->Lock();		
				if ((NULL != m_methodResult) && (m_methodResult->hasCallback()))
				{
					LOG(INFO) + " Notify ruby/javascript callback with state: " + state;	
					m_methodResult->set(state);
				}		
				m_syncLock->Unlock();
				
				bHandled = true;
			}			
			else
			{
				LOG(INFO) + " WM_SETTINGCHANGE recevied. Orientation is invalid or unchanged since the last state : " + state;
				bHandled = false;
			}
		}
	}
	return bHandled;
}

/**
 * Common initialiazation code called during startup. 
 * Sets the auto rotate state to true 
 * 
 * @author GXV738 (6/12/2013)
 */
void CScreenOrientationSingleton::setDefaults()
{
	// Motorola Solutions exposes turning on/off the auto rotate state
	// via the properitory IST interfaces, applicable for MotorolaSolutions
	// device alone
	if (this->m_isISTEnabled)
	{
		screenorientation::CIstDll ist;
		if(ist.Open())
		{
			ist.EnableAutoRotate(true);
			this->m_autoRotate = true;			
			ist.Close();				
		}
	}
	else if (this->m_isSensorEnabled)
	{
		if(screenorientation::CSensor::EnableAutoRotate(true))
			this->m_autoRotate = true;
		
	}
	else
	{
		this->m_autoRotate = false;
		// non Motorola devices are TODO: as there no API
		// exposed by Microsoft to directly set the auto rotate state
		LOG(WARNING) + "Default Autorotate feature is currently enabled only on Motorola devices with IST/Sensor support"; 
	}
}

/**
 * Restore application state to saved once focus is regained.
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @param bActivate 
 * @param oExtData 
 */
void CScreenOrientationSingleton::OnAppActivate(bool bActivate, const common::CRhoExtData& oExtData)
{
	
	if (!this->m_supportsScreenOrientation)
	{
		LOG(INFO) + "Screen orientation is unsupported. Ignoring application activate";
		return;
	}

	if (!bActivate && this->m_hasFocus)
	{
		LOG(INFO) + " Focus lost";

		if (this->m_supportsScreenOrientation)
		{
			// cache the last known orientation mode
			screenorientation::ScreenOrientationModes orientationMode = screenorientation::COrientationSettings::GetOrientation();
			this->m_currentOrientation = (orientationMode != screenorientation::SOM_BAD_ORIENTATION)? orientationMode : screenorientation::SOM_NORMAL;
			LOG(INFO) + " Orientation cached : " + this->modesToString(this->m_currentOrientation);
		}
		// app has just lost focus
		if (this->m_isISTEnabled)
		{
			
			//cache the last auto 
			
			screenorientation::CIstDll ist;
			if(ist.Open())
			{
				this->m_autoRotate = ist.IsAutoRotateEnabled();
				ist.Close();
				LOG(INFO) + " AutoRotate cached : " + (this->m_autoRotate?"on" : "off");
			}
			else
			{
				this->m_autoRotate = false;				
				LOG(INFO) + " AutoRotate cached : " + (this->m_autoRotate?"on" : "off");
			}
			
			this->m_hasFocus = false;
		}
		else if (this->m_isSensorEnabled)
		{
			this->m_autoRotate = screenorientation::CSensor::IsAutoRotateEnabled();			
			LOG(INFO) + " AutoRotate cached : " + (this->m_autoRotate?"on" : "off");
		}
		
		this->m_hasFocus = false;			
	}
	else if (bActivate && !this->m_hasFocus)
	{
		LOG(INFO) + " Regained focus";
		// app has just regained focus
		this->m_hasFocus = true;

		if (m_supportsScreenOrientation)
		{
			screenorientation::ScreenOrientationModes orientationMode = screenorientation::COrientationSettings::GetOrientation();
			if(screenorientation::SOM_BAD_ORIENTATION == orientationMode)
				screenorientation::SOM_NORMAL;
			
			if (orientationMode != this->m_currentOrientation)
			{
				// on getting the focus restore the orientation
				screenorientation::COrientationSettings::SetOrientation(this->m_currentOrientation);//SetOrientation(pData->storedOrientation);
				LOG(INFO) + " Orientation back to : " + this->modesToString(this->m_currentOrientation);
			}
		}

		if (this->m_isISTEnabled)
		{
			screenorientation::CIstDll ist;
			if(ist.Open())
			{
				bool autoRotate = ist.IsAutoRotateEnabled();
				if (autoRotate != this->m_autoRotate)
				{
					ist.EnableAutoRotate(this->m_autoRotate);					
					LOG(INFO) + " AutoRotate back to : " + (this->m_autoRotate?"on" : "off");
				}
				ist.Close();
			}
			
		}		
		else if (this->m_isSensorEnabled)
		{
			bool autoRotate = screenorientation::CSensor::IsAutoRotateEnabled();
			if (autoRotate != m_autoRotate)
			{
				screenorientation::CSensor::EnableAutoRotate(this->m_autoRotate);
				LOG(INFO) + " AutoRotate back to : " + (this->m_autoRotate?"on" : "off");
			}
		}
	}
}
/**
 *	Convert scren orientation modes to strings 
 * 
 * @author GXV738 (6/15/2013)
 * 
 * @param modes 
 * 
 * @return rho::String 
 */
rho::String CScreenOrientationSingleton::modesToString(screenorientation::ScreenOrientationModes& modes)
{
	rho::String state = g_normal;
	switch (modes)
	{
		case screenorientation::SOM_NORMAL:
			break;
		case screenorientation::SOM_RIGHT_HANDED:
			state = g_rightHanded;
			break;
		case screenorientation::SOM_UPSIDE_DOWN:
			state = g_upsideDown;
			break;
		case screenorientation::SOM_LEFT_HANDED:
			state = g_leftHanded;
			break;
		default:
			state = g_invalid;
			break;
	}
	return state;

}


} // namespace ends
