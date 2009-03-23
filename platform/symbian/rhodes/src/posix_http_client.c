/*
 * posix_http_client.c
 *
 *  Created on: Jan 30, 2009
 *      Author: Anton
 */

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

#define  MAXBUF  	1024
#define  LINE_SIZE  512

#include "posix_http_client.h"
//#include "tcmalloc/rhomem.h"

//Utils
int   startsWith(const char *a, const char *b);
char* rtrim(char *s);
char* ltrim(char *s);
char* trim(char *s);
char* chomp(char *s);
char  socreadByte(int sockfd);
int   socreadln(int sockfd, char *s, int size);


int parse_url(const char *url, UrlInfo *urlinfo)
{
	// see if we have a proto
	char *s = strstr(url, "://");
	int ret;

	bzero(urlinfo->host, MAX_HOST_LEN);
	
	if (s)
		url = s + strlen("://");

	/* search for a port separator */
	if (strchr(url, ':') != NULL)
	{
		ret = sscanf(url, "%[^:]:%hu/", urlinfo->host, (short unsigned int*) &urlinfo->port);
		
		if (urlinfo->port < 1)
			return 1;
		
		ret -= 1;
	}
	else
	{
		urlinfo->port = 80;
		ret = sscanf(url, "%[^/]/", urlinfo->host);
	}

	if (ret < 1)
		return 1;

	return 0;
}

int sendHttpRequest(const char * url, 
					const char* cookie,
					const char* requestBody,
					const char* contentType,
					int bodySize, 
					char** responseBody, //[out] 
					char** authCookie, //[out]
					int isPost)
{
	int sockfd;
	struct sockaddr_in dest;
	struct hostent *he;
	int nRet;
	char buffer[LINE_SIZE];
	UrlInfo urlInfo;
	int content_length = 0;
	char* requestBuf = 0;
	int retval = 0;
	int found_set_cookie = 0;

	if (!url)
	{
		printf("Bad parameters, expected [url]\n");
		goto ERROR_HANDLER; //sorry there is no exceptions in pure C
	}
	
	//parse host from url
	if ( parse_url(url, &urlInfo) )
	{
		printf("Unable to parse url\n");
		goto ERROR_HANDLER;
	}
	
	if (requestBody && bodySize > 0)
		requestBuf = (char*) malloc(MAXBUF + bodySize);
	else
		requestBuf = (char*) malloc(MAXBUF);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		printf("Unable to open socket\n");
		goto ERROR_HANDLER;
	}
	
	//Initialize server address/port struct
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(urlInfo.port); /*default HTTP Server port */

	dest.sin_addr.s_addr = inet_addr(urlInfo.host);
	if (dest.sin_addr.s_addr == INADDR_NONE)
	{
		he = gethostbyname(urlInfo.host);
		if (!he)
		{
			printf("gethostbyname - failed!\n");
			goto ERROR_HANDLER;
		}
		
		if (he->h_length <= sizeof(dest.sin_addr.s_addr))
			bcopy(he->h_addr, &dest.sin_addr.s_addr, he->h_length);
		else
			bcopy(he->h_addr, &dest.sin_addr.s_addr,
					sizeof(dest.sin_addr.s_addr));
	}

	//Connect to server
	if ( connect(sockfd, (struct sockaddr*) &dest, sizeof(dest)) )
	{
		printf("Unable to connect to server %s:%d!\n", urlInfo.host, urlInfo.port);
		goto ERROR_HANDLER;
	}

	sprintf( requestBuf,
			"%s %s HTTP/1.1\r\n"
				"Host: %s\r\n"
				"UserAgent: SymRhodes\r\n"
				"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
				"Accept-Language: en-us,en;q=0.5\r\n"
				"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
				"Keep-Alive: 300\r\n"
				"Connection: keep-alive\r\n"
				"Content-Type: %s\r\n", 
				isPost ? "POST" : "GET",
				url, 
				urlInfo.host,
				contentType ? contentType : "application/x-www-form-urlencoded");

	
	if (cookie)
	{
		char cookieHeader[LINE_SIZE] = {0};
		sprintf(cookieHeader, "Cookie: %s\r\n", cookie);

		strcat(requestBuf, cookieHeader);
	}
	
	if (requestBody && bodySize > 0)
	{
		char contentLength[LINE_SIZE] = {0};
		sprintf(contentLength, "Content-Length: %d\r\n", bodySize);

		strcat(requestBuf, contentLength);
	}
	
	strcat(requestBuf, "\r\n"); //end headers
	
	if (requestBody && bodySize > 0)
		strcat(requestBuf, requestBody);
	
	nRet = send(sockfd, requestBuf, strlen(requestBuf), 0);

	if ( nRet <= 0 )
	{
		printf("Error sending data -- %d\n", nRet);
		goto ERROR_HANDLER;
	}
	
	//While there's data, read and parse it
	bzero(buffer, sizeof(buffer));
	
	if (socreadln(sockfd, buffer, sizeof(buffer)))
	{
		int status = 0;
	
		chomp(buffer);
		
		if(sscanf(buffer, "%*s %d", &status) !=1) 
		{
			printf("HTTP error: cannot parse HTTP status in response: %s\n", buffer);
			goto ERROR_HANDLER;
		}
		
		printf("Response status:%d",status);
		
		if ( status == 200 )
		{
			do
			{
				if(startsWith(buffer, "\r\n") || startsWith(buffer, "\n") || buffer[0] == 0)
					break;
			
				if(startsWith(buffer, "Content-Length")) 
				{
					if(1 != sscanf(buffer, "%*s%*[: ]%ld", &content_length)) 
					{
						printf("HTTP error: parsing Content-Length response header '%s'\n", buffer);
						goto ERROR_HANDLER;
					}
				      
					if(content_length < 0) 
					{
						printf("HTTP error: Illegal Content-Length response header '%s'\n", buffer);
						goto ERROR_HANDLER;
					}
				}
			
				if(startsWith(buffer, "Set-Cookie")) 
				{
					char *s;
					char set_cookie[LINE_SIZE];
					char *p= strchr(buffer, ':');
				      
					if(!p) 
					{
						printf("HTTP error: parsing Cookie response header '%s'\n", buffer);
						goto ERROR_HANDLER;
					}
				    
					trim(++p);
				      
					/* Strip away cookie-av elements. */ 
					while ( 1 )
					{
						if((s= strstr(p, "Comment")) || (s= strstr(p, "Domain"))  || 
								(s= strstr(p, "expires")) || (s= strstr(p, "Max-Age")) || (s= strstr(p, "Secure")) || (s= strstr(p, "Version")) ) 
						{
							*s = 0;
							continue;
						}
						break;
					}
					
					if ( !(s = strstr(p, "auth_token=;")) )
					{
						snprintf(set_cookie, LINE_SIZE, "%s", trim(p));
						
						if ( !found_set_cookie )
						{
							found_set_cookie = 1;
							
							if ( authCookie )
							{
								*authCookie = (char*)malloc(MAXBUF);
								bzero(*authCookie, sizeof(MAXBUF));
							}
						}
						
						if ( authCookie && *authCookie )
							strcat(*authCookie, set_cookie);
					}
				}
				
				bzero(buffer, sizeof(buffer));
				if ( socreadln(sockfd, buffer, sizeof(buffer)) )
					chomp(buffer);
				else
					break;
			}
			while (1);
		}
	}
	
	if ( responseBody && content_length > 0 )
	{
		int bytes_read = 0;
		int curpos = 0;
		*responseBody = (char*)malloc(content_length + 1);
		
		//read response body
		do
		{
			bytes_read = recv(sockfd, (*responseBody) + curpos, content_length - curpos, 0);
			curpos += bytes_read; 
		}
		while ( bytes_read > 0 && curpos < content_length );
		
		(*responseBody)[content_length] = 0;	
	}
	else if ( responseBody )
	{
		*responseBody = NULL;
	}
	
	if ( !found_set_cookie && authCookie )
		*authCookie = NULL;
	
	//Clean up
	close(sockfd);
	
	retval = 1; //well done!

