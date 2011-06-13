/*
 * ports_mngt.cpp
 *
 *  Created on: Apr 28, 2009
 *      Author: Anton
 */
#include "tcmalloc/rhomem.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "common/RhoMutexLock.h"

char* g_listening_port = NULL; 
char* home_url = NULL;

RHO_INIT_LOCK(change_listening_port);
RHO_INIT_LOCK(change_home_url);

char* get_free_listening_port()
{
	int noerrors = 1;

	RHO_LOCK(change_listening_port);
	
	if ( g_listening_port == NULL )
	{
		//get free port
		int sockfd = -1;
		struct sockaddr_in serv_addr = {0};
		struct hostent *server = {0};

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if ( sockfd < 0 )
		{
			printf("Unable to open socket\n");
			noerrors = 0;
		}
		
		if ( noerrors )
		{
			server = gethostbyname( "127.0.0.1" );

			bzero((char *) &serv_addr, sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			serv_addr.sin_port = htons(0);

			if ( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) )
			{
				printf("Unable to connect to socket\n");
				noerrors = 0;
			}
			else
			{
				char buf[10] = {0};
				socklen_t length = sizeof( serv_addr );

				getsockname( sockfd, (struct sockaddr *)&serv_addr, &length );
				
				sprintf(buf,"%d",ntohs(serv_addr.sin_port));
				
				g_listening_port = strdup(buf);
			}
			//Clean up
			close(sockfd);
		}

	}

	if ( !noerrors )
		g_listening_port = strdup("8080");
	
	RHO_UNLOCK(change_listening_port);
	
	return g_listening_port;
}

char* get_home_url() {
	
	if ( home_url == NULL ) {
		char buf[200] = {0};	
		RHO_LOCK(change_home_url);
		
		sprintf( buf, "http://127.0.0.1:%s", get_free_listening_port());
		home_url = strdup(buf);
		
		RHO_UNLOCK(change_home_url);
	}
	
	return home_url;
}
