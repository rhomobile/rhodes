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
		void CEngineEventMngr::injectDOMElements()
		{
			for ( HashtablePtr<String, IEngineEventListner*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
			{
				(it->second)->injectDOMElements();
			}
		}
		void CEngineEventMngr::setEngineInterface(IDOMInjectorEngineInterface* pEngineIf)
		{
			m_pEngineIf = pEngineIf;
		}
		bool CEngineEventMngr::executAnonymousJs(wchar_t* szFunctionText)
		{
			int nTabID = RHODESAPP().getExtManager().getCurrentTabID();
			if(m_pEngineIf)
			{
				return m_pEngineIf->executAnonymousJs(szFunctionText, nTabID);
			}

		}
		void CEngineEventMngr::onBeforeNavigate(const wchar_t* onBeforeNavigate)
		{
			for ( HashtablePtr<String, IEngineEventListner*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
			{
				(it->second)->onBeforeNavigate(onBeforeNavigate);
			}
		}
		void CEngineEventListner::injectDOMElements()
		{
			//default implementation
		}
		void CEngineEventListner::onBeforeNavigate(const wchar_t* szUrlBeingNavigatedTo)
		{
			//default implementation
		}

	}
}