#include "stdafx.h"
#include "EngineEventListner.h"
namespace rho
{
	namespace engineeventlistner
	{
		void CEngineEventMngr::notifyEngineCreate(eEngineType eEngType)
		{
			m_eEngType = eEngType;
			/*for ( HashtablePtr<String, IEngineEventListner*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
			{
				(it->second)->notifyEngineCreate( eEngType );
			}*/
		}
		void CEngineEventMngr::registerExtension(const String& strName, IEngineEventListner* pExt)
		{
			m_hashExtensions.put(strName, pExt);
		}
		eEngineType CEngineEventMngr::getEngineType()
		{
			return m_eEngType;
		}

	}
}