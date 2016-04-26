#include "stdafx.h"

//-----------------------------------------------------------------------------

Exception::Exception(int err_no, char *format, ...)
	{
	ErrNo = err_no;

	va_list marker;
	va_start(marker, format);
	vsprintf(Message, format, marker);
	va_end(marker);
	}

//-----------------------------------------------------------------------------

void Exception::Print()
	{
	printf("Error %d: \n%s\n", ErrNo, Message);
	}
