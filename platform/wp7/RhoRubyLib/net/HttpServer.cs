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

using System;
using rho.common;
using rho;
using System.Collections.Generic;
using System.Collections;
using System.Text;

namespace rho.net
{
    public class CHttpServer : CThreadQueue
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
                    new RhoLogger("HttpServer");

        String m_root;

        static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }
        private CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

        static public String HTTP_REDIRECT_CODE = "302";
        static public String PARAM_PREFIX_REQ = "request-";
        static public String PARAM_PREFIX_RESP = "response-";
        static public String RESPONSE_BODY_FILEPATH = "response-body-filepath";
        static public String JS_EVAL_FUNCNAME = "_rho_execJsWrapper";
        static public String AJAX_CALLBACK_FUNCNAME = "_rho_ajaxProxyCallback";
        static public String AJAX_PARAM_CALLBACK_ID = "_rho_callbackId";
        static public String AJAX_PARAM_RESPONSE = "response";
        static public String RESPONSE_STATUS = "status";
        static public String RESPONSE_STATUS_MESSAGE = "message";

        class CRoute
        {
            public String application = "";
            public String model = "";
            public String id = "";
            public String action = "";

            public static bool isid(String s)
            {
                return s.Length > 2 && s[0] == '{' && s[s.Length - 1] == '}';
            }
        };
        /*public class CResponse
        {
            public String m_strUrl = "";
            public bool m_bRedirect = true;

            public CResponse(String strUrl) { m_strUrl = strUrl; m_bRedirect = true;  }
            public CResponse(bool bRedirect) { m_bRedirect = bRedirect; }
        };*/
        class CServerCommand : IQueueCommand
        {
            String m_method, m_uri, m_query, m_body;
            Dictionary<String, String> m_headers;
            CRoute m_route;
            CHttpServer m_Parent;
            int m_nCommand;
            int m_iTabIndex;
            String m_strAjaxContext;

            public const int scDispatch = 1, scIndex = 2;

            public CServerCommand(CHttpServer Parent, CRoute route,
                String method, String uri, IDictionary headers, IDictionary data, int nCommand, String strAjaxContext, int tabIndex)
            {
                m_Parent = Parent;
                m_method = (null != method) ? method.toUpperCase() : "GET";
                m_uri = (0 <= uri.indexOf('?')) ? uri.substring(0, uri.indexOf('?')) : uri;
                m_route = route;
                m_nCommand = nCommand;
                m_strAjaxContext = strAjaxContext;
                m_iTabIndex = tabIndex;

                m_body = "";

                m_headers = new Dictionary<String, String>();
                if (null != headers)
                {
                    foreach (object key in headers.Keys)
                    {
                        m_headers[key.ToString()] = headers[key].ToString();
                    }
                }

                // Query parameters from URL are added first, then data parameters added.
                m_query = (0 <= uri.indexOf('?')) ? uri.substring(uri.indexOf('?') + 1) : "";
                if (null != data)
                {
                    StringBuilder sb = new StringBuilder(m_query);
                    foreach (object key in data.Keys)
                    {
                        if (0 < sb.Length) sb.Append("&");
                        sb.Append(key.ToString() + "=" + (null != data[key] ? data[key].ToString() : ""));
                    }
                    m_query = sb.ToString();
                }
            }

            public boolean isAjaxRequest()
            {
                return (m_strAjaxContext != null && 0 < m_strAjaxContext.length());
            }

            public void execute()
            {
                String strUrl = "";
                IDictionary result = null;
                if (m_nCommand == scDispatch)
                {
                    result = m_Parent.processDispatch(m_route, m_method, m_uri, m_headers, m_query, m_body);
                    strUrl = result[RESPONSE_BODY_FILEPATH].ToString();

                    if (isAjaxRequest() && result[RESPONSE_STATUS].ToString().equals(HTTP_REDIRECT_CODE))
                    {
                        IDictionary headers = new Dictionary<String, String>();
                        headers["X-Requested-With"] = "XMLHttpRequest";
                        if (!RHODESAPP().HttpServer.processBrowserRequest(
                            "GET",
                            new Uri(strUrl, UriKind.Relative),
                            headers,
                            new Dictionary<String, String>(),
                            m_strAjaxContext,
                            m_iTabIndex
                            ))
                        {
                            LOG.ERROR("Ajax request redirection failed.");
                        }
                        return;
                    }
                }
                else if (m_nCommand == scIndex)
                {
                    result = m_Parent.processIndex(m_route, m_method, m_uri, m_headers, m_query, m_body);
                    strUrl = result[RESPONSE_BODY_FILEPATH].ToString();
                }

                if (!isAjaxRequest())
                {
                    RHODESAPP().processWebNavigate(strUrl, -1);
                }
                else
                {
                    String res = CRhoFile.isResourceFileExist(strUrl) ? CRhoFile.readStringFromResourceFile(strUrl) : CRhoFile.readFileToString(strUrl);

                    
                    IDictionary headers = new Dictionary<String, String>();
                    StringBuilder jsonHeaders = new StringBuilder();
                    foreach (object key in result.Keys)
                    {
                        if (
                               key.ToString().startsWith(PARAM_PREFIX_REQ)
                            || key.ToString().startsWith(PARAM_PREFIX_RESP)
                            || key.ToString().equals(RESPONSE_STATUS_MESSAGE)
                            || key.ToString().equals(RESPONSE_STATUS)
                            ) continue;
                        headers[key.ToString()] = result[key].ToString();
                        jsonHeaders.AppendFormat("{0}\"{1}\": \"{2}\"",
                            (0 < jsonHeaders.Length) ? ", " : "",
                            key.ToString(), result[key].ToString());
                    }

                    String[] args = new String[5];
                    args[0] = m_strAjaxContext;
                    args[1] = res;
                    args[2] = "{" + jsonHeaders + "}";
                    args[3] = result[RESPONSE_STATUS_MESSAGE] != null ? result[RESPONSE_STATUS_MESSAGE].ToString() : "";
                    args[4] = result[RESPONSE_STATUS] != null ? result[RESPONSE_STATUS].ToString() : "";

                    RHODESAPP().processInvokeScriptArgs(AJAX_CALLBACK_FUNCNAME, args, m_iTabIndex);
                }
            }

            public boolean equals(IQueueCommand cmd) { return false; }
            public void cancel() { }
            public String toString() { return "uri :" + m_uri + "; cmd: " + (m_nCommand == scDispatch ? "dispatch" : "index"); }
        };

        public CHttpServer(String strFileRoot)
        {
            m_root = strFileRoot;

            base.setLogCategory(LOG.getLogCategory());

            setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);

            start(epNormal);
        }

        public override void run()
        {
            RHODESAPP().startApp();
            base.run();

            RHODESAPP().stopApp();
        }

        public override void processCommand(IQueueCommand pCmd)
        {
            (pCmd as CServerCommand).execute();
        }

        public boolean processBrowserRequest(Uri uri, String strAjaxContext, int tabIndex = -1)
        {
            return processBrowserRequest("get", uri, null, null, strAjaxContext, tabIndex);
        }

        public boolean processBrowserRequest(String reqType, Uri uri, IDictionary headers, IDictionary data, String strAjaxContext, int tabIndex = -1)
        {
            boolean bAjaxCall = !(strAjaxContext == null || strAjaxContext.length() == 0);
            
            if (!uri.OriginalString.StartsWith(RHODESAPP().getHomeUrl())
                           && (uri.IsAbsoluteUri || uri.OriginalString.StartsWith("res:")))
                return false;

            String query = "";
            String url = uri.OriginalString;
            url = url.StartsWith(RHODESAPP().getHomeUrl()) ? url.substring(RHODESAPP().getHomeUrl().length()) : url;

            int nFrag = url.LastIndexOf('#');
            if (nFrag >= 0)
                url = url.Substring(0, nFrag);

            int nQuery = url.IndexOf('?');
            if (nQuery >= 0)
            {
                query = url.Substring(nQuery + 1);
                url = url.Substring(0, nQuery);
            }

            if (url.EndsWith(".gen.html") || (isknowntype(url) && url.StartsWith(m_root)))
                return false;

            CRoute route = new CRoute();
            if (dispatch(url, route))
            {
                addQueueCommand(new CServerCommand(this, route, reqType,
                    url + (0 < query.Length ? ("?"+query) : ""),
                    headers, data, CServerCommand.scDispatch, strAjaxContext, tabIndex));
                //addQueueCommand(new CServerCommand(this, route, bAjaxCall ? "GET" : "GET", url, query, "", CServerCommand.scDispatch, strAjaxContext));

                return true;
            }

            String fullPath = url.StartsWith(m_root) ? url : CFilePath.join(m_root, url);

            String strIndexFile = getIndex(fullPath);
            if (strIndexFile.Length > 0)
            {
                addQueueCommand(new CServerCommand(this, route, reqType, url, headers, data, CServerCommand.scIndex, strAjaxContext, tabIndex));
                //addQueueCommand(new CServerCommand(this, route, bAjaxCall ? "GET" : "GET", url, query, "", CServerCommand.scIndex, strAjaxContext));

                return true;
            }

            return false;
        }

        IDictionary processDispatch(CRoute route, String method, String uri, Dictionary<String, String> headers, String query, String body)
        {
            Object rhoReq = create_request_hash(route, method, uri, query, headers, body);
            Object rhoResp = RhoRuby.callServe(rhoReq);

            String strRedirectUrl = getRedirectUrl(rhoResp);
            if (strRedirectUrl.Length > 0)
            {
                ((IDictionary)rhoResp)[RESPONSE_BODY_FILEPATH] = strRedirectUrl;
                return (IDictionary)rhoResp;
            }

            String strFilePath = RHODESAPP().canonicalizeRhoPath(uri) + ".gen.html";
            if (route.id.Length > 0)
                strFilePath = CFilePath.join(m_root, route.application + "/" + route.model + "/" + route.action) + ".gen.html";

            CRhoFile.recursiveCreateDir(strFilePath);
            CRhoFile.writeDataToFile(strFilePath, getResponseBody(rhoResp));

            if (method == "GET")
                RHODESAPP().keepLastVisitedUrl(uri);
            ((IDictionary)rhoResp)[RESPONSE_BODY_FILEPATH] = strFilePath;
            return (IDictionary)rhoResp;
        }

        IDictionary processIndex(CRoute route, String method, String uri, Dictionary<String, String> headers, String query, String body)
        {
            Object rhoResp = new Dictionary<String, String>();

            String fullPath = uri.StartsWith(m_root) ? uri : CFilePath.join(m_root, uri);
            String strIndexFile = getIndex(fullPath);

            if (!CRhoFile.isResourceFileExist(strIndexFile))
            {
                String error = "<html><font size=\"+4\"><h2>404 Not Found.</h2> The file " + uri + " was not found.</font></html>";
                String strFilePath = CFilePath.join(m_root, "rhodes_error") + ".gen.html";
                CRhoFile.recursiveCreateDir(strFilePath);
                CRhoFile.writeStringToFile(strFilePath, error);
                ((IDictionary)rhoResp)[RESPONSE_BODY_FILEPATH] = strFilePath;
                ((IDictionary)rhoResp)[RESPONSE_STATUS] = "404";
                ((IDictionary)rhoResp)[RESPONSE_STATUS_MESSAGE] = "Not Found";
                return (IDictionary)rhoResp;
            }

            if (CFilePath.getExtension(fullPath).Length > 0)
            {
                ((IDictionary)rhoResp)[RESPONSE_BODY_FILEPATH] = strIndexFile;
                ((IDictionary)rhoResp)[RESPONSE_STATUS] = "200";
                ((IDictionary)rhoResp)[RESPONSE_STATUS_MESSAGE] = "Ok";
                return (IDictionary)rhoResp;
            }

            Object rhoReq = create_request_hash(route, method, uri, query, headers, body);
            rhoResp = RhoRuby.callServeIndex(strIndexFile, rhoReq);

            String strRedirectUrl = getRedirectUrl(rhoResp);
            if (strRedirectUrl.Length > 0)
            {
                ((IDictionary)rhoResp)[RESPONSE_BODY_FILEPATH] = strRedirectUrl;
                return (IDictionary)rhoResp;
            }

            strIndexFile += ".gen.html";
            CRhoFile.recursiveCreateDir(strIndexFile);
            CRhoFile.writeDataToFile(strIndexFile, getResponseBody(rhoResp));

            if (method == "GET")
                RHODESAPP().keepLastVisitedUrl(uri);

            ((IDictionary)rhoResp)[RESPONSE_BODY_FILEPATH] = strIndexFile;
            return (IDictionary)rhoResp;
        }
/*
        CResponse decide(String method, String uri, String query, String body)
        {
            //if (process_registered(uri))
            //    return new CResponse(false);

            CRoute route = new CRoute();
            if (dispatch(uri, route))
            {
                Object rhoReq = create_request_hash(route, method, uri, query, null, body);
                Object rhoResp = RhoRuby.callServe(rhoReq);

                String strRedirectUrl = getRedirectUrl(rhoResp);
                if (strRedirectUrl.Length > 0)
                    return new CResponse(strRedirectUrl);

                String strFilePath = RHODESAPP().canonicalizeRhoPath(uri) + ".gen.html";
                if ( route.id.Length > 0 )
                    strFilePath = CFilePath.join(m_root, route.application + "/" + route.model + "/" + route.action) + ".gen.html";

                CRhoFile.recursiveCreateDir(strFilePath);
                CRhoFile.writeDataToFile(strFilePath, getResponseBody(rhoResp));

                if (method == "GET")
                    RHODESAPP().keepLastVisitedUrl(uri);

                return new CResponse(strFilePath);
            }

            String fullPath = uri.StartsWith(m_root) ? uri : CFilePath.join(m_root, uri);

            String strIndexFile = getIndex(fullPath);
            if (strIndexFile.Length > 0 )
            {
                if (!CRhoFile.isResourceFileExist(strIndexFile))
                {
                    String error = "<html><font size=\"+4\"><h2>404 Not Found.</h2> The file " + uri + " was not found.</font></html>";
                    String strFilePath = CFilePath.join(m_root,"rhodes_error") + ".gen.html";
                    CRhoFile.recursiveCreateDir(strFilePath);
                    CRhoFile.writeStringToFile(strFilePath, error);
                    return new CResponse(strFilePath);
                }

                if (CFilePath.getExtension(fullPath).Length > 0)
                    return new CResponse(strIndexFile);

                Object rhoReq = create_request_hash(route, method, uri, query, null, body);
                Object rhoResp = RhoRuby.callServeIndex(strIndexFile, rhoReq);

                String strRedirectUrl = getRedirectUrl(rhoResp);
                if (strRedirectUrl.Length > 0)
                    return new CResponse(strRedirectUrl);

                strIndexFile += ".gen.html";
                CRhoFile.recursiveCreateDir(strIndexFile);
                CRhoFile.writeDataToFile(strIndexFile, getResponseBody(rhoResp));

                if (method == "GET")
                    RHODESAPP().keepLastVisitedUrl(uri);

                return new CResponse(strIndexFile);
            }

            return new CResponse(false);
        }*/

        public bool call_ruby_method(String uri, String body, out String strReply)
        {
            CRoute route = new CRoute();
            strReply = String.Empty;
            if (!dispatch(uri, route))
            {
                if (route.application.equalsIgnoreCase("system"))
                {
                    if (route.model.equalsIgnoreCase("geolocation"))
                    {
                        //TODO: geolocation
                        //showGeoLocation();
                        return true;
                    }
                    else if (route.model.equalsIgnoreCase("loadserversources"))
                    {
                        RhoRuby.loadServerSources(body);
                        return true;
                    }
                    else if (route.model.equalsIgnoreCase("loadallsyncsources"))
                    {
                        RhoRuby.loadAllSyncSources();
                        return true;
                    }
                    else if (route.model.equalsIgnoreCase("resetDBOnSyncUserChanged"))
                    {
                        RhoRuby.resetDBOnSyncUserChanged();
                        return true;
                    }
                }

                return false;
            }

            Dictionary<String, String> headers = new Dictionary<String, String>();
            headers.Add("Content-Type","application/x-www-form-urlencoded");
            Object rhoReq = create_request_hash(route, "POST", uri, String.Empty, headers, body);
            Object rhoResp = RhoRuby.callServe(rhoReq);
            strReply = getResponseBodyString(rhoResp);

            return true;
        }

        byte[] getResponseBody(Object resp)
        {
            Object body = RhoRuby.hashGet(resp, "request-body");
            return RhoRuby.getBytesFromString(body);
        }

        String getResponseBodyString(Object resp)
        {
            Object body = RhoRuby.hashGet(resp, "request-body");
            return RhoRuby.getStringFromObject(body);
        }

        String getRedirectUrl(Object resp)
        {
            int nStatus = RhoRuby.hashGetInt(resp, "status");
            if ( nStatus != 302 )
                return String.Empty;

            return RhoRuby.hashGetString(resp, "Location");
        }

        bool dispatch(String uri, CRoute route)
        {
            if (isknowntype(uri))
                return false;

            // Trying to parse route
            if (!parse_route(uri, route))
                return false;

            // Convert CamelCase to underscore_case
            String controllerName = "";
            for (int i = 0; i < route.model.Length; i++)
            {
                if (Char.IsUpper(route.model[i]) && i > 0)
                    controllerName += '_';

                controllerName += route.model[i];
            }
            controllerName = controllerName.ToLower();

            String model_name_controller = m_root + "/" + route.application + "/" + route.model + "/" + controllerName + "_controller.rb";
            String controller = m_root + "/" + route.application + "/" + route.model + "/controller.rb";

            return CRhoFile.isResourceFileExist(model_name_controller) || CRhoFile.isResourceFileExist(controller);
        }

        bool parse_route(String uri, CRoute route)
        {
            if (uri.StartsWith(RHODESAPP().getHomeUrl()))
                uri = uri.Substring(RHODESAPP().getHomeUrl().Length + 1);

            if (uri.StartsWith(m_root))
                uri = uri.Substring(m_root.Length+1);
            else if ( uri.StartsWith("/") )
                uri = uri.Remove(0, 1 );

            String[] arParts = uri.Split('/');
            if (arParts.Length > 0 )
                route.application = arParts[0];

            if (arParts.Length < 2)
                return false;

            route.model = arParts[1];

            if (arParts.Length < 3)
                return true;

            String aoi = arParts[2];
            if (CRoute.isid(aoi))
            {
                route.id = aoi;
                if (arParts.Length > 3)
                    route.action = arParts[3];
            }
            else 
            {
                if (arParts.Length > 3)
                    route.id = arParts[3];

                route.action = aoi;
            }

            return true;
        }

        bool isknowntype(String uri)
        {
            String[] ignored_exts = { ".css", ".js", ".html", ".htm", ".png", ".bmp", ".jpg", ".jpeg", ".gif" };

            String strExt = CFilePath.getExtension(uri).ToLower();
            return find_in_string_array(strExt, ignored_exts);
        }

        String getIndex(String path)
        {
            String[] index_files = { "index_erb.rb", "index.html", "index.htm", "index.php", "index.cgi" };

            if (CFilePath.getExtension(path).Length == 0)
                return CFilePath.join(path, index_files[0]);

            String strFileName = CFilePath.getBaseName(path);
            return find_in_string_array(strFileName, index_files) ? path : "";
        }

        public bool process_registered(String uri)
        {
            //TODO: process_registered
            return false;
        }

        Object create_request_hash(CRoute route, String method, String uri, String query,
                                 Dictionary<String, String> headers, String body)
        {
            Object hash = RhoRuby.createHash();

            if ( route != null )
            {
                RhoRuby.hashAdd(hash, "application", route.application);
                RhoRuby.hashAdd(hash, "model", route.model);

                if ( route.action.Length > 0 )
                    RhoRuby.hashAdd(hash, "action", route.action);
                if ( route.id.Length > 0 )
                    RhoRuby.hashAdd(hash, "id", route.id);
            }

            RhoRuby.hashAdd(hash, "request-method", method);
            RhoRuby.hashAdd(hash, "request-uri", uri);
            RhoRuby.hashAdd(hash, "request-query", query);

            Object hash_headers = RhoRuby.createHash();
            if (headers != null)
            {
                Dictionary<String, String>.Enumerator hashEnum = headers.GetEnumerator();
                while (hashEnum.MoveNext())
                {
                    RhoRuby.hashAdd(hash_headers, hashEnum.Current.Key, hashEnum.Current.Value);
                }
            }
            RhoRuby.hashAdd(hash, "headers", hash_headers);
	
            if ( null != body && body.Length > 0 )
                RhoRuby.hashAdd(hash, "request-body", body);
    
            return hash;
        }

        bool find_in_string_array(String str, String[] ar)
        {
            for (int i = 0; i < ar.Length; i++)
            {
                if (str.CompareTo(ar[i]) == 0)
                    return true;
            }

            return false;
        }
    }
}