ERROR_HANDLER:
	
	if ( requestBuf )
		free(requestBuf);
	
	return retval;
}

/**
 * Reads in at most one less than size <code>characters</code> and
 * stores them into the buffer pointed to by s. Reading stops after
 * an EOF or a newline.  
 */
int socreadln(int sockfd, char *s, int size) {
  
  char c;
  unsigned char *p = (unsigned char *)s;
  
  if (!s)
	  return 0;
  
  while(--size && ((c= socreadByte(sockfd)) > 0)) {
    *p++= c;
    if(c == '\n' || c == '\0')
      break;
  }
  
  *p = 0;
  return 1;
}

/**
 * Read a single byte.
 */
char socreadByte(int sockfd) {
  
	char ch;
	if (!sockfd)
	  return 0;
  
	if ( recv(sockfd, &ch, sizeof(char), 0) > 0 )
		return ch;
	
	return 0;
}

/**
 * Return TRUE if the string 'a' starts with the string 'b'
 */
int startsWith(const char *a, const char *b)
	{

	if ((!a || !b) || toupper((int) *a) != toupper((int) *b))
		return 0;

	while (*a && *b)
		{

		if (toupper((int) *a++) != toupper((int) *b++))
			return 0;

		}

	return 1;

	}

/**
  * Removes everything from the first line break or newline (CR|LF)
  */
char *chomp(char *s) 
{	
	if ( !s )
		return NULL;
  
	for (; *s; s++) 
	{
		if (('\r' == *s) || ('\n' == *s)) 
		{
			*s= 0; break;
		}
	}

	return s;
}


/**
 * Remove leading and trailing space from the string
 */
char *trim(char *s) 
{
	if ( !s )
		return NULL;
  
	ltrim(s);
	rtrim(s);

	return s;
}


/**
 * Remove leading white space [ \t\r\n] from the string.
 */
char * ltrim(char *s) 
{
	char *t= s;

	if( !s)
		return NULL;

	while(*t==' ' || *t=='\t' || *t=='\r' || *t=='\n') 
		t++;
  
	if(t!=s) 
	{
		char *r= s;
		do 
		{
			*r++= *t;
		} 
		while(*t++);
	}

	return s;
}


/**
 * Remove trailing white space [ \t\r\n] from the string
 */
char *rtrim(char *s) 
{
	char *t= s;

	if(!s)
		return NULL;

	while(*s) 
		s++;
  
	while(*--s==' ' || *s=='\t' || *s=='\r' || *s=='\n') 
		*s= 0;
  
	return t;
}

