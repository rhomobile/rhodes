#include "StdAfx.h"

static int Err;
char ErrMsgBuf[512];

//-----------------------------------------------------------------------------

void ClearError(void)
	{
	Err = 0;
	}

//-----------------------------------------------------------------------------

int StoreError(int err, char *format, ...)
	{
	Err = err;

	va_list marker;
	va_start(marker, format);
	vsprintf(ErrMsgBuf, format, marker);
	va_end(marker);

	return Err;
	}

//-----------------------------------------------------------------------------

int ReturnErr(void)
	{
	return Err;
	}

//-----------------------------------------------------------------------------

char *ReturnErrStr(void)
	{
	return ErrMsgBuf;
	}