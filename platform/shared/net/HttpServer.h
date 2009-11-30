#ifndef RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD
#define RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD

#include "common/RhoStd.h"
#include "common/RhoPort.h"
#include "logging/RhoLog.h"

#if !defined(OS_WINDOWS) && !defined(OS_WINCE)
typedef int SOCKET;
#  define INVALID_SOCKET -1
#  define SOCKET_ERROR -1
#  define RHO_NET_ERROR_CODE errno
#  define closesocket close
#else
#  define RHO_NET_ERROR_CODE ::WSAGetLastError()
#endif

namespace rho
{
namespace net
{

struct HttpHeader
{
    String name;
    String value;
    
    HttpHeader() {}
    
    HttpHeader(String const &n, String const &v)
        :name(n), value(v)
    {}
};

typedef Vector<HttpHeader> HttpHeaderList;

class CHttpServer
{
    DEFINE_LOGCLASS;
    
    enum {BUF_SIZE = 4096};
    
    typedef HttpHeader Header;
    typedef HttpHeaderList HeaderList;
    
    typedef Vector<char> ByteVector;
    
    struct Route
    {
        String application;
        String model;
        String id;
        String action;
    };
    
public:
    typedef void (*callback_t)(void *);
    
public:
    CHttpServer(int port, String const &root);
    ~CHttpServer();
    
    void register_uri(String const &uri, callback_t const &callback);
    
    bool run();
    
private:
    bool process(SOCKET sock);
    bool parse_request(ByteVector &request, String &method, String &uri,
                       String &query, HeaderList &headers, String &body);
    bool parse_startline(String const &line, String &method, String &uri, String &query);
    bool parse_header(String const &line, Header &hdr);
    bool parse_route(String const &uri, Route &route);
    
    bool receive_request(SOCKET sock, ByteVector &request);
    bool send_response(SOCKET sock, String const &response);
    
    String create_response(String const &reason);
    String create_response(String const &reason, HeaderList const &headers);
    String create_response(String const &reason, String const &body);
    String create_response(String const &reason, HeaderList const &headers, String const &body);
    
    bool decide(SOCKET sock, String const &method, String const &uri,
                String const &query, HeaderList const &headers, String const &body);
    
    bool dispatch(String const &uri, Route &route);
    
    bool isknowntype(String const &uri);
    bool isindex(String const &uri);
    
    bool send_file(SOCKET sock, String const &path);
    
    callback_t registered(String const &uri);
    
private:
    int m_port;
    String m_root;
    SOCKET m_listener;
    std::map<String, callback_t> m_registered;
};

} // namespace net
} // namespace rho

#endif // RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD
