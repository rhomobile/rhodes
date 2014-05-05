#include "AudioCapture.h"


LPCWSTR	lpRawFileName = L"\\AudioCapture.raw";

CAudioCapture::CAudioCapture(bool bHasFocus)
{
	m_hWaveIn		= NULL;
	m_hFile			= INVALID_HANDLE_VALUE;
	m_bUseBufferA	= TRUE;
	m_hEvents[0]	= NULL;
	m_hEvents[1]	= NULL;
	m_bAppHasFocus	= bHasFocus;
	m_pStatusEvent	= NULL;
	ResetToDefaults();

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
	

}

CAudioCapture::~CAudioCapture()
{
	if (m_hWaveIn)
		Cancel();

	m_pStatusEvent	= NULL;
	delete [] m_lpzFilename;	
}

void CAudioCapture::ResetToDefaults()
{
	m_iMaxDuration	= AC_DEFAULT_DURATION;
	m_lpzFilename	= setString(AC_DEFAULT_FILENAME);
}

unsigned int CAudioCapture::RoundUp(double dValue)
{
	unsigned int iTemp = (int)dValue;
	double dTemp = (double)iTemp;
	if(dValue > dTemp) return ++iTemp;
	return iTemp;

}


LPWSTR CAudioCapture::SetFilename(LPCWSTR lpFilename)
{
	if(m_hWaveIn){
		LOG(INFO) +  L"Name cannot be changed during Wave capture";
		return NULL;
	}

	// if new filename doesn't include either a \ at front or .wav at the end.. add them
	int iFilenameLen = wcslen(lpFilename);
	if ((lpFilename[0] != L'\\') || (iFilenameLen < 4) ||
		(wcsicmp(lpFilename+(iFilenameLen-4),L".wav") != 0))
	{
		LPWSTR szTempStr = new WCHAR[iFilenameLen+6];
		if (lpFilename[0] != L'\\')
			wsprintf(szTempStr, L"\\%s",lpFilename);
		else
			wcscpy(szTempStr, lpFilename);


		if ((iFilenameLen < 4) || (wcsicmp(lpFilename+(iFilenameLen-4),L".wav") != 0))
			wcscat(szTempStr, L".wav");

		m_lpzFilename = setString(szTempStr);
		delete [] szTempStr;
	}
	else
	{
		m_lpzFilename = setString(lpFilename); 
	}
	return m_lpzFilename;	
}

int CAudioCapture::Start()
{
	MMRESULT res = MMSYSERR_NOERROR;

	if (!m_bAppHasFocus) // if we aren't focussed we can't use the hardware so ignore
		return res;

	m_iMaxFileSizeBytes = RoundUp((double)m_iMaxDuration/1000) * m_wfx.nAvgBytesPerSec;


	if (m_hWaveIn == NULL)
    {
		ZeroMemory(&m_waveHdrA, sizeof(WAVEHDR));
		ZeroMemory(&m_waveHdrB, sizeof(WAVEHDR));
		m_ulCapturedBytes = 0;
		
		LOG(INFO) + L"WAV: Starting the Wave capture";
		

		if ((m_hFile != INVALID_HANDLE_VALUE) ||
			((m_hFile = CreateFile(lpRawFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL)) == INVALID_HANDLE_VALUE))
		{
			if(m_pStatusEvent->hasCallback()){
				rho::Hashtable<rho::String, rho::String> statusData;
				statusData.put( rho::common::convertToStringA(L"status"), 
				rho::common::convertToStringA(L"unsuccessful") );			
				m_pStatusEvent->set(statusData);
			}
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
				LOG(WARNING) + L"Unable to allocate memory for audio capture buffers";
				waveInClose(m_hWaveIn);
				m_hWaveIn = NULL;
				delete [] m_waveHdrA.lpData;
				m_waveHdrA.lpData = NULL;
				delete [] m_waveHdrB.lpData;
				m_waveHdrB.lpData = NULL;
				if(m_pStatusEvent->hasCallback()){
					rho::Hashtable<rho::String, rho::String> statusData;
					statusData.put( rho::common::convertToStringA(L"status"), 
					rho::common::convertToStringA(L"unsuccessful") );			
					m_pStatusEvent->set(statusData);
				}
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
				LOG(WARNING) + L"WAV: Wavein: MMSYSERR_ALLOCATED";
				
			}
			else if(res == MMSYSERR_NODRIVER)
			{
				LOG(WARNING) + L"WAV: Wavein: MMSYSERR_NODRIVER";
				
			}
			else if(res == MMSYSERR_BADDEVICEID)
			{
				LOG(WARNING) + L"WAV: Wavein: MMSYSERR_BADDEVICEID";
				
			}
			else if(res == MMSYSERR_NOMEM)
			{
				LOG(WARNING) + L"WAV: Wavein: MMSYSERR_NOMEM";
				
			}
			else if(res == WAVERR_BADFORMAT)
			{
				LOG(WARNING) + L"WAV: Waven WAVERR_BADFORMAT";
				
			}
			if(m_pStatusEvent->hasCallback()){
				rho::Hashtable<rho::String, rho::String> statusData;
				statusData.put( rho::common::convertToStringA(L"status"), 
				rho::common::convertToStringA(L"unsuccessful") );			
				m_pStatusEvent->set(statusData);
			}
			LOG(WARNING) + L"WAV: Wavin failure";			
		}	
	}
	else{
		LOG(INFO) + L"WAV: Audio Capturing is already in process.";
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
			LOG(INFO) + L"WAV: WaveIn device open";
		}
		else if(uMsg == WIM_CLOSE)
		{
			LOG(INFO) + L"WAV: WaveIn device closed";
			
		}
	}
}

