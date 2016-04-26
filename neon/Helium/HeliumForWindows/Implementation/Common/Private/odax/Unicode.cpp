#include "StdAfx.h"

LPWSTR MakeUniStr(const char *str)
	{
	int size;
	PWCHAR buf;

	size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, NULL, 0);

	buf = new WCHAR[size];

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, buf, size);

	return buf;
	}
//----------------------------------------------------------------------------

char *MakeAnsiStr(LPWSTR str)
	{
	int size;
	char *buf;
	int err;

	size = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	err = GetLastError();

	buf = new char[size];

	WideCharToMultiByte(CP_ACP, 0, str, -1, buf, size, NULL, NULL);

	return buf;
	}
