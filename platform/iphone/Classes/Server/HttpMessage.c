/*
 *  HttpMessage.cpp
 *  Browser
 *
 *  Created by adam blum on 9/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 *  Some bits of code here borrowed from Sergey Lyubka's shttpd
 *  under "THE BEER-WARE LICENSE"
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined (_WIN32)
#define	O_BINARY 0
#include <unistd.h>
#endif

#include "defs.h"
#include "HttpContext.h"
#include "HttpMessage.h"
#include "Dispatcher.h"

#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "HttpMessage"

const struct vec _known_http_methods[] = {
{"GET",		3},
{"POST",	4},
{"PUT",		3},
{"DELETE",	6},
{"HEAD",	4},
{NULL,		0}
};

/*
 * This structure tells how HTTP headers must be parsed.
 * Used by _HTTPParseHeaders() function.
 */
#define	OFFSET(x)	offsetof(struct headers, x)
static const struct http_header http_headers[] = {
{16, HDR_INT,	 OFFSET(cl),		"Content-Length: "	},
{14, HDR_STRING, OFFSET(ct),		"Content-Type: "	},
{12, HDR_STRING, OFFSET(useragent),	"User-Agent: "		},
{19, HDR_DATE,	 OFFSET(ims),		"If-Modified-Since: "	},
{15, HDR_STRING, OFFSET(auth),		"Authorization: "	},
{9,  HDR_STRING, OFFSET(referer),	"Referer: "		},
{8,  HDR_STRING, OFFSET(cookie),	"Cookie: "		},
{10, HDR_STRING, OFFSET(location),	"Location: "		},
{8,  HDR_INT,	 OFFSET(status),	"Status: "		},
{7,  HDR_STRING, OFFSET(range),		"Range: "		},
{12, HDR_STRING, OFFSET(connection),"Connection: "		},
{19, HDR_STRING, OFFSET(transenc),	"Transfer-Encoding: "	},
{8,  HDR_STRING, OFFSET(accept),	"Accept: "		},
{17, HDR_STRING, OFFSET(accept_encoding), "Accept-Encoding: " },
{17, HDR_STRING, OFFSET(accept_language), "Accept-Language: " },
{6,  HDR_STRING, OFFSET(host),		"Host: "	},
{18, HDR_STRING, OFFSET(x_requested_with), "X-Requested-With: "	},
{0,  HDR_INT,	 0,			NULL			}
};

/*
 * Convert month to the month number. Return -1 on error, or month number
 */
