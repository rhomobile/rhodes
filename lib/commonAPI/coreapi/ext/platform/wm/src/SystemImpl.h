#include "../../../shared/SystemImplBase.h"

#include <windows.h>
#include <atlbase.h>

#include "common/app_build_capabilities.h"
#include "common/RhoDefs.h"
#include "common/ExtManager.h"

#if defined( OS_WINCE )
#include "EmdkDefines.h"
#include "Keyboard.h"
#endif

namespace rho {

class CSystemImpl: public CSystemImplBase
{
private:
#if defined( OS_WINCE )
	CSIP* m_pSip;
#endif
    bool m_bLockedWindowSize;
public:
    CSystemImpl(): CSystemImplBase()
	{
#if defined( OS_WINCE )
	    m_pSip = NULL;
#endif
        m_bLockedWindowSize = false;
	}

    virtual void getScreenWidth(CMethodResult& oResult);
    virtual void getScreenHeight(CMethodResult& oResult);
    virtual void getScreenOrientation(CMethodResult& oResult);
    virtual void getPpiX(CMethodResult& oResult);
    virtual void getPpiY(CMethodResult& oResult);
    virtual void getPhoneId(CMethodResult& oResult);
    virtual void getDeviceName(CMethodResult& oResult);
    virtual void getOsVersion(CMethodResult& oResult);
    virtual void getIsMotorolaDevice(CMethodResult& oResult);
    virtual void getLocale(CMethodResult& oResult);
    virtual void getCountry(CMethodResult& oResult);
    virtual void getIsEmulator(CMethodResult& oResult);
    virtual void getHasCalendar(CMethodResult& oResult);
    virtual void getHasCamera(CMethodResult& oResult);
    virtual void getOemInfo(CMethodResult& oResult);
    virtual void getUuid(CMethodResult& oResult);
#if defined( OS_WINCE )
	bool populateUUID(UNITID_EX* uuid);
	void bytesToHexStr(LPTSTR lpHexStr, LPBYTE lpBytes, int nSize);
#endif
    virtual void getLockWindowSize(CMethodResult& oResult);
    virtual void setLockWindowSize( bool value, CMethodResult& oResult);
    virtual void getKeyboardState(CMethodResult& oResult);
	virtual void setKeyboardState( const rho::String &, CMethodResult& oResult);
    virtual void getScreenAutoRotate(CMethodResult& oResult);
    virtual void setScreenAutoRotate( bool value, CMethodResult& oResult);
    virtual void getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult);
    virtual void getScreenSleeping(rho::apiGenerator::CMethodResult& oResult);
    virtual void setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult);

    virtual void applicationInstall( const rho::String& applicationUrl, CMethodResult& oResult);
    virtual void isApplicationInstalled( const rho::String& applicationName, CMethodResult& oResult);
    virtual void applicationUninstall( const rho::String& applicationName, CMethodResult& oResult);
    virtual void openUrl( const rho::String& url, CMethodResult& oResult);
    virtual void setRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void getRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void deleteRegistrySetting(const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
	virtual void setWindowFrame( int x,  int y,  int width,  int height, CMethodResult& oResult);
    virtual void setWindowPosition( int x,  int y, CMethodResult& oResult);
    virtual void setWindowSize( int width,  int height, rho::apiGenerator::CMethodResult& oResult);
    virtual void getWebviewFramework(rho::apiGenerator::CMethodResult& oResult);
    virtual void bringToFront(rho::apiGenerator::CMethodResult& oResult);
    virtual void runApplicationShowWindow( const rho::String& appName,  const rho::String& params, bool bShow, bool blockingCall, rho::apiGenerator::CMethodResult& oResult);
    virtual void runApplication( const rho::String& appName,  const rho::String& params,  bool blockingCall, rho::apiGenerator::CMethodResult& oResult);
    virtual void getMain_window_closed(rho::apiGenerator::CMethodResult& oResult);

	virtual long OnSIPState(bool bSIPState, const rho::common::CRhoExtData& oExtData)
	{
#if defined( OS_WINCE )
		if (!m_pSip)
			m_pSip = new CSIP();
		if (m_pSip->getCurrentStatus() == SIP_CONTROL_AUTOMATIC)
			m_pSip->ToggleSIPReliably(bSIPState);
#endif
		return S_OK;
	}
};


////////////////////////////////////////////////////////////////////////

class CSystemFactory: public CSystemFactoryBase
{
public:
    ~CSystemFactory(){}

    ISystemSingleton* createModuleSingleton(){ return new CSystemImpl(); }
};

}
