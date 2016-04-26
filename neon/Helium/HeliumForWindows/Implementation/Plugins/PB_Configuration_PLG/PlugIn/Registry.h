/******************************************************************************/
BOOL WriteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting, DWORD type, const void *pvalue, DWORD size);
BOOL WriteRegSetting (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting, DWORD type, const void *pvalue, DWORD size);
BOOL DeleteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting);
BOOL DeleteRegSetting (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting);
BOOL CreateDeleteRegFile (HKEY hroot, LPCWSTR psubkey, LPCWSTR psetting);

/******************************************************************************/
#define WRITELOG(format, ...) WriteLog (_T(__FUNCTION__), __LINE__, format, __VA_ARGS__)

void WriteLog (LPCWSTR pfunction, int line, LPCWSTR pformat, ...);
