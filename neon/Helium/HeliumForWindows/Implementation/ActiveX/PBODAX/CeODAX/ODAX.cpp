// ODAX.cpp : Implementation of CODAX

#include "stdafx.h"
#include "ODAX.h"

extern CResultSet *pResultSet;
extern char ErrMsgBuf[512];

// CODAX

/******************************************************************************/
STDMETHODIMP CODAX::GetLastErrorString(BSTR* perror)
{
	LPWSTR wbuf = MakeUniStr(ErrMsgBuf);

	*perror = SysAllocString(wbuf);
	delete []wbuf;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::FileExists(BSTR filename, VARIANT_BOOL* ret)
{
	// -1 = failure, so return false, file does not exist
	*ret = GetFileAttributes((LPWSTR)filename) == -1 ? 0 : -1; 	// 0 == FALSE, -1 == TRUE

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::Copy(BSTR srcFileName, BSTR destFileName, LONG* status)
{
	*status = CopyFile((LPWSTR)srcFileName, (LPWSTR)destFileName, FALSE) ? 0 : GetLastError();

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::Delete(BSTR FileName, LONG* status)
{
	*status = DeleteFile((LPWSTR)FileName) ? 0 : GetLastError();

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::Move(BSTR srcFileName, BSTR destFileName, LONG* status)
{
	*status = MoveFile((LPWSTR)srcFileName, (LPWSTR)destFileName) ? 0 : GetLastError();

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::Execute(BSTR sql_statement, int format, BSTR delimiter, VARIANT_BOOL firstrow, int* pcount)
{
	char *stat = MakeAnsiStr((LPWSTR)sql_statement);
	char *delim = MakeAnsiStr((LPWSTR)delimiter);

	if (strlen(stat) == 0)
		{
		*pcount = StoreError(E_INVALID_STAT_PARAM, "Empty SQL statement parameter string not allowed");
		goto Exit;
		}

	if ((format != 0) && (format != 1))
		{
		*pcount = StoreError(E_INVALID_FORMAT_PARAM, "Firstrow parameter not valid value");
		goto Exit;
		}

	if (strlen(delim) != 1)
		{
		*pcount = StoreError(E_INVALID_DELIMITER_PARAM, "Delimiter parameter must be one character only");
		goto Exit;
		}

	if ((firstrow != 0)  && (firstrow != -1))
		{
		*pcount = StoreError(E_INVALID_FIRSTROW_PARAM, "Firstrow parameter not valid value");
		goto Exit;
		}

	*pcount = DoExecute(stat, format, *delim, abs(firstrow));

Exit:
	delete []stat;
	delete []delim;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::Select(BSTR sql_statement, BSTR delimiter, VARIANT_BOOL firstrow, int* pcount)
{
	char *stat = MakeAnsiStr((LPWSTR)sql_statement);
	char *delim = MakeAnsiStr((LPWSTR)delimiter);

	if (strlen(stat) == 0)
		{
		*pcount = StoreError(E_INVALID_STAT_PARAM, "Empty SQL statement parameter string not allowed");
		goto Exit;
		}

	if (strlen(delim) != 1)
		{
		*pcount = StoreError(E_INVALID_DELIMITER_PARAM, "Delimiter parameter must be one character only");
		goto Exit;
		}

	if ((firstrow != 0)  && (firstrow != -1))
		{
		*pcount = StoreError(E_INVALID_FIRSTROW_PARAM, "Firstrow parameter not valid value");
		goto Exit;
		}

	*pcount = DoSelect(stat, *delim, abs(firstrow));

Exit:
	delete []stat;
	delete []delim;

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::MoveFirst(int* presult)
{
	if (pResultSet)
		*presult  = pResultSet->MoveFirst();
	else
		*presult = StoreError(E_NO_RESULT_SET, "No result set");

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::MoveLast(int* presult)
{
	if (pResultSet)
		*presult  = pResultSet->MoveLast();
	else
		*presult = StoreError(E_NO_RESULT_SET, "No result set");

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::MoveNext(int* presult)
{
	if (pResultSet)
		*presult  = pResultSet->MoveNext();
	else
		*presult = StoreError(E_NO_RESULT_SET, "No result set");

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::MovePrev(int* presult)
{
	if (pResultSet)
		*presult  = pResultSet->MovePrev();
	else
		*presult = StoreError(E_NO_RESULT_SET, "No result set");

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::Get(BSTR column_name, BSTR* pvar)
{
	WCHAR *wbuf;
	char *buf, *name;

	if (pResultSet)
		{
		name = MakeAnsiStr((LPWSTR) column_name);

		buf = new char[pResultSet->GetFieldSize() + 1];

		pResultSet->GetField(name, buf);

		delete []name;

		wbuf = MakeUniStr(buf);

		delete []buf;

		*pvar = SysAllocString(wbuf);

		delete []wbuf;
		}

	return S_OK;
}

/******************************************************************************/
STDMETHODIMP CODAX::Clear(void)
{
	delete pResultSet;
	pResultSet = NULL;

	return S_OK;
}
