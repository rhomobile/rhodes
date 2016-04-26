package com.rho.rhoelements.plugins;

import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.Array;
import java.util.Arrays;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;

import android.graphics.Bitmap;

/**
 * Provides a set of functions to convert a 32 bit ARGB8888 bitmap into a single bit per pixel bitmap.
 * The original bitmap is passed into the contructor for this class.
 * @author Barry Piper
 * 
 */
public class FormatBitmap
{ 
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


	/*
	 * writeBitmapFileHeader writes the bitmap file header to the file.
	 *
	 */
	private static void writeBitmapFileHeader (FileOutputStream fOut, Bitmap bitmap, int iBitmapFileSize) throws IOException 
	{
		fOut.write (bfType);
		fOut.write (intToDWord (iBitmapFileSize));
		fOut.write (intToWord (RESERVED));
		fOut.write (intToWord (RESERVED));
		fOut.write (intToDWord (iFileOffsetIntoPixelArray));
	}

	/*
	 *
	 * writeBitmapInfoHeader writes the bitmap information header
	 * to the file.
	 *
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
	 * @param fOut
	 * @param bitmap
	 * @param canvasWidth
	 * @param canvasHeight
	 * @param iBackgroundColour
	 * @throws IOException
	 * @author Ben Kennedy
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
	
	//private static void writeBitmapData (FileOutputStream fOut, Bitmap bitmap, int canvasWidth, int canvasHeight/*, int iSizeImage*/, int iBackgroundColour/*, int iPenColour, int iBytesPerLine, int iBitmapFileSize*/) throws IOException
	//{
		// find all pixel plot points
/*		int total_x = iWidth;
		int total_y = iHeight;                                        
		int bitpos = 0;
		int new_y = 0;
		int row=0;
		int byteposition=0;
		int bitposition=0;
		int pixel=0;
		int x=0,y=0;

		byte[] bytesar = new byte[iSizeImage];
		Arrays.fill(bytesar, (byte) 0);
//		for(int i=0;i<iSizeImage;i++)
//		{
//			bytesar[i] = (byte)0x00;
//		}

		// now collect all points and lets pump them into a bit array
		for(x = 0; x < total_x; x++)
		{
			for(y = 0; y < total_y; y++)
			{
				pixel = bitmap.getPixel(x, y);

				if(pixel != iBackgroundColour )
				{
					// Calculate new y co-ordinate due to bitmap being reversed
					new_y = (total_y - 1) - y;

					// calculate raw bitposition without pad bytes
					bitpos =  ((new_y * iWidth) + x);

					// calculate row we start on
					row = bitpos / total_x;

					// calculate byte position with in Pixel/ByteArray we need to modify include pad byte calculation iBytesPerLine
					//byteposition = (row * 52) + (x / 8) ;
					byteposition = (row * iBytesPerLine) + (x / 8) ;

					// calculate bit position within byte field
					bitposition = 7 - (bitpos % 8);  

					bytesar[byteposition] = (byte) (bytesar[byteposition] | (1 << bitposition)); 
				}
			}
		}*/
		
		

		/************************************************/
		// Write buffer out to file

		/* append data to bitmap */
//		fOut.write (bytesar);
//	}

	/**
	 * 
	 * @param bitmap
	 * @param fOut
	 * @param iPenColour
	 * @param iBackgroundColour
	 * @throws IOException
	 * @author Barry Piper, Ben Kennedy (Trying to fix the Exceptions)
	 */
	public static void writeBitmapImage(Bitmap bitmap, FileOutputStream fOut, int iPenColour, int iBackgroundColour) throws IOException
	{
		if (bitmap == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "bitmap is null"));
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
//		writeBitmapData(fOut, bitmap, iWidth, iHeight, iSizeImage, iBackgroundColour, iPenColour, iBytesPerLine, iBitmapFileSize);
	}
}