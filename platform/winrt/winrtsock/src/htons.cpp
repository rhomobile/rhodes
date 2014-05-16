
#include "..\include\winrtsock.h"

u_short __stdcall htons(u_short x)
{
	u_char *s = (u_char *) &x;
	return (u_short) (s[0] << 8 | s[1]);
}