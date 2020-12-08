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

package com.rhomobile.rhodes;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.URL;
import java.net.HttpURLConnection;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class NetRequest
{
    private class JCMultipartItem
    {
        public String m_strFilePath;
        public String m_strBody;
        public String m_strName, m_strFileName, m_strContentType;
        public String m_strDataPrefix;
        public JCMultipartItem() {}
    }

    private static final String TAG = "JNetRequest";

    private HttpURLConnection connection = null;
    private String url = null;
    private String body = null;
    private String method = null;
    private String file = null;
    private boolean sslVerify = true;
    private long timeout = 30;
    private String responseBody = null;
    HashMap<String, String> headers = null;
    Map<String, List<String>> response_headers = null;
    List<JCMultipartItem> multipartItems = new ArrayList<JCMultipartItem>();

    public NetRequest()
    {
    }

    public void AddMultiPartData(String strFilePath, String strBody, String strName, String strFileName, String strContentType, String strDataPrefix) {
        JCMultipartItem item = new JCMultipartItem();
        item.m_strFilePath = strFilePath;
        item.m_strBody = strBody;
        item.m_strName = strName;
        item.m_strFileName = strFileName;
        item.m_strContentType = strContentType;
        item.m_strDataPrefix = strDataPrefix;
        multipartItems.add(item);
    }

    public int doPull(String u, String m, String b, String f, HashMap<String, String> h, boolean verify, long t) {
        url = u;
        body = b;
        method = m;
        file = f;
        sslVerify = verify;
        timeout = t;
        headers = h;

        try {
            if(method.equals("POST")) {
                int code = postData();
                return code;
            }
            else {
               int code = getData();
               return code;
            }

        }
        catch (java.io.IOException e) {
            Logger.E( TAG,  e.getClass().getSimpleName() + ": " + e.getMessage() );
        }
        catch (java.lang.Exception e) {
            Logger.E( TAG,  e.getClass().getSimpleName() + ": " + e.getMessage() );
        }

        return 0;
    }

    private void fillHeaders() {
        for(Map.Entry<String, String> entry : headers.entrySet()) {
            String key = entry.getKey();
            String value = entry.getValue();
            connection.setRequestProperty(key, value);
        }
    }

    private String readFromStream(InputStream stream) throws java.io.IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(stream, StandardCharsets.UTF_8));
        String inputLine;

        StringBuilder response = new StringBuilder();
        while ((inputLine = in.readLine()) != null) {
            response.append(inputLine);
        }
        in.close();

        return response.toString();
    }

    private static class RequestThread extends Thread
    {
        protected int code = 0;
        protected String response = null;
        protected Map<String, List<String>> response_headers = null;

        public RequestThread() {}
        public String getResponse() {
            return response;
        }
        public Map<String, List<String>> getResponseHeaders() { return response_headers; }
        public int getResponseCode() {
            return code;
        }
    }

    private class GetRequestThread extends RequestThread
    {
        public GetRequestThread() {
        }

        @Override
        public void run() {

            try {
                code = connection.getResponseCode();
                response = null;
                if (code == HttpURLConnection.HTTP_OK) {
                    response = readFromStream(connection.getInputStream());
                } else if (code >= HttpURLConnection.HTTP_BAD_REQUEST) {
                    response = readFromStream(connection.getErrorStream());
                }

                response_headers = connection.getHeaderFields();
            }
            catch (java.io.IOException e) {
                Logger.E( TAG,  e.getClass().getSimpleName() + ": " + e.getMessage() );
            }
            finally {
                connection.disconnect();
            }

        }

    }

    private class PostRequestThread extends RequestThread
    {
        public PostRequestThread() {}

        @Override
        public void run() {
            try {

                OutputStreamWriter writer = new OutputStreamWriter(connection.getOutputStream(), StandardCharsets.UTF_8);
                writer.write(body);
                writer.flush();

                code = connection.getResponseCode();
                response = null;
                if (code == HttpURLConnection.HTTP_OK) {
                    response = readFromStream(connection.getInputStream());
                } else if (code >= HttpURLConnection.HTTP_BAD_REQUEST) {
                    response = readFromStream(connection.getErrorStream());
                }

                response_headers = connection.getHeaderFields();
            }
            catch (java.io.IOException e) {
                Logger.E( TAG,  e.getClass().getSimpleName() + ": " + e.getMessage() );
            }
            finally {
                connection.disconnect();
            }
        }
    }

    public String[] getKeysFromResponseHeaders() {
        if(response_headers == null  || response_headers.size() == 0) return null;

        ArrayList<String> values = new ArrayList<String>();
        for(Map.Entry<String, List<String>> entry : response_headers.entrySet()) {
            for(String item : entry.getValue()) {
                values.add(entry.getKey());
            }
        }


        Object[] array_values = values.toArray();
        return Arrays.copyOf(array_values, array_values.length, String[].class);
    }

    public String[] getValuesFromResponseHeaders() {
        if(response_headers == null || response_headers.size() == 0) return null;

        ArrayList<String> values = new ArrayList<String>();
        for(Map.Entry<String, List<String>> entry : response_headers.entrySet()) {
            values.addAll(entry.getValue());
        }

        Object[] array_values = values.toArray();
        return Arrays.copyOf(array_values, array_values.length, String[].class);
    }

    public String getResponseBody() {
        return responseBody;
    }

    private int postData() throws java.io.IOException, java.lang.InterruptedException {
        URL _url = new URL(url);
        connection = (HttpURLConnection) _url.openConnection();
        connection.setReadTimeout((int)timeout);
        connection.setConnectTimeout(1000);
        connection.setRequestMethod(method);
        connection.setDoOutput(true);
        connection.setUseCaches(false);
        fillHeaders();

        PostRequestThread post_thread = new PostRequestThread();
        post_thread.start();
        post_thread.join();
        int responseCode = post_thread.getResponseCode();
        responseBody = post_thread.getResponse();
        response_headers = post_thread.getResponseHeaders();

        return responseCode;
    }

    private int getData() throws java.io.IOException, java.lang.InterruptedException {
        URL _url = new URL(url);
        connection = (HttpURLConnection) _url.openConnection();
        connection.setReadTimeout((int)timeout);
        connection.setConnectTimeout(1000);
        connection.setRequestMethod(method);
        fillHeaders();

        GetRequestThread get_thread = new GetRequestThread();
        get_thread.start();
        get_thread.join();
        int responseCode = get_thread.getResponseCode();
        responseBody = get_thread.getResponse();
        response_headers = get_thread.getResponseHeaders();

        return responseCode;
    }


} 