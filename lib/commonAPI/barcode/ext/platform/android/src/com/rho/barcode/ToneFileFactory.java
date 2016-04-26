package com.rho.barcode;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.ShortBuffer;
import android.os.Environment;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;

/**
 * A Factory class that generates WAV files which describe a single tone.
 * Users can choose which frequency, volume and duration for the tone
 * @author Ben Kennedy (NCVT73)
 *
 */
public class ToneFileFactory
{
	private final int sampleRate = 44100; 	// 44.1KHz sample rate
	//private final int bytesPerSample = 2; 	// 16 Bits (2 Bytes) per sample
	private final String LOGTAG = "ToneFileFactory";
	//private double loopDuration;// = 5000;	// maximum loop duration in milliseconds
	private int sampleCount;
	private ShortBuffer audioBuffer;
	private short[] audioData;
	private final double maxAmplitude;
	private double wavePeriod;
	private double partialEquation;
	
	private final int DEFAULT_FREQUENCY = 3000;
	private final int DEFAULT_VOLUME = 5;
	private final int DEFAULT_DURATION = 250;
	
	private final byte[] EMPTY_WAV = {0x52,0x49,0x46,0x46,0x2e,0x00,0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x44,(byte)0xac,0x00,0x00,(byte)0x88,0x58,0x01,0x00,0x02,0x00,0x10,0x00,0x64,0x61,0x74,0x61,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	private int volumeLevel;

	/**
	 * Sets up the Factory to generate the sound
	 * @param frequency The frequency of the sound (0x0 -> 0xFFFF)
	 * @param volumeLevel The volume of the sound (0 -> 5)
	 * @param duration the length of the sound in milliseconds (max 5000)
	 * @author Ben Kennedy (NCVT73)
	 */
	public ToneFileFactory(double frequency, int volumeLevel, int duration)
	{
		if(frequency > 0xFFFF || frequency < 0)
		{
			Logger.W(LOGTAG, "Out-of-range decode sound frequency");
			frequency = DEFAULT_FREQUENCY;
		}
		
		if(volumeLevel > 5 || volumeLevel < 0)
		{
			Logger.W(LOGTAG, "Out-of-range decode sound volume set");
			this.volumeLevel = DEFAULT_VOLUME;
		}
		else
		{
			this.volumeLevel = volumeLevel;
		}
		
		if(duration < 0)
		{
			Logger.W(LOGTAG, "Out-of-range decode sound duration set");
			duration = DEFAULT_DURATION;
		}
		
		sampleCount = (int) ((duration * sampleRate) / 1000); // total number of calculated samples
		audioBuffer = ShortBuffer.allocate(sampleCount);
		audioData = (audioBuffer.hasArray() ? audioBuffer.array() : new short[sampleCount]); // PCM track data
		
		maxAmplitude = 6553 * volumeLevel; //6553 ~= 32767 / 5
		wavePeriod = sampleRate / frequency; // in samples
		partialEquation = 2 * Math.PI / wavePeriod;
	}
	
	/**
	 * Generates the Wav file (data and file object) from the values given into the factory constructor.
	 * @return the file object of the wav file, or null if it couldnt be made.
	 * @author Ben Kennedy (NCVT73)
	 */
	public File generateWavFile()
	{
		if(volumeLevel == 0)
		{
			return generateEmptySound();
		}
		
		// Create a sine wave of the required frequency at maximum amplitude
		for (int i = 0; i < sampleCount; ++i)
		{
			audioData[i] = (short) (Math.sin(partialEquation * i) * maxAmplitude);
			if ((i > 0) && (audioData[i] == 0) && (audioData[i-1] < 0))
			{
				// we've completed a sine wave that will loop
				sampleCount = i;
				//loopDuration = sampleCount * 1000 / sampleRate;
				break;
			}
		}
		
		audioBuffer.position(sampleCount);
		//Fill the array completely
		for(int i = sampleCount; i < audioData.length; i += sampleCount)
		{
			audioBuffer.put(audioData, 0, sampleCount);
		}
		
		return writeWavFile(false);
	}
	
	/**
	 * Generates a silent wav file.
	 * @return the file object of the empty wav file, or null if it couldnt be made.
	 * @author Ben Kennedy (NCVT73)
	 */
	private File generateEmptySound()
	{
		return writeWavFile(true);
	}
	
	/**
	 * Writes the generated wav data to a file, or writes the silent wav data to a file
	 * @param isEmpty whether to write the empty wav file(true) or the generated data(false).
	 * @return the File object of the created wav file, or null if it cannot be written.
	 * @author Ben Kennedy (NCVT73)
	 */
	private File writeWavFile(boolean isEmpty)
	{
		byte[] wavHeader = null;
		if(!isEmpty)
		{
			wavHeader= makeWavHeader(audioBuffer.position() * 2);
		}
		
		File wavFile;
		FileOutputStream fos = null;
		
		try
		{		
			String decodeWavPath = RhodesActivity.getDecodeWav();
			if(decodeWavPath.contains("file:/")){
				decodeWavPath = decodeWavPath.substring(6);
			}
			File f = new File(decodeWavPath);
			//Write to the wav file
			if((decodeWavPath != null) && (decodeWavPath != "") && (f.getAbsoluteFile().exists())){
				wavFile = new File(decodeWavPath);
				return wavFile;
			}else if(BarcodeFactory.getEMDKDeviceVersion() == 3){
				wavFile = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), "decode.wav");
			}else{
				wavFile = new File(RhodesActivity.safeGetInstance().getCacheDir(), "decodeWav.wav");
			}
			wavFile.delete();
			
			if(wavFile.exists())
			{
				Logger.E(LOGTAG, "Could not remove cached Scan Decode wave file.");
				return null;
			}

			wavFile.createNewFile();
			fos = new FileOutputStream(wavFile);
			
			if(isEmpty)
			{
				fos.write(EMPTY_WAV);
			}
			else
			{
				byte[] bytesOfData = new byte[audioData.length * 2];
				ByteBuffer.wrap(bytesOfData).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().put(audioData);
				
				fos.write(wavHeader);
				fos.write(bytesOfData);
			}
			
			fos.close();
			fos = null;
		}
		catch (FileNotFoundException e)
		{
			Logger.E(LOGTAG, "Could not write Scan-Decode wave file to cache.");
			return null;
		}
		catch (IOException e)
		{
			Logger.E(LOGTAG, "Could not write Scan-Decode wave file to cache.");
			return null;
		}
		catch(NullPointerException e)
		{
			Logger.E(LOGTAG, "Could not find activity");
			return null;
		}
		finally
		{
			if(fos != null)
			{
				try
				{
					fos.close();
				}
				catch (IOException e)
				{
					//ignore
					Logger.D(LOGTAG, "Could not close file output stream after exception");
				}
			}
		}
		return wavFile;
	}

	/**
	 * Writes a 44100Hz 16bit mono wav header
	 * @param dataLength the length of the actual data of the wave
	 * @return a 44 byte Wav header.
	 * @author Ben Kennedy (NCVT73)
	 */
	private byte[] makeWavHeader(int dataLength)
	{
		//Write header
		byte[] header = new byte[44];
		
		int fullLength = dataLength + 36;
		int newDataLength = dataLength -1;

		//RIFF Chunk
		header[0] = 'R'; header[1] = 'I';header[2] = 'F';header[3] = 'F'; //bigEndian
		header[4] = (byte)( (fullLength << 24) >> 24 ); header[5] = (byte)( (fullLength << 16) >> 24 ); header[6] = (byte)( (fullLength << 8) >> 24 ); header[7] = (byte)( fullLength >> 24 ); //TODO //littleEndian
		header[8] = 'W'; header[9] = 'A'; header[10] = 'V'; header[11] = 'E'; //bigEndian
		
		//FORMAT Chunk
		header[12] = 'f'; header[13] = 'm';	header[14] = 't'; header[15] = ' '; // "fmt_" bigEndian
		header[16] = 0x10; header[17] = 0; header[18] = 0; header[19] = 0; // length of format chunk. Always 0x10, littleEndian
		header[20] = 0x01; header[21] = 0; //Always 0x01, littleEndian
		header[22] = 0x01; header[23] = 0; //Channel Numbers (mono), littleEndian
		header[24] = 0x44; header[25] = (byte) 0xAC; header[26] = 0; header[27] = 0; // Sample Rate (binary, in Hz), littleEndian
		header[28] = (byte) 0x88; header[29] = 0x58; header[30] = 0x01; header[31] = 0; // WBytes per second, littleEndian
		header[32] = 0x2; header[33] = 0; // Bytes per sample, littleEndian
		header[34] = 0x10; header[35] = 0; // Bits per sample, littleEndian
		
		//DATA chunk
		header[36] = 'd'; header[37] = 'a'; header[38] = 't'; header[39] = 'a'; //data bigEndian
		header[40] = (byte)( (newDataLength << 24) >> 24 ); header[41] = (byte)( (newDataLength << 16) >> 24 ); header[42] = (byte)( (newDataLength << 8) >> 24 ); header[43] = (byte)( newDataLength >> 24 ); // length of data to follow, littleEndian
		
		return header;
	}
}
