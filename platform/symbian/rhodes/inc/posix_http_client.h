/*
 * posix_http_client.h
 *
 *  Created on: Jan 31, 2009
 *      Author: Anton
 */

#ifndef POSIX_HTTP_CLIENT_H_
#define POSIX_HTTP_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_HOST_LEN 512

typedef struct _UrlInfo
{
	char host[MAX_HOST_LEN];
	unsigned short port;
} UrlInfo;

int sendHttpRequest(const char * url, 
		const char* cookie,
		const char* requestBody,
		const char* contentType,
		int bodySize, 
		char** responseBody, //[out] 
		char** authCookie, //[out]
		int isPost //0 - get, otherwise post
		);

#ifdef __cplusplus
}
#endif

#endif /* POSIX_HTTP_CLIENT_H_ */
