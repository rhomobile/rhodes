/*
 * Copyright (c) 2004-2005 Sergey Lyubka <valenok@gmail.com>
 * All rights reserved
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Sergey Lyubka wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

#ifndef CONFIG_HEADER_DEFINED
#define	CONFIG_HEADER_DEFINED

#define	VERSION		"1.42"		/* Version			*/
#define	CONFIG_FILE	"shttpd.conf"	/* Configuration file		*/
#define	HTPASSWD	".htpasswd"	/* Passwords file name		*/

//#ifdef __SYMBIAN32__
//	#define	URI_MAX		16384
//#else
//	#define	URI_MAX		4096    /*16384 /Default max request size	*/
//#endif
//#define	URI_MAX		16384 /* Default max request size	*/
//This is maximum URI length
#define	URI_MAX		2048
//Buffer size for input request data
#define INPUT_BUF_MAX 2048
//Buffer size for output data
#define OUT_BUF_MAX 4096

#define	LISTENING_PORTS	"8080"		/* Default listening ports	*/
#define	INDEX_FILES	"index_erb.iseq,index.html,index.htm,index.php,index.cgi"
#define	CGI_EXT		"cgi,pl,php"	/* Default CGI extensions	*/
#define	SSI_EXT		"shtml,shtm"	/* Default SSI extensions	*/
#define	REALM		"mydomain.com"	/* Default authentication realm	*/
#define	DELIM_CHARS	","		/* Separators for lists		*/
#define	EXPIRE_TIME	3600		/* Expiration time, seconds	*/
#define	ENV_MAX		4096		/* Size of environment block	*/
#define	CGI_ENV_VARS	64		/* Maximum vars passed to CGI	*/
#define	SERVICE_NAME	"SHTTPD " VERSION	/* NT service name	*/
#define NO_AUTH 1
#ifndef NO_CGI
#define NO_CGI 1
#endif //NO_CGI
#define NO_SSL 1

#endif /* CONFIG_HEADER_DEFINED */
