package com.rhomobile.rhodes;

import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.mainview.SimpleMainView;
import android.webkit.WebView;
import java.security.SecureRandom;
import java.security.spec.AlgorithmParameterSpec;
import java.util.Formatter;

public class SecurityTokenGenerator
{
    byte[] token;
    String currentAgent;
    SecureRandom random;
    String nameParam = "RHO-SECURE-TOKEN";
    static SecurityTokenGenerator instance = null;

    public SecurityTokenGenerator()
    {
        currentAgent = ((WebView) RhodesActivity.safeGetInstance().getMainView().getWebView(0).getView()).getSettings().getUserAgentString();
        token = new byte[32];
        random = new SecureRandom();
        random.nextBytes(token);
    }

    public static SecurityTokenGenerator getInstance()
    {
        if (instance == null) instance = new SecurityTokenGenerator();
        return instance;
    }

    public String getTokenString()
    {
        Formatter formatter = new Formatter();
        for (byte b : token) {
            formatter.format("%02x", b);
        }
        return formatter.toString();
    }

    public String getSecureAgent()
    {
        return currentAgent + " " + nameParam + "=" + getTokenString();
    }

    public String getCurrentUserAgent()
    {
        return currentAgent;
    }

    public byte[] getToken()
    {
        return token;
    }

}