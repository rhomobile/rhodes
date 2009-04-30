/*
 * ports_mngt.cpp
 *
 *  Created on: Apr 28, 2009
 *      Author: Anton
 */
#include "stdafx.h"

#include "ports_mngt.h"

#include "tcmalloc/rhomem.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <winsock.h>

#include "common/RhoMutexLock.h"

using namespace rho::general;

extern "C" {

extern char* wce_wctomb(const wchar_t* w);
extern wchar_t* wce_mbtowc(const char* a);

char* g_listening_port = NULL; 
char* home_url = NULL;
wchar_t* home_url_w = NULL;

const char* get_free_listening_port()
{
	int noerrors = 1;

	CLocalMutexLock change_listening_port;
	
	if ( g_listening_port == NULL )
	{
		LOG(INFO) + "Trying to get free listening port.";
		
		//get free port
		SOCKET sockfd = -1;
		struct sockaddr_in serv_addr = {0};
		struct hostent *server = {0};
		int result = -1;

		//initialising winsock
		WSADATA WsaData;
		if(result = WSAStartup(MAKEWORD(1,1),&WsaData) != 0)
		{
			LOG(WARNING) + "WSAStartup Failed!";
			noerrors = 0;
		}
	
		if ( noerrors )
		{
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if ( sockfd < 0 )
			{
				LOG(WARNING) + ("Unable to open socket");
				noerrors = 0;
			}
			
			if ( noerrors )
			{
				server = gethostbyname( "localhost" );

				memset((void *) &serv_addr, 0, sizeof(serv_addr));
				serv_addr.sin_family = AF_INET;
				serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
				serv_addr.sin_port = htons(0);

				if ( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) )
				{
					LOG(WARNING) + "Unable to bind";
					noerrors = 0;
				}
				else
				{
					char buf[10] = {0};
					int length = sizeof( serv_addr );

					getsockname( sockfd, (struct sockaddr *)&serv_addr, &length );
					
					sprintf(buf,"%d",ntohs(serv_addr.sin_port));
					
					g_listening_port = strdup(buf);
				}
				//Clean up
				closesocket(sockfd);
			}

			WSACleanup();
		}
	}

	if ( !noerrors )
		g_listening_port = strdup("8080");
	
	return g_listening_port;
}

const char* get_home_url() {
	
	if ( home_url == NULL ) {
		char buf[200] = {0};	
		CLocalMutexLock change_home_url;
		
		sprintf( buf, "http://localhost:%s", get_free_listening_port());
		home_url = strdup(buf);
	}
	
	return home_url;
}

const wchar_t* get_home_url_w() {

	if ( home_url_w == NULL ) {
		CLocalMutexLock change_home_url_w;
		home_url_w = wce_mbtowc(get_home_url());
	}

	return home_url_w;
}

}