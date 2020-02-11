package com.rho.barcode;

import java.io.InputStream;
import java.util.Hashtable;
import java.util.List;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.google.zxing.BinaryBitmap;
import com.google.zxing.DecodeHintType;
import com.google.zxing.LuminanceSource;
import com.google.zxing.MultiFormatReader;
import com.google.zxing.Reader;
import com.google.zxing.ReaderException;
import com.google.zxing.Result;
import com.google.zxing.common.GlobalHistogramBinarizer;
import com.rho.barcode.BarcodeSingletonBase;
import com.rho.barcode.IBarcodeSingleton;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rho.barcode.InternalAndroidBarcodeScanner;

import com.google.android.gms.vision.MultiProcessor;
import com.google.android.gms.vision.barcode.Barcode;
import com.google.android.gms.vision.barcode.BarcodeDetector;
import com.google.android.gms.vision.Frame;
import android.util.SparseArray;
import com.rhomobile.rhodes.util.ContextFactory;

/**
 * Barcode Singleton class
 * @author Ben Kennedy NCVT73
 */
public class BarcodeSingleton extends BarcodeSingletonBase implements IBarcodeSingleton
{
	private static String LOGTAG = "BarcodeSingleton";

	private static BarcodeFactory factory;
	
	public BarcodeSingleton(BarcodeFactory factory)
	{
		super(factory);
		BarcodeSingleton.factory = factory; 
		Logger.D(LOGTAG, "BarcodeSingleton Constructor");
	}

	public List<String> getIDs()
	{
		Logger.D(LOGTAG, "BarcodeSingleton getIDs");
		return factory.getIds();
	}

	@Override
	protected String getInitialDefaultID()
	{
		Logger.I(LOGTAG, "BarcodeSingleton getInitialDefaultID");
		int defaultScanner = factory.getDefaultEMDKScanner();
		
		String defaultStr=null;
		
		if(defaultScanner >= 0)
		{
			return getIDs().get(defaultScanner);
		}
		
		try
		{
			Logger.D(LOGTAG, "BarcodeSingleton get Default Scanner");
			defaultStr=getIDs().get(0);
			Logger.I(LOGTAG, "BarcodeSingleton-Default scanner found");
		}
		catch(Exception ex)
		{
			Logger.W(LOGTAG,"BarcodeSingleton-No default Scanner found.Please make sure usedwforscanning value is not set to 1--"+ex.getMessage());
			defaultStr=null;
		}
		return defaultStr;
	}

	@SuppressWarnings("unchecked")
	@Override
	public void enumerate(IMethodResult res)
	{
		Logger.D(LOGTAG, "BarcodeSingleton enumerate");
		List<String> barcodes = factory.enumerate();
		res.set((List<Object>)(List<?>) barcodes);
	}

	/**
	 * Recognises a barcode from an image by using the ZXing library
	 * @param imageFilePath The path of the file to be recognised
	 */
	public static void barcode_recognize(String imageFilePath, IMethodResult result)
	{
		Logger.D(LOGTAG, "BarcodeSingleton recognise");
		try
		{
			InputStream istream = RhoFileApi.open(imageFilePath);
			Bitmap image = BitmapFactory.decodeStream(istream);
			RhoFileApi.close(istream);
			if (image != null)
			{

				// we have image
				if (EngineChooser.usingGoogleEngine){



					BarcodeDetector barcodeDetector = new BarcodeDetector.Builder(ContextFactory.getAppContext())
					.setBarcodeFormats(Barcode.ALL_FORMATS).build();
					if (barcodeDetector.isOperational())
		            {
		                Frame frame = new Frame.Builder().setBitmap(image).build();
		                SparseArray<Barcode> barcodes = barcodeDetector.detect(frame);
		                barcodeDetector.release();

		                if (barcodes.size() > 0) {
   							String resultText = barcodes.valueAt(0).toString();
							Logger.I(LOGTAG, "Barcode is successfully recognized: " + resultText + ", file: " + imageFilePath);
							result.set(resultText);
							return;
		                }
		                else
		                {
							Logger.E(LOGTAG, "Error decoding image file: " + imageFilePath);
							result.setError("Could not find a barcode within the specified file");
							return;
		                }
		            }
		            else
		            {
		                Logger.E(LOGTAG, "Error decoding image file: " + imageFilePath);
						result.setError("Native barcode detector is not operational");
						return;
		            }
		            

				}else{
				
					LuminanceSource source = new RhoLuminanceSource(image);
					BinaryBitmap bitmap = new BinaryBitmap(new GlobalHistogramBinarizer(source));
					Result decodeResult;
					try
					{
						Reader reader = new MultiFormatReader();
						Hashtable<DecodeHintType, Object> hints = new Hashtable<DecodeHintType, Object>(1);
						hints.put(DecodeHintType.TRY_HARDER, Boolean.TRUE);
						decodeResult = reader.decode(bitmap, hints);
						if (decodeResult != null)
						{
							String resultText = decodeResult.getText();
							Logger.I(LOGTAG, "Barcode is successfully recognized: " + resultText + ", file: " + imageFilePath);
							result.set(resultText);
							return;
						}
						else
						{
							//Unknown error. This shouldnt happen as the decode call should Exception or return a string.
							Logger.E(LOGTAG, "Error decoding image file: " + imageFilePath);
							result.setError("Could not find a barcode within the specified file");
							return;
						}
					}
					catch (ReaderException e)
					{
						// image not decoded - not found any barcodes
						Logger.E(LOGTAG, "Error decoding image file: " + imageFilePath);
						result.setError("Could not find a barcode within the specified file");
						return;
					}
				}





			}
			else
			{
				Logger.E(LOGTAG, "An image could not be found at the specified file path: " + imageFilePath);
				result.setError("An image could not be found at the specified file path");
				return;
			}
		}
		catch (Throwable e)
		{
			Logger.E(LOGTAG, e);
		}
		Logger.E(LOGTAG, "Error during processing file: " + imageFilePath);
		return;
	}
}
