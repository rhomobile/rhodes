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

import android.content.res.AssetFileDescriptor;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.net.HttpURLConnection;
import java.net.URLConnection;
import java.nio.charset.StandardCharsets;
import java.security.GeneralSecurityException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLSession;

import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.socket.SSLImpl;
import java.text.SimpleDateFormat;

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
    private int fd = -1;
    private boolean sslVerify = true;
    private long timeout = 30;
    private String responseBody = null;
    HashMap<String, String> headers = null;
    Map<String, List<String>> response_headers = null;
    List<JCMultipartItem> multipartItems = new ArrayList<JCMultipartItem>();

    private String user = null;
    private String pwd = null;
    private String realm = null;
    private String nonce = null;
    private boolean is_digest = false;
    private String authHeader = null;


    public void SetAuthSettings(String u, String p, boolean is_d) {
        user = u;
        pwd = p;
        is_digest = is_d;
    }

    public NetRequest()
    {
    }

    private void SetAuthentificationHeader(URL u) {
        if(pwd == null || user == null) return;
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

    public int doPull(String u, String m, String b, int _fd, HashMap<String, String> h, boolean verify, long t) {
        url = u;
        body = b;
        method = m;
        fd = _fd;
        sslVerify = verify;
        timeout = t;
        headers = h;

        try {
            if(method.equals("POST")) {
                int code = postData(false);
                return code;
            }
            else {
               int code = getData(false);
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

        if(authHeader != null)
            connection.setRequestProperty("Authorization", authHeader);
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

    private long getMultiPartDataSize() {
        long size = 0;
        String m_multipartPostfix = body;
        for (JCMultipartItem item : multipartItems) {
            if(!item.m_strFilePath.isEmpty()) {
                File ofile = new File(item.m_strFilePath);
                if(ofile.exists())
                    size += ofile.length();
                else {
                    AssetFileDescriptor asfd = RhoFileApi.openAssetFd(item.m_strFilePath);
                    size += asfd.getLength();
                }
                size += item.m_strDataPrefix.length();
            }
            else {
                size += item.m_strDataPrefix.length();
                size += item.m_strBody.length();
            }
        }
        size += m_multipartPostfix.length();
        return size;
    }

    private boolean writeMultiPartData(OutputStreamWriter stream) throws java.io.IOException {

        char buffer[] = new char[4096];
        int recv = 0;
        String m_multipartPostfix = body;

        for (JCMultipartItem item : multipartItems) {
            if(!item.m_strFilePath.isEmpty()) {
                stream.write(item.m_strDataPrefix);

                InputStreamReader in = new InputStreamReader(RhoFileApi.open(item.m_strFilePath));
                while((recv = in.read(buffer)) > 0) {
                    stream.write(buffer, 0, recv);
                }

                in.close();
            }
            else {
                stream.write(item.m_strDataPrefix);
                stream.write(item.m_strBody);
            }
        }
        stream.write(m_multipartPostfix);
        return true;
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
                if(multipartItems.isEmpty())
                    writer.write(body);
                else {
                    writeMultiPartData(writer);
                }
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
            catch (java.lang.Exception e) {
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

    private String byteToHex(byte num) {
        char[] hexDigits = new char[2];
        hexDigits[0] = Character.forDigit((num >> 4) & 0xF, 16);
        hexDigits[1] = Character.forDigit((num & 0xF), 16);
        return new String(hexDigits);
    }

    public String encodeHexString(byte[] byteArray) {
        StringBuffer hexStringBuffer = new StringBuffer();
        for (int i = 0; i < byteArray.length; i++) {
            hexStringBuffer.append(byteToHex(byteArray[i]));
        }
        return hexStringBuffer.toString();
    }

    private String calculateNonce() throws NoSuchAlgorithmException, UnsupportedEncodingException {
        Date d = new Date();
        SimpleDateFormat f = new SimpleDateFormat("yyyy:MM:dd:hh:mm:ss");
        String fmtDate = f.format(d);
        Random rand = new Random(100000);
        Integer randomInt = rand.nextInt();

        MessageDigest hash = MessageDigest.getInstance("MD5");
        byte[] cnonce = hash.digest((fmtDate + randomInt.toString()).getBytes("UTF-8"));
        return encodeHexString(cnonce);
    }

    private HashMap<String, String> parseHeader(String headerString) {
        String headerStringWithoutScheme = headerString.substring(headerString.indexOf(" ") + 1).trim();
        HashMap<String, String> values = new HashMap<String, String>();
        String keyValueArray[] = headerStringWithoutScheme.split(",");
        for (String keyval : keyValueArray) {
            if (keyval.contains("=")) {
                String key = keyval.substring(0, keyval.indexOf("="));
                String value = keyval.substring(keyval.indexOf("=") + 1);
                values.put(key.trim(), value.replaceAll("\"", "").trim());
            }
        }
        return values;
    }

    private int Authetentificate() throws java.io.IOException, java.lang.InterruptedException {
        List<String> headers = response_headers.get("WWW-Authenticate");
        HashMap<String, String> values = null;
        if(!headers.isEmpty())
            values = parseHeader(headers.get(0));
        else
            return 0;

        URL _url = new URL(url);
        String method = connection.getRequestMethod();
        String nonce = values.get("nonce");
        String opaque = values.get("opaque");
        String algo = values.get("algorithm");
        String qop = values.get("qop");
        realm = values.get("realm");
        String cnonce = null;
        String uri = _url.getPath();
        String nc = "00000001";

        String ha1 = null, ha2 = null, serverResponse = null;

        try {
            cnonce = calculateNonce();

            MessageDigest hash = MessageDigest.getInstance("MD5");
            byte[] sub_ha1 = hash.digest((user + ":" + realm + ":" + pwd).getBytes("UTF-8"));
            hash.reset();
            //hash.update(sub_ha1);
            //byte[] ha1_ = hash.digest((":" + nonce + ":" + cnonce).getBytes("UTF-8"));
            //byte[] ha1_ = hash.digest();
            byte[] ha1_ = hash.digest((encodeHexString(sub_ha1) + ":" + nonce + ":" + cnonce).getBytes("UTF-8"));

            hash.reset();
            byte[] ha2_ = hash.digest((method + ":" + uri).getBytes("UTF-8"));

            ha1 = encodeHexString(ha1_);
            ha2 = encodeHexString(ha2_);
            hash.reset();
            //hash.update(ha1_);
            byte[] response = hash.digest((ha1 + ":" + nonce + ":" + nc + ":" +
                    cnonce + ":" + qop + ":" + ha2).getBytes("UTF-8"));
            //hash.update(ha2_);
            //byte[] response = hash.digest();

            ha1 = encodeHexString(ha1_);
            ha2 = encodeHexString(ha2_);
            serverResponse = encodeHexString(response);

        }
        catch (NoSuchAlgorithmException e) {
            Logger.E( TAG,  e.getClass().getSimpleName() + ": " + e.getMessage() );
            return 0;
        }

        //String ha1 = DigestUtils.md5Hex(DigestUtils.md5Hex(user + ":" + realm + ":" + pwd) + ":" + nonce + ":" + cnonce);
        //String ha2 = DigestUtils.md5Hex(method + ":" + uri);
        //String serverResponse = DigestUtils.md5Hex(ha1 + ":" + nonce + ":" + nc + ":" + cnonce + ":" + qop + ":" + ha2);


        authHeader = String.format("Digest username=\"%s\", realm=\"%s\", " +
                "nonce=\"%s\", uri=\"%s\", qop=auth, nc=%s, cnonce=\"%s\", " +
                "response=\"%s\"",
                user, realm, nonce, uri, nc, cnonce, serverResponse);

        if(opaque != null)
            authHeader += String.format(", opaque=\"%s\"", opaque);
        if(algo != null)
            authHeader += String.format(", algorithm=\"%s\"", algo);

        if(method.equals("POST")) {
           int code = postData(true);
           return code;
        }
        else {
           int code = getData(true);
           return code;
        }
    }

    private int postData(boolean auth) throws java.io.IOException, java.lang.InterruptedException {
        URL _url = new URL(url);
        connection = getConnection(_url);
        connection.setReadTimeout((int)30000);
        connection.setConnectTimeout(5000);
        connection.setRequestMethod(method);
        connection.setDoOutput(true);
        connection.setUseCaches(false);

        if(!multipartItems.isEmpty()) {
            long size = getMultiPartDataSize();
            connection.setRequestProperty("Content-Length", Long.toString(size));
            connection.setFixedLengthStreamingMode(size);
        }
        else {
            if(body.length() > 0)
                connection.setFixedLengthStreamingMode(body.length());
        }
        fillHeaders();

        PostRequestThread post_thread = new PostRequestThread();
        post_thread.start();
        post_thread.join();
        int responseCode = post_thread.getResponseCode();
        responseBody = post_thread.getResponse();
        response_headers = post_thread.getResponseHeaders();

        if(responseCode == HttpURLConnection.HTTP_UNAUTHORIZED && !auth) {
            responseCode = Authetentificate();
        }

        multipartItems.clear();

        return responseCode;
    }

    class RhoHostVerifier implements HostnameVerifier {
        @Override
        public  boolean verify(String hostname, SSLSession session) {
            return true;
        }
    }

    private HttpURLConnection getConnection(URL u) throws java.io.IOException {
        try {
            HttpURLConnection conn = null;
            if (u.getProtocol().toLowerCase().matches("https")) {
                conn = (HttpsURLConnection) u.openConnection();
                ((HttpsURLConnection) conn).setSSLSocketFactory(SSLImpl.getFactory(sslVerify));
                ((HttpsURLConnection) conn).setHostnameVerifier(new RhoHostVerifier());
            }
            else {
                conn = (HttpURLConnection) u.openConnection();
            }
            return conn;
        }
        catch (GeneralSecurityException e) {
            Logger.E( TAG,  e.getClass().getSimpleName() + ": " + e.getMessage() );
            return null;
        }
    }

    private int getData(boolean auth) throws java.io.IOException, java.lang.InterruptedException {
        URL _url = new URL(url);
        connection = getConnection(_url);
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

        if(responseCode == HttpURLConnection.HTTP_UNAUTHORIZED && !auth) {
            responseCode = Authetentificate();
        }

        if(fd > 0) {
            //new FileOutputStream(new File() )
        }

        return responseCode;
    }


} 