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


#ifndef __wcecompat__WINSOCK_EXTRAS_H__
#define __wcecompat__WINSOCK_EXTRAS_H__


#ifdef __cplusplus
extern "C" {
#endif

struct servent {
    char* s_name;
    char** s_aliases;
    short int s_port;
    char* s_proto;
} servent;

struct servent* getservbyname(const char* name, const char* proto);



#ifdef __cplusplus
}
#endif


#endif // __wcecompat__WINSOCK_EXTRAS_H__
