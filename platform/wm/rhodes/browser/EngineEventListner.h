#pragma once
#include "common/RhoStd.h"
namespace rho
{
	namespace engineeventlistner
	{
		enum eEngineType
		{
			eCeIe,
			eWmIe,
			eWebkit
		};
		struct IEngineEventListner
		{
			//virtual void notifyEngineCreate(eEngineType eEngType)=0;	
		};
		class CEngineEventMngr : public IEngineEventListner
		{
		private:
			eEngineType m_eEngType;
			rho::Hashtable/*Ptr*/<String, IEngineEventListner*> m_hashExtensions;

		public:
            eEngineType getEngineType();
			virtual void notifyEngineCreate(eEngineType eEngType);
			virtual void registerExtension(const String& strName, IEngineEventListner* pExt);

		};
		class CEngineEventListner : public IEngineEventListner
		{
		};
	}
}
