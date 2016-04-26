/**
 * 
 */
package com.rho.rhoelements.common;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;

/**
 * @author FPC843
 * This class is an utility for unzipping zip files.
 * Its main purpose is to unzip the APD files if they don't exist in the default location
 */
public class UnzipUtility
{
	/**
	 * @param zipFileName the file to unzip
	 * @param destination is the destination folder
	 * @return true if successful, false otherwise 
	 */
	public static boolean unzipFile(String zipFileName, String destination)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		final int BUFFER = 2048;
		boolean res = false;
		
		try
		{
			BufferedOutputStream dest = null;
	        FileInputStream fis = new FileInputStream(zipFileName);
	        ZipInputStream zis = new ZipInputStream(new BufferedInputStream(fis));
	        ZipEntry entry;
	        while((entry = zis.getNextEntry()) != null)
	        {
	        	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Extracting: " + destination + entry.getName()));
	        	if (entry.isDirectory())
	        		new File(destination + entry.getName()).mkdir();
	        	else
	        	{
		            int count;
		            byte data[] = new byte[BUFFER];
		            // write the files to the disk
		            FileOutputStream fos = new FileOutputStream(destination + entry.getName());
		            dest = new BufferedOutputStream(fos, BUFFER);
		            while ((count = zis.read(data, 0, BUFFER)) != -1)
		            {
		               dest.write(data, 0, count);
		            }
		            dest.flush();
		            dest.close();
	        	}
	        }
	        zis.close();
	        res = true;
		} 
		catch(Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
			e.printStackTrace();
	    }
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	/**
	 * 
	 * @param zipFileName the file to unzip
	 * @param fileName is a file (or directory) that the method will check if it exists or not. If not, it unzips zipFileName
	 * @param destination is the destination folder
	 * @return true if successful, false otherwise 
	 */
	public static boolean unzipFileIfRequired(String zipFileName, String destination, String fileName)
	{
		File file = new File(fileName);
		if (file.exists() == false)
			return unzipFile(zipFileName, destination);
		else
			return true;
	}
}
