package com.rho.rubyext;

import com.google.zxing.BinaryBitmap;
import com.google.zxing.DecodeHintType;
import com.google.zxing.LuminanceSource;
import com.google.zxing.MultiFormatReader;
import com.google.zxing.Reader;
import com.google.zxing.ReaderException;
import com.google.zxing.Result;
import com.google.zxing.common.GlobalHistogramBinarizer;
import com.google.zxing.client.j2me.LCDUIImageLuminanceSource;
import com.rho.RhodesApp;
import com.rho.file.SimpleFile;
import com.rho.net.IHttpConnection;
import com.rho.net.RhoConnection;
import com.rho.net.URI;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;
import javax.microedition.lcdui.Image;
import java.io.IOException;
import java.io.InputStream;
import java.util.Hashtable;

import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhoRuby;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.*;

public class BarcodeRecognizer extends RubyOneArgMethod implements Runnable {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("BarcodeRecognizer");

	public static RubyClass BarcodeClass;    

	
	public String doBarcodeRecognize( String filename ) {
	      InputStream is = null;
	      Image capturedImage = null;
	      if (filename == null) {
	    	  LOG.ERROR("filename is null !");
	    	  return "";
	      }
 	      //try {

 	    	  is = RhoRuby.loadFile(filename);
 	    	  if ( is != null )
 	    	  {
	 	    	   try{ 
		 	    	   capturedImage = Image.createImage(is);
	 	    		   if ( is != null ) 
	 	    			   is.close(); 
	 	    	   }
	 	    	   catch(java.io.IOException exc){
	 	    		  LOG.ERROR("IO Exception during close image file stream ["+filename+"] !",exc);
	 	    	   }
 	    	  }
 	    	  else {
 
				 SimpleFile oFile = null;
				 try{
					 oFile = RhoClassFactory.createFile();
					 oFile.open( filename, true, false);
				 
					 if ( oFile.isOpened() )
					 {
						 is = oFile.getInputStream();
						 capturedImage = Image.createImage(is);
					 }
				 }catch(Exception exc){
	 	    		  LOG.ERROR("Exception during read image from file ["+filename+"] !",exc);
				 }finally
				 {
					 if ( oFile != null ) {
						 try{
							 oFile.close(); 
						 }catch(IOException exc2){
			 	    		  LOG.ERROR("IO Exception during close image file stream 2 ["+filename+"] !",exc2);
						 }
					 }
				 }
 	    	  }  
 	    	  
 	    	/*  
			if ( filename.startsWith("/apps")) {
		    	URI uri = new URI("file://"+filename);
		    	RhoConnection rho_connection = new RhoConnection(uri);
		    	rho_connection.setRequestMethod(IHttpConnection.GET);
		    	is = rho_connection.openInputStream(); 
			}
			else {
				FileConnection file = null;
				String fixed_name = RhodesApp.getInstance().resolveDBFilesPath(filename);
		    	file = (FileConnection) Connector.open(fixed_name, Connector.READ);
		        is = file.openInputStream();
			}
	        capturedImage = Image.createImage(is);
	        */
	      /*	
	      } catch (IOException e) {
	    	// error while create image from file
	    	LOG.ERROR("IO Exception during open image file ["+filename+"] !",e);  
	        return "";
	      } finally {
	        try {
	          if (is != null) {
	            is.close();
	          }
	        } catch (IOException ioe) {
	        	// error while closing file
	        	LOG.ERROR("IO Exception during close image file stream ["+filename+"] !",ioe);
	        }
	      }
	      */

	      if (capturedImage != null) {
	    	// we have image
	        LuminanceSource source = new LCDUIImageLuminanceSource(capturedImage);
	        BinaryBitmap bitmap = new BinaryBitmap(new GlobalHistogramBinarizer(source));
	        Result result;
	        try {
				  Reader reader = new MultiFormatReader();
				  Hashtable readerHints = new Hashtable(1);
				  readerHints.put(DecodeHintType.TRY_HARDER, Boolean.TRUE);
				  result = reader.decode(bitmap, readerHints);
	        } catch (ReaderException e) {
	          // image not decoded - not found any barcodes	
	          LOG.INFO("Barcode not found in image ["+filename+"] !");	
	          return "";
	        }
	        if (result != null) {
	          String resultText = result.getText();
	          // return recognized string !
	          LOG.INFO("Barcode successful recoginzed in image ["+filename+"], Barcode = ["+resultText+"] !");
	          return resultText;
	        }
	      }
	    LOG.ERROR("Unrecognized Error during process file ["+filename+"] !");	      
		return "";
	}
	


	public void run() {
   	  // register Ruby class
          BarcodeClass = RubyAPI.defineClass("Barcode", RubyRuntime.ObjectClass);        
	  // register Ruby method
          BarcodeClass.getSingletonClass().defineMethod("barcode_recognize", this);
		
	}
	
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) 
	{
		try {
			if ( arg0 instanceof RubyString )
			{
				String filename = arg0.toString();
				String result = doBarcodeRecognize(filename);
				return ObjectFactory.createString(result);
			}
			else {
				  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in Barcode.barcode_recognize: wrong argument type.Should be String");
			}
		} catch(Exception e) {
			e.printStackTrace();
			LOG.ERROR("barcode_recognize failed with exception", e);
			throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		}
		//return RubyConstant.QNIL;
	}	

}