DWORD WINAPI CAudioCapture::threadProc(LPVOID lpParam)
{
	CAudioCapture *pAudioCapture = (CAudioCapture*)lpParam;
	if (pAudioCapture)
	{
		if(pAudioCapture->eventHandler()==AC_SAVED){
			LOG(INFO) + L"WAV: Successfully saved the file";
			return 0;
		}
		return 1;
	}
	return -1;
}

AC_RETURN CAudioCapture::eventHandler()
{
	AC_RETURN iRet = AC_FAILED;

	// no need for an event loop as we're only interested in handling one event...
	// it will either be a capture event or a cancel event
	DWORD dwEvent = WaitForMultipleObjects(2, m_hEvents, FALSE, INFINITE);

	// which ever of the two events we receive, we want to stop the wave device
	if (m_hWaveIn)
	{
		LOG(INFO) + L"WAV: WaveIn capture stopping";
		waveInStop(m_hWaveIn);

		LOG(INFO) + L"WAV: WaveIn capture resetting";
		waveInReset(m_hWaveIn);

		LOG(INFO) + L"WAV: WaveIn capture unpreparing headers";
		
		waveInUnprepareHeader(m_hWaveIn, &m_waveHdrA, sizeof(WAVEHDR));
		waveInUnprepareHeader(m_hWaveIn, &m_waveHdrB, sizeof(WAVEHDR));

		waveInClose(m_hWaveIn);
		LOG(INFO) + L"WAV: Audio recording stopped successfully";
		
		delete [] m_waveHdrA.lpData;
		m_waveHdrA.lpData = NULL;
		delete [] m_waveHdrB.lpData;
		m_waveHdrB.lpData = NULL;
	    CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;

		
		if (dwEvent == WAIT_OBJECT_0 + CAPTURE_EVENT)
		{
			// only process the raw file on capture event
			processRawFile();
			//set unsuccessful status
			if(m_pStatusEvent->hasCallback()){
				//Used for storing the appropriate recording status
				rho::Hashtable<rho::String, rho::String> statusData;
				statusData.put( rho::common::convertToStringA(L"status"), 
				rho::common::convertToStringA(L"successful") );			
				m_pStatusEvent->set(statusData);
			}
			iRet = AC_SAVED;
		}
		else if(dwEvent == WAIT_OBJECT_0 + CANCEL_EVENT){
			//set unsuccessful status
			if(m_pStatusEvent->hasCallback()){
				//Used for storing the appropriate recording status
				rho::Hashtable<rho::String, rho::String> statusData;
				statusData.put( rho::common::convertToStringA(L"status"), 
				rho::common::convertToStringA(L"unsuccessful") );			
				m_pStatusEvent->set(statusData);
			}
		}
		m_hWaveIn = NULL;
	}

	

	// delete the temporary file
	DeleteFile(lpRawFileName);

	// clean-up
	CloseHandle(m_hEvents[0]);
	m_hEvents[0] = NULL;
	CloseHandle(m_hEvents[1]);
	m_hEvents[1] = NULL;
	return iRet;

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
			LOG(WARNING) + L"Unable to get enough memory for copy operation";
			return;
		}

		// Open wave file
		hWavFile = CreateFile(m_lpzFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
		if( hWavFile == INVALID_HANDLE_VALUE ) {
			LOG(WARNING) + L"Unable to open file";
			RETAILMSG(1, (TEXT("Error opening %s. Error code = 0x%08x\n"), m_lpzFilename, GetLastError()));
			goto ERROR_EXIT;
		}

	    m_hFile = CreateFile(lpRawFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if( m_hFile == INVALID_HANDLE_VALUE ) {
			LOG(WARNING) + L"Unable to open Raw file";
			RETAILMSG(1, (TEXT("Error opening %s. Error code = 0x%08x\n"), lpRawFileName, GetLastError()));
			goto ERROR_EXIT;
		}

		// write the riff file
		if (! WriteFile(hWavFile, &FileHeader, sizeof(FileHeader), &dwBytesWritten, NULL)) {
			LOG(WARNING) + L"Error writing file header";
			goto ERROR_EXIT;
		}

		// write the wave header
		if (! WriteFile(hWavFile, &WaveHeader, sizeof(WaveHeader), &dwBytesWritten, NULL)) {
			LOG(WARNING) + L"Error writing wave header";
			goto ERROR_EXIT;
		}

		// write the wave format
		if (! WriteFile(hWavFile, &m_wfx, WaveHeader.dwSize, &dwBytesWritten, NULL)) {
			LOG(WARNING) + L"Error writing wave format";
			goto ERROR_EXIT;
		}

		// write the data header
		if (! WriteFile(hWavFile, &DataHeader, sizeof(DataHeader), &dwBytesWritten, NULL)) {
			LOG(WARNING) + L"Error writing PCM data header";
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
				LOG(WARNING) + L"Error reading raw file";
				goto ERROR_EXIT;
			}

			// write the PCM data
			if (! WriteFile(hWavFile, pCopyBuf, dwBytesRead, &dwBytesWritten, NULL)) {
				LOG(WARNING) + L"Error writing PCM data";
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


//use try/catch for mem fail
LPWSTR CAudioCapture::setString(LPCWSTR pStr)
{
	LPWSTR pTemp;
	try{
		pTemp = new WCHAR[wcslen(pStr)+1];
		wcscpy(pTemp,pStr);
	}
	catch(...){
		
		return NULL;
	}
	return pTemp;
}


void CAudioCapture::SetDuration(int iMilliSeconds)
{
	if (m_hWaveIn){
		LOG(INFO) +  L"Duration cannot be changed during Wave capture";
	}
	else{
		if(iMilliSeconds >= AC_MIN_DURATION){
			m_iMaxDuration = iMilliSeconds;
		}
		else{
			m_iMaxDuration = AC_DEFAULT_DURATION;
		}
	}
}

BOOL CAudioCapture::ApplicationFocusChange(bool bActivated)
{
	// if we've lost focus during audio capture we must release the audio hardware
	if (!bActivated && m_bAppHasFocus)
		Cancel();

	m_bAppHasFocus = bActivated;
	return TRUE;
}

BOOL CAudioCapture::cmp(LPCTSTR tc1, LPCTSTR tc2)
{
	if (!tc1 || !tc2)
		return false;
	return !_tcsicmp(tc1, tc2);

}


BOOL CAudioCapture::SetPropertyOrMethod(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	// start the parsing of the Meta tag info
	if(cmp(szPropertyName, L"fileName"))
	{
		SetFilename(szPropertyValue);
		return TRUE;
	}
	else if(cmp(szPropertyName, L"maxDuration"))
	{
		SetDuration(_ttoi(szPropertyValue));
		return TRUE;
	}
	return FALSE;
}

//  Call this to retrieve the value of an EMML Tag
BOOL CAudioCapture::RetrieveProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)
{
	if(cmp(szParameterName, L"fileName"))
	{
		wcscpy(szParameterValue,m_lpzFilename);
		return TRUE;
	}
	else if(cmp(szParameterName, L"maxDuration"))
	{
		wcscpy(szParameterValue,rho::common::convertToStringW(m_iMaxDuration).c_str());
		return TRUE;
	}	
	return FALSE;
}

void CAudioCapture::SetCallback(rho::apiGenerator::CMethodResult* pCallback){
	m_pStatusEvent = pCallback;
}
