#include "net/HttpServer.h"
#include "common/RhodesApp.h"
#include "common/RhoFilePath.h"
#include "ruby/ext/rho/rhoruby.h"

#include <algorithm>

#if !defined(OS_WINCE)
#include <common/stat.h>
#else
#include "CompatWince.h"

#ifdef EAGAIN
#undef EAGAIN
#endif
#define EAGAIN EWOULDBLOCK

#endif

#if defined(OS_WINDOWS) || defined(OS_WINCE)
typedef unsigned __int16 uint16_t;

#  ifndef S_ISDIR
#    define S_ISDIR(m) ((_S_IFDIR & m) == _S_IFDIR)
#  endif

#  ifndef S_ISREG
#    define S_ISREG(m) ((_S_IFREG & m) == _S_IFREG)
#  endif

#  ifndef EAGAIN
#    define EAGAIN WSAEWOULDBLOCK
#  endif

#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "HttpServer"

extern "C" void rho_sync_addobjectnotify_bysrcname(const char* szSrcName, const char* szObject);

namespace rho
{
namespace net
{
using namespace rho::common;

IMPLEMENT_LOGCLASS(CHttpServer, "HttpServer");
    
static bool isid(String const &s)
{
    return s.size() > 2 && s[0] == '{' && s[s.size() - 1] == '}';
}

static bool isdir(String const &path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

static bool isfile(String const &path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

static String decode(String const &url)
{
    String ret;
    for (const char *s = url.c_str(); *s != '\0'; ++s) {
        if (*s != '%') {
            ret.push_back(*s);
            continue;
        }
        
        unsigned int c1 = (unsigned char)*++s;
        if (c1 >= (unsigned char)'0' && c1 <= (unsigned char)'9')
            c1 = c1 - (unsigned char)'0';
        else if (c1 >= (unsigned char)'a' && c1 <= (unsigned char)'f')
            c1 = c1 - (unsigned char)'a' + 10;
        else if (c1 >= (unsigned char)'A' && c1 <= (unsigned char)'F')
            c1 = c1 - (unsigned char)'A' + 10;
        else
            break;
        unsigned int c2 = (unsigned char)*++s;
        if (c2 >= (unsigned char)'0' && c2 <= (unsigned char)'9')
            c2 = c2 - (unsigned char)'0';
        else if (c2 >= (unsigned char)'a' && c2 <= (unsigned char)'f')
            c2 = c2 - (unsigned char)'a' + 10;
        else if (c2 >= (unsigned char)'A' && c2 <= (unsigned char)'F')
            c2 = c2 - (unsigned char)'A' + 10;
        else
            break;
        
        char c = (char)((c1 << 4) | c2);
        ret.push_back(c);
    }
    return ret;
}

static bool isindex(String const &uri)
{
    static struct {
        const char *s;
        size_t len;
    } index_files[] = {
        {"index_erb.iseq", 14},
        {"index.html", 10},
        {"index.htm", 9},
        {"index.php", 9},
        {"index.cgi", 9}
    };
    
    // Convert uri to lower case
    String luri;
    std::transform(uri.begin(), uri.end(), std::back_inserter(luri), &::tolower);
    
    for (size_t i = 0, lim = sizeof(index_files)/sizeof(index_files[0]); i != lim; ++i) {
        size_t pos = luri.find(index_files[i].s);
        if (pos == String::npos)
            continue;
        
        if (pos + index_files[i].len != luri.size())
            continue;
        
        return true;
    }
    
    return false;
}

static bool isknowntype(String const &uri)
{
    static struct {
        const char *s;
        size_t len;
    } ignored_exts[] = {
        {".css", 4},
        {".js", 3},
        {".html", 5},
        {".htm", 4},
        {".png", 4},
        {".bmp", 4},
        {".jpg", 4},
        {".jpeg", 5}
    };
    
    // Convert uri to lower case
    String luri;
    std::transform(uri.begin(), uri.end(), std::back_inserter(luri), &::tolower);
    
    for (size_t i = 0, lim = sizeof(ignored_exts)/sizeof(ignored_exts[0]); i != lim; ++i) {
        size_t pos = luri.find(ignored_exts[i].s);
        if (pos == String::npos)
            continue;
        
        if (pos + ignored_exts[i].len != luri.size())
            continue;
        
        return true;
    }
    
    return false;
}
    

static String get_mime_type(String const &path)
{
    static const struct {
        const char	*extension;
        int		ext_len;
        const char	*mime_type;
    } builtin_mime_types[] = {
        {".html",       5,	"text/html"                     },
        {".htm",		4,	"text/html"                     },
        {".txt",		4,	"text/plain"                    },
        {".css",		4,	"text/css"                      },
        {".js",         3,  "text/javascript"               },
        {".ico",		4,	"image/x-icon"                  },
        {".gif",		4,	"image/gif"                     },
        {".jpg",		4,	"image/jpeg"                    },
        {".jpeg",       5,	"image/jpeg"                    },
        {".png",		4,	"image/png"                     },
        {".svg",		4,	"image/svg+xml"                 },
        {".torrent",	8,	"application/x-bittorrent"      },
        {".wav",		4,	"audio/x-wav"                   },
        {".mp3",		4,	"audio/x-mp3"                   },
        {".mid",		4,	"audio/mid"                     },
        {".m3u",		4,	"audio/x-mpegurl"               },
        {".ram",		4,	"audio/x-pn-realaudio"          },
        {".ra",         3,	"audio/x-pn-realaudio"          },
        {".doc",		4,	"application/msword",           },
        {".exe",		4,	"application/octet-stream"      },
        {".zip",		4,	"application/x-zip-compressed"	},
        {".xls",		4,	"application/excel"             },
        {".tgz",		4,	"application/x-tar-gz"          },
        {".tar.gz",     7,	"application/x-tar-gz"          },
        {".tar",		4,	"application/x-tar"             },
        {".gz",         3,	"application/x-gunzip"          },
        {".arj",		4,	"application/x-arj-compressed"	},
        {".rar",		4,	"application/x-arj-compressed"	},
        {".rtf",		4,	"application/rtf"               },
        {".pdf",		4,	"application/pdf"               },
        {".swf",		4,	"application/x-shockwave-flash"	},
        {".mpg",		4,	"video/mpeg"                    },
        {".mpeg",       5,	"video/mpeg"                    },
        {".asf",		4,	"video/x-ms-asf"                },
        {".avi",		4,	"video/x-msvideo"               },
        {".bmp",		4,	"image/bmp"                     },
    };
    
    // Convert path to lower case
    String lpath;
    std::transform(path.begin(), path.end(), std::back_inserter(lpath), &::tolower);
    
    String mime_type;
    for (int i = 0, lim = sizeof(builtin_mime_types)/sizeof(builtin_mime_types[0]); i != lim; ++i) {
        size_t pos = lpath.find(builtin_mime_types[i].extension);
        if (pos == String::npos)
            continue;
        
        if (pos + builtin_mime_types[i].ext_len != lpath.size())
            continue;
        
        mime_type = builtin_mime_types[i].mime_type;
        break;
    }
    
    if (mime_type.empty())
        mime_type = "text/plain";
    
    return mime_type;
}
    

static VALUE create_request_hash(String const &application, String const &model,
                                 String const &action, String const &id,
                                 String const &method, String const &uri, String const &query,
                                 HttpHeaderList const &headers, String const &body)
{
    CHoldRubyValue hash(rho_ruby_createHash());

    addStrToHash(hash, "application", application.c_str());
	addStrToHash(hash, "model", model.c_str());
    if (!action.empty())
        addStrToHash(hash, "action", action.c_str());
    if (!id.empty())
        addStrToHash(hash, "id", id.c_str());
	
	addStrToHash(hash, "request-method", method.c_str());
	addStrToHash(hash, "request-uri", uri.c_str());
    addStrToHash(hash, "request-query", query.c_str());
	
    CHoldRubyValue hash_headers(rho_ruby_createHash());
    for (HttpHeaderList::const_iterator it = headers.begin(), lim = headers.end(); it != lim; ++it)
        addStrToHash(hash_headers, it->name.c_str(), it->value.c_str());
	addHashToHash(hash,"headers",hash_headers);
	
    if (!body.empty())
		addStrToHash(hash, "request-body", body.c_str());
    
    return hash;
}

CHttpServer::CHttpServer(int port, String const &root)
    :m_exit(false), m_port(port), verbose(true)
{
    RAWTRACE("Open listening socket...");

    m_root = CFilePath::normalizePath(root);
    m_bPause = false;
    m_listener = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listener == SOCKET_ERROR) {
        RAWLOG_ERROR1("Can not create listener: %d", RHO_NET_ERROR_CODE);
        return;
    }
    
    int enable = 1;
    if (setsockopt(m_listener, SOL_SOCKET, SO_REUSEADDR, (const char *)&enable, sizeof(enable)) == SOCKET_ERROR) {
        RAWLOG_ERROR1("Can not set socket option (SO_REUSEADDR): %d", RHO_NET_ERROR_CODE);
        return;
    }
    
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons((uint16_t)m_port);
    sa.sin_addr.s_addr = INADDR_ANY;
    if (bind(m_listener, (const sockaddr *)&sa, sizeof(sa)) == SOCKET_ERROR) {
        RAWLOG_ERROR2("Can not bind to port %d: %d", m_port, RHO_NET_ERROR_CODE);
        return;
    }
    
    if (listen(m_listener, 128) == SOCKET_ERROR) {
        RAWLOG_ERROR1("Can not listen on socket: %d", RHO_NET_ERROR_CODE);
        return;
    }
    
    RAWTRACE1("Listen for connections on port %d", m_port);
}

CHttpServer::~CHttpServer()
{
}

void CHttpServer::stop()
{
	m_exit = true;
	RAWTRACE("Close listening socket");
	closesocket(m_listener);
}

void CHttpServer::register_uri(String const &uri, CHttpServer::callback_t const &callback)
{
    if (uri.empty())
        return;
    String ruri = uri;
    if (ruri[ruri.size() - 1] != '/')
        ruri.push_back('/');
    m_registered[ruri] = callback;
}

CHttpServer::callback_t CHttpServer::registered(String const &uri)
{
    if (uri.empty())
        return (callback_t)0;
    String ruri = uri;
    if (ruri[ruri.size() - 1] != '/')
        ruri.push_back('/');
    std::map<String, callback_t>::const_iterator it = m_registered.find(ruri);
    if (it == m_registered.end())
        return (callback_t)0;
    return it->second;
}

extern "C" void rb_gc(void);

bool CHttpServer::run()
{
    if (m_listener == INVALID_SOCKET)
        return false;
    
    for(;;) {
        RAWTRACE("Waiting for connections...");
        rho_ruby_start_threadidle();
        SOCKET conn = accept(m_listener, NULL, NULL);
        rho_ruby_stop_threadidle();
		if (m_exit) {
			RAWTRACE("Stop HTTP server");
			return true;
		}
        if (conn == INVALID_SOCKET) {
#if !defined(OS_WINDOWS) && !defined(OS_WINCE)
            if (RHO_NET_ERROR_CODE == EINTR)
                continue;
#endif
            
            RAWLOG_ERROR1("Can not accept connection: %d", RHO_NET_ERROR_CODE);
            return false;
        }

        bool bProcessed = false;
        if (!m_bPause){
            RAWTRACE("Connection accepted, process it...");
            bProcessed = process(conn);
        }

        RAWTRACE("Close connected socket");
        closesocket(conn);

        if ( bProcessed )
            rb_gc();
    }
    
    return true;
}

typedef Vector<char> ByteVector;

bool receive_request_test(ByteVector &request, int attempt)
{
	String data;
	switch (attempt) {
		case 0:
			data += "GET / HTTP/1.1\r\n";
			data += "Accept: */*\r\n";
			break;
		case 1:
			break;
		case 2:
			data += "User-Agent: Test\r\n";
			data += "Host";
			break;
		case 3:
			data += ": localhost\r\n";
			data += "Content-Length: 4\r\n";
			break;
		case 4:
			data += "\r\n";
			break;
		case 5:
			break;
		case 6:
			data += "12";
			break;
		case 7:
			data += "34";
			break;
		default:
			return false;
	}
	
	request.insert(request.end(), data.begin(), data.end());
	return true;
}

bool CHttpServer::receive_request(ByteVector &request)
{
	if (verbose) RAWTRACE("Receiving request...");

	ByteVector r;
    char buf[BUF_SIZE];
    for(;;) {
        if (verbose) RAWTRACE("Read portion of data from socket...");
        int n = recv(m_sock, &buf[0], sizeof(buf), 0);
        if (n == -1) {
            int e = RHO_NET_ERROR_CODE;
#if !defined(OS_WINDOWS) && !defined(OS_WINCE)
            if (e == EINTR)
                continue;
#else
			if (e == WSAEINTR)
				continue;
#endif
            if (e == EAGAIN) {
                if (!r.empty())
                    break;
                
                fd_set fds;
                FD_ZERO(&fds);
                FD_SET(m_sock, &fds);
                select(m_sock + 1, &fds, 0, 0, 0);
                continue;
            }
            
            RAWLOG_ERROR1("Error when receiving data from socket: %d", e);
            return false;
        }
        
        if (n == 0) {
            RAWLOG_ERROR("Connection gracefully closed before we send any data");
            return false;
        }
        
        if (verbose) RAWTRACE1("Actually read %d bytes", n);
        r.insert(r.end(), &buf[0], &buf[0] + n);
    }
    
    if (!r.empty()) {
        request.insert(request.end(), r.begin(), r.end());
        RAWTRACE1("Received request:\n%s", &request[0]);
    }
    return true;
}

bool CHttpServer::send_response_impl(String const &data, bool continuation)
{
    if (verbose) {
        if (continuation)
            RAWTRACE("Send continuation data...");
        else
            RAWTRACE("Sending response...");
    }
    
    // First of all, make socket blocking
#if defined(OS_WINDOWS) || defined(OS_WINCE)
	unsigned long optval = 0;
	if(::ioctlsocket(m_sock, FIONBIO, &optval) == SOCKET_ERROR) {
		RAWLOG_ERROR1("Can not set blocking socket mode: %d", RHO_NET_ERROR_CODE);
		return false;
	}
#else
    int flags = fcntl(m_sock, F_GETFL);
    if (flags == -1) {
        RAWLOG_ERROR1("Can not get current socket mode: %d", errno);
        return false;
    }
    if (fcntl(m_sock, F_SETFL, flags & ~O_NONBLOCK) == -1) {
        RAWLOG_ERROR1("Can not set blocking socket mode: %d", errno);
        return false;
    }
#endif
    
    size_t pos = 0;
    for(; pos < data.size();) {
        int n = send(m_sock, data.c_str() + pos, data.size() - pos, 0);
        if (n == -1) {
            int e = RHO_NET_ERROR_CODE;
#if !defined(OS_WINDOWS) && !defined(OS_WINCE)
            if (e == EINTR)
                continue;
#endif
            
            RAWLOG_ERROR1("Can not send response data: %d", e);
            return false;
        }
        
        if (n == 0)
            break;
        
        pos += n;
    }
    
    //String dbg_response = response.size() > 100 ? response.substr(0, 100) : response;
    //RAWTRACE2("Sent response:\n%s%s", dbg_response.c_str(), response.size() > 100 ? "..." : "   ");
    if (continuation)
        RAWTRACE1("Sent response body: %d bytes", data.size());
    else
        RAWTRACE1("Sent response:\n%s", data.c_str());
    return true;
}

String CHttpServer::create_response(String const &reason)
{
    return create_response(reason, "");
}

String CHttpServer::create_response(String const &reason, HeaderList const &headers)
{
    return create_response(reason, headers, "");
}

String CHttpServer::create_response(String const &reason, String const &body)
{
    return create_response(reason, HeaderList(), body);
}

String CHttpServer::create_response(String const &reason, HeaderList const &hdrs, String const &body)
{
    String response = "HTTP/1.1 ";
    response += reason;
    response += "\r\n";
    
    char buf[50];
    snprintf(buf, sizeof(buf), "%d", m_port);
    
    HeaderList headers;
    headers.push_back(Header("Host", String("localhost:") + buf));
    headers.push_back(Header("Connection", "close"));
    std::copy(hdrs.begin(), hdrs.end(), std::back_inserter(headers));
    
    for(HeaderList::const_iterator it = headers.begin(), lim = headers.end();
        it != lim; ++it) {
        response += it->name;
        response += ": ";
        response += it->value;
        response += "\r\n";
    }
    
    response += "\r\n";
    
    response += body;
    
    return response;
}

bool CHttpServer::process(SOCKET sock)
{
    m_sock = sock;

	// First of all, make socket non-blocking
#if defined(OS_WINDOWS) || defined(OS_WINCE)
	unsigned long optval = 1;
	if(::ioctlsocket(m_sock, FIONBIO, &optval) == SOCKET_ERROR) {
		RAWLOG_ERROR1("Can not set non-blocking socket mode: %d", RHO_NET_ERROR_CODE);
		return false;
	}
#else
	int flags = fcntl(m_sock, F_GETFL);
	if (flags == -1) {
		RAWLOG_ERROR1("Can not get current socket mode: %d", errno);
		return false;
	}
	if (fcntl(m_sock, F_SETFL, flags | O_NONBLOCK) == -1) {
		RAWLOG_ERROR1("Can not set non-blocking socket mode: %d", errno);
		return false;
	}
#endif

    // Read request from socket
    ByteVector request;

    String method, uri, query;
    HeaderList headers;
    String body;
    if (!parse_request(method, uri, query, headers, body)) {
        RAWLOG_ERROR("Parsing error");
        send_response(create_response("500 Internal Error"));
        return false;
    }
    
    return decide(method, uri, query, headers, body);
}

bool CHttpServer::parse_request(String &method, String &uri, String &query, HeaderList &headers, String &body)
{
    method.clear();
    uri.clear();
    headers.clear();
    body.clear();

    size_t s = 0;
    ByteVector request;

    bool parsing_headers = true;
    size_t content_length = 0;

//	int attempt = 0;
//#define receive_request(request) receive_request_test(request, attempt++)
    for (;;) {
        if (!receive_request(request))
            return false;

        size_t lim = request.size();
        while (parsing_headers) {
            size_t e;
            for(e = s; e < lim && request[e] != '\r'; ++e);
            if (e >= lim - 1) {
                // Incomplete line, will read further
                break;
            }
            if (request[e + 1] != '\n') {
                RAWLOG_ERROR("Wrong request syntax, line should ends by '\\r\\n'");
                return false;
            }
            
            String line(&request[s], e - s);
            s = e + 2;
            
            if (line.empty()) {
                parsing_headers = false;
                break;
            }
            
            if (uri.empty()) {
                // Parse start line
                if (!parse_startline(line, method, uri, query) || uri.empty())
                    return false;
            }
            else {
                Header hdr;
                if (!parse_header(line, hdr) || hdr.name.empty())
                    return false;
                headers.push_back(hdr);
                
                String low;
                std::transform(hdr.name.begin(), hdr.name.end(), std::back_inserter(low), &::tolower);
                if (low == "content-length") {
                    content_length = ::atoi(hdr.value.c_str());
                }
            }
        }

        if (!parsing_headers) {
            if (content_length == 0)
                return true;

            if (lim - s < content_length)
                continue;

            body.assign(&request[s], &request[s] + content_length);
            return true;
        }
    }
}

bool CHttpServer::parse_startline(String const &line, String &method, String &uri, String &query)
{
    const char *s, *e;
    
    // Find first space
    for(s = line.c_str(), e = s; *e != ' ' && *e != '\0'; ++e);
    if (*e == '\0') {
        RAWLOG_ERROR1("Parse startline (1): syntax error: \"%s\"", line.c_str());
        return false;
    }
    
    method.assign(s, e);
    
    // Skip spaces
    for(s = e; *s == ' '; ++s);
    
    for(e = s; *e != '?' && *e != ' ' && *e != '\0'; ++e);
    if (*e == '\0') {
        RAWLOG_ERROR1("Parse startline (2): syntax error: \"%s\"", line.c_str());
        return false;
    }
    
    uri.assign(s, e);
    uri = decode(uri);
    
    query.clear();
    if (*e == '?') {
        s = ++e;
        for(e = s; *e != ' ' && *e != '\0'; ++e);
        if (*e != '\0')
            query.assign(s, e);
    }
    
    return true;
}

bool CHttpServer::parse_header(String const &line, Header &hdr)
{
    const char *s, *e;
    for(s = line.c_str(), e = s; *e != ' ' && *e != ':' && *e != '\0'; ++e);
    if (*e == '\0') {
        RAWLOG_ERROR1("Parse header (1): syntax error: %s", line.c_str());
        return false;
    }
    hdr.name.assign(s, e);
    
    // Skip spaces and colon
    for(s = e; *s == ' ' || *s == ':'; ++s);
    
    hdr.value = s;
    return true;
}

bool CHttpServer::parse_route(String const &line, Route &route)
{
    if (line.empty())
        return false;
    
    const char *s = line.c_str();
    if (*s == '/')
        ++s;
    
    const char *application_begin = s;
    for(; *s != '/' && *s != '\0'; ++s);
    if (*s == '\0')
        return false;
    const char *application_end = s;
    
    const char *model_begin = ++s;
    for(; *s != '/' && *s != '\0'; ++s);
    const char *model_end = s;
    
    route.application.assign(application_begin, application_end);
    route.model.assign(model_begin, model_end);
    
    if (*s == '\0')
        return true;
    
    const char *actionorid_begin = ++s;
    for (; *s != '/' && *s != '\0'; ++s);
    const char *actionorid_end = s;
    
    if (*s == '/')
        ++s;
    
    String aoi(actionorid_begin, actionorid_end);
    if (isid(aoi)) {
        route.id = aoi;
        route.action = s;
    }
    else {
        route.id = s;
        route.action = aoi;
    }
    
    return true;
}

bool CHttpServer::dispatch(String const &uri, Route &route)
{
    if (isknowntype(uri))
        return false;
    
    // Trying to parse route
    if (!parse_route(uri, route))
        return false;
    
    // Convert CamelCase to underscore_case
    // There has to be a better way?
    char tmp[3];
    const char *tmpstr = route.model.c_str();
    String controllerName = "";
    for(int i = 0; tmpstr[i] != '\0'; i++) {
        if(tmpstr[i] >= 'A' && tmpstr[i] <= 'Z') {
            if(i == 0) {
                tmp[0] = tmpstr[i] + 0x20;
                tmp[1] = NULL;
            } else {
                tmp[0] = '_';
                tmp[1] = tmpstr[i] + 0x20;
                tmp[2] = NULL;
            }
        } else {
            tmp[0] = tmpstr[i];
            tmp[1] = NULL;
        }
        controllerName += tmp;
    }
        
    //check if there is controller.rb to run
	struct stat st;

    String newfilename = m_root + "/" + route.application + "/" + route.model + "/" + controllerName + "_controller.iseq";
    String filename = m_root + "/" + route.application + "/" + route.model + "/controller.iseq";

    //look for controller.rb or model_name_controller.rb
    if ((stat(filename.c_str(), &st) != 0 || !S_ISREG(st.st_mode)) && (stat(newfilename.c_str(), &st) != 0 || !S_ISREG(st.st_mode)))
        return false;
    
    return true;
}

bool CHttpServer::send_file(String const &path)
{
    String fullPath = CFilePath::normalizePath(path);
    if (String_startsWith(fullPath,"/app/db/db-files") )
        fullPath = CFilePath::join( RHODESAPP().getRhoRootPath(), path.substr(4) );
    else if (fullPath.find(m_root) != 0)
        fullPath = CFilePath::join( m_root, path );
	
    if (verbose) RAWTRACE1("Sending file %s...", fullPath.c_str());
    
    struct stat st;
    if (stat(fullPath.c_str(), &st) != 0 || !S_ISREG(st.st_mode)) {
        RAWLOG_ERROR1("The file %s was not found", path.c_str());
        String error = "<html><font size=\"+4\"><h2>404 Not Found.</h2> The file " + path + " was not found.</font></html>";
        send_response(create_response("404 Not Found",error));
        return false;
    }
    
    FILE *fp = fopen(fullPath.c_str(), "rb");
    if (!fp) {
        RAWLOG_ERROR1("The file %s could not be opened", path.c_str());
        String error = "<html><font size=\"+4\"><h2>404 Not Found.</h2> The file " + path + " could not be opened.</font></html";
        send_response(create_response("404 Not Found",error));
        return false;
    }
    
    HeaderList headers;
    
    // Detect mime type
    headers.push_back(Header("Content-Type", get_mime_type(path)));
    //headers.push_back(Header("Expires", "Thu, 15 Apr 2020 20:00:00 GMT") );
    //headers.push_back(Header("Cache-Control", "max-age=2592000") );

    // Content length
    char buf[8192];
    
    size_t fileSize = st.st_size;
    snprintf(buf, sizeof(buf), "%lu", (unsigned long)fileSize);
    headers.push_back(Header("Content-Length", buf));
    
    // Send headers
    if (!send_response(create_response("200 OK", headers))) {
        RAWLOG_ERROR1("Can not send headers while sending file %s", path.c_str());
        fclose(fp);
        return false;
    }
    
    // Send body
    while (!feof(fp)) {
        size_t n = fread(buf, 1, sizeof(buf), fp);
        if (!send_response_body(String(buf, n))) {
            RAWLOG_ERROR1("Can not send part of data while sending file %s", path.c_str());
            fclose(fp);
            return false;
        }
    }
    
    fclose(fp);
    if (verbose) RAWTRACE1("File %s was sent successfully", path.c_str());
    return true;
}

bool CHttpServer::decide(String const &method, String const &uri, String const &query,
                         HeaderList const &headers, String const &body)
{
    RAWTRACE1("Decide what to do with uri %s", uri.c_str());
    callback_t callback = registered(uri);
    if (callback) {
        RAWTRACE1("Uri %s is registered callback, so handle it appropriately", uri.c_str());
        callback(this, query.length() ? query : body);
        return true;
    }
    
    String fullPath = CFilePath::join(m_root,uri);
    
    Route route;
    if (dispatch(uri, route)) {
        RAWTRACE1("Uri %s is correct route, so enable MVC logic", uri.c_str());
        
        VALUE req = create_request_hash(route.application, route.model, route.action, route.id,
                                        method, uri, query, headers, body);
        VALUE data = callFramework(req);
        String reply(getStringFromValue(data), getStringLenFromValue(data));
        rho_ruby_releaseValue(data);

        if (!send_response(reply))
            return false;

        if (method == "GET")
            rho_rhodesapp_keeplastvisitedurl(uri.c_str());

        if (!route.id.empty())
            rho_sync_addobjectnotify_bysrcname(route.model.c_str(), route.id.c_str());
        
        return true;
    }
    
    if (isdir(fullPath)) {
        RAWTRACE1("Uri %s is directory, redirecting to index", uri.c_str());
        String q = query.empty() ? "" : "?" + query;
        
        HeaderList headers;
        headers.push_back(Header("Location", CFilePath::join( uri, "index_erb.iseq") + q));
        
        send_response(create_response("301 Moved Permanently", headers));
        return false;
    }
    
    if (isindex(uri)) {
        if (!isfile(fullPath)) {
            RAWLOG_ERROR1("The file %s was not found", fullPath.c_str());
            String error = "<html><font size=\"+4\"><h2>404 Not Found.</h2> The file " + uri + " was not found.</font></html>";
            send_response(create_response("404 Not Found",error));
            return false;
        }
        
        RAWTRACE1("Uri %s is index file, call serveIndex", uri.c_str());
        
        VALUE data = callServeIndex((char *)fullPath.c_str());
        String reply(getStringFromValue(data), getStringLenFromValue(data));
        rho_ruby_releaseValue(data);

        if (!send_response(reply))
            return false;

        if (method == "GET")
            rho_rhodesapp_keeplastvisitedurl(uri.c_str());

        return true;
    }
    //RAWLOG_INFO("Sending File");
    
    // Try to send requested file
    RAWTRACE1("Uri %s should be regular file, trying to send it", uri.c_str());
    return send_file(uri);
}

} // namespace net
} // namespace rho
