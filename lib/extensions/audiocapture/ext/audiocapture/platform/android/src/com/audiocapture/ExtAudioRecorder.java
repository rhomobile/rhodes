// source based on http://i-liger.com/article/android-wav-audio-recording

package com.audiocapture;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.media.MediaRecorder.AudioSource;
import android.util.Log;

public class ExtAudioRecorder 
{
	private final static int[] sampleRates = {22050, 11025, 8000};
	
	public static ExtAudioRecorder getInstanse(Boolean recordingCompressed)
	{
		ExtAudioRecorder result = null;
		
		if(recordingCompressed)
		{
			result = new ExtAudioRecorder(	false, 
											AudioSource.MIC, 
											sampleRates[2], 
											AudioFormat.CHANNEL_CONFIGURATION_MONO,
											AudioFormat.ENCODING_PCM_16BIT);
		}
		else
		{
			int i=0;
			do
			{
				result = new ExtAudioRecorder(	true, 
												AudioSource.MIC, 
												sampleRates[i], 
												AudioFormat.CHANNEL_CONFIGURATION_MONO,
												AudioFormat.ENCODING_PCM_16BIT);
				
			} while((++i<sampleRates.length) & !(result.getState() == ExtAudioRecorder.State.INITIALIZING));
		}
		return result;
	}
	
	/**
	* INITIALIZING : recorder is initializing;
	* READY : recorder has been initialized, recorder not yet started
	* RECORDING : recording
	* ERROR : reconstruction needed
	* STOPPED: reset needed
	*/
	public enum State {INITIALIZING, READY, RECORDING, ERROR, STOPPED};
	
	public static final boolean RECORDING_UNCOMPRESSED = true;
	public static final boolean RECORDING_COMPRESSED = false;
	
	// The interval in which the recorded samples are output to the file
	// Used only in uncompressed mode
	private static final int TIMER_INTERVAL = 120;
	
	// Toggles uncompressed recording on/off; RECORDING_UNCOMPRESSED / RECORDING_COMPRESSED
	private boolean         rUncompressed;
	
	// Recorder used for uncompressed recording
	private AudioRecord     audioRecorder = null;
	
	// Recorder used for compressed recording
	private MediaRecorder   mediaRecorder = null;
	
	// Stores current amplitude (only in uncompressed mode)
	private int             cAmplitude= 0;
	
	// Output file path
	private String          filePath = null;
	
	// Recorder state; see State
	private State          	state;
	
	// File writer (only in uncompressed mode)
	private RandomAccessFile randomAccessWriter;
		       
	// Number of channels, sample rate, sample size(size in bits), buffer size, audio source, sample size(see AudioFormat)
	private short                    nChannels;
	private int                      sRate;
	private short                    bSamples;
	private int                      bufferSize;
	private int                      aSource;
	private int                      aFormat;
	
	// Number of frames written to file on each output(only in uncompressed mode)
	private int                      framePeriod;
	
	// Buffer for output(only in uncompressed mode)
	private byte[]                   buffer;
	
	// Number of bytes written to file after header(only in uncompressed mode)
	// after stop() is called, this size is written to the header/data chunk in the wave file
	private int                      payloadSize;
	
	/**
	*
	* Returns the state of the recorder in a RehearsalAudioRecord.State typed object.
	* Useful, as no exceptions are thrown.
	*
	* @return recorder state
	*/
	public State getState()
	{
		return state;
	}
	
	/*
	*
	* Method used for recording.
	*
	*/
	private AudioRecord.OnRecordPositionUpdateListener updateListener = new AudioRecord.OnRecordPositionUpdateListener()
	{
		public void onPeriodicNotification(AudioRecord recorder)
		{
			audioRecorder.read(buffer, 0, buffer.length); // Fill buffer
			try
			{ 
				randomAccessWriter.write(buffer); // Write buffer to file
				payloadSize += buffer.length;
				if (bSamples == 16)
				{
					for (int i=0; i<buffer.length/2; i++)
					{ // 16bit sample size
						short curSample = getShort(buffer[i*2], buffer[i*2+1]);
						if (curSample > cAmplitude)
						{ // Check amplitude
							cAmplitude = curSample;
						}
					}
				}
				else	
				{ // 8bit sample size
					for (int i=0; i<buffer.length; i++)
					{
						if (buffer[i] > cAmplitude)
						{ // Check amplitude
							cAmplitude = buffer[i];
						}
					}
				}
			}
			catch (IOException e)
			{
				Log.e(ExtAudioRecorder.class.getName(), "Error occured in updateListener, recording is aborted");
				//stop();
			}
		}
	
		public void onMarkerReached(AudioRecord recorder)
		{
			// NOT USED
		}
	};
	/** 
	 * 
	 * 
	 * Default constructor
	 * 
	 * Instantiates a new recorder, in case of compressed recording the parameters can be left as 0.
	 * In case of errors, no exception is thrown, but the state is set to ERROR
	 * 
	 */ 
	public ExtAudioRecorder(boolean uncompressed, int audioSource, int sampleRate, int channelConfig, int audioFormat)
	{
		try
		{
			rUncompressed = uncompressed;
			if (rUncompressed)
			{ // RECORDING_UNCOMPRESSED
				if (audioFormat == AudioFormat.ENCODING_PCM_16BIT)
				{
					bSamples = 16;
				}
				else
				{
					bSamples = 8;
				}
				
				if (channelConfig == AudioFormat.CHANNEL_CONFIGURATION_MONO)
				{
					nChannels = 1;
				}
				else
				{
					nChannels = 2;
				}
				
				aSource = audioSource;
				sRate   = sampleRate;
				aFormat = audioFormat;

				framePeriod = sampleRate * TIMER_INTERVAL / 1000;
				bufferSize = framePeriod * 2 * bSamples * nChannels / 8;
				if (bufferSize < AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat))
				{ // Check to make sure buffer size is not smaller than the smallest allowed one 
					bufferSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat);
					// Set frame period and timer interval accordingly
					framePeriod = bufferSize / ( 2 * bSamples * nChannels / 8 );
					Log.w(ExtAudioRecorder.class.getName(), "Increasing buffer size to " + Integer.toString(bufferSize));
				}
				
				audioRecorder = new AudioRecord(audioSource, sampleRate, channelConfig, audioFormat, bufferSize);

				if (audioRecorder.getState() != AudioRecord.STATE_INITIALIZED)
					throw new Exception("AudioRecord initialization failed");
				audioRecorder.setRecordPositionUpdateListener(updateListener);
				audioRecorder.setPositionNotificationPeriod(framePeriod);
			} else
			{ // RECORDING_COMPRESSED
				mediaRecorder = new MediaRecorder();
				mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
				mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
				mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);				
			}
			cAmplitude = 0;
			filePath = null;
			state = State.INITIALIZING;
		} catch (Exception e)
		{
			if (e.getMessage() != null)
			{
				Log.e(ExtAudioRecorder.class.getName(), e.getMessage());
			}
			else
			{
				Log.e(ExtAudioRecorder.class.getName(), "Unknown error occured while initializing recording");
			}
			state = State.ERROR;
		}
	}
	
	/**
	 * Sets output file path, call directly after construction/reset.
	 *  
	 * @param output file path
	 * 
	 */
	public void setOutputFile(String argPath)
	{
		try
		{
			if (state == State.INITIALIZING)
			{
				filePath = argPath;
				if (!rUncompressed)
				{
					mediaRecorder.setOutputFile(filePath);					
				}
			}
		}
		catch (Exception e)
		{
			if (e.getMessage() != null)
			{
				Log.e(ExtAudioRecorder.class.getName(), e.getMessage());
			}
			else
			{
				Log.e(ExtAudioRecorder.class.getName(), "Unknown error occured while setting output path");
			}
			state = State.ERROR;
		}
	}
	
	/**
	 * 
	 * Returns the largest amplitude sampled since the last call to this method.
	 * 
	 * @return returns the largest amplitude since the last call, or 0 when not in recording state. 
	 * 
	 */
	public int getMaxAmplitude()
	{
		if (state == State.RECORDING)
		{
			if (rUncompressed)
			{
				int result = cAmplitude;
				cAmplitude = 0;
				return result;
			}
			else
			{
				try
				{
					return mediaRecorder.getMaxAmplitude();
				}
				catch (IllegalStateException e)
				{
					return 0;
				}
			}
		}
		else
		{
			return 0;
		}
	}
	

	/**
	 * 
	* Prepares the recorder for recording, in case the recorder is not in the INITIALIZING state and the file path was not set
	* the recorder is set to the ERROR state, which makes a reconstruction necessary.
	* In case uncompressed recording is toggled, the header of the wave file is written.
	* In case of an exception, the state is changed to ERROR
	* 	 
	*/
	public void prepare()
	{
		try
		{
			if (state == State.INITIALIZING)
			{
				if (rUncompressed)
				{
					if ((audioRecorder.getState() == AudioRecord.STATE_INITIALIZED) & (filePath != null))
					{
						// write file header

						randomAccessWriter = new RandomAccessFile(filePath, "rw");
						
						randomAccessWriter.setLength(0); // Set file length to 0, to prevent unexpected behavior in case the file already existed
						randomAccessWriter.writeBytes("RIFF");
						randomAccessWriter.writeInt(0); // Final file size not known yet, write 0 
						randomAccessWriter.writeBytes("WAVE");
						randomAccessWriter.writeBytes("fmt ");
						randomAccessWriter.writeInt(Integer.reverseBytes(16)); // Sub-chunk size, 16 for PCM
						randomAccessWriter.writeShort(Short.reverseBytes((short) 1)); // AudioFormat, 1 for PCM
						randomAccessWriter.writeShort(Short.reverseBytes(nChannels));// Number of channels, 1 for mono, 2 for stereo
						randomAccessWriter.writeInt(Integer.reverseBytes(sRate)); // Sample rate
						randomAccessWriter.writeInt(Integer.reverseBytes(sRate*bSamples*nChannels/8)); // Byte rate, SampleRate*NumberOfChannels*BitsPerSample/8
						randomAccessWriter.writeShort(Short.reverseBytes((short)(nChannels*bSamples/8))); // Block align, NumberOfChannels*BitsPerSample/8
						randomAccessWriter.writeShort(Short.reverseBytes(bSamples)); // Bits per sample
						randomAccessWriter.writeBytes("data");
						randomAccessWriter.writeInt(0); // Data chunk size not known yet, write 0
						
						buffer = new byte[framePeriod*bSamples/8*nChannels];
						state = State.READY;
					}
					else
					{
						Log.e(ExtAudioRecorder.class.getName(), "prepare() method called on uninitialized recorder");
						state = State.ERROR;
					}
				}
				else
				{
					mediaRecorder.prepare();
					state = State.READY;
				}
			}
			else
			{
				Log.e(ExtAudioRecorder.class.getName(), "prepare() method called on illegal state");
				release();
				state = State.ERROR;
			}
		}
		catch(Exception e)
		{
			if (e.getMessage() != null)
			{
				Log.e(ExtAudioRecorder.class.getName(), e.getMessage());
			}
			else
			{
				Log.e(ExtAudioRecorder.class.getName(), "Unknown error occured in prepare()");
			}
			state = State.ERROR;
		}
	}
	
	/**
	 * 
	 * 
	 *  Releases the resources associated with this class, and removes the unnecessary files, when necessary
	 *  
	 */
	public void release()
	{
		if (state == State.RECORDING)
		{
			stop();
		}
		else
		{
			if ((state == State.READY) & (rUncompressed))
			{
				try
				{
					randomAccessWriter.close(); // Remove prepared file
				}
				catch (IOException e)
				{
					Log.e(ExtAudioRecorder.class.getName(), "I/O exception occured while closing output file");
				}
				(new File(filePath)).delete();
			}
		}
		
		if (rUncompressed)
		{
			if (audioRecorder != null)
			{
				audioRecorder.release();
			}
		}
		else
		{
			if (mediaRecorder != null)
			{
				mediaRecorder.release();
			}
		}
	}
	
	/**
	 * 
	 * 
	 * Resets the recorder to the INITIALIZING state, as if it was just created.
	 * In case the class was in RECORDING state, the recording is stopped.
	 * In case of exceptions the class is set to the ERROR state.
	 * 
	 */
	public void reset()
	{
		try
		{
			if (state != State.ERROR)
			{
				release();
				filePath = null; // Reset file path
				cAmplitude = 0; // Reset amplitude
				if (rUncompressed)
				{
					audioRecorder = new AudioRecord(aSource, sRate, nChannels+1, aFormat, bufferSize);
				}
				else
				{
					mediaRecorder = new MediaRecorder();
					mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
					mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
					mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
				}
				state = State.INITIALIZING;
			}
		}
		catch (Exception e)
		{
			Log.e(ExtAudioRecorder.class.getName(), e.getMessage());
			state = State.ERROR;
		}
	}
	
	/**
	 * 
	 * 
	 * Starts the recording, and sets the state to RECORDING.
	 * Call after prepare().
	 * 
	 */
	public void start()
	{
		if (state == State.READY)
		{
			if (rUncompressed)
			{
				payloadSize = 0;
				audioRecorder.startRecording();
				audioRecorder.read(buffer, 0, buffer.length);
			}
			else
			{
				mediaRecorder.start();
			}
			state = State.RECORDING;
		}
		else
		{
			Log.e(ExtAudioRecorder.class.getName(), "start() called on illegal state");
			state = State.ERROR;
		}
	}
	
	/**
	 * 
	 * 
	 *  Stops the recording, and sets the state to STOPPED.
	 * In case of further usage, a reset is needed.
	 * Also finalizes the wave file in case of uncompressed recording.
	 * 
	 */
	public void stop()
	{
		if (state == State.RECORDING)
		{
			if (rUncompressed)
			{
				audioRecorder.stop();
				
				try
				{
					randomAccessWriter.seek(4); // Write size to RIFF header
					randomAccessWriter.writeInt(Integer.reverseBytes(36+payloadSize));
				
					randomAccessWriter.seek(40); // Write size to Subchunk2Size field
					randomAccessWriter.writeInt(Integer.reverseBytes(payloadSize));
				
					randomAccessWriter.close();
				}
				catch(IOException e)
				{
					Log.e(ExtAudioRecorder.class.getName(), "I/O exception occured while closing output file");
					state = State.ERROR;
				}
			}
			else
			{
				mediaRecorder.stop();
			}
			state = State.STOPPED;
		}
		else
		{
			Log.e(ExtAudioRecorder.class.getName(), "stop() called on illegal state");
			state = State.ERROR;
		}
	}
	
	/* 
	 * 
	 * Converts a byte[2] to a short, in LITTLE_ENDIAN format
	 * 
	 */
	private short getShort(byte argB1, byte argB2)
	{
		return (short)(argB1 | (argB2 << 8));
	}
	
}