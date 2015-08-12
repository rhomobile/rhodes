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
		struct IDOMInjectorEngineInterface
		{
			virtual void executAnonymousJs(wchar_t* szFunctionText, int nTabID)=0;
		};

		struct IEngineEventListner
		{
			//virtual void notifyEngineCreate(eEngineType eEngType)=0;	
			virtual void notifyDOMInjector()=0;
		};
		class CEngineEventMngr : public IEngineEventListner
		{
		private:
			eEngineType m_eEngType;
			IDOMInjectorEngineInterface* m_pEngineIf;
			rho::Hashtable/*Ptr*/<String, IEngineEventListner*> m_hashExtensions;

		public:
            eEngineType getEngineType();
			void setEngineType(eEngineType eEngType);
			void setEngineInterface(IDOMInjectorEngineInterface* pEngineIf);
			void registerExtension(const String& strName, IEngineEventListner* pExt);
			void executAnonymousJs(wchar_t* szFunctionText);
			virtual void notifyDOMInjector();

		};
		class CEngineEventListner : public IEngineEventListner
		{
		public:
			virtual void notifyDOMInjector();
		};
		
	}
}
