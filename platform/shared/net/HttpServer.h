#ifndef RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD
#define RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD

#include "common/RhoStd.h"
#include "logging/RhoLog.h"

#if !defined(OS_WINDOWS) && !defined(OS_WINCE)
typedef int SOCKET;
#  define INVALID_SOCKET -1
#  define SOCKET_ERROR -1
#  define RHO_NET_ERROR_CODE errno
#  define closesocket close
#else
#  if defined(OS_WINCE)
#    include <winsock.h>
#  endif
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
    
    HttpHeader(String const &n, int v)
        :name(n)
    {
        char buf[30];
        snprintf(buf, sizeof(buf), "%d", v);
        value = buf;
    }
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
    typedef void (*callback_t)(void *arg, String const &query);
    
public:
    CHttpServer(int port, String const &root);
    ~CHttpServer();
    
    void register_uri(String const &uri, callback_t const &callback);

    bool started() const {return !m_exit;}
    
    bool run();
    void stop();

    bool send_response(String const &response) {return send_response_impl(response, false);}
    bool send_response_body(String const &data) {return send_response_impl(data, true);}
    
    String create_response(String const &reason);
    String create_response(String const &reason, HeaderList const &headers);
    String create_response(String const &reason, String const &body);
    String create_response(String const &reason, HeaderList const &headers, String const &body);
    
private:
    bool init();
    bool process(SOCKET sock);
    bool parse_request(String &method, String &uri, String &query, HeaderList &headers, String &body);
    bool parse_startline(String const &line, String &method, String &uri, String &query);
    bool parse_header(String const &line, Header &hdr);
    bool parse_route(String const &uri, Route &route);
    
    bool receive_request(ByteVector &request);
    
    bool decide(String const &method, String const &uri,
                String const &query, HeaderList const &headers, String const &body);
    
    bool dispatch(String const &uri, Route &route);
    
    bool send_file(String const &path);
    
    bool send_response_impl(String const &data, bool continuation);
    
    callback_t registered(String const &uri);
    
private:
    bool m_exit;
    int m_port;
    String m_root, m_strRhoRoot;
    SOCKET m_listener;
    SOCKET m_sock;
    std::map<String, callback_t> m_registered;
    bool verbose;
};

} // namespace net
} // namespace rho

#endif // RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD
