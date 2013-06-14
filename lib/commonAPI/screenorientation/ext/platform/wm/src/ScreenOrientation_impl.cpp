#include "../../../shared/generated/cpp/ScreenOrientationBase.h"
#include "common/RhodesApp.h"
#include "common/RhoMutexLock.h"
//#include <list>
#include "IstDll.h"
#include "Sensor.h"
#include "OrientationSettings.h"

namespace rho {

using namespace apiGenerator;

//typedef std::list<CMethodResult*> MethodListeners;

// TODO: this strings needs to come form the api xml
// so that the strings can be standard across the all platforms (andoid, wm, ios, wp8 etc)
static const rho::String g_normal = "normal";
static const rho::String g_rightHanded = "righthanded";
static const rho::String g_leftHanded = "lefthanded";
static const rho::String g_upsideDown = "upsidedown";

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
private:	
	bool										m_isISTEnabled;
	bool										m_isSensorEnabled;
	bool										m_supportsScreenOrientation;
	bool										m_hasFocus;
	bool										m_autoRotate;
	screenorientation::ScreenOrientationModes	m_currentOrientation;
	//MethodListeners* 							m_listeners;
	CMethodResult*						m_methodResult;	// is there need to synchronize access to the saved callback
																// who will (delete?) the method result object when ist replaced
	DWORD										m_dwLastSettingChangeTime;
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

CScreenOrientationSingleton::CScreenOrientationSingleton() : 
									m_isISTEnabled(screenorientation::CIstDll::IsPresent()),
									m_isSensorEnabled(screenorientation::CSensor::IsSupported()),
									m_supportsScreenOrientation(screenorientation::COrientationSettings::IsSupported()),
									m_hasFocus(/*NULL != GetActiveWindow()*/true), //TODO : need to figure out what's the best way to figureout if the bapp/rowser window is active.
									m_autoRotate(false),
									m_currentOrientation(screenorientation::COrientationSettings::GetOrientation()),
									//m_listeners(NULL),
									m_methodResult(NULL),
									m_dwLastSettingChangeTime(0)

{
	//m_listeners = new MethodListeners();
	setDefaults();	// set the default state to autoroate on supported motorola devices
}

CScreenOrientationSingleton::~CScreenOrientationSingleton()
{
	
	/*if (NULL != m_listeners)
	{
		m_listeners->clear();
		m_listeners = NULL;
	}*/
}


void CScreenOrientationSingleton::getAutoRotate(rho::apiGenerator::CMethodResult& oResult)
{
	// Motorola Solutions exposes turning on/off the auto rotate state
	// via the properitory IST/Sensor interfaces, applicable for MotorolaSolutions
	// device alone
	bool autoRotateState = false;
	
	if (/*screenorientation::CIstDll::IsPresent()*/m_isISTEnabled)
	{
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
	else if (/*screenorientation::CSensor::IsSupported()*/m_isSensorEnabled)
	{
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
		// non Motorola devices are TODO: as there no API
		// exposed by Microsoft to directly set the auto rotate state
		LOG(WARNING) + "Get: Autorotate feature is currently enabled only on Motorola devices with IST support"; 
	}
	oResult.set(autoRotateState);
}

void CScreenOrientationSingleton::setAutoRotate( bool autoRotate, rho::apiGenerator::CMethodResult& oResult)
{
	// Motorola Solutions exposes turning on/off the auto rotate state
	// via the properitory IST interfaces, applicable for MotorolaSolutions
	// device alone
	bool bDone = false;
	if (/*screenorientation::CIstDll::IsPresent()*/this->m_isISTEnabled)
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
	else if (/*screenorientation::CSensor::IsSupported()*/this->m_isSensorEnabled)
	{
		bDone = screenorientation::CSensor::EnableAutoRotate(autoRotate);
		this->m_autoRotate = autoRotate;
	}
	else
	{
		this->m_autoRotate = false;
		// non Motorola devices are TODO: as there no API
		// exposed by Microsoft to directly set the auto rotate state
		LOG(WARNING) + "Set: Autorotate feature is currently enabled only on Motorola devices with IST support"; 
	}
	oResult.set(bDone);
}

void CScreenOrientationSingleton::normal(rho::apiGenerator::CMethodResult& oResult)
{
	bool bDone = false;
	if (/*screenorientation::CDisplaySettings::IsSupported()*/this->m_supportsScreenOrientation)
	{
		bDone = screenorientation::COrientationSettings::SetOrientation(screenorientation::SOM_NORMAL);
		if (bDone)
		{
			LOG(INFO) + " Screen orientation changed to potrait"; 
			/*if (m_listeners->empty())
				this->m_currentOrientation = screenorientation::SOM_NORMAL;*/
		}
	}
	else
	{
		LOG(WARNING) + " Screen orientation change is unsupported"; 
	}
	oResult.set(bDone);

}

void CScreenOrientationSingleton::rightHanded(rho::apiGenerator::CMethodResult& oResult)
{
	bool bDone = false;
	if (/*screenorientation::CDisplaySettings::IsSupported()*/this->m_supportsScreenOrientation)
	{
		bDone = screenorientation::COrientationSettings::SetOrientation(screenorientation::SOM_RIGHT_HANDED);
		if (bDone)
		{
			LOG(INFO) + " Screen orientation changed to right handed"; 
			/*if (m_listeners->empty())
				this->m_currentOrientation = screenorientation::SOM_RIGHT_HANDED;*/
		}
	}
	else
	{
		LOG(WARNING) + " Screen orientation change is unsupported"; 
	}
	oResult.set(bDone);
}

void CScreenOrientationSingleton::leftHanded(rho::apiGenerator::CMethodResult& oResult)
{
	bool bDone = false;
	if (/*screenorientation::CDisplaySettings::IsSupported()*/this->m_supportsScreenOrientation)
	{
		bDone = screenorientation::COrientationSettings::SetOrientation(screenorientation::SOM_LEFT_HANDED);
		if (bDone)
		{
			LOG(INFO) + " Screen orientation changed to left handed"; 
			/*if (m_listeners->empty())
				this->m_currentOrientation = screenorientation::SOM_LEFT_HANDED;*/
		}
	}
	else
	{
		LOG(WARNING) + " Screen orientation change is unsupported"; 
	}
	oResult.set(bDone);
}
void CScreenOrientationSingleton::upsideDown(rho::apiGenerator::CMethodResult& oResult)
{
	bool bDone = false;
	if (/*screenorientation::CDisplaySettings::IsSupported()*/this->m_supportsScreenOrientation)
	{
		bDone = screenorientation::COrientationSettings::SetOrientation(screenorientation::SOM_UPSIDE_DOWN);
		if (bDone)
		{
			LOG(INFO) + " Screen orientation changed to upside down"; 
			/*if (m_listeners->empty())
				this->m_currentOrientation = screenorientation::SOM_UPSIDE_DOWN;*/
		}
	}
	else
	{
		LOG(WARNING) + " Screen orientation change is unsupported"; 
	}
	oResult.set(bDone);;
}

void CScreenOrientationSingleton::setScreenOrientationEvent(rho::apiGenerator::CMethodResult& oResult)
{
#if 0
	// There is a problem here, multiple call to register a screen orientation
	// may succeed, but a single call to set the screen orientation event with a
	// null or empty value can cause all of the existing providers removed
	if (oResult.hasCallback()){
		m_listeners->insert(m_listeners->end(), &oResult);
		LOG(INFO) + " Added callback to chain"; 
	}
	else
	{
		m_listeners->clear();
		LOG(INFO) + " No callback found in registration"; 
	}
#else
	if (oResult.hasCallback()){
		m_methodResult = &oResult;
		LOG(INFO) + " Added callback to chain"; 
	}
	else
	{
		m_methodResult = NULL;
		LOG(INFO) + " No callback found in registration"; 
	}
#endif
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
			}
			if ((modes != screenorientation::SOM_BAD_ORIENTATION) && (this->m_currentOrientation != modes))
			{
				LOG(INFO) + "Orientation changed to " + state;
				this->m_currentOrientation = modes;
#if 0
				//call all listeners
				MethodListeners::const_iterator citr;
				for(citr = m_listeners->begin(); citr != m_listeners->end(); citr++)
				{

					CMethodResult* oResult = *citr;
					if ((NULL != oResult)&& (oResult->hasCallback()))
					{						
						LOG(INFO) + " Call ruby/javascript callback with state: " + state;	
						oResult->set(state);
					}
				}
#else
				if ((NULL != m_methodResult) && (m_methodResult->hasCallback()))
				{
					LOG(INFO) + " Call ruby/javascript callback with state: " + state;	
					m_methodResult->set(state);
				}
#endif
				
				bHandled = true;
			}			
			else
			{
				LOG(INFO) + " WM_SETTINGCHANGE recevied. Orientation is ibvalid or unchanged since the last state : " + state;
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
		LOG(WARNING) + "Autorotate feature is currently enabled only on Motorola devices with IST support"; 
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
		// app has just lost focus
		if (this->m_isISTEnabled)
		{
			// cache the last known orientation mode
			screenorientation::ScreenOrientationModes orientationMode = screenorientation::COrientationSettings::GetOrientation();
			this->m_currentOrientation = (orientationMode != screenorientation::SOM_BAD_ORIENTATION)? orientationMode : screenorientation::SOM_NORMAL;
			//cache the last auto 
			
			screenorientation::CIstDll ist;
			if(ist.Open())
			{
				this->m_autoRotate = ist.IsAutoRotateEnabled();
				ist.Close();
			}
			else
			{
				this->m_autoRotate = false;
			}
			
			this->m_hasFocus = false;
		}
		else if (this->m_isSensorEnabled)
		{
			// cache the last known orientation mode
			screenorientation::ScreenOrientationModes orientationMode = screenorientation::COrientationSettings::GetOrientation();
			this->m_currentOrientation = (orientationMode != screenorientation::SOM_BAD_ORIENTATION)? orientationMode : screenorientation::SOM_NORMAL;

			this->m_autoRotate = screenorientation::CSensor::IsAutoRotateEnabled();
		}
		this->m_hasFocus = false;			
	}
	else if (bActivate && !this->m_hasFocus)
	{
		LOG(INFO) + " Regained focus";
		// app has just regained focus
		this->m_hasFocus = true;

		screenorientation::ScreenOrientationModes orientationMode = screenorientation::COrientationSettings::GetOrientation();
		if(screenorientation::SOM_BAD_ORIENTATION == orientationMode)
			screenorientation::SOM_NORMAL;
		
		if (orientationMode != this->m_currentOrientation)
		{
			// on getting the focus restore the orientation
			screenorientation::COrientationSettings::SetOrientation(this->m_currentOrientation);//SetOrientation(pData->storedOrientation);
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
			}
		}
	}
}

} // namespace ends
