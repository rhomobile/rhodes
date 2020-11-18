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
import java.net.URL;
import java.net.HttpURLConnection;
import java.util.*;

public class NetRequest
{
    private static final String TAG = "JNetRequest";

    private HttpURLConnection connection = null;
    private String url = null;
    private String body = null;
    private String method = null;
    private String file = null;
    private boolean sslVerify = true;
    private long timeout = 30;
    HashMap<String, String> headers = null;

    public NetRequest()
    {
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

    private StringBuffer readFromStream(InputStream stream) throws java.io.IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(stream));
        String inputLine;
        StringBuffer response = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            response.append(inputLine);
        }

        return response;
    }

    private class GetRequestThread extends Thread
    {
        private int code = 0;
        private StringBuffer response = null;
        public GetRequestThread() {
        }

        @Override
        public void run() {

            try {
                int responseCode = connection.getResponseCode();
                StringBuffer response = null;
                if (responseCode == HttpURLConnection.HTTP_OK) {
                    response = readFromStream(connection.getInputStream());
                } else if (responseCode >= HttpURLConnection.HTTP_BAD_REQUEST) {
                    response = readFromStream(connection.getErrorStream());
                }
            }
            catch (java.io.IOException e) {
                Logger.E( TAG,  e.getClass().getSimpleName() + ": " + e.getMessage() );
            }
            finally {
                connection.disconnect();
            }

        }

        public StringBuffer getResponse() {
            return response;
        }

        public int getResponseCode() {
            return code;
        }
    }

    private int getData() throws java.io.IOException, java.lang.InterruptedException {
        URL _url = new URL(url);
        connection = (HttpURLConnection) _url.openConnection();
        connection.setReadTimeout((int)timeout);
        connection.setRequestMethod(method);
        fillHeaders();

        GetRequestThread get_thread = new GetRequestThread();
        get_thread.start();
        get_thread.join();
        int responseCode = get_thread.getResponseCode();
        StringBuffer response = get_thread.getResponse();

        return responseCode;
    }


} 