package com.rho.signature;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.URISyntaxException;
import java.util.Arrays;

import android.graphics.Bitmap;

import com.rhomobile.rhodes.Logger;

/**
 * Provides a set of functions to convert a 32 bit ARGB8888 bitmap into a single bit per pixel bitmap.
 * The original bitmap is passed into the contructor for this class.
 * @author Barry Piper, Ben Kennedy (NCVT73)
 */
public class FormatBitmap
{
	private static final String LOGTAG = "MotoSignatureBitmap";
	private static final int BITMAPINFOHEADER_SIZE = 40;
	private static final int BITMAPFILEHEADER_SIZE = 14;
	private static final int BITMAPCOLORINFO_SIZE = 8;
	private static final int RESERVED = 0x0;
	private static final int iPlanes = 1;
	private static final int iBitCount = 1;
	private static final int iCompression = 0;
	private static final int iFileOffsetIntoPixelArray =  BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_SIZE + BITMAPCOLORINFO_SIZE;
	private static final int iXPelsPerMeter = 0x0;
	private static final int iYPelsPerMeter = 0x0;
	private static final int iClrUsed = 2;
	private static final int iClrImportant = 0; 
	private static final byte bfType [] = {'B', 'M'};

	/**
	 * intToWord converts an int to a word, where the return
	 * value is stored in a 2-byte array.
	 * 
	 * @author Barry Piper
	 * @param parValue
	 * @return a two byte array of type byte []
	 */
	private static byte [] intToWord (int parValue) 
	{
		byte retValue [] = new byte [2];
		retValue [0] = (byte) (parValue & 0x00FF);
		retValue [1] = (byte) ((parValue >>	8) & 0x00FF);
		return (retValue);
	}

	/**
	 * intToDWord converts an int to a double word, where the return
	 * value is stored in a 4-byte array.
	 * 
	 * @author Barry Piper
	 * @param parValue of type int
	 * @return a four byte array of type byte []
	 */
	private static byte [] intToDWord (int parValue) 
	{
		byte retValue [] = new byte [4];
		retValue [0] = (byte) (parValue & 0x00FF);
		retValue [1] = (byte) ((parValue >>	8) & 0x000000FF);
		retValue [2] = (byte) ((parValue >>	16) & 0x000000FF);
		retValue [3] = (byte) ((parValue >>	24) & 0x000000FF);
		return (retValue);
	}

	/**
	 * Writes bitmap file headers to the file
	 * @param fOut the file stream to write
	 * @param bitmap the bitmap to convert
	 * @param iBitmapFileSize the total size of the file (in bytes)
	 * @throws IOExceptioncan occurs if writing the file fails
	 * @author Barry Piper
	 */
	private static void writeBitmapFileHeader (FileOutputStream fOut, Bitmap bitmap, int iBitmapFileSize) throws IOException 
	{
		fOut.write (bfType);
		fOut.write (intToDWord (iBitmapFileSize));
		fOut.write (intToWord (RESERVED));
		fOut.write (intToWord (RESERVED));
		fOut.write (intToDWord (iFileOffsetIntoPixelArray));
	}

	/**
	 * Writes bitmap information headers to the file
	 * @param fOut the file stream to write
	 * @param bitmap the bitmap to convert
	 * @param iWidth the width of the image (in pixels)
	 * @param iHeight the height of the image (in pixels)
	 * @param iSizeImage the number of bytes for the pixel data
	 * @param iBackgroundColour the ARGB8888 formatted color of the background 
	 * @param iPenColour the ARGB8888 formatted color of the pen
	 * @throws IOException can occur if writing the file fails
	 * @author Barry Piper
	 */
	private static void writeBitmapInfoHeader (FileOutputStream fOut, Bitmap bitmap, int iWidth, int iHeight, int iSizeImage, int iBackgroundColour, int iPenColour) throws IOException 
	{
		fOut.write (intToDWord (BITMAPINFOHEADER_SIZE));
		fOut.write (intToDWord (iWidth));
		fOut.write (intToDWord (iHeight));
		fOut.write (intToWord (iPlanes));
		fOut.write (intToWord (iBitCount));
		fOut.write (intToDWord (iCompression));
		fOut.write (intToDWord (iSizeImage));
		fOut.write (intToDWord (iXPelsPerMeter));
		fOut.write (intToDWord (iYPelsPerMeter));
		fOut.write (intToDWord (iClrUsed));
		fOut.write (intToDWord (iClrImportant));
		fOut.write (intToDWord (iBackgroundColour)); // colour 1 aarrggbb
		fOut.write (intToDWord (iPenColour)); // colour 2 aarrggbb
	}

	/**
	 * Converts the bitmap into a bit array and then outputs it to a file output stream (via a byte array).
	 * @param fOut the File stream to which to save the image.
	 * @param bitmap The source image to convert
	 * @param canvasWidth The width of the image (in pixels)
	 * @param canvasHeight The height of the image (in pixels)
	 * @param iBackgroundColour the ARGB8888 formatted color of the background
	 * @throws IOException can occur if writing the file fails
	 * @author Ben Kennedy (NCVT73)
	 */
	private static void writeBitmapData (FileOutputStream fOut, Bitmap bitmap, int canvasWidth, int canvasHeight, int iBackgroundColour) throws IOException
	{
		int bitmapWidth = 0;
		if(canvasWidth % 32 != 0) bitmapWidth = 32 - (canvasWidth % 32); // if it is not divisible by 32
		bitmapWidth += canvasWidth;
		
		boolean[] booleanArray = new boolean[bitmapWidth * canvasHeight]; 
		Arrays.fill(booleanArray, false);  // blank out the array to 0's
		
		byte[] byteArray = new byte[booleanArray.length / 8];
		
		for(int y = 0; y < canvasHeight; y++)
		{
			for(int x = 0; x < canvasWidth; x++)
			{
				int pixel = bitmap.getPixel(x, y);
				if(pixel != iBackgroundColour )
				{
					booleanArray[(((canvasHeight - 1) - y) * bitmapWidth) + x] = true; // For some reason, the file wants the image rows in reverse order. Thats why ((canvasHeight - 1) - y) is there
				}
			}
		}
		
		//Convert the bit array into a byte array
		int currentByte = 0;
		int currentBit = 0;
		byte modifyingByte = 0;
		
		for(boolean bit: booleanArray)
		{
			modifyingByte = (byte) (modifyingByte << 1);
			
			if(bit) modifyingByte += 1;

			if(++currentBit == 8)
			{
				currentBit = 0;
				byteArray[currentByte] = modifyingByte;
				currentByte++;
				modifyingByte = 0;
			}
		}
		
		fOut.write(byteArray);
	}
	/**
	 * Converts a Bitmap object into a bmp formatted file.
	 * @param bitmap The source image to convert
	 * @param fileName The name (name + extension) to give the new file. The file will be saved in the Pictures gallery
	 * @param penColour the ARGB8888 formatted color of the pen
	 * @param backgroundColour the ARGB8888 formatted color of the background
	 * @return the String format URI of the written file
	 * @throws IOException If a IO exception occurs when writing the file
	 * @throws URISyntaxException If the file name contains illegal characters
	 * @author Ben Kennedy (NCVT73)
	 */
	public static String writeBitmapImage(Bitmap bitmap, String fileName, int penColour, int backgroundColour) throws IOException, URISyntaxException
	{
		//File outputFile = ImageCapture.getGalleryFile(fileName);
//		File outputFile = new File(fileName);
//		outputFile.mkdirs();
		FileOutputStream fOut = null;
		Exception throwing = null;
		
		try
		{
			fOut = ImageCapture.getOutputStream(fileName);
			writeBitmapImage(bitmap, fOut, penColour, backgroundColour);
			fOut.flush();
		}
		catch(IOException e)
		{
			Logger.W(LOGTAG, "Error in compressing signature image");
			throwing = e;	
		}
		finally
		{
			try{if (fOut != null) fOut.close();}catch(IOException e){/*Nothing we can do*/};
			if(throwing != null) throw new IOException(throwing); 
		}

		Logger.D(LOGTAG, "writeBitmapImage-: " + fileName);
		return fileName;
	}
	
	/**
	 * Converts a Bitmap object into a bmp formatted file.
	 * @param bitmap The source image to convert
	 * @param fOut the File stream to which to save the image.
	 * @param iPenColour the ARGB8888 formatted color of the pen
	 * @param iBackgroundColour the ARGB8888 formatted color of the background
	 * @throws IOException if a file exception occurs, this is thrown.
	 * @author Barry Piper, Ben Kennedy (Trying to fix the Exceptions)
	 */
	public static void writeBitmapImage(Bitmap bitmap, FileOutputStream fOut, int iPenColour, int iBackgroundColour) throws IOException
	{
		if (bitmap == null)
		{
			Logger.E(LOGTAG,"bitmap is null");
			return;
		}
		int iWidth = bitmap.getWidth();
		int iHeight = bitmap.getHeight();

		int iBytesPerLine = ((iWidth+ 31) & (~31)) >> 3; 
		int iSizeImage = iBytesPerLine * iHeight;
		int iBitmapFileSize = iSizeImage + BITMAPINFOHEADER_SIZE + BITMAPFILEHEADER_SIZE + BITMAPCOLORINFO_SIZE;

		writeBitmapFileHeader(fOut, bitmap, iBitmapFileSize);
		writeBitmapInfoHeader(fOut, bitmap, iWidth, iHeight, iSizeImage, iBackgroundColour, iPenColour);
		writeBitmapData(fOut, bitmap, iWidth, iHeight, iBackgroundColour);
	}
}