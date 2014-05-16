
#include "..\include\winrtsock.h"
#include "include\inet_aton.h"

unsigned long __stdcall inet_addr(const char * cp)
{
	struct in_addr val;
	if (inet_aton(cp, &val))
		return (val.s_addr);
	return (INADDR_NONE);
}