#include "net/HttpServer.h"
#include "common/RhodesApp.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "HttpServer"
        
namespace rho
{
namespace net
{

IMPLEMENT_LOGCLASS(CHttpServer, "HttpServer");

CHttpServer::CHttpServer(int port, String const &root)
    :m_port(port), m_root(root)
{
    RAWTRACE("Open listening socket...");
    
    m_listener = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listener == SOCKET_ERROR) {
        RAWLOG_ERROR1("Can not create listener: %d", RHO_NET_ERROR_CODE);
        return;
    }
    
    int enable = 1;
    if (setsockopt(m_listener, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == SOCKET_ERROR) {
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
    RAWTRACE("Close listening socket");
    closesocket(m_listener);
}

void CHttpServer::register_uri(String const &uri, CHttpServer::callback_t const &callback)
{
    m_registered[uri] = callback;
}

CHttpServer::callback_t CHttpServer::registered(String const &uri)
{
    std::map<String, callback_t>::const_iterator it = m_registered.find(uri);
    if (it == m_registered.end())
        return (callback_t)0;
    return it->second;
}

bool CHttpServer::run()
{
    if (m_listener == INVALID_SOCKET)
        return false;
    
    for(;;) {
        RAWTRACE("Waiting for connections...");
        SOCKET conn = accept(m_listener, NULL, NULL);
        if (conn == INVALID_SOCKET) {
            if (RHO_NET_ERROR_CODE == EINTR)
                continue;
            
            RAWLOG_ERROR1("Can not accept connection: %d", RHO_NET_ERROR_CODE);
            return false;
        }
        
        RAWTRACE("Connection accepted, process it...");
        process(conn);
        
        RAWTRACE("Close connected socket");
        closesocket(conn);
    }
    
    return true;
}

bool CHttpServer::receive_request(SOCKET sock, ByteVector &request)
{
    request.clear();

    RAWTRACE("Receiving request...");
    
    // First of all, make socket non-blocking
    int flags = fcntl(sock, F_GETFL);
    if (flags == -1) {
        RAWLOG_ERROR1("Can not get current socket mode: %d", errno);
        return false;
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        RAWLOG_ERROR1("Can not set non-blocking socket mode: %d", errno);
        return false;
    }
    
    char buf[BUF_SIZE];
    for(;;) {
        RAWTRACE1("Read next portion of data from socket (%d bytes)...", sizeof(buf));
        int n = recv(sock, &buf[0], sizeof(buf), 0);
        if (n == -1) {
            int e = RHO_NET_ERROR_CODE;
            if (e == EINTR)
                continue;
            if (e == EAGAIN)
                break;
            
            RAWLOG_ERROR1("Error when receiving data from socket: %d", e);
            return false;
        }
        
        if (n == 0) {
            RAWLOG_ERROR("Connection gracefully closed before we send any data");
            return false;
        }
        
        RAWTRACE1("Actually read %d bytes", n);
        request.insert(request.end(), &buf[0], &buf[0] + n);
    }
    request.push_back('\0');
    
    RAWTRACE1("Received request:\n%s", &request[0]);
    return true;
}

bool CHttpServer::send_response(SOCKET sock, String const &response)
{
    RAWTRACE("Sending response...");
    // First of all, make socket blocking
    int flags = fcntl(sock, F_GETFL);
    if (flags == -1) {
        RAWLOG_ERROR1("Can not get current socket mode: %d", errno);
        return false;
    }
    if (fcntl(sock, F_SETFL, flags & ~O_NONBLOCK) == -1) {
        RAWLOG_ERROR1("Can not set blocking socket mode: %d", errno);
        return false;
    }
    
    size_t pos = 0;
    for(;;) {
        int n = send(sock, response.c_str() + pos, response.size() - pos, 0);
        if (n == -1) {
            int e = RHO_NET_ERROR_CODE;
            if (e == EINTR)
                continue;
            
            RAWLOG_ERROR1("Can not send response: %d", e);
            return false;
        }
        
        if (n == 0)
            break;
        
        pos += n;
    }
    
    RAWTRACE1("Sent response:\n%s", response.c_str());
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
    char buf[50];
    snprintf(buf, sizeof(buf), "%d", m_port);
    
    HeaderList headers;
    headers.push_back(Header("Host", String("localhost:") + buf));
    headers.push_back(Header("Connection", "close"));
    
    return create_response(reason, headers, body);
}

String CHttpServer::create_response(String const &reason, HeaderList const &headers, String const &body)
{
    String response = "HTTP/1.1 ";
    response += reason;
    response += "\r\n";
    
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
    // Read request from socket
    ByteVector request;
    if (!receive_request(sock, request))
        return false;
    
    RAWTRACE("Parsing request...");
    String method;
    String uri;
    HeaderList headers;
    String body;
    if (!parse_request(request, method, uri, headers, body)) {
        RAWLOG_ERROR("Parsing error");
        send_response(sock, create_response("500 Internal Error"));
        return false;
    }
    
    return decide(sock, method, uri, headers, body);
}

bool CHttpServer::parse_request(ByteVector &request, String &method, String &uri,
                                HeaderList &headers, String &body)
{
    method.clear();
    uri.clear();
    headers.clear();
    body.clear();
    
    char *s = &request[0];
    for(;;) {
        char *e;
        for(e = s; *e != '\r' && *e != '\0'; ++e);
        if (*e == '\0' || *(e + 1) != '\n')
            return false;
        *e = 0;
        
        String line = s;
        s = e + 2;
        
        if (!line.empty()) {
            if (uri.empty()) {
                // Parse start line
                if (!parse_startline(line, method, uri) || uri.empty())
                    return false;
            }
            else {
                Header hdr;
                if (!parse_header(line, hdr) || hdr.name.empty())
                    return false;
                headers.push_back(hdr);
            }
        }
        else {
            // Stop parsing
            body = s;
            return true;
        }
    }
}

bool CHttpServer::parse_startline(String const &line, String &method, String &uri)
{
    const char *s, *e;
    
    // Find first space
    for(s = line.c_str(), e = s; *e != ' ' && *e != '\0'; ++e);
    if (*e == '\0')
        return false;
    
    method.assign(s, e);
    
    // Skip spaces
    for(s = e; *s == ' '; ++s);
    
    for(e = s; *e != ' ' && *e != '\0'; ++e);
    if (*e == '\0')
        return false;
    
    uri.assign(s, e);
    
    return true;
}

bool CHttpServer::parse_header(String const &line, Header &hdr)
{
    const char *s, *e;
    for(s = line.c_str(), e = s; *e != ' ' && *e != ':' && *e != '\0'; ++e);
    if (*e == '\0')
        return false;
    hdr.name.assign(s, e);
    
    // Skip spaces and colon
    for(s = e; *s == ' ' || *s == ':'; ++s);
    
    hdr.value = s;
    return true;
}

bool CHttpServer::parse_route(String const &line, Route &route)
{
    return false;
}

struct static_string
{
    const char *s;
    size_t len;
};

bool CHttpServer::dispatch(String const &uri)
{
    static static_string ignored_exts[] = {
        {"css", 3},
        {"js", 2},
        {"html", 4},
        {"htm", 3},
        {"png", 3},
        {"bmp", 3},
        {"jpg", 3}
    };
    
    bool ignore = false;
    for (size_t i = 0, lim = sizeof(ignored_exts)/sizeof(ignored_exts[0]); i != lim; ++i) {
        size_t pos = uri.find(ignored_exts[i].s);
        if (pos == String::npos)
            continue;
        
        if (pos + ignored_exts[i].len != uri.size())
            continue;
        
        ignore = true;
        break;
    }
    
    if (ignore)
        return false;
    
    // Trying to parse route
    Route route;
    if (!parse_route(uri, route))
        return false;
    
    return true;
}

bool CHttpServer::isindex(String const &uri)
{
    static static_string index_files[] = {
        {"index_erb.iseq", 14},
        {"index.html", 10},
        {"index.htm", 9},
        {"index.php", 9},
        {"index.cgi", 9}
    };
    
    for (size_t i = 0, lim = sizeof(index_files)/sizeof(index_files[0]); i != lim; ++i) {
        size_t pos = uri.find(index_files[i].s);
        if (pos == String::npos)
            continue;
        
        if (pos + index_files[i].len != uri.size())
            continue;
        
        return true;
    }
    
    return false;
}

bool CHttpServer::send_file(SOCKET sock, String const &path)
{
    // TODO:
    static const char *resp = "<html><title>TEST</title><body><h1>This is test</h1></body></html>";
    send_response(sock, create_response("200 OK", resp));
    return true;
}

bool CHttpServer::decide(SOCKET sock, String const &method, String const &uri,
                         HeaderList const &headers, String const &body)
{
    callback_t callback = registered(uri);
    if (callback) {
        // TODO: Call this callback
        return true;
    }
    
    if (dispatch(uri)) {
        if (method == "GET")
            rho_rhodesapp_keeplastvisitedurl(uri.c_str());
        // TODO: Call rho_serve
        return true;
    }
    
    if (isindex(uri)) {
        if (method == "GET")
            rho_rhodesapp_keeplastvisitedurl(uri.c_str());
        // TODO: call rho_serve_index
        return true;
    }
    
    // Try to send requested file
    return send_file(sock, uri);
}

} // namespace net
} // namespace rho
