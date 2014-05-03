#pragma once

#include <windows.h>
#include <dsound.h>
#include "logging/RhoLog.h"
#include "../shared/api_generator/MethodResult.h"
#include "common/StringConverter.h"

//class CAudioCaptureModule;
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
#define AC_MIN_DURATION		1000
#define AC_DEFAULT_FILENAME L"\\AudioCapture.wav"


class CAudioCapture
{
public:
	CAudioCapture(bool bHasFocus);
	~CAudioCapture();

	int Start();
	void Stop();
	void Cancel();
	void SetDuration(int iMilliSeconds);
	LPWSTR SetFilename(LPCWSTR lpFilename);
	LPWSTR setString(LPCWSTR pStr);//use try/catch for mem fail
	BOOL ApplicationFocusChange(bool bActivated);
	void ResetToDefaults();
	/**
	* checks if two strings are equal
	*/
	BOOL cmp(LPCTSTR tc1, LPCTSTR tc2);
	/**
	* Sets a property or a method of the card reader module
	*/
	BOOL SetPropertyOrMethod(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);

	/**
	* Retrieves a property of the card reader module
	*/
	BOOL RetrieveProperty(LPCTSTR szParameterName, WCHAR* szParameterValue);

	/**
	* Retrieves all the properties of the card reader module
	*/
	void RetrieveAllProperties(rho::apiGenerator::CMethodResult* pCallback);
	/**
	* Sets the callback function for swipe event
	*/
	void SetCallback(rho::apiGenerator::CMethodResult* pCallback);
private:
	
	unsigned int RoundUp(double dValue);
	

	static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, 
									DWORD dwParam1, DWORD dwParam2);

	static DWORD WINAPI threadProc(LPVOID lpParam);

	AC_RETURN eventHandler();
	
	void processRawFile();

	unsigned int	m_iMaxDuration;
	unsigned long	m_iMaxFileSizeBytes;
	LPWSTR			m_lpzFilename;
	HANDLE			m_hFile;
	HANDLE			m_hEvents[2];
	HWAVEIN			m_hWaveIn;
	DWORD			m_dwDeviceId; //microphone device id
	WAVEFORMATEX	m_wfx;
	bool			m_bUseBufferA;
	WAVEHDR			m_waveHdrA;
	WAVEHDR			m_waveHdrB;
	unsigned long	m_ulCapturedBytes;
	bool			m_bAppHasFocus;
	rho::apiGenerator::CMethodResult* m_pStatusEvent; //Status Event: Will give the status that the audio has been recorded succesfully or not  
};
