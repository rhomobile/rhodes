#pragma once

#include <windows.h>
#include <dsound.h>

class CAudioCaptureModule;
// define the max size of each double buffer as 64K which equates to 8 seconds at low
// quality (64kbps) or 40ms at stereo CD (44.1KHz) quality.
#define MAX_CAPTURE_BUFFER_SIZE	64000

/// -----------------------------------------------------------------------------
///                              FileHeader
/// -----------------------------------------------------------------------------
typedef struct
{
   DWORD   dwRiff;     // Type of file header.
   DWORD   dwSize;     // Size of file header.
   DWORD   dwWave;     // Type of wave.
} RIFF_FILEHEADER, *PRIFF_FILEHEADER;


/// -----------------------------------------------------------------------------
///                              ChunkHeader
/// -----------------------------------------------------------------------------
typedef struct
{
   DWORD   dwCKID;        // Type Identification for current chunk header.
   DWORD   dwSize;        // Size of current chunk header.
} RIFF_CHUNKHEADER, *PRIFF_CHUNKHEADER;

///
///  Chunk Types  
///
#define RIFF_FILE       mmioFOURCC('R','I','F','F')
#define RIFF_WAVE       mmioFOURCC('W','A','V','E')
#define RIFF_FORMAT     mmioFOURCC('f','m','t',' ')
#define RIFF_CHANNEL    mmioFOURCC('d','a','t','a')

/// Event IDs
#define CAPTURE_EVENT	0
#define CANCEL_EVENT	1

enum AC_RETURN 
{ 
	AC_SAVED, AC_FAILED 
};



#define AC_DEFAULT_DURATION 20000
#define AC_MIN_DURATION		500
#define AC_DEFAULT_FILENAME L"\\AudioCapture.wav"


class CAudioCapture
{
public:
	CAudioCapture(CAudioCaptureModule *pModule, bool bHasFocus);
	~CAudioCapture();

	int Start();
	void Stop();
	void Cancel();
	void SetDuration(int iMilliSeconds);
	LPWSTR SetFilename(LPCWSTR lpFilename);
	LPWSTR setString(LPCWSTR pStr);//use try/catch for mem fail
	BOOL ApplicationFocusChange(bool bActivated);
	void ResetToDefaults();
private:
	
	unsigned int RoundUp(double dValue);
	

	static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, 
									DWORD dwParam1, DWORD dwParam2);

	static DWORD WINAPI threadProc(LPVOID lpParam);

	AC_RETURN eventHandler();
	
	void processRawFile();

	CAudioCaptureModule *m_pModule;

	unsigned int	m_iMaxDuration;
	unsigned long	m_iMaxFileSizeBytes;
	LPWSTR			m_lpzFilename;
	HANDLE			m_hFile;
	HANDLE			m_hEvents[2];
	HWAVEIN			m_hWaveIn;
	DWORD			m_dwDeviceId;		/// microphone device id
	WAVEFORMATEX	m_wfx;
	bool			m_bUseBufferA;
	WAVEHDR			m_waveHdrA;
	WAVEHDR			m_waveHdrB;
	unsigned long	m_ulCapturedBytes;
	bool			m_bAppHasFocus;
};