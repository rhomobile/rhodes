/*  wcecompat: Windows CE C Runtime Library "compatibility" library.
 *
 *  Copyright (C) 2001-2002 Essemer Pty Ltd.  All rights reserved.
 *  http://www.essemer.com.au/
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __wcecompat__PROCESS_H__
#define __wcecompat__PROCESS_H__

#include <winsock2.h>
#ifdef __cplusplus
extern "C" {
#endif


int _getpid(void);
#define getpid _getpid

__inline unsigned long _beginthreadex( void *security, unsigned stack_size, void* start_address , void *arglist, unsigned initflag, unsigned *thrdaddr ) 
{
	return (unsigned long) CreateThread( (SECURITY_ATTRIBUTES*)security, stack_size, (LPTHREAD_START_ROUTINE)start_address,arglist,initflag,(DWORD*)thrdaddr);
}

__inline unsigned long _beginthread( void*start_address , unsigned stack_size, void *arglist) 
{
	return _beginthreadex(NULL,stack_size,start_address,arglist,0,NULL);
}

__inline void _endthreadex (unsigned long retval)
{
	ExitThread(retval);
}

__inline void _endthread ()
{
	_endthreadex(0);
}

#ifdef __cplusplus
}
#endif


#endif // __wcecompat__PROCESS_H__
