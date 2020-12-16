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

    private static final char[] HEXADECIMAL = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
            'e', 'f'
    };

    private class JCMultipartItem
    {
        public String m_strFilePath;
        public String m_strBody;
        public String m_strName, m_strFileName, m_strContentType;
        public String m_strDataPrefix;
        public JCMultipartItem() {}
    }

    private static class AuthSettings {
        private static final int QOP_MISSING = 0;
        private static final int QOP_AUTH_INT = 1;
        private static final int QOP_AUTH = 2;

        public static int qopVariant = QOP_MISSING;
        public static String method = null;
        public static String nonce = null;
        public static String opaque = null;
        public static String algo = null;
        public static String qop = null;
        public static String realm = null;
        public static String cnonce = null;
        public static String charset = null;
        public static String uri = null;
        public static String nc = "00000001";
        public static String serverResponse = null;

        public static String user = null;
        public static String pwd = null;
        public static String authHeader = null;

        public AuthSettings() {}
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


    public void SetAuthSettings(String u, String p, boolean is_d) {
        AuthSettings.user = u;
        AuthSettings.pwd = p;
        //is_digest = is_d;
    }

    public NetRequest()
    {
    }

    private void SetAuthentificationHeader(URL u) {

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

        if(AuthSettings.authHeader != null)
            connection.setRequestProperty("Authorization", AuthSettings.authHeader);
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

    /**
     * Encodes the 128 bit (16 bytes) MD5 digest into a 32 characters long
     * <CODE>String</CODE> according to RFC 2617.
     *
     * @param binaryData array containing the digest
     * @return encoded MD5, or <CODE>null</CODE> if encoding failed
     */
    private static String encode(byte[] binaryData) {
        if (binaryData.length != 16) {
            return null;
        }
        char[] buffer = new char[32];
        for (int i = 0; i < 16; i++) {
            int low = (binaryData[i] & 0x0f);
            int high = ((binaryData[i] & 0xf0) >> 4);
            buffer[i * 2] = HEXADECIMAL[high];
            buffer[(i * 2) + 1] = HEXADECIMAL[low];
        }
        return new String(buffer);
    }

    public static byte[] getAsciiBytes(final String data) {
        if (data == null) {
            throw new IllegalArgumentException("Parameter may not be null");
        }

        return data.getBytes(StandardCharsets.US_ASCII);
    }


    public static String calculateNonce() throws NoSuchAlgorithmException, UnsupportedEncodingException {
        String date = Long.toString(System.currentTimeMillis());

        MessageDigest hash = MessageDigest.getInstance("MD5");
        byte[] cnonce = hash.digest(date.getBytes("UTF-8"));
        return encode(cnonce);
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

    private String getQopVariantString() {
        String qopOption;
        if (AuthSettings.qopVariant == AuthSettings.QOP_AUTH_INT) {
            qopOption = "auth-int";
        } else {
            qopOption = "auth";
        }
        return qopOption;
    }


    private String CreateDigest() throws NoSuchAlgorithmException, UnsupportedEncodingException, IllegalStateException {
        String uri = AuthSettings.uri;
        String realm = AuthSettings.realm;
        String nonce = AuthSettings.nonce;
        String method = AuthSettings.method;
        String algorithm = AuthSettings.algo;
        if (uri == null) {
            throw new IllegalStateException("URI may not be null");
        }
        if (realm == null) {
            throw new IllegalStateException("Realm may not be null");
        }
        if (nonce == null) {
            throw new IllegalStateException("Nonce may not be null");
        }

        // If an algorithm is not specified, default to MD5.
        if (algorithm == null) {
            algorithm = "MD5";
        }
        // If an charset is not specified, default to ISO-8859-1.
        String charset = AuthSettings.charset;
        if (charset == null) {
            charset = "ISO-8859-1";
        }

        if (AuthSettings.qopVariant == AuthSettings.QOP_AUTH_INT) {
            throw new IllegalStateException("Unsupported qop in HTTP Digest authentication");
        }

        MessageDigest md5Helper =  MessageDigest.getInstance("MD5");
        String uname = AuthSettings.user;
        String pwd = AuthSettings.pwd;

        StringBuilder tmp = new StringBuilder(uname.length() + realm.length() + pwd.length() + 2);
        tmp.append(uname);
        tmp.append(':');
        tmp.append(realm);
        tmp.append(':');
        tmp.append(pwd);

        String a1 = tmp.toString();

        if(algorithm.equalsIgnoreCase("MD5-sess")) { // android-changed: ignore case
            // H( unq(username-value) ":" unq(realm-value) ":" passwd )
            //      ":" unq(nonce-value)
            //      ":" unq(cnonce-value)
            String cnonce = AuthSettings.cnonce;

            String tmp2 = encode(md5Helper.digest(a1.getBytes(charset)));
            StringBuilder tmp3 = new StringBuilder(tmp2.length() + nonce.length() + cnonce.length() + 2);
            tmp3.append(tmp2);
            tmp3.append(':');
            tmp3.append(nonce);
            tmp3.append(':');
            tmp3.append(cnonce);
            a1 = tmp3.toString();
        } else if (!algorithm.equalsIgnoreCase("MD5")) { // android-changed: ignore case
            throw new IllegalStateException("Unhandled algorithm " + algorithm + " requested");
        }

        String md5a1 = encode(md5Helper.digest(a1.getBytes(charset)));
        String a2 = null;
        if (AuthSettings.qopVariant == AuthSettings.QOP_AUTH_INT) {
            // Unhandled qop auth-int
            //we do not have access to the entity-body or its hash
            //TODO: add Method ":" digest-uri-value ":" H(entity-body)
        } else {
            a2 = method + ':' + uri;
        }

        String md5a2 = encode(md5Helper.digest(getAsciiBytes(a2)));

        String serverDigestValue;
        if (AuthSettings.qopVariant == AuthSettings.QOP_MISSING) {
            StringBuilder tmp2 = new StringBuilder(md5a1.length() + nonce.length() + md5a2.length());
            tmp2.append(md5a1);
            tmp2.append(':');
            tmp2.append(nonce);
            tmp2.append(':');
            tmp2.append(md5a2);
            serverDigestValue = tmp2.toString();
        } else {
            String qopOption = getQopVariantString();
            String cnonce = AuthSettings.cnonce;

            StringBuilder tmp2 = new StringBuilder(md5a1.length() + nonce.length()
                    + AuthSettings.nc.length() + cnonce.length() + qopOption.length() + md5a2.length() + 5);
            tmp2.append(md5a1);
            tmp2.append(':');
            tmp2.append(nonce);
            tmp2.append(':');
            tmp2.append(AuthSettings.nc);
            tmp2.append(':');
            tmp2.append(cnonce);
            tmp2.append(':');
            tmp2.append(qopOption);
            tmp2.append(':');
            tmp2.append(md5a2);
            serverDigestValue = tmp2.toString();
        }

        String serverDigest =
                encode(md5Helper.digest(getAsciiBytes(serverDigestValue)));
        return serverDigest;

    }

    private void AuthInit(URL _url, HashMap<String, String> values) throws NoSuchAlgorithmException, UnsupportedEncodingException, IllegalStateException  {
        AuthSettings.method = connection.getRequestMethod();
        AuthSettings.nonce = values.get("nonce");
        AuthSettings.opaque = values.get("opaque");
        AuthSettings.algo = values.get("algorithm");
        AuthSettings.qop = values.get("qop");
        AuthSettings.realm = values.get("realm");
        AuthSettings.cnonce =  calculateNonce();;
        AuthSettings.uri = _url.getPath();
        AuthSettings.charset = values.get("charset");

        boolean unsupportedQop = false;
        if (AuthSettings.qop != null) {
            StringTokenizer tok = new StringTokenizer(AuthSettings.qop,",");
            while (tok.hasMoreTokens()) {
                String variant = tok.nextToken().trim();
                if (variant.equals("auth")) {
                    AuthSettings.qopVariant = AuthSettings.QOP_AUTH;
                    break; //that's our favourite, because auth-int is unsupported
                } else if (variant.equals("auth-int")) {
                    AuthSettings.qopVariant = AuthSettings.QOP_AUTH_INT;
                } else {
                    unsupportedQop = true;
                }
            }
        }

        if (unsupportedQop && (AuthSettings.qopVariant == AuthSettings.QOP_MISSING)) {
            throw new IllegalStateException("None of the qop methods is supported");
        }

        if (AuthSettings.realm == null) {
            throw new IllegalStateException("missing realm in realm");
        }

        if (AuthSettings.nonce == null) {
            throw new IllegalStateException("missing nonce in realm");
        }
    }

    private int Authetentificate() throws java.io.IOException, java.lang.InterruptedException {

        List<String> headers = response_headers.get("WWW-Authenticate");
        HashMap<String, String> values = null;
        if(!headers.isEmpty())
            values = parseHeader(headers.get(0));
        else
            return 0;

        try {

            URL _url = new URL(url);
            AuthInit(_url, values);
            AuthSettings.serverResponse = CreateDigest();
        }
        catch (NoSuchAlgorithmException e) {
            Logger.E( TAG,  e.getClass().getSimpleName() + ": " + e.getMessage() );
            return 0;
        }
        catch (IllegalStateException e) {
            Logger.E( TAG,  e.getClass().getSimpleName() + ": " + e.getMessage() );
            return 0;
        }


        AuthSettings.authHeader = String.format("Digest username=\"%s\", realm=\"%s\", " +
                "nonce=\"%s\", uri=\"%s\", qop=auth, nc=%s, cnonce=\"%s\", " +
                "response=\"%s\"",
                AuthSettings.user, AuthSettings.realm, AuthSettings.nonce, AuthSettings.uri, AuthSettings.nc, AuthSettings.cnonce, AuthSettings.serverResponse);

        if(AuthSettings.opaque != null)
            AuthSettings.authHeader += String.format(", opaque=\"%s\"", AuthSettings.opaque);
        if(AuthSettings.algo != null)
            AuthSettings.authHeader += String.format(", algorithm=\"%s\"", AuthSettings.algo);

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
        connection.setConnectTimeout(30000);
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
        connection.setReadTimeout(30000);
        connection.setConnectTimeout(30000);
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