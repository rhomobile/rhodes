#include "StdAfx.h"

//------------------------------------------------------------------------------

CFile::CFile()
	{
	H = INVALID_HANDLE_VALUE;
	}

//------------------------------------------------------------------------------

CFile::~CFile()
	{
	CloseHandle(H);
	}

//------------------------------------------------------------------------------

BOOL CFile::FileCreate(LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
	{
	WCHAR *wstr = MakeUniStr(lpFileName);

	Name.SetStr(lpFileName);

	H = CreateFile(wstr,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);

	delete []wstr;

	if (H == INVALID_HANDLE_VALUE)
		return 0;

	return 1;
	}

//------------------------------------------------------------------------------

BOOL CFile::FileRead(LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped)
	{
	return ReadFile(H,
		lpBuffer,
		nNumberOfBytesToRead,
		lpNumberOfBytesRead,
		lpOverlapped);
	}

//------------------------------------------------------------------------------

BOOL CFile::FileWrite(LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped)
	{
	BOOL ret;

	ret = WriteFile(H, lpBuffer,
		nNumberOfBytesToWrite,
		lpNumberOfBytesWritten,
		lpOverlapped);

//	FlushFileBuffers(H);

	return ret;
	}

//------------------------------------------------------------------------------

void CFile::SetPos(long pos, DWORD method)
	{
	SetFilePointer(H, pos, NULL, method);
	}

//------------------------------------------------------------------------------

BOOL CFile::Remove(void)
	{
	int ret;
	WCHAR *wstr = MakeUniStr(Name.GetStr());

	FileClose();
	ret = DeleteFile(wstr);

	delete []wstr;

	return ret;
	}

//------------------------------------------------------------------------------

BOOL CFile::Rename(char *newname)
	{
	BOOL ret;
	WCHAR *wsrcstr = MakeUniStr(Name.GetStr());
	WCHAR *wdeststr = MakeUniStr(newname);

	FileClose();

	ret = MoveFile(wsrcstr, wdeststr);

	if (ret)
		Name.SetStr(newname);

	delete []wsrcstr;
	delete []wdeststr;

	return ret;
	}

//------------------------------------------------------------------------------

void CFile::FileClose(void)
	{
	CloseHandle(H);
	H = INVALID_HANDLE_VALUE;
	}

//------------------------------------------------------------------------------

bool CFile::FileIsOpen(void)
	{
	return H == INVALID_HANDLE_VALUE ? false : true;
	}

//------------------------------------------------------------------------------

BOOL CFile::FileWriteString(DWORD *written, char *format, ...)
	{
	char buf[512];
	va_list args;

	va_start(args, format);

	vsprintf(buf, format, args);

	va_end(args);

	return FileWrite(buf, strlen(buf), written, NULL);
	}

//------------------------------------------------------------------------------

int CFile::FileGetString(char *buf, int bufsize)
	{
	char ch;
	int ct;
	DWORD read;
	
	for (ct=0; (ReadFile(H, &ch, 1, &read, NULL) != 0) && (read == 1) && (ch != 0x0D) && (ct < bufsize); ct++)
		buf[ct] = ch;

	if ((ReadFile(H, &ch, 1, &read, NULL) != 0) && (read == 1))
		{
		if (ch != 0x0A)
			SetPos(-1, FILE_CURRENT);
		}

	buf[ct] = 0;

	return ct;
	}

//------------------------------------------------------------------------------

char *CFile::GetName(void)
	{
	return Name.GetStr();
	}