/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#ifndef RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD
#define RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD

#include "common/RhoStd.h"
#include "logging/RhoLog.h"

#include "common/RhoThread.h"

#if !defined(WINDOWS_PLATFORM)
typedef int SOCKET;
#  define INVALID_SOCKET -1
#  define SOCKET_ERROR -1
#  define RHO_NET_ERROR_CODE errno
#  define closesocket close
#else
#  if defined(OS_WINCE)
#    include <winsock.h>
#  elif defined(OS_WP8) || defined(OS_UWP)
#    //include "ruby/wp8/winsock_wp8.h"
#      include <winsock2.h>
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

class CDirectHttpRequestQueue;

class CHttpServer
{
    DEFINE_LOGCLASS;
    
    enum {BUF_SIZE = 4096};
    
  
    typedef Vector<char> ByteVector;
    
    struct Route
    {
        String application;
        String model;
        String id;
        String action;
    };
  
    class ResponseWriter
    {
      String m_response;
      
    public:
      void writeResponse( const String& data ) {
        m_response += data;
      }
      
      const String& getResponse() const { return m_response; }
    };
    
public:

    typedef HttpHeader Header;
    typedef HttpHeaderList HeaderList;

    typedef void (*callback_t)(void *arg, String const &query);
    
public:
    CHttpServer(int port, String const &root);
    CHttpServer(int port, String const &root, String const &user_root, String const &runtime_root);
    CHttpServer(int port, String const &root, String const &user_root, String const &runtime_root, bool enable_external_access, bool started_as_separated_simple_server);
    ~CHttpServer();
    
    void register_uri(String const &uri, callback_t const &callback);

    bool started() const {return m_active;}
    
    bool run();
    void stop();

    bool send_response(String const &response, bool redirect = false);
    bool send_response_body(String const &data) {return send_response_impl(data, true);}
    
    String create_response(String const &reason);
    String create_response(String const &reason, HeaderList const &headers);
    String create_response(String const &reason, String const &body);
    String create_response(String const &reason, HeaderList const &headers, String const &body);

    static int isIndex(String const &uri);
  
    int getPort();
    rho::String getIPAdress();
    void disableAllLogging();

    bool call_ruby_method(String const &uri, String const &body, String& strReply);
  
#ifdef OS_MACOSX
    //can be run only from ruby thread!
    String directRequest( const String& method, const String& uri, const String& query, const HeaderList& headers ,const String& body );
#endif
  
private:
    bool init();
    void close_listener();
    bool process(SOCKET sock);
    bool parse_request(String &method, String &uri, String &query, HeaderList &headers, String &body);
    bool parse_startline(String const &line, String &method, String &uri, String &query);
    bool parse_header(String const &line, Header &hdr);
    bool parse_route(String const &uri, Route &route);
    
    bool receive_request(ByteVector &request);
    
    bool decide(String const &method, String const &uri,
                String const &query, HeaderList const &headers, String const &body);
    
    bool dispatch(String const &uri, Route &route);
    
    bool send_file(String const &path, HeaderList const &hdrs);
    
    bool send_response_impl(String const &data, bool continuation);
    
    callback_t registered(String const &uri);
    void call_ruby_proc( rho::String const &query, String const &body );

    int select_internal( SOCKET listener, fd_set& readfds );

private:
    bool m_active;
    int m_port;
    String m_IP_adress;
    String m_root, m_userroot, m_strRhoRoot, m_strRhoUserRoot, m_strRuntimeRoot;
    SOCKET m_listener;
    SOCKET m_sock;
    std::map<String, callback_t> m_registered;
    bool verbose;
    bool m_enable_external_access;
    bool m_started_as_separated_simple_server;

#ifdef OS_MACOSX
    common::CMutex m_mxSyncRequest;
    ResponseWriter* m_localResponseWriter;
  
    friend class CDirectHttpRequestQueue;
    CDirectHttpRequestQueue* m_pQueue;
    void setQueue( CDirectHttpRequestQueue* q ) { m_pQueue = q; }
#endif
};

#ifdef OS_MACOSX
class CDirectHttpRequestQueue
{
public:
  CDirectHttpRequestQueue( CHttpServer& server, common::CRhoThread& ownerThread ) : m_server(server), m_thread( ownerThread ), m_request(0) {}
  
  struct CDirectHttpRequest
  {
    CDirectHttpRequest() : /*method(0), uri(0), query(0), headers(0), body(0), */signal(0), mutex(0) {}
    
    void clear()
    {
      method = ""; uri = ""; query = ""; headers.clear(); body = ""; signal = 0; mutex = 0;
    }

    const String& getResponse() const { return m_response; }

    String method;
    String uri;
    String query;
    CHttpServer::HeaderList headers;
    String body;
    pthread_cond_t* signal;
    pthread_mutex_t* mutex;
    String m_response;
  };
  
  bool run();
  void doRequest( CDirectHttpRequest& req );

private:
  CHttpServer& m_server;
  common::CRhoThread& m_thread;
  CDirectHttpRequest* m_request;
};
#endif //OS_MACOSX
void rho_http_ruby_proc_callback(void *arg, rho::String const &query );

} // namespace net
} // namespace rho

#endif // RHO_HTTP_SERVER_F5FFD21AD3EE463E850C5E2C789397BD