static int
montoi(const char *s)
{
	static const char *ar[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	size_t	i;
    
	for (i = 0; i < sizeof(ar) / sizeof(ar[0]); i++)
		if (!strcmp(s, ar[i]))
			return (i);
    
	return (-1);
}

/*
 * Parse date-time string, and return the corresponding time_t value
 */
static time_t
date_to_epoch(const char *s)
{
	struct tm	tm, *tmp;
	char		mon[32];
	int		sec, min, hour, mday, month, year;
    
	(void) memset(&tm, 0, sizeof(tm));
	sec = min = hour = mday = month = year = 0;
    
	if (((sscanf(s, "%d/%3s/%d %d:%d:%d",
                 &mday, mon, &year, &hour, &min, &sec) == 6) ||
         (sscanf(s, "%d %3s %d %d:%d:%d",
                 &mday, mon, &year, &hour, &min, &sec) == 6) ||
         (sscanf(s, "%*3s, %d %3s %d %d:%d:%d",
                 &mday, mon, &year, &hour, &min, &sec) == 6) ||
         (sscanf(s, "%d-%3s-%d %d:%d:%d",
                 &mday, mon, &year, &hour, &min, &sec) == 6)) &&
	    (month = montoi(mon)) != -1) {
		tm.tm_mday	= mday;
		tm.tm_mon	= month;
		tm.tm_year	= year;
		tm.tm_hour	= hour;
		tm.tm_min	= min;
		tm.tm_sec	= sec;
	}
    
	if (tm.tm_year > 1900)
		tm.tm_year -= 1900;
	else if (tm.tm_year < 70)
		tm.tm_year += 100;
    
	/* Set Daylight Saving Time field */
    time_t	_current_time = time(0);
	tmp = localtime(&_current_time);
	tm.tm_isdst = tmp->tm_isdst;
    
	return (mktime(&tm));
}

int
_strncasecmp(const char *str1, const char *str2, size_t len)
{
	register const unsigned char	*s1 = (unsigned char *) str1,
    *s2 = (unsigned char *) str2, *e;
	int				ret;
    
	for (e = s1 + len - 1; s1 < e && *s1 != '\0' && *s2 != '\0' &&
         tolower(*s1) == tolower(*s2); s1++, s2++) ;
	ret = tolower(*s1) - tolower(*s2);
    
	return (ret);
}

static int
_HttpGetHeadersLen(const char *buf, size_t buflen)
{
	const char	*s, *e;
	int		len = 0;
    
	for (s = buf, e = s + buflen - 1; len == 0 && s < e; s++)
    /* Control characters are not allowed but >=128 is. */
		if (!isprint(* (unsigned char *) s) && *s != '\r' &&
		    *s != '\n' && * (unsigned char *) s < 128)
			len = -1;
		else if (s[0] == '\n' && s[1] == '\n')
			len = s - buf + 2;
		else if (s[0] == '\n' && &s[1] < e &&
                 s[1] == '\r' && s[2] == '\n')
			len = s - buf + 3;
    
	return (len);
}

static bool
_HTTPGetRequestMethod(HttpContextRef context, const char *buf)
{
	const struct vec *v;
    
	/* Set the request method */
	for (v = _known_http_methods; v->ptr != NULL; v++)
		if (!memcmp(buf, v->ptr, v->len)) {
			context->_request->_method = v - _known_http_methods;
            RAWTRACE2("Request method %s = %d", v->ptr, context->_request->_method);
			break;
		}
    
	return (v->ptr == NULL);
}

const char*
HTTPGetMethod(int m) {
	if ( m < (sizeof(_known_http_methods)/sizeof(struct vec)-1) )
		return _known_http_methods[m].ptr;
	return "Unknown";
}

void
_HTTPParseHeaders(const char *s, int len, struct headers *parsed)
{
	const struct http_header    *h;
	struct parsed_header        *v;
	const char                  *p, *e = s + len;
    
	RAWTRACE3("parsing headers (len %d): [%.*s]", len, len, s);
    
	/* Loop through all headers in the request */
	while (s < e) {
        
		/* Find where this header ends */
		for (p = s; p < e && *p != '\n'; ) p++;
        
		/* Is this header known to us ? */
		for (h = http_headers; h->len != 0; h++)
			if (e - s > h->len &&
			    !_strncasecmp(s, h->name, h->len))
				break;
        
		/* If the header is known to us, store its value */
		if (h->len != 0) {
            
			/* Shift to where value starts */
			s += h->len;
            
			/* Find place to store the value */
			v = (struct parsed_header *) ((char *) parsed + h->offset);
            
			/* Store name and type of the header */
			v->_name = h->name;
			v->_type = h->type;
			
			/* Fetch header value into the connection structure */
			if (h->type == HDR_STRING) {
				v->_v.v_vec.ptr = s;
				v->_v.v_vec.len = p - s;
				if (p[-1] == '\r' && v->_v.v_vec.len > 0)
					v->_v.v_vec.len--;
			} else if (h->type == HDR_INT) {
				v->_v.v_big_int = strtoul(s, NULL, 10);
			} else if (h->type == HDR_DATE) {
				v->_v.v_time = date_to_epoch(s);
			}
		}
        
		s = p + 1;	/* Shift to the next header */
	}
}

/*
 * Returns:
 * 0 if headers are not fully recieved
 * 1 if headers recieved and parsed
 * -1 if some sort error happened
 */
static int 
_HTTPParseMessage(HttpContextRef context) {
    char* buffer = (char*)CFDataGetBytePtr(context->_rcvdBytes);
    int buflen = CFDataGetLength(context->_rcvdBytes);
    
    int req_len = _HttpGetHeadersLen(buffer, buflen);
    RAWTRACE4("Parsing new request (context 0x%X (%d - 0x%x)).Headers len = %d", 
		 context, sizeof(context[0]), context+sizeof(context[0]), req_len );
    
    if (req_len == 0) {
        if (buflen > 2048) {
            HttpSendErrorToTheServer(context, 400, "Request is too big");
            return -1;
        }
        return 0;
    } else if (req_len < 16) { /* Minimal: "GET / HTTP/1.0\n\n" */
        HttpSendErrorToTheServer(context, 400, "Bad request");
        return -1;
    } else if (_HTTPGetRequestMethod(context, buffer)) { 
        HttpSendErrorToTheServer(context, 501, "Method Not Implemented");
        return -1;
    } 
    
	/*
	 * Now make a copy of the URI, because it will be URL-decoded,
	 * and we need a copy of unmodified URI for the access log.
	 * First, we skip the REQUEST_METHOD and shift to the URI.
	 */
	char *e, *p, *start;

	for (p = buffer, e = p + req_len; *p != ' ' && p < e; p++);
	while (p < e && *p == ' ') p++;
    
	/* Now remember where URI starts, and shift to the end of URI */
	for (start = p; p < e && !isspace((unsigned char)*p); ) p++;
	
    int uri_len = p - start;
         
    // Make a copy of uri
    context->_request->_uri = CFAllocatorAllocate(context->_alloc, uri_len+1, 0);
    if ( context->_request->_uri == NULL ) {
		HttpSendErrorToTheServer(context, 500, "Internal server error - can't allocate uri");
        return -1;
    }
    memcpy(context->_request->_uri, start, uri_len);
    context->_request->_uri[uri_len] = '\0';
    RAWTRACE1("URI = [%s]", context->_request->_uri);
    
	/* Skip space following the URI */
	while (p < e && *p == ' ') p++;

    /* Now comes the HTTP-Version in the form HTTP/<major>.<minor> */
	if (sscanf(p, "HTTP/%lu.%lu",
               &context->_request->_major_version, &context->_request->_minor_version) != 2) {
		HttpSendErrorToTheServer(context, 400, "Bad HTTP version");
        return -1;
	} else if (context->_request->_major_version > 1 ||
               (context->_request->_major_version == 1 && context->_request->_minor_version > 1)) {
		HttpSendErrorToTheServer(context, 505, "HTTP version not supported");
        return -1;
	} else if (uri_len <= 0) {
		HttpSendErrorToTheServer(context, 400, "Bad URI");
        return -1;
	}     
   
	/* Skip to the end of the line */
	while (p < e && *p != '\n') p++;
    while (p < e && isspace((unsigned char)*p)) p++;
    int headers_len = req_len - (p - buffer);
   
    // Allocate space for a copy of headers 
    context->_request->_headers = CFAllocatorAllocate(context->_alloc, headers_len+2, 0);
    if ( context->_request->_headers == NULL ) {
		HttpSendErrorToTheServer(context, 500, "Internal server error - can't allocate headers");
        return -1;
    } 
    // Make a copy of headers
    memcpy(context->_request->_headers, p, headers_len+1);
    context->_request->_headers[headers_len]='\0';
    
    // Parse headers
    _HTTPParseHeaders(context->_request->_headers, headers_len, &context->_request->_cheaders);
    RAWTRACE1("Content-Length: %d",context->_request->_cheaders.cl._v.v_big_int);
        
    // Done with headers
    context->_request->_flags |= _FLAG_HEADERS_PARSED;
    
    // Remove headers from the buffer.
    CFDataDeleteBytes(context->_rcvdBytes, CFRangeMake(0, req_len));

    return 1;
}

/* Returns:
 * 1 if message recieved
 * 0 if headers are not here yet
 * -1 in case of error
 */
int HTTPParseRequest(HttpContextRef context) {
    int ret = 0;
    
    if (context->_request->_flags & _FLAG_HEADERS_PARSED) {
        ret = 1; // headers parsed
    } else {
        ret = _HTTPParseMessage(context);
    }
    
    if ( (ret == 1) && (context->_request->_flags & _FLAG_HEADERS_PARSED) ) {
        int buflen = CFDataGetLength(context->_rcvdBytes);
        if ( context->_request->_cheaders.cl._v.v_big_int <= buflen ) {
            return 1; 
        } else {
            return 0;
        }
    }
    
    return ret;
}

int HTTPUrlDecode(const char *src, int src_len, char *dst, int dst_len)
{
	int	i, j, a, b;
#define	HEXTOI(x)  (isdigit(x) ? x - '0' : x - 'W')
	
	for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++)
		switch (src[i]) {
			case '%':
				if (isxdigit(((unsigned char *) src)[i + 1]) &&
					isxdigit(((unsigned char *) src)[i + 2])) {
					a = tolower(((unsigned char *)src)[i + 1]);
					b = tolower(((unsigned char *)src)[i + 2]);
					dst[j] = (HEXTOI(a) << 4) | HEXTOI(b);
					i += 2;
				} else {
					dst[j] = '%';
				}
				break;
			default:
				dst[j] = src[i];
				break;
		}
	
	dst[j] = '\0';	/* Null-terminate the destination */
	
	return (j);
}

