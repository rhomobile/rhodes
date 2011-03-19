using System;
using rho.common;
using rho;
using System.Collections.Generic;

namespace rho.net
{
    public class CHttpServer
    {
        String m_root;

        private CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }
        private CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

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
        public class CResponse
        {
            public String m_strUrl = "";
            public bool m_bRedirect = true;

            public CResponse(String strUrl) { m_strUrl = strUrl; m_bRedirect = true;  }
            public CResponse(bool bRedirect) { m_bRedirect = bRedirect; }
        };

        public CHttpServer(String strFileRoot)
        {
            m_root = strFileRoot;
        }

        public bool process_registered(String uri)
        {
            return false;
        }


        public CResponse decide(String method, String uri, String query, String body)
        {
            if ( uri.EndsWith(".gen.html") || (isknowntype(uri) && uri.StartsWith(m_root)) )
                return new CResponse(false);

            if (process_registered(uri))
                return new CResponse(false);

            CRoute route = new CRoute();
            if (dispatch(uri, route))
            {
                Object rhoReq = create_request_hash(route, method, uri, query, null, body);
                Object rhoResp = RhoRuby.callServe(rhoReq);

                String strRedirectUrl = getRedirectUrl(rhoResp);
                if (strRedirectUrl.Length > 0)
                    return new CResponse(strRedirectUrl);

                String strFilePath = RHODESAPP().canonicalizeRhoUrl(uri) + ".gen.html";
                if ( route.id.Length > 0 )
                    strFilePath = CFilePath.join(m_root, route.application + "/" + route.model + "/" + route.action) + ".gen.html";

                CRhoFile.recursiveCreateDir(strFilePath);
                CRhoFile.writeDataToFile(strFilePath, getResponseBody(rhoResp));

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
                return new CResponse(strIndexFile);
            }

            return new CResponse(false);
        }

        byte[] getResponseBody(Object resp)
        {
            Object body = RhoRuby.hashGet(resp, "request-body");
            return RhoRuby.getBytesFromString(body);
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
            if (uri.StartsWith(m_root))
                uri = uri.Substring(m_root.Length+1);
            else if ( uri.StartsWith("/") )
                uri = uri.Remove(0, 1 );

            String[] arParts = uri.Split('/');

            if (arParts.Length < 2)
                return false;

            route.application = arParts[0];
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
                //TODO: add headers
            }
            RhoRuby.hashAdd(hash, "headers", hash_headers);
	
            if ( body.Length > 0 )
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
