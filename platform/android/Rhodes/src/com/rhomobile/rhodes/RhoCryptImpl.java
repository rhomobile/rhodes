package com.rhomobile.rhodes;

import java.security.SecureRandom;
import java.security.spec.AlgorithmParameterSpec;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.nio.ByteBuffer;

//import android.util.Base64;
import android.content.SharedPreferences;
import android.app.Activity;
import com.rhomobile.rhodes.Logger;

public class RhoCryptImpl 
{
    private static final String TAG = "RhoCryptJava";

	byte[] m_dbKeyData;
	String m_strDBPartition;
	final String m_strPrefName = "rho_internal_data";
	Cipher m_encryptCipher, m_decryptCipher;
	final int m_nKeyLenBit = 128;  // 192 and 256 bits may not be available
	
	private void readKeyFromStorage()
	{
    	SharedPreferences settings = RhodesActivity.getInstance().getSharedPreferences( m_strPrefName, Activity.MODE_PRIVATE);
    	String strOldKey = settings.getString(m_strDBPartition, "");
    	if ( strOldKey != null && strOldKey.length() > 0 )
    	{
    		m_dbKeyData = Base64.decode(strOldKey);//, Base64.DEFAULT);
    	}
	}

	private void generateNewKey()throws Exception
	{
	    KeyGenerator kgen = KeyGenerator.getInstance("AES");  
	    SecureRandom sr = SecureRandom.getInstance("SHA1PRNG");  
	    kgen.init(m_nKeyLenBit, sr);  
	    SecretKey skey = kgen.generateKey();  
	    m_dbKeyData = skey.getEncoded();
	}

	private void writeKeyToStorage()
	{
		SharedPreferences settings = RhodesActivity.getInstance().getSharedPreferences(m_strPrefName, Activity.MODE_PRIVATE);		
    	SharedPreferences.Editor editor = settings.edit();
    	String strKey = Base64.encodeToString(m_dbKeyData, false);//, Base64.DEFAULT);
    	editor.putString(m_strDBPartition, strKey );
    	editor.commit();
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
	    if ( m_dbKeyData != null )
	        return;

	    initKey(szPartition);
	    
		byte[] iv = new byte[]
     	{
     	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00
     	};
	    
		AlgorithmParameterSpec paramSpec = new IvParameterSpec(iv);
    	
		SecretKeySpec skeySpec = new SecretKeySpec(m_dbKeyData, "AES");  
		m_encryptCipher = Cipher.getInstance("AES/CBC/NoPadding");  
		m_encryptCipher.init(Cipher.ENCRYPT_MODE, skeySpec, paramSpec);

		m_decryptCipher = Cipher.getInstance("AES/CBC/NoPadding");  
		m_decryptCipher.init(Cipher.DECRYPT_MODE, skeySpec, paramSpec);
		
	}
	
	private static void reportFail(String name, Exception e) 
	{
	    Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getClass().getSimpleName() + ": " + e.getMessage());
	}
	
    public boolean db_encrypt( String szPartition, ByteBuffer dataIn, ByteBuffer dataOut )
    {
    	try
    	{
	    	initContext(szPartition);

            dataOut.rewind();
	    	m_encryptCipher.doFinal(dataIn, dataOut);
	    	
	    	return true;
    	}catch(Exception exc)
    	{
    		reportFail("db_encrypt", exc);
    		return false;
    	}
    }
    
    public boolean db_decrypt( String szPartition, ByteBuffer dataIn, ByteBuffer dataOut )
    {
    	try
    	{
	    	initContext(szPartition);

            dataOut.rewind();
	    	m_decryptCipher.doFinal(dataIn, dataOut);
	    	
	    	return true;
		}catch(Exception exc)
		{
			reportFail("db_encrypt", exc);
			return false;
		}
    }
    
    public boolean set_db_CryptKey( String szPartition, String szKey, boolean bPersistent )
    {
	    m_strDBPartition = szPartition;
	    byte[] keyData = Base64.decode(szKey);//, Base64.DEFAULT);
	    
	    if ( keyData.length != (m_nKeyLenBit/8))
	    {
	    	reportFail("set_db_CryptKey", new Exception("Incorrect key size : " + keyData.length + "; Should be: " + (int)(m_nKeyLenBit/8)));
	    	return false;
	    }
	    m_dbKeyData = keyData;

		if ( bPersistent )
			writeKeyToStorage();
	    
    	return true;
    }

}
