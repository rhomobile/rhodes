#include "stdafx.h"
#include "EngineEventListner.h"
#include "../../../shared/common/RhodesApp.h"

namespace rho
{
	namespace engineeventlistner
	{
		void CEngineEventMngr::setEngineType(eEngineType eEngType)
		{
			m_eEngType = eEngType;
			
		}
		void CEngineEventMngr::registerExtension(const String& strName, IEngineEventListner* pExt)
		{
			m_hashExtensions.put(strName, pExt);
		}
		eEngineType CEngineEventMngr::getEngineType()
		{
			return m_eEngType;
		}
		void CEngineEventMngr::notifyDOMInjector()
		{
			for ( HashtablePtr<String, IEngineEventListner*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
			{
				(it->second)->notifyDOMInjector();
			}
		}
		void CEngineEventMngr::setEngineInterface(IDOMInjectorEngineInterface* pEngineIf)
		{
			m_pEngineIf = pEngineIf;
		}
		void CEngineEventMngr::executAnonymousJs(wchar_t* szFunctionText)
		{
			int nTabID = RHODESAPP().getExtManager().getCurrentTabID();
			if(m_pEngineIf)
			{
				m_pEngineIf->executAnonymousJs(szFunctionText, nTabID);
			}

		}
		void CEngineEventListner::notifyDOMInjector()
		{
			//default implementation
		}

	}
}