#include "../../../shared/generated/cpp/KeyStateBase.h"
#include "KeyState.h"
#include "common/RhodesApp.h"
#include <algorithm>

namespace rho {
using namespace apiGenerator;
using namespace common;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "KeyState"

class CKeyStateImpl: public CKeyStateSingletonBase
{
private:
	CKeyState* m_pKeyState;
	CRhoExtData rhodesData;
public:
    CKeyStateImpl(): CKeyStateSingletonBase() 
	{
		m_pKeyState = NULL;
		rhodesData = RHODESAPP().getExtManager().makeExtData();
	}

	void createObject()
	{
		m_pKeyState = new CKeyState((HINSTANCE)rhodesData.m_hInstance, rhodesData.m_hBrowserWnd);
		m_pKeyState->init();
	}
	virtual void showStates( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pKeyState)
			createObject();
	
		rho::String comparitor;
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			comparitor.assign(iterator->first);
			std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);
			if (comparitor.compare("right") == 0)
				m_pKeyState->SetRight(atoi(iterator->second.c_str()));
			else if (comparitor.compare("top") == 0)
				m_pKeyState->SetTop(atoi(iterator->second.c_str()));
			else if (comparitor.compare("width") == 0)
				m_pKeyState->SetWidth(atoi(iterator->second.c_str()));
			else if (comparitor.compare("height") == 0)
				m_pKeyState->SetHeight(atoi(iterator->second.c_str()));
			else
				LOG(WARNING) + "Unrecognised parameter passed to showIcon (showStates): " + iterator->first;
		}

		if (m_pKeyState->GetIconShown())
		{
			LOG(INFO) + "Did not show icon as it is already shown";
		}
		else
		{
			m_pKeyState->SetIconShown(true);
			m_pKeyState->Show();
		}
		oResult.set("true");	
	}

	virtual void hideStates(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pKeyState)
			createObject();

		if (!m_pKeyState->GetIconShown())
		{
			LOG(INFO) + "Did not hide icon as it is already hidden";
		}
		else
		{
			m_pKeyState->SetIconShown(false);
			m_pKeyState->Hide();
		}
		oResult.set("true");	
	}

};

class CKeyStateSingleton: public CKeyStateSingletonBase
{
    ~CKeyStateSingleton(){}
};

class CKeyStateFactory: public CKeyStateFactoryBase
{
    ~CKeyStateFactory(){}
    virtual IKeyStateSingleton* createModuleSingleton()
	{
		return new CKeyStateImpl();
	}
};

extern "C" void Init_Keystate_extension()
{
    CKeyStateFactory::setInstance( new CKeyStateFactory() );
    Init_KeyState_API();
}


}