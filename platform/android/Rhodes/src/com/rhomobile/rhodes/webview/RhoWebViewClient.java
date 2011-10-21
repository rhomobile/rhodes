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

package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;

import android.graphics.Bitmap;
import android.net.http.SslError;
import android.view.Window;
import android.webkit.SslErrorHandler;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class RhoWebViewClient extends WebViewClient
{
    private final String TAG = RhoWebViewClient.class.getSimpleName();

    @Override
    public boolean shouldOverrideUrlLoading(WebView view, String url) {
        Logger.I(TAG, "Loading URL: " + url);
        return RhodesService.getInstance().handleUrlLoading(url);
    }

    @Override
    public void onPageStarted(WebView view, String url, Bitmap favicon) {
        if (RhodesActivity.ENABLE_LOADING_INDICATION)
            RhodesActivity.safeGetInstance().getWindow().setFeatureInt(Window.FEATURE_PROGRESS, 0);
        super.onPageStarted(view, url, favicon);
    }

    @Override
    public void onPageFinished(WebView view, String url) {
        // Set title
        String title = view.getTitle();
        RhodesActivity.safeGetInstance().setTitle(title);
        if (RhodesActivity.ENABLE_LOADING_INDICATION)
            RhodesActivity.safeGetInstance().getWindow().setFeatureInt(
                    Window.FEATURE_PROGRESS, RhodesActivity.MAX_PROGRESS);

        super.onPageFinished(view, url);
    }

    public void onReceivedError(android.webkit.WebView view, int errorCode, java.lang.String description, java.lang.String failingUrl)
    {
        StringBuilder msg = new StringBuilder(failingUrl);
        msg.append(" failed: ");
        msg.append(errorCode);
        msg.append(" - " + description);
        Logger.E(TAG, msg.toString());
    }

    @Override
    public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {

        if(RhoConf.getBool("no_ssl_verify_peer")) {
            Logger.D(TAG, "Skip SSL error.");
            handler.proceed();
        } else {
            StringBuilder msg = new StringBuilder();
            msg.append("SSL error - ");
            switch(error.getPrimaryError()) {
            case SslError.SSL_NOTYETVALID:
                msg.append("The certificate is not yet valid: ");
                break;
            case SslError.SSL_EXPIRED:
                msg.append("The certificate has expired: ");
                break;
            case SslError.SSL_IDMISMATCH:
                msg.append("Hostname mismatch: ");
                break;
            case SslError.SSL_UNTRUSTED:
                msg.append("The certificate authority is not trusted: ");
                break;
            }
            msg.append(error.getCertificate().toString());
            Logger.W(TAG, msg.toString());
            handler.cancel();
        }
    }
}
