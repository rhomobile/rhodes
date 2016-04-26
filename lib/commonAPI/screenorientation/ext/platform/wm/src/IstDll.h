/** 
 * Abstarcts the Symbol properitory IST APIs. 
 * The APIs are avilable only on selected Symbol devices 
 * \file
 */
#ifndef ISTDLL_H_34612E1C_97AA_43C1_B1E1_61EB28C378DE
#define ISTDLL_H_34612E1C_97AA_43C1_B1E1_61EB28C378DE

#pragma once


#define IST_DEFAULT_DEVICE_NAME			L"IST1:"
#define IST_AUTO_ROTATION				(0x01 << 1)
#define IST_ERROR_SUCCESS				ERROR_SUCCESS

namespace ScreenOrientationExt
{
	class CIstDll 
	{
		public:
			static bool IsPresent();

			CIstDll();
			virtual ~CIstDll();

			bool	Open();
			bool	Close();
			void	EnableAutoRotate(bool flag = true);
			bool	IsAutoRotateEnabled();

		protected:
			bool	LoadDll(const wchar_t* szDll);
			void	UnloadDll();

			bool	ISTOpen(const wchar_t *pszDev = IST_DEFAULT_DEVICE_NAME);
			bool	ISTClose();
			bool	ISTGetSystemConfig(DWORD &dwConfig);
			bool	ISTSetSystemConfig(DWORD dwConfig);

		private:
			CIstDll(const CIstDll&);
			CIstDll& operator=(const CIstDll&);

			HMODULE		m_hISTModule;
			HANDLE		m_hIST;
	};
}
#endif // ISTDLL_H_34612E1C_97AA_43C1_B1E1_61EB28C378DE