#include "../../../shared/generated/cpp/MediaplayerBase.h"

namespace rho {

using namespace apiGenerator;

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

class CMediaplayerImpl: public CMediaplayerBase
{
public:
    CMediaplayerImpl(const rho::String& strID): CMediaplayerBase()
    {
    }
};

class CMediaplayerSingleton: public CMediaplayerSingletonBase
{

	static HANDLE	m_hPlayThread;			/// Handle to player thread
	static HANDLE	m_hPlayProcess;			/// Handle to player process
	static StringW  lFilename;

    ~CMediaplayerSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);

	bool playLocalAudio()
	{
		LOG(INFO) + __FUNCTION__ + ": playLocalAudio called";
		m_hPlayThread = CreateThread(NULL, 0, (&rho::CMediaplayerSingleton::playThreadProc), this, 0, NULL);
		return (m_hPlayThread != NULL);
	}

	static DWORD WINAPI playThreadProc(LPVOID lpParam)
	{
		DWORD dwRet = S_OK;

		CMediaplayerSingleton *pMediaplayer = (CMediaplayerSingleton *)lpParam;

		if (pMediaplayer)
		{
			LOG(INFO) + __FUNCTION__ + "pMediaplayer object exists: using lFilename: " + lFilename.c_str(); 
			if (!PlaySound(lFilename.c_str(), NULL, SND_FILENAME|SND_SYNC|SND_NODEFAULT)) {
				LOG(INFO) + __FUNCTION__ + " PlaySound function failed ";
				dwRet = false;
			}

			CloseHandle(pMediaplayer->m_hPlayThread);
			pMediaplayer->m_hPlayThread = NULL;
		}

		return dwRet;
	}
	
	// Play an audio file.
	virtual void start( const rho::String& filename, rho::apiGenerator::CMethodResult& oResult)
	{
		// Check that the filename is not empty or NULL.
		if (!filename.empty() && (!m_hPlayThread))
		{
			// Store the local filename away.
			//common::convertToStringW(filename, lFilename);
			lFilename = s2ws(filename);

			// Launch the audio player.
			playLocalAudio();
		}
	}

	// Stop playing an audio file.
    virtual void stop(rho::apiGenerator::CMethodResult& oResult)
	{
		// If the player is currently playing an audio file, stop it.
		PlaySound(NULL, NULL, 0);
		LOG(INFO) + __FUNCTION__ + " Stopping audio playback";
		if (WaitForSingleObject(m_hPlayThread, 500) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hPlayThread, -1);
			CloseHandle(m_hPlayThread);
			m_hPlayThread = NULL;
		}
	}

	// Start playing a video file.
    virtual void startvideo( const rho::String& filename, rho::apiGenerator::CMethodResult& oResult)
	{
		// Check that the filename is not empty or NULL.
		if (!filename.empty() &&  (!m_hPlayProcess))
		{
			PROCESS_INFORMATION pi;
			StringW m_lpzFilename = s2ws(filename);

			// Launch the video player.
			if (!CreateProcess(L"\\windows\\WMPlayer.exe", m_lpzFilename.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))
			{
				// for WinCE CEPlayer we need to set a registry key to make sure it launches full screen
				HKEY hKey = 0;
				LPCWSTR subkey = L"SOFTWARE\\Microsoft\\CEPlayer";

				if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,0,&hKey) == ERROR_SUCCESS)
				{
					DWORD dwType = REG_DWORD;
					DWORD dwData = 1; // Set AlwaysFullSize to 1
					RegSetValueEx(hKey, L"AlwaysFullSize", 0, dwType, (BYTE*)&dwData, sizeof(dwData));
					RegCloseKey(hKey);
				}

				if (!CreateProcess(L"\\windows\\CEPlayer.exe", m_lpzFilename.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))
				{
					// if CEPlayer doesn't exist either, try VPlayer
					if (!CreateProcess(L"\\windows\\VPlayer.exe", m_lpzFilename.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))
					{
						LOG(INFO) + __FUNCTION__ +  "Error launching MediaPlayer";
						return;
					}
				}

				m_hPlayProcess = pi.hProcess;
				m_hPlayThread = pi.hThread;
			}
		}
	}

	// Stop playing a video file.
    virtual void stopvideo(rho::apiGenerator::CMethodResult& oResult)
	{
		// If the player is currently playing a video file, stop it.
		TerminateProcess(m_hPlayProcess, -1);
		WaitForSingleObject(m_hPlayProcess, 500);
		CloseHandle(m_hPlayThread);
		m_hPlayThread = NULL;
		CloseHandle(m_hPlayProcess);
		m_hPlayProcess = NULL;
		LOG(INFO) + __FUNCTION__ + " stopping video player.";
	}

    virtual void getAllRingtones(rho::apiGenerator::CMethodResult& oResult)
	{
		// Not implemented in CE
	}

    virtual void playRingTone( const rho::String& name, rho::apiGenerator::CMethodResult& oResult)
	{
		// Not implemented in CE
	}

    virtual void stopRingTone(rho::apiGenerator::CMethodResult& oResult)
	{
		// Not implemented in CE
	}
};

StringW CMediaplayerSingleton::lFilename;

HANDLE CMediaplayerSingleton::m_hPlayThread = NULL;
HANDLE CMediaplayerSingleton::m_hPlayProcess = NULL;

class CMediaplayerFactory: public CMediaplayerFactoryBase
{
    ~CMediaplayerFactory(){}
    virtual IMediaplayerSingleton* createModuleSingleton();
    virtual IMediaplayer* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Mediaplayer_extension()
{
    CMediaplayerFactory::setInstance( new CMediaplayerFactory() );
    Init_Mediaplayer_API();
}

IMediaplayer* CMediaplayerFactory::createModuleByID(const rho::String& strID)
{
    return new CMediaplayerImpl(strID);
}

IMediaplayerSingleton* CMediaplayerFactory::createModuleSingleton()
{
    return new CMediaplayerSingleton();
}

void CMediaplayerSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CMediaplayerSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}