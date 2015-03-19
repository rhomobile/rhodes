package com.rho.devicememory;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.LineNumberReader;

import android.os.Environment;
import android.os.StatFs;
import android.util.Log;

import com.rhomobile.rhodes.Logger;

public class DeviceMemoryCalculator {
	
	
	private static long TOTAL_RAM_SIZE=0;
	private static long AVAILABLE_RAM_SIZE=0;
	private static long EXTERNAL_STORAGE_MEMORY=0;
	private static long DATA_STORAGE_MEMORY=0;
	
	private static final String TAG="DeviceMemoryCalculator";
	
	public static int getAvailableMemory()
	{
		calculateRAMMemory();
		String str=String.valueOf(AVAILABLE_RAM_SIZE);
		int convertStr=0;
		try{
			convertStr=Integer.parseInt(str);
		}
		catch(Exception ex)
		{
			Log.d(TAG,"Exception="+ex.getMessage());
		}
		return convertStr;
	}
	
	public static int getAvailableStorageMemory()
	{
		calculateExternalStorageMemory();
		String str=String.valueOf(EXTERNAL_STORAGE_MEMORY);
		int convertStr=0;
		try{
			convertStr=Integer.parseInt(str);
		}
		catch(Exception ex)
		{
			Log.d(TAG,"Exception ex="+ex.getMessage());
		}
		return convertStr;
	}
	public static int getAvailableDataMemory()
	{
		calculateDataStorageMemory();
		String str=String.valueOf(DATA_STORAGE_MEMORY);
		int convertStr=0;
		try{
		convertStr=Integer.parseInt(str);
		}
		catch(Exception ex){
			Log.d(TAG,"Exception in converting string to int="+ex.getMessage());
		}
		return convertStr;
	}
	private static void calculateRAMMemory()
	{
		FileInputStream fi = null;
		InputStreamReader reader = null;
		LineNumberReader lre = null;
		try
		{
			fi = new FileInputStream(new File("/proc/meminfo")); 

			reader = new InputStreamReader(fi);
			lre = new LineNumberReader(reader);

			String line;
			String totalMem = null;
			String freeMem  = null;
			while ( ( line = lre.readLine( ) ) != null )
			{
				if ( line.startsWith( "MemTotal" ) ) 
				{
					totalMem = line;
					//Log.d(TAG,"total_ram="+totalMem);

				}
				if( line.startsWith( "MemFree"))
				{
					//logger.finest("SystemManagerService:MemFree Found");
					freeMem = line;
					//Log.d(TAG,"free_ram="+freeMem);
					break;
				}
			}
			if ( totalMem != null )
			{
				int idx = totalMem.indexOf( ':' );

				if ( idx != -1 )
				{
					totalMem = totalMem.substring( idx + 1 ).trim( );

					idx = totalMem.lastIndexOf( ' ' );

					if ( idx != -1 )
					{
						String unit = totalMem.substring( idx + 1 );

						try
						{
							long size = Long.parseLong( totalMem.substring( 0,
									idx ).trim( ) );

							if ( "kb".equalsIgnoreCase( unit ) ) //$NON-NLS-1$
							{
								//size *= 1024;
								size=size;//Return now as KB and NOT in bytes
							}
							else if ( "mb".equalsIgnoreCase( unit ) ) //$NON-NLS-1$
							{
								size *= 1024 ;
							}
							else if ( "gb".equalsIgnoreCase( unit ) ) //$NON-NLS-1$
							{
								size *= 1024 * 1024;
							}
							//totalPhyMemory = size;
							TOTAL_RAM_SIZE=size;
							//logger.finest("UpdateTotalNAvailPhyMem:totalPhyMemory="+String.valueOf(totalPhyMemory));
						}
						catch ( Exception e )
						{

						}
					}//if end

				}//if end
			}//if end ( totalMem != null )

			//To get the AvailPhysicalMem
			if ( freeMem != null )
			{
				//logger.finest("SystemManagerService:FreeMem not null"+String.valueOf(freeMem));
				int idx = freeMem.indexOf( ':' );

				if ( idx != -1 )
				{
					freeMem = freeMem.substring( idx + 1 ).trim( );

					idx = freeMem.lastIndexOf( ' ' );

					if ( idx != -1 )
					{
						String unit = freeMem.substring( idx + 1 );

						try
						{
							long size = Long.parseLong( freeMem.substring( 0,
									idx ).trim( ) );

							if ( "kb".equalsIgnoreCase( unit ) ) //$NON-NLS-1$
							{
								size =size;
							}
							else if ( "mb".equalsIgnoreCase( unit ) ) //$NON-NLS-1$
							{
								size *= 1024 ;
							}
							else if ( "gb".equalsIgnoreCase( unit ) ) //$NON-NLS-1$
							{
								size *= 1024 * 1024 ;
							}
							//availPhyMemory = size;
							AVAILABLE_RAM_SIZE=size;
							//logger.finest("UpdateTotalNAvailPhyMem:availPhyMemory="+String.valueOf(availPhyMemory));
						}
						catch ( Exception e )
						{

						}
					}//if end
				}//if end
			}//if end ( totalMem != null )
		}//try end
		
		catch(FileNotFoundException e)
		{	
			//logger.severe("SystemManagerService:File Not Found Exception");
		}
	
		catch(IOException ioe)
		{
			//logger.severe("SystemManagerService:IOException,while readinf file");
		}
		finally
		{
			// Don't forget to close the stream when we finish reading the file.
			if(fi != null)
			{
				try {
					fi.close();
				} catch (IOException e) {
					//logger.severe(e.toString());
				}
			}
			if (reader != null)
			{
				try {
					reader.close();
				} catch (IOException e) {
					//logger.severe(e.toString());
				}
			}
			if (lre != null)
			{
				try {
					lre.close();
				} catch (IOException e) {
					//logger.severe(e.toString());
				}
			}
		}

	}
	private static void calculateExternalStorageMemory()
	{
		
		String state = Environment.getExternalStorageState();
		Logger.D("DeviceMemorySingleton","External Storage State="+String.valueOf(state));
		File path = Environment.getExternalStorageDirectory();
		
		StatFs stat = new StatFs(path.getPath());
		long blockSize = stat.getBlockSize();
		long availableBlocks = stat.getAvailableBlocks();
		EXTERNAL_STORAGE_MEMORY = (availableBlocks*blockSize)/1024;//calculate in KB
		Logger.D("DeviceMemorySingleton","ExternalStorMemory="+EXTERNAL_STORAGE_MEMORY);
		
		//return externalAvailStorMemory;
		
	}
	private static void calculateDataStorageMemory()
	{
		File path = Environment.getDataDirectory();
		StatFs stat = new StatFs(path.getPath());
		long blockSize = stat.getBlockSize();
		long availableBlocks = stat.getAvailableBlocks();
		DATA_STORAGE_MEMORY =(availableBlocks*blockSize)/1024;//calculate in KB
		Logger.D("DeviceMemorySingleton","ExternalStorMemory="+DATA_STORAGE_MEMORY);
		
	}

}
