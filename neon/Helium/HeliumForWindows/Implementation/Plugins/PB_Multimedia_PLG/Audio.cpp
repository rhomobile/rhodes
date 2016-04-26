
#include "Audio.h"
#include "CAudioCapture.h"

CAudio::CAudio(void)
{
	m_iDuration		= AC_DEFAULT_DURATION;
	m_pDestination	= NULL;
	m_pUser;		= NULL;
	m_pPass;		= NULL;
	m_pFileName;	= NULL;
	m_pAudio		= NULL;
	m_capturingEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
}

CAudio::~CAudio(void)
{
	delete [] m_pDestination;
	delete [] m_pUser;
	delete [] m_pPass;
	delete [] m_pFileName;
	CloseHandle(m_capturingEvent);
	delete m_pAudio;
}

CAudio::Start();
{
	if(pAudio->m_pFileName){
		m_pAudio = new CAudioCapture(m_pFileName, m_iDuration);
	}
}


CAudio::Stop();
{
	if(m_pAudio){
		m_pAudio->Stop();
	}
}

CAudio::Cancel();
{
	if(m_pAudio){
		m_pAudio->Cancel();
	}
}

//
// capture audio
//
//DWORD WINAPI CAudio::audioCaptureThread(LPVOID lParam)
//{
//	CAudio *pAudio = (CAudio *)lParam;
//	if(pAudio->m_pFileName && pAudio->m_iDuration > AC_MIN_DURATION)
//
//
//}
