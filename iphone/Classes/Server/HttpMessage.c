/*
 *  HttpMessage.cpp
 *  Browser
 *
 *  Created by adam blum on 9/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 *  Some bits of code here borrowed from Sergey Lyubka's shttpd
 */

#include "defs.h"
#include "HttpContext.h"
#include "HttpMessage.h"

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
enum {HDR_DATE, HDR_INT, HDR_STRING};	/* HTTP header types */
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
            DBG(("Request method %s = %d\n", v->ptr, context->_request->_method));
			break;
		}
    
	return (v->ptr == NULL);
}


void
_HTTPParseHeaders(const char *s, int len, struct headers *parsed)
{
	const struct http_header    *h;
	union variant               *v;
	const char                  *p, *e = s + len;
    
	DBG(("parsing headers (len %d): [%.*s]\n", len, len, s));
    
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
			v = (union variant *) ((char *) parsed + h->offset);
            
			/* Fetch header value into the connection structure */
			if (h->type == HDR_STRING) {
				v->v_vec.ptr = s;
				v->v_vec.len = p - s;
				if (p[-1] == '\r' && v->v_vec.len > 0)
					v->v_vec.len--;
			} else if (h->type == HDR_INT) {
				v->v_big_int = strtoul(s, NULL, 10);
			} else if (h->type == HDR_DATE) {
				v->v_time = date_to_epoch(s);
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
    DBG(("Headers len = %d\n", req_len));
    
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
    DBG(("URI = [%s]\n", context->_request->_uri));
    
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
    context->_request->_headers = CFAllocatorAllocate(context->_alloc, headers_len, 0);
    if ( context->_request->_headers == NULL ) {
		HttpSendErrorToTheServer(context, 500, "Internal server error - can't allocate headers");
        return -1;
    } 
    // Make a copy of headers
    memcpy(context->_request->_headers, p, headers_len+1);
    context->_request->_headers[headers_len]='\0';
    
    // Parse headers
    _HTTPParseHeaders(context->_request->_headers, headers_len, &context->_request->_cheaders);
    DBG(("Content-Length: %d\n",context->_request->_cheaders.cl.v_big_int));
        
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
        if ( context->_request->_cheaders.cl.v_big_int <= buflen ) {
            return 1; 
        } else {
            return 0;
        }
    }
    
    return ret;
}

int HTTPProcessMessage(HttpContextRef context) {
    HttpSendErrorToTheServer(context, 500, "Under construction");
    return -1;        
}
