#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

bool WriteRegHandler (rho::String proot, LPCTSTR psubkey, LPCTSTR psetting, rho::String ptype, LPCTSTR pvalue_string, bool persistent);
bool DeleteRegHandler (const rho::String& proot, LPCTSTR psubkey, LPCTSTR psetting, bool persistent, bool create_reg_file);
BOOL WriteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting, DWORD type, const void *pvalue, DWORD size);
BOOL WriteRegSetting (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting, DWORD type, const void *pvalue, DWORD size);
BOOL DeleteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting);
BOOL DeleteRegSetting (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting);
BOOL CreateDeleteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting);
void GetRegistrySetting(const rho::String& hKey, const rho::String& subKey, const rho::String& setting, rho::apiGenerator::CMethodResult& oResult);
HKEY ConvertToHKEY(rho::String neonKey);
DWORD ConvertToType(rho::String neonType);