/*
 * Sane snprintf(). Acts like snprintf(), but never return -1 or the
 * value bigger than supplied buffer.
 * Thanks Adam Zeldis to pointing snprintf()-caused vulnerability
 * in his audit report.
 */
int
HttpSnprintf(char *buf, size_t buflen, const char *fmt, ...)
{
	va_list		ap;
	int		n;
	
	if (buflen == 0)
		return (0);
	
	va_start(ap, fmt);
	n = vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
	
	if (n < 0 || (size_t) n >= buflen)
		n = buflen - 1;
	buf[n] = '\0';
	
	return (n);
}

static int
_HTTPServeDirectoryListing(HttpContextRef context, char* dir) {

	static const char footer[] = "</table></body></html>\n";
	
	DIR *dp;
	struct dirent *dirp;
	struct stat	st;
	char file[FILENAME_MAX], line[FILENAME_MAX + 512], size[64], mod[64];
	const char* slash = dir[strlen(dir)-1] == '/' ? "" : "/";
	
	if((dp  = opendir(dir)) == NULL) {
		HttpSendErrorToTheServer(context, 500, "Cannot open directory");
		return -1;
	}
	
	CFMutableDataRef message_body = CFDataCreateMutable(context->_alloc, 0);
	
	int n = HttpSnprintf(line, sizeof(line),
						  "<html><head><title>Index of %s</title>"
						  "<style>th {text-align: left;} * {font-size: 30px; font-weight: bold;}</style></head>"
						  "<body><h1>Index of %s</h1><pre><table cellpadding=\"0\">"
						  "<tr><th>Name</th><th>Modified</th><th>Size</th></tr>"
						  "<tr><td colspan=\"3\"><hr></td></tr>",
						  context->_request->_uri, context->_request->_uri);
	CFDataAppendBytes(message_body, (UInt8*)line, (CFIndex)n);
	
	while ((dirp = readdir(dp)) != NULL) {
		/* Do not show current dir */
		if ( strcmp(dirp->d_name, ".") == 0 )
			continue;

		HttpSnprintf(file, sizeof(file), "%s%s", dir, dirp->d_name);
		stat(file, &st);
		
		if (S_ISDIR(st.st_mode)) {
			HttpSnprintf(size,sizeof(size),"%s","&lt;DIR&gt;");
		} else {
			if (st.st_size < 1024)
				HttpSnprintf(size, sizeof(size),
							  "%lu", (unsigned long) st.st_size);
			else if (st.st_size < 1024 * 1024)
				HttpSnprintf(size, sizeof(size), "%luk",
							  (unsigned long) (st.st_size >> 10)  + 1);
			else
				HttpSnprintf(size, sizeof(size),
							  "%.1fM", (float) st.st_size / 1048576);
		}
		strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M", localtime(&st.st_mtime));
		
		n = HttpSnprintf(line, sizeof(line),
						  "<tr><td><a href=\"%s%s%s\">%s%s</a></td>"
						  "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
						  context->_request->_uri, slash, dirp->d_name, dirp->d_name,
						  S_ISDIR(st.st_mode) ? "/" : "", mod, size);
		
		CFDataAppendBytes(message_body, (UInt8*)line, (CFIndex)n);
	}
	
	CFDataAppendBytes(message_body, (UInt8*)footer, (CFIndex)strlen(footer));
	
	n = HttpSnprintf(line, sizeof(line),
					  "HTTP/1.1 200 OK\r\n"
					  "Content-Length: %u\r\n"
					  "Connection: close\r\n"
					  "Content-Type: text/html; charset=utf-8\r\n\r\n",
					   CFDataGetLength(message_body));

	CFDataAppendBytes(context->_sendBytes, (UInt8*)line, (CFIndex)n);
	CFDataAppendBytes(context->_sendBytes, CFDataGetBytePtr(message_body), 
					  CFDataGetLength(message_body));
	
	RAWTRACE("Message to send: ");
	RAWTRACE_DATA((UInt8*)CFDataGetBytePtr(context->_sendBytes), 
						   CFDataGetLength(context->_sendBytes));	
	RAWTRACE("Message -- eof --");

	CFRelease(message_body);

	closedir(dp);
	return 0;
}

static const struct {
	const char	*extension;
	int		ext_len;
	const char	*mime_type;
} builtin_mime_types[] = {
{"html",	4,	"text/html"			},
{"htm",		3,	"text/html"			},
{"txt",		3,	"text/plain"			},
{"css",		3,	"text/css"			},
{"ico",		3,	"image/x-icon"			},
{"gif",		3,	"image/gif"			},
{"jpg",		3,	"image/jpeg"			},
{"jpeg",	4,	"image/jpeg"			},
{"png",		3,	"image/png"			},
{"svg",		3,	"image/svg+xml"			},
{"torrent",	7,	"application/x-bittorrent"	},
{"wav",		3,	"audio/x-wav"			},
{"mp3",		3,	"audio/x-mp3"			},
{"mid",		3,	"audio/mid"			},
{"m3u",		3,	"audio/x-mpegurl"		},
{"ram",		3,	"audio/x-pn-realaudio"		},
{"ra",		2,	"audio/x-pn-realaudio"		},
{"doc",		3,	"application/msword",		},
{"exe",		3,	"application/octet-stream"	},
{"zip",		3,	"application/x-zip-compressed"	},
{"xls",		3,	"application/excel"		},
{"tgz",		3,	"application/x-tar-gz"		},
{"tar.gz",	6,	"application/x-tar-gz"		},
{"tar",		3,	"application/x-tar"		},
{"gz",		2,	"application/x-gunzip"		},
{"arj",		3,	"application/x-arj-compressed"	},
{"rar",		3,	"application/x-arj-compressed"	},
{"rtf",		3,	"application/rtf"		},
{"pdf",		3,	"application/pdf"		},
{"swf",		3,	"application/x-shockwave-flash"	},
{"mpg",		3,	"video/mpeg"			},
{"mpeg",	4,	"video/mpeg"			},
{"asf",		3,	"video/x-ms-asf"		},
{"avi",		3,	"video/x-msvideo"		},
{"bmp",		3,	"image/bmp"			},
{NULL,		0,	NULL				}
};

void
_HttpGetMimeType(HttpContextRef context, const char *uri, int len, struct vec *vec)
{
	int		i, ext_len;
	
	/* Loop through built-in mime types */
	for (i = 0; builtin_mime_types[i].extension != NULL; i++) {
		ext_len = builtin_mime_types[i].ext_len;
		if (len > ext_len && uri[len - ext_len - 1] == '.' &&
		    !_strncasecmp(builtin_mime_types[i].extension,
								 &uri[len - ext_len], ext_len)) {
			vec->ptr = builtin_mime_types[i].mime_type;
			vec->len = strlen(vec->ptr);
			return;
		}
	}
	
	/* Oops. This extension is unknown to us. Fallback to text/plain */
	vec->ptr = "text/plain";
	vec->len = strlen(vec->ptr);
}

int 
HTTPSendReply(HttpContextRef context, char* body) {
	char headers[512], date[64], lm[64], etag[64];
	size_t status = 200;
	const char *fmt = "%a, %d %b %Y %H:%M:%S GMT", *msg = "OK";
	int cl = strlen(body);
	
	/* Prepare Etag, Date, Last-Modified headers */
	time_t	_current_time = time(0);
	strftime(date, sizeof(date), fmt, localtime(&_current_time));
	strftime(lm, sizeof(lm), fmt, localtime(&_current_time));
	HttpSnprintf(etag, sizeof(etag), "%lx.%lx",
				 (unsigned long) _current_time, (unsigned long) cl);
	
	/* Format reply headers */
	int headers_len = HttpSnprintf(headers,
								   sizeof(headers),
								   "HTTP/1.1 %d %s\r\n"
								   "Date: %s\r\n"
								   "Last-Modified: %s\r\n"
								   "Etag: \"%s\"\r\n"
								   "Connection: close\r\n"
								   "Content-Type: text/html\r\n"
								   "Content-Length: %lu\r\n"
								   "\r\n",
								   status, msg, date, lm, etag, cl);
	
	/* Add reply headers to the send buffer */
	CFDataAppendBytes(context->_sendBytes, (UInt8*)headers, (CFIndex)headers_len);
	
	RAWTRACE("Reply send:");
	RAWTRACE_DATA( (UInt8*)headers, headers_len );
	RAWTRACE("Reply -- eof --");
	
	/* Add file to the send buffer */
	CFDataAppendBytes(context->_sendBytes, (UInt8*)body, (CFIndex)cl);			
	
	return 1;
	
}

void 
HttpSendErrorToTheServer(HttpContextRef context, int status, const char *reason) {
    
    // Allocate tem buffer on the stack
    char buffer[1024];
    
    // Format http message
    sprintf(buffer,"HTTP/1.1 %d %s\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %d\r\n"
			"Connection: close\r\n"
            "\r\n"
            "Error: %03d - %s\r\n",
            status, reason, 15 + strlen(reason), status, reason);
    
	RAWLOG_ERROR2("Error %d - %s", status, reason);
	
    // Add the bytes of data to the send buffer.
    CFDataAppendBytes(context->_sendBytes, (UInt8*)buffer, (CFIndex)strlen(buffer));
}

//TBD - loading whole file in the memory is not efficient, it is better to 
//just open file here and provide filestream to HttpContext so 
//it can read file in small chunks and add these chunks directly to the output 
//stream
static int
_HTTPServeFile(HttpContextRef context, struct stat *st, char* file) {

	char			headers[512], date[64], lm[64], etag[64], range[64] = "";
	size_t			n, status = 200, ret = 1;
	unsigned long	r1, r2;
	const char		*fmt = "%a, %d %b %Y %H:%M:%S GMT", *msg = "OK";
	big_int_t		cl = (big_int_t) st->st_size;
	int				fd;

	if ((fd = open(file, O_RDONLY | O_BINARY, 0644)) != -1) {

		/* If Range: header specified, act accordingly */
		if (context->_request->_cheaders.range._v.v_vec.len > 0 &&
			(n = sscanf(context->_request->_cheaders.range._v.v_vec.ptr,
						"bytes=%lu-%lu",&r1, &r2)) > 0) {
			status = 206;
			lseek(fd, r1, SEEK_SET);
			cl = n == 2 ? r2 - r1 + 1: cl - r1;
			HttpSnprintf(range, sizeof(range),
						  "Content-Range: bytes %lu-%lu/%lu\r\n",
						  r1, r1 + cl - 1, (unsigned long) st->st_size);
			msg = "Partial Content";
		}
				
		char* buf = CFAllocatorAllocate(context->_alloc, cl, 0);
		if ( read(fd, buf, cl) > 0) {
			
			/* Get mime type */
			_HttpGetMimeType(context, file, strlen(file), &context->_request->_mime_type);
					
			/* Prepare Etag, Date, Last-Modified headers */
			time_t	_current_time = time(0);
			strftime(date, sizeof(date), fmt, localtime(&_current_time));
			strftime(lm, sizeof(lm), fmt, localtime(&st->st_mtime));
			HttpSnprintf(etag, sizeof(etag), "%lx.%lx",
						  (unsigned long) st->st_mtime, (unsigned long) st->st_size);
			
			/* Format reply headers */
			int headers_len = HttpSnprintf(headers,
											sizeof(headers),
											"HTTP/1.1 %d %s\r\n"
											"Date: %s\r\n"
											"Last-Modified: %s\r\n"
											"Etag: \"%s\"\r\n"
											"Connection: close\r\n"
											"Content-Type: %.*s\r\n"
											"Content-Length: %lu\r\n"
										    "Cache-Control: max-age=2592000\r\n"
											"Accept-Ranges: bytes\r\n"
											"%s\r\n",
											status, msg, date, lm, etag,
											context->_request->_mime_type.len, 
											context->_request->_mime_type.ptr, 
											cl, range);
			
			/* Add reply headers to the send buffer */
			CFDataAppendBytes(context->_sendBytes, (UInt8*)headers, (CFIndex)headers_len);
			
			RAWTRACE("File to send:");
			RAWTRACE_DATA( (UInt8*)headers, headers_len );
			RAWTRACE("File -- eof --");
			
			/* Add file to the send buffer */
			CFDataAppendBytes(context->_sendBytes, (UInt8*)buf, (CFIndex)cl);			
		
		} else {
			HttpSendErrorToTheServer(context, 500, "Internal Server Error - Can't read requested file");
			ret = -1;
		}
		CFAllocatorDeallocate(context->_alloc,buf);
		close(fd);
		return ret;
	} else {
		HttpSendErrorToTheServer(context, 500, "Internal Server Error - Can't open requested file");
		return -1;
	}
}

int 
HTTPRedirect(HttpContextRef context, char* location) {

	char buf[256+strlen(location)];
	int msg_len = HttpSnprintf(buf,sizeof(buf),
								"HTTP/1.1 301 Moved Permanently\r\n"
								"Location: %s\r\n"
								"Content-Type: text/plain\r\n"
								"Content-Length: 0\r\n"
								"Connection: close\r\n"
								"\r\n", 
								location);	
	
	
	RAWTRACE("Message to send:");
	RAWTRACE_DATA((UInt8*)buf, msg_len);
	RAWTRACE("Message -- eof --");
	
	CFDataAppendBytes(context->_sendBytes, (UInt8*)buf, (CFIndex)msg_len);
	
	return 1;
}

static const char *indexfile[] = { "controller.iseq", "index_erb.iseq", "index.html", "index.htm" };

static int 
_HTTPGetIndexFile(HttpContextRef context, char* path) {
	char file[FILENAME_MAX];
	struct stat	st;
	const char* slash = path[strlen(path)-1] == '/' ? "" : "/";
	
	for (int i = 0; i < sizeof(indexfile) / sizeof(indexfile[0]); i++) {
		HttpSnprintf(file, sizeof(file), "%s%s%s", path, slash, indexfile[i]);
		if ( (stat(file, &st) == 0) && (!S_ISDIR(st.st_mode)) ) {
			
			if ( i == 0 ) {// there is a controller in this folder
				return -2;
			}
			
			slash = context->_request->_uri[strlen(context->_request->_uri)-1] == '/' ? "" : "/";
			char location[URI_MAX];
			if (context->_request->_query) {
				HttpSnprintf(location, sizeof(location), "%s%s%s?%s", 
							 context->_request->_uri, slash, indexfile[i],context->_request->_query);	
			} else {
				HttpSnprintf(location, sizeof(location), "%s%s%s", 
						  context->_request->_uri, slash, indexfile[i]);	
			}
			return HTTPRedirect(context, location);
		}
	}
	return 0;
}

static bool
_isindex(char* path) {
	int pathlen = strlen(path);
	for (int i = 1; i < sizeof(indexfile) / sizeof(indexfile[0]); i++) {
		int len = strlen(indexfile[i]);
		if ( len > pathlen )
			continue;
		if( strncmp(path+pathlen-len,indexfile[i],len) == 0 )
			return true;
	}
	return false;
}

static int
_HTTPGetFile(HttpContextRef context, char* path)
{
	struct stat	st;
	int ret;
	
	if ( stat(path, &st) == -1 ) {
        //HttpSendErrorToTheServer(context, 404, "Not Found");
        return 0;				
	} else if ( S_ISDIR(st.st_mode) ) {
		if ( (ret = _HTTPGetIndexFile(context, path)) == 0) {
			return _HTTPServeDirectoryListing(context, path);
		} 
		return ret == -2 ? 0 : ret;
	} else {
		if (_isindex(path)) {
			return ServeIndex(context, path);
		}
		return _HTTPServeFile(context, &st, path);
	}
}

static char* 
_HTTPResolveIndex(char* url,char* path) {
	char file[FILENAME_MAX];
	struct stat	st;
	const char* slash = path[strlen(path)-1] == '/' ? "" : "/";
	
	for (int i = 0; i < sizeof(indexfile) / sizeof(indexfile[0]); i++) {
		HttpSnprintf(file, sizeof(file), "%s%s%s", path, slash, indexfile[i]);
		if ( (stat(file, &st) == 0) && (!S_ISDIR(st.st_mode)) ) {
			
			if ( i == 0 ) {// there is a controller in this folder
				return url;
			}
			
			int url_len = strlen(url);
			slash = url[strlen(url)-1] == '/' ? "" : "/";

			int full_len = url_len + strlen(slash)+strlen(indexfile[i])+1;
			char* resolved_url = malloc(full_len);
			HttpSnprintf(resolved_url, full_len, "%s%s%s", url, slash, indexfile[i]);	
			free(url);
			
			return resolved_url;
		}
	}
	return url;
}
/*
static char* localhost = "http://localhost:8080";

char* HTTPResolveUrl(char* url) {
	char path[URI_MAX];
	struct stat	st;
	char* tmp_url;
	
	char* full_path = strstr(url,"http://");
	if (full_path) {
		return full_path;
	}

	const char *root = HttpGetSiteRoot();
	if (strlen(url) + strlen(root) >= sizeof(path)) {
		tmp_url = url;
	} else {
		HttpSnprintf(path, sizeof(path), "%s%s", root, url);
		if ( stat(path, &st) == -1 ) {
			tmp_url = url;				
		} else if ( S_ISDIR(st.st_mode) ) {
			tmp_url = _HTTPResolveIndex(url,path);
		} else {
			tmp_url = url;				
		}
	}
	
	int full_len = strlen(localhost)+strlen(tmp_url)+1;
	char* ret = malloc(full_len);
	HttpSnprintf(ret, full_len, "%s%s", localhost, tmp_url);	
	free(tmp_url);
	
	return ret;
}*/

int 
HTTPProcessMessage(HttpContextRef context) {
	int			res;
	
	if ((context->_request->_query = strchr(context->_request->_uri, '?')) != NULL)
		*context->_request->_query++ = '\0';
	
	HTTPUrlDecode(context->_request->_uri, strlen(context->_request->_uri), 
				  context->_request->_uri, strlen(context->_request->_uri) + 1);

	{
		char path[URI_MAX];
		
		const char *root = HttpGetSiteRoot();
		if (strlen(context->_request->_uri) + strlen(root) >= sizeof(path)) {
			HttpSendErrorToTheServer(context, 400, "URI is too long");
			return -1;
		}
	
		HttpSnprintf(path, sizeof(path), "%s%s", root, context->_request->_uri);
		RAWTRACE1("Path = %s", path);
		 
		if (context->_request->_method == METHOD_GET) {
			if ((res = _HTTPGetFile(context, path))!=0) {
				return res;
			}
		} 
	}
	
	if ((res = Dispatch(context))!=0) {
		return res;
	}
	
	HttpSendErrorToTheServer(context, 404, "Not Found");
	//HttpSendErrorToTheServer(context, 500, "Under construction");
    return -1;        
}
