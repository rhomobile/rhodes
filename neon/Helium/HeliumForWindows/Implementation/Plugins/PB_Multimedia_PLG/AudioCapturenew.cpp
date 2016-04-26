#include "AudioCapture.h"

LPCWSTR	lpRawFileName = L"\\AudioCapture.raw";

CAudioCapture::CAudioCapture()
{
	m_hWaveIn = NULL;
	m_hFile = INVALID_HANDLE_VALUE;
	m_bUseBufferA = TRUE;
	m_hEvents[0] = NULL;
	m_hEvents[1] = NULL;
	m_iMaxDuration = 20;
	m_lpzFilename = NULL;

	// The following defaults (with the exception of cbSize) may need to be configurable
	// based on user config in future, but it will require probing the hardware for its
	// available devices and supported formats.  The available sample rates and resolution
	// will be different depending on the format selected.

	ZeroMemory(&m_wfx, sizeof(WAVEFORMATEX));
	m_dwDeviceId = 0;					// Device ID of Microphone (default device = 0)
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;	// sampling format (default = RAW PCM)
	m_wfx.nChannels = 1;				// number of channels to sample (1 = mono, 2 = stereo etc)
	m_wfx.nSamplesPerSec = 8000;		// sample rate in ms (8000 to 44100)
	m_wfx.wBitsPerSample = 8;			// sample resolution (8 or 16 bits)
	m_wfx.cbSize = 0;					// size of additional header info (default = 0)

	// calculated values - do not change
	m_wfx.nBlockAlign = (WORD)((m_wfx.nChannels * m_wfx.wBitsPerSample) / 8);
	m_wfx.nAvgBytesPerSec = m_wfx.nBlockAlign * m_wfx.nSamplesPerSec;
	m_iMaxFileSizeBytes = m_iMaxDuration * m_wfx.nAvgBytesPerSec;

}

CAudioCapture::~CAudioCapture()
{
	if (m_hWaveIn)
		Stop();
	delete [] m_lpzFilename;
	m_lpzFilename = NULL;
}

int CAudioCapture::Start()
{
	MMRESULT res = MMSYSERR_NOERROR;

	if (m_hWaveIn == NULL)
    {
		ZeroMemory(&m_waveHdrA, sizeof(WAVEHDR));
		ZeroMemory(&m_waveHdrB, sizeof(WAVEHDR));
		m_ulCapturedBytes = 0;

		OutputDebugStringW(L"WAV: Starting the Wave capture..\n");

		if ((m_hFile != INVALID_HANDLE_VALUE) ||
			((m_hFile = CreateFile(lpRawFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL)) == INVALID_HANDLE_VALUE))
		{
			return MMSYSERR_NOMEM;
		}

		if ((res = waveInOpen((LPHWAVEIN )&m_hWaveIn, 0, &m_wfx, (DWORD_PTR)waveInProc, 
			(DWORD_PTR)this, (WAVE_FORMAT_DIRECT | CALLBACK_FUNCTION))) == MMSYSERR_NOERROR) 
		{			
			// we want our double buffers to be multiples of 1 second, 500ms or at worst case 100ms
			// so that we can count up the capture time based on buffer fills.
			if (m_wfx.nAvgBytesPerSec > 64000)
			{	// over 512 kbps use 100ms buffers
				m_waveHdrA.dwBufferLength = m_wfx.nAvgBytesPerSec / 10;
				m_waveHdrB.dwBufferLength = m_waveHdrA.dwBufferLength;
			}
			else if (m_wfx.nAvgBytesPerSec > 32000)
			{	// 256-512 kbps use 500ms buffers
				m_waveHdrA.dwBufferLength = m_wfx.nAvgBytesPerSec / 2;
				m_waveHdrB.dwBufferLength = m_waveHdrA.dwBufferLength;
			}
			else
			{	// under 254 kbps use 1s buffers
				m_waveHdrA.dwBufferLength = m_wfx.nAvgBytesPerSec;
				m_waveHdrB.dwBufferLength = m_waveHdrA.dwBufferLength;
			}
			m_waveHdrA.lpData = new char[m_waveHdrA.dwBufferLength];
			m_waveHdrB.lpData = new char[m_waveHdrB.dwBufferLength];

			if ((!m_waveHdrA.lpData) || (!m_waveHdrB.lpData))
			{
				OutputDebugStringW(L"Unable to allocate memory for audio capture buffers\n");
				waveInClose(m_hWaveIn);
				m_hWaveIn = NULL;
				delete [] m_waveHdrA.lpData;
				m_waveHdrA.lpData = NULL;
				delete [] m_waveHdrB.lpData;
				m_waveHdrB.lpData = NULL;
				return MMSYSERR_NOMEM;
			}

			m_bUseBufferA = TRUE;

            if ((res = waveInPrepareHeader(m_hWaveIn, &m_waveHdrA, sizeof(WAVEHDR))) == MMSYSERR_NOERROR)
			{
				if ((res = waveInAddBuffer(m_hWaveIn, &m_waveHdrA, sizeof(WAVEHDR))) == MMSYSERR_NOERROR)
				{
					if ((res = waveInPrepareHeader(m_hWaveIn, &m_waveHdrB, sizeof(WAVEHDR))) == MMSYSERR_NOERROR)
					{
						if ((res = waveInAddBuffer(m_hWaveIn, &m_waveHdrB, sizeof(WAVEHDR))) == MMSYSERR_NOERROR)
						{
							m_hEvents[CAPTURE_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
							m_hEvents[CANCEL_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL);
							if (m_hEvents[CAPTURE_EVENT] && m_hEvents[CANCEL_EVENT])
							{
								CloseHandle(CreateThread(NULL, 0, threadProc, (LPVOID)this, 0, NULL));
								if ((res = waveInStart(m_hWaveIn)) == MMSYSERR_NOERROR)
								{
									return res;
								}
								SetEvent(m_hEvents[CANCEL_EVENT]);
							}
							else
							{
								CloseHandle(m_hEvents[CAPTURE_EVENT]);
								m_hEvents[CAPTURE_EVENT] = NULL;
								CloseHandle(m_hEvents[CANCEL_EVENT]);
								m_hEvents[CANCEL_EVENT] = NULL;
							}
						}
						waveInUnprepareHeader(m_hWaveIn, &m_waveHdrB, sizeof(WAVEHDR));
					}
				}
				waveInUnprepareHeader(m_hWaveIn, &m_waveHdrA, sizeof(WAVEHDR));
			}
			waveInClose(m_hWaveIn);
			m_hWaveIn = NULL;
			delete [] m_waveHdrA.lpData;
			m_waveHdrA.lpData = NULL;
			delete [] m_waveHdrB.lpData;
			m_waveHdrB.lpData = NULL;
		}
		else 
		{
			if(res == MMSYSERR_ALLOCATED)
			{
				OutputDebugStringW(L"WAV: Wavein: MMSYSERR_ALLOCATED..\n");
			}
			else if(res == MMSYSERR_NODRIVER)
			{
				OutputDebugStringW(L"WAV: Wavein: MMSYSERR_NODRIVER..\n");
			}
			else if(res == MMSYSERR_BADDEVICEID)
			{
				OutputDebugStringW(L"WAV: Wavein: MMSYSERR_BADDEVICEID..\n");
			}
			else if(res == MMSYSERR_NOMEM)
			{
				OutputDebugStringW(L"WAV: Wavein: MMSYSERR_NOMEM..\n");
			}
			else if(res == WAVERR_BADFORMAT)
			{
				OutputDebugStringW(L"WAV: Waven WAVERR_BADFORMAT..\n");
			}
			OutputDebugStringW(L"WAV: Wavin failure..\n");
		}	
	}
	else 
	{
		OutputDebugStringW(L"WAV: Restarting WaveIn capture ..\n");
		m_waveHdrA.dwBytesRecorded = 0;
		m_waveHdrB.dwBytesRecorded = 0;
		m_bUseBufferA = TRUE;
		waveInAddBuffer(m_hWaveIn, &m_waveHdrA, sizeof(WAVEHDR));
		waveInAddBuffer(m_hWaveIn, &m_waveHdrB, sizeof(WAVEHDR));
		OutputDebugStringW(L"WAV: WaveIn capture restarted..\n");
		
	}
	return res;
}

void CAudioCapture::Stop(void)
{
	if (m_hEvents[CAPTURE_EVENT])
		SetEvent(m_hEvents[CAPTURE_EVENT]);
}

void CAudioCapture::Cancel(void)
{
	if (m_hEvents[CANCEL_EVENT])
		SetEvent(m_hEvents[CANCEL_EVENT]);
}

void CALLBACK CAudioCapture::waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, 
										DWORD dwParam1, DWORD dwParam2)
{
	CAudioCapture* pObj = (CAudioCapture*)dwInstance;
	DWORD dwBytesWritten = 0;

	if(hwi != NULL)
	{
	    if(uMsg == WIM_DATA)
		{
//			RETAILMSG(1, (L"WAV: WaveIn Data.. time %u \n", GetTickCount()));
			if(pObj->m_bUseBufferA == TRUE)
			{
				if (pObj->m_waveHdrA.dwBytesRecorded > 0)
				{
					WriteFile(pObj->m_hFile, pObj->m_waveHdrA.lpData, pObj->m_waveHdrA.dwBytesRecorded, &dwBytesWritten, NULL);
					pObj->m_waveHdrA.dwBytesRecorded = 0;
					waveInAddBuffer(pObj->m_hWaveIn, &pObj->m_waveHdrA, sizeof(WAVEHDR));
				}
				pObj->m_bUseBufferA = FALSE;
			}
			else 
			{
				if (pObj->m_waveHdrB.dwBytesRecorded > 0)
				{
					WriteFile(pObj->m_hFile, pObj->m_waveHdrB.lpData, pObj->m_waveHdrB.dwBytesRecorded, &dwBytesWritten, NULL);
					pObj->m_waveHdrB.dwBytesRecorded = 0;
					waveInAddBuffer(pObj->m_hWaveIn, &pObj->m_waveHdrB, sizeof(WAVEHDR));
				}
				pObj->m_bUseBufferA = TRUE;
			}
			pObj->m_ulCapturedBytes += dwBytesWritten;
			if (pObj->m_ulCapturedBytes >= pObj->m_iMaxFileSizeBytes)
				pObj->Stop();
		}
		else if(uMsg == WIM_OPEN)
		{
			OutputDebugStringW(L"WAV: WaveIn device open..\n");
		}
		else if(uMsg == WIM_CLOSE)
		{
			OutputDebugStringW(L"WAV: WaveIn device closed..\n");
		}
	}
}

DWORD WINAPI CAudioCapture::threadProc(LPVOID lpParam)
{
	if (lpParam)
	{
		((CAudioCapture*)lpParam)->eventHandler();
		return 0;
	}
	return -1;
}

void CAudioCapture::eventHandler()
{
	// no need for an event loop as we're only interested in handling one event...
	// it will either be a capture event or a cancel event
	DWORD dwEvent = WaitForMultipleObjects(2, m_hEvents, FALSE, INFINITE);

	// which ever of the two events we receive, we want to stop the wave device
	if (m_hWaveIn)
	{
		OutputDebugStringW(L"WAV: WaveIn capture stopping..\n");
		waveInStop(m_hWaveIn);

		OutputDebugStringW(L"WAV: WaveIn capture resetting..\n");
		waveInReset(m_hWaveIn);

		OutputDebugStringW(L"WAV: WaveIn capture unpreparing headers..\n");
		waveInUnprepareHeader(m_hWaveIn, &m_waveHdrA, sizeof(WAVEHDR));
		waveInUnprepareHeader(m_hWaveIn, &m_waveHdrB, sizeof(WAVEHDR));

		waveInClose(m_hWaveIn);
		OutputDebugStringW(L"WAV: Audio recording stopped successfully.\n");

		delete [] m_waveHdrA.lpData;
		m_waveHdrA.lpData = NULL;
		delete [] m_waveHdrB.lpData;
		m_waveHdrB.lpData = NULL;
	    CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		
		m_hWaveIn = NULL;
	}

	if (dwEvent == WAIT_OBJECT_0 + CAPTURE_EVENT)
	{
		// only process the raw file on capture event
		processRawFile();
	}

	// delete the temporary file
	DeleteFile(lpRawFileName);

	// clean-up
	CloseHandle(m_hEvents[0]);
	m_hEvents[0] = NULL;
	CloseHandle(m_hEvents[1]);
	m_hEvents[1] = NULL;
}

void CAudioCapture::processRawFile()
{
	RIFF_FILEHEADER FileHeader;
	RIFF_CHUNKHEADER WaveHeader;
	RIFF_CHUNKHEADER DataHeader;
	DWORD dwBytesWritten, dwBytesRead;
	HANDLE hWavFile;
	BYTE* pCopyBuf = NULL;

	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		// Fill in the file, wave and data headers
		WaveHeader.dwCKID = RIFF_FORMAT;
		WaveHeader.dwSize = sizeof(WAVEFORMATEX) + m_wfx.cbSize;

		// the DataHeader chunk contains the audio data
		DataHeader.dwCKID = RIFF_CHANNEL;
		DataHeader.dwSize = m_ulCapturedBytes;

		// The FileHeader
		FileHeader.dwRiff = RIFF_FILE;
		FileHeader.dwWave = RIFF_WAVE;
		FileHeader.dwSize = sizeof(WaveHeader) + WaveHeader.dwSize + sizeof(DataHeader) + DataHeader.dwSize;

		DWORD iCopyBufSize = DataHeader.dwSize;
		// if the raw file is huge we need to limit the size of the copy buffer
		if (iCopyBufSize > 1000000)
			iCopyBufSize = 1000000;
		pCopyBuf = new BYTE[iCopyBufSize];
		if (pCopyBuf == NULL) {
			OutputDebugStringW(L"Unable to get enough memory for copy operation\n");
			return;
		}

		// Open wave file
		hWavFile = CreateFile(m_lpzFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
		if( hWavFile == INVALID_HANDLE_VALUE ) {
			RETAILMSG(1, (TEXT("Error opening %s. Error code = 0x%08x\n"), m_lpzFilename, GetLastError()));
			goto ERROR_EXIT;
		}

	    m_hFile = CreateFile(lpRawFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if( m_hFile == INVALID_HANDLE_VALUE ) {
			RETAILMSG(1, (TEXT("Error opening %s. Error code = 0x%08x\n"), lpRawFileName, GetLastError()));
			goto ERROR_EXIT;
		}

		// write the riff file
		if (! WriteFile(hWavFile, &FileHeader, sizeof(FileHeader), &dwBytesWritten, NULL)) {
			RETAILMSG(1, (TEXT("Error writing file header\r\n")));
			goto ERROR_EXIT;
		}

		// write the wave header
		if (! WriteFile(hWavFile, &WaveHeader, sizeof(WaveHeader), &dwBytesWritten, NULL)) {
			RETAILMSG(1, (TEXT("Error writing wave header\r\n")));
			goto ERROR_EXIT;
		}

		// write the wave format
		if (! WriteFile(hWavFile, &m_wfx, WaveHeader.dwSize, &dwBytesWritten, NULL)) {
			RETAILMSG(1, (TEXT("Error writing wave format\r\n")));
			goto ERROR_EXIT;
		}

		// write the data header
		if (! WriteFile(hWavFile, &DataHeader, sizeof(DataHeader), &dwBytesWritten, NULL)) {
			RETAILMSG(1, (TEXT("Error writing PCM data header\r\n")));
			goto ERROR_EXIT;
		}

		// read the PCM data from the raw file and write it to the WAV file
		DWORD dwBytesToCopy = DataHeader.dwSize;
		do {
			DWORD dwChunkSize = iCopyBufSize;
			if (dwBytesToCopy < dwChunkSize)
				dwChunkSize = dwBytesToCopy;

			// read the PCM data
			if (! ReadFile(m_hFile, pCopyBuf, dwChunkSize, &dwBytesRead, NULL) || dwBytesRead < dwChunkSize) {
				RETAILMSG(1, (TEXT("Error reading raw file\n")));
				goto ERROR_EXIT;
			}

			// write the PCM data
			if (! WriteFile(hWavFile, pCopyBuf, dwBytesRead, &dwBytesWritten, NULL)) {
				RETAILMSG(1, (TEXT("Error writing PCM data\r\n")));
				goto ERROR_EXIT;
			}

			dwBytesToCopy -= dwBytesRead;
		} while (dwBytesToCopy > 0);
	}

ERROR_EXIT:
	delete [] pCopyBuf;
    CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
	CloseHandle(hWavFile);
}
