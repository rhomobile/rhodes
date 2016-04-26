#pragma once

#include <windows.h>
#include "..\..\Common\Public\pb_defines.h"

class CMediaPlayerModule;

enum MP_TYPE
{
	MP_TYPE_INVALID	= 0,
	MP_TYPE_AUDIO,
	MP_TYPE_VIDEO,
};

class CMediaPlayer
{
public:

	/**
	* Constructor
	*/
	CMediaPlayer(CMediaPlayerModule *pModule, bool bHasFocus);

	/**
	* Destructor
	*/
	~CMediaPlayer();

	/**
	* Start playing the preconfigured media file
	*/
	void Start();

	/**
	* Stop playing the media
	*/
	void Stop();

	/**
	* Set the type of the media file to play from an enumeration.
	* /param eType  video or audio.  See MP_TYPE above.
	*/
	void SetType(MP_TYPE eType);

	/**
	* Set the type of the media file to play from a string.
	* /param szType  "video" or "audio" (not case sensitive).
	*/
	void SetType(LPCWSTR szType);

	/**
	* Set the URI (filename/location) of the media to play.
	* /param lpFilename  NULL terminate wide string containing URI or the media file.
	* /return pointer to the new filename/URI.
	*/
	LPWSTR SetFilename(LPCWSTR lpFilename);

	/**
	* Get the current media file URI
	* /return pointer to the current URI (NULL terminated wide string).
	*/
	LPCWSTR GetFilename() {return m_lpzFilename;};

	/**
	* Kill any running player and release the lock on the media file
	*/
	static void KillPlayer();

	/**
	* Handler for onAppFocus event.
	* /param bActivated  the NEW focus state.
	* /return TRUE indicates successful transition.
	*/
	BOOL ApplicationFocusChange(bool bActivated);

private:
	
	CMediaPlayerModule *m_pModule;		/// Module pointer

	MP_TYPE			m_eType;				/// Media type of file
	LPWSTR			m_lpzFilename;			/// URI of file
	bool			m_bAppHasFocus;			/// Current focus state
	static HANDLE	m_hPlayThread;			/// Handle to player thread
	static HANDLE	m_hPlayProcess;			/// Handle to player process
	bool			m_bDeleteMediaFile;		/// Flag to indicate whether the file should be deleted after playing

	/**
	* ThreadProc for audio player thread.
	* /param lparam  this pointer (CMediaPlayer).
	* /return Exit code (S_OK or S_FALSE depending on whether the sound could be played or not.
	*/
	static DWORD WINAPI playThreadProc(LPVOID lparam);

	/**
	* IMO Callback function called when the file transfer IMO has completed
	* a file download (see transfer below and CMediaPlayerModule::GetFile()).
	* /param pvars   struct containing the transfer result.
	* /param id      not used.
	* /param lparam  this pointer (CMediaPlayer).
	*/
	static int getFileIMOCallback(PVARSTRUCT pvars, int id, LPARAM lparam);

	/**
	* get a buffer in heap memory to store the given wide string
	* /param pStr  Wide Character null terminated string to store.
	* /return pointer to new wide character buffer or NULL if not enough memory
	*/
	LPWSTR		setString(LPCWSTR pStr);//use try/catch for mem fail

	/**
	* local function for creating the audio player thread
	* /return  whether the thread was successfully created or not.
	*/
	bool		playLocalAudio();

	/**
	* Create a WMPlayer (for WM) or CEPlayer (for CE) process to play video media.
	* /return  whether or not the player could be launched.
	*/
	bool		launchPlayer();
};
