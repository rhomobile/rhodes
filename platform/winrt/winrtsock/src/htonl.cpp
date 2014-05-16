
#include "..\include\winrtsock.h"

u_long __stdcall htonl(u_long x)
{
	u_char *s = (u_char *) &x;
	return (u_long) (s[0] << 24 | s[1] << 16 | s[2] << 8 | s[3]);
}