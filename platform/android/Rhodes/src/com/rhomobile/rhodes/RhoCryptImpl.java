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

import java.security.SecureRandom;
import java.security.spec.AlgorithmParameterSpec;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.nio.ByteBuffer;

import android.content.Context;
import android.content.SharedPreferences;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.ContextFactory;

public class RhoCryptImpl 
{
    private static final String TAG = "RhoCryptJava";
    private static final boolean DEBUG = false;

	byte[] m_dbKeyData;
	String m_strDBPartition;
	final String m_strPrefName = "rho_internal_data";
	Cipher m_encryptCipher, m_decryptCipher;
	int m_nKeyLenBit = 256;  // 192 and 256 bits may not be available

    private void readKeyFromStorage()
    {
        SharedPreferences settings = ContextFactory.getContext().getSharedPreferences( m_strPrefName, Context.MODE_PRIVATE);
        String strOldKey = settings.getString(m_strDBPartition, "");
        if (strOldKey != null && strOldKey.length() > 0)
        {
            
            m_dbKeyData = Base64.decode(strOldKey);//, Base64.DEFAULT);
            m_nKeyLenBit = m_dbKeyData.length * 8;
            Logger.I(TAG, "Key is successfully read from Shared Preferences");
            if(DEBUG) {
                Logger.I(TAG, "Key: " + dumpKey(m_dbKeyData));
                Logger.I(TAG, "Encoded key: " + strOldKey);
            }
        } else {
            Logger.I(TAG, "No key is found in Shared Preferences");
        }
    }

    private static String dumpKey(byte[] key) {
        StringBuffer buffer = new StringBuffer();
        for(byte b:key) {
            buffer.append(String.format("%x ", b));
        }
        return buffer.toString();
    }

	private void generateNewKey()throws Exception
	{
	    KeyGenerator kgen = KeyGenerator.getInstance("AES");  
	    SecureRandom sr = SecureRandom.getInstance("SHA1PRNG");  
	    kgen.init(m_nKeyLenBit, sr);  
	    SecretKey skey = kgen.generateKey();  
	    m_dbKeyData = skey.getEncoded();
	    Logger.I(TAG, "New key is generated");
	    if(DEBUG)
	        Logger.I(TAG, "Key: " + dumpKey(m_dbKeyData));
	}

    private void writeKeyToStorage()
    {
        SharedPreferences settings =  ContextFactory.getContext().getSharedPreferences(m_strPrefName, Context.MODE_PRIVATE);		
        SharedPreferences.Editor editor = settings.edit();
        String strKey = Base64.encodeToString(m_dbKeyData, false);//, Base64.DEFAULT);
        
        editor.putString(m_strDBPartition, strKey );
        if (editor.commit()) {
            Logger.I(TAG, "Key is successfully saved to Shared Preferences");
        } else {
            Logger.E(TAG, "Write key to Shared Preferences is failed");
        }

        if (DEBUG) {
            Logger.I(TAG, "Key: " + dumpKey(m_dbKeyData));
            Logger.I(TAG, "Encoded key: " + strKey);
        }
	}

	private void initKey(String szPartition)throws Exception
	{
	    if ( m_dbKeyData != null )
	        return;

	    m_strDBPartition = szPartition;
		
		readKeyFromStorage();
	    if ( m_dbKeyData != null )
	        return;

		generateNewKey();
		writeKeyToStorage();
	}
	
	private void initContext(String szPartition)throws Exception
	{
	    if ( m_dbKeyData != null ) {
	        Logger.T(TAG, "RhoCrypt context already initialized, partition: " + szPartition);
	        return;
	    }

	    initKey(szPartition);
	    
		byte[] iv = new byte[]
     	{
     	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
     	};
	    
		AlgorithmParameterSpec paramSpec = new IvParameterSpec(iv);
    	
		SecretKeySpec skeySpec = new SecretKeySpec(m_dbKeyData, "AES");  
		m_encryptCipher = Cipher.getInstance("AES/CBC/NoPadding");  
		m_encryptCipher.init(Cipher.ENCRYPT_MODE, skeySpec, paramSpec);

		m_decryptCipher = Cipher.getInstance("AES/CBC/NoPadding");  
		m_decryptCipher.init(Cipher.DECRYPT_MODE, skeySpec, paramSpec);
		
        Logger.T(TAG, "RhoCrypt context initialized for partition: " + szPartition);
    }

    public boolean db_encrypt( String szPartition, ByteBuffer dataIn, ByteBuffer dataOut )
    {
        try
        {
            initContext(szPartition);

            dataOut.rewind();

            if (m_encryptCipher == null)
                throw new NullPointerException("m_encryptCipher == null");

            /* *********************************************************
             * Work around Android 4 bug:
             * http://code.google.com/p/android/issues/detail?id=24327 
             */
            ByteBuffer input = ByteBuffer.allocate(dataIn.capacity());
            input.put(dataIn);
            input.rewind();
            /* ******************************************************* */

            m_encryptCipher.doFinal(input, dataOut);

            return true;
        }catch(Exception exc)
        {
            Logger.E(TAG, exc);
            return false;
        }
    }

    public boolean db_decrypt( String szPartition, ByteBuffer dataIn, ByteBuffer dataOut )
    {
        try
        {
            initContext(szPartition);
            
            dataOut.rewind();

            if (m_decryptCipher == null)
                throw new NullPointerException("m_decryptCipher == null");

            /* *********************************************************
             * Work around Android 4 bug:
             * http://code.google.com/p/android/issues/detail?id=24327 
             */
            ByteBuffer input = ByteBuffer.allocate(dataIn.capacity());
            input.put(dataIn);
            input.rewind();
            /* ******************************************************* */

            m_decryptCipher.doFinal(input, dataOut);

            return true;
		}catch(Exception exc)
		{
			Logger.E(TAG, exc);
			return false;
		}
    }
    
    public boolean set_db_CryptKey( String szPartition, String szKey, boolean bPersistent )
    {
	    m_strDBPartition = szPartition;
	    byte[] keyData = Base64.decode(szKey);//, Base64.DEFAULT);
	    
	    if ( keyData.length != (m_nKeyLenBit/8))
	    {
	    	Logger.E(TAG, "Incorrect key size : " + keyData.length + "; Should be: " + (int)(m_nKeyLenBit/8));
	    	return false;
	    }
	    m_dbKeyData = keyData;

		if ( bPersistent )
			writeKeyToStorage();
	    
    	return true;
    }

}
