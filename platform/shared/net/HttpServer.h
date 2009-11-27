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

class CHttpServer
{
    DEFINE_LOGCLASS;
    
    enum {BUF_SIZE = 4096};
    
    struct Header
    {
        String name;
        String value;
        
        Header() {}
        
        Header(String const &n, String const &v)
            :name(n), value(v)
        {}
    };
    
public:
    CHttpServer(int port);
    ~CHttpServer();
    
    bool run();
    
private:
    bool process(SOCKET sock);
    bool parse_request(Vector<char> &request, String &method, String &uri,
                       Vector<Header> &headers, String &body);
    bool parse_startline(String const &line, String &method, String &uri);
    bool parse_header(String const &line, Header &hdr);
    
    bool receive_request(SOCKET sock, Vector<char> &request);
    bool send_response(SOCKET sock, String const &response);
    
    String create_response(String const &reason);
    String create_response(String const &reason, Vector<Header> const &headers);
    
private:
    int m_port;
    SOCKET m_listener;
};

} // namespace net
} // namespace rho

#endif // RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD
