using System;
using rho.common;
using System.Collections.Generic;

namespace rho.net
{
    public class CHttpServer
    {
        String m_root;

        class CRoute
        {
            public String application;
            public String model;
            public String id;
            public String action;

            public static bool isid(String s)
            {
                return s.Length > 2 && s[0] == '{' && s[s.Length - 1] == '}';
            }
        };

        public CHttpServer(String strFileRoot)
        {
            m_root = strFileRoot;
        }

        public bool process_registered(Uri uri)
        {
            return false;
        }

        public String decide(String strMethod, Uri uri, String strBody)
        {
            if (process_registered(uri))
                return "";

            CRoute route = new CRoute();
            if (dispatch(uri, route))
            {
                return "";
            }

            String fullPath = CFilePath.join(m_root, uri.OriginalString);
            String strIndexFile = getIndex(fullPath);
            if (strIndexFile.Length > 0 )
            {
                if (!CRhoFile.isResourceFileExist(strIndexFile))
                {
                    String error = "<html><font size=\"+4\"><h2>404 Not Found.</h2> The file " + uri + " was not found.</font></html>";
                    //TODO: return error page
                    return "";
                }

                return "";
            }

            //TODO: return file
            return "";
        }

        bool dispatch(Uri uri, CRoute route)
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

        bool parse_route(Uri uri, CRoute route)
        {
            String strPath = uri.OriginalString;//.GetComponents(UriComponents.Path, UriFormat.SafeUnescaped);
            String[] arParts = strPath.Split('/');

            if (arParts.Length < 2)
                return false;

            route.application = arParts[0];
            route.model = arParts[1];

            if (arParts.Length < 4)
                return true;

            String aoi = arParts[2];
            if (CRoute.isid(aoi))
            {
                route.id = aoi;
                route.action = arParts[3];
            }
            else 
            {
                route.id = arParts[3];
                route.action = aoi;
            }

            return true;
        }

        bool isknowntype(Uri uri)
        {
            String[] ignored_exts = { ".css", ".js", ".html", ".htm", ".png", ".bmp", ".jpg", ".jpeg", ".gif" };

            String strExt = CFilePath.getExtension(uri.ToString()).ToLower();
            return find_in_string_array(strExt, ignored_exts);
        }

        String getIndex(String path)
        {
            String[] index_files = { "index_erb.rb", "index.html", "index.htm", "index.php", "index.cgi" };

            String strExt = CFilePath.getExtension(path);
            if (strExt.Length == 0)
                return CFilePath.join( path, index_files[0] );

            String strFileName = CFilePath.getBaseName(path);
            return find_in_string_array(strFileName, index_files) ? path : "";
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
