#include "../../../shared/generated/cpp/MediaplayerBase.h"
#include "net/INetRequest.h"
#include "RhoFile.h"

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

	static HANDLE	m_hMediaThread;			/// Handle to play media thread
	static HANDLE	m_hPlayThread;			/// Handle to play audio wav file thread
	static HANDLE m_hStopMediaEvent;
	String szVideoFileName;
	static StringW  lFilename;
	static bool m_bIsRunning;               //start and stop of playthread decided by this flag
	~CMediaplayerSingleton()
	{
		CloseHandle(m_hMediaThread);
		m_hMediaThread = NULL;
		CloseHandle(m_hPlayThread);
		m_hMediaThread = NULL;
		CloseHandle(m_hStopMediaEvent);
		m_hMediaThread = NULL;
	}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
	
    rho::StringW replaceString(rho::StringW inputString,
        rho::StringW toReplace,
        rho::StringW replaceWith)
    {		
        size_t f;
        while( (f= inputString.find(toReplace))!= -1)
        {
            inputString.replace(f, toReplace.length(), replaceWith);
        }
        return inputString;
    }
	static DWORD WINAPI mediaThreadProc(LPVOID lpParam)
	{
		DWORD dwRet = S_OK;
		CMediaplayerSingleton *pMediaplayer = (CMediaplayerSingleton *)lpParam;

		if (pMediaplayer)
		{
			m_bIsRunning = true;
			pMediaplayer->playMedia();
			m_bIsRunning = false;
		}		

		return dwRet;
	}
	void playMedia()
	{
		
		LOG(INFO) + __FUNCTION__ + " starting media player.";

		// Check that the filename is not empty or NULL.
		if (!szVideoFileName.empty())
		{
			PROCESS_INFORMATION pi;
			StringW m_lpzFilename;

			if (String_startsWith(szVideoFileName, "http://") || String_startsWith(szVideoFileName, "https://"))
			{
				// Networked code
				LOG(INFO) + __FUNCTION__ + "Attempting to download the file. " + szVideoFileName;
				NetRequest oNetRequest;
				Hashtable<String,String> mapHeaders;
				bool overwriteFile = true;
				bool createFolders = false;
				bool fileExists = false;
				//get the file extention 
				String szFileExtn;
				size_t extIndex = szVideoFileName.rfind(".");
				if(-1 != extIndex)
				{
					szFileExtn = szVideoFileName.substr( extIndex );
					if(!szFileExtn.empty())
					{
						String& newfilename = String("download") + szFileExtn;

						//delete the folder	
						rho::common::CRhoFile::deleteFile(newfilename.c_str());
				

						// Call the download function with the url and new filename the temp filename to be used.
						net::CNetRequestHolder *requestHolder = new net::CNetRequestHolder();
						requestHolder->setSslVerifyPeer(false);

						NetResponse resp = getNetRequest(requestHolder).pullFile( szVideoFileName, newfilename, NULL, &mapHeaders,overwriteFile,createFolders,&fileExists);

						delete requestHolder;

						if (!resp.isOK())
						{
							LOG(INFO) + __FUNCTION__ + " Could not download the file";
							return; // Don't attempt to play the file.
						}
						else
						{
							LOG(INFO) + __FUNCTION__ + " Could download the file";
							m_lpzFilename = s2ws(newfilename);
						}


					}
					else
					{
						LOG(INFO) + __FUNCTION__ + " file extention is empty ";
					}
				}
				else
				{
					LOG(INFO) + __FUNCTION__ + " invalid file extention ";

				}
			}
			else
			{
				// Local file, just change the name to a format the WM/CE understands.
				m_lpzFilename = s2ws(szVideoFileName);
			}
			if(!m_lpzFilename.empty())
			{
				String szFile = rho::common::convertToStringA(m_lpzFilename.c_str());
				if(rho::common::CRhoFile::isFileExist(szFile.c_str()))
				{


					/****************************************/
					/* 
					SR ID - EMBPD00142480
					Issue Description - Video files are not getting played in CE devices through Rhoelements application
					Fix Provided - WinCE devices accepts only back slash in the path to play media files.
					Also CE CreateProcess API doesn't accept quates in the API for meadia file path to handle space.
					A conditional fix is given for WM devices and CE devices.
					Developer Name - Sabir VT
					File Name - Mediaplayer_impl.cpp
					Function Name - startvideo
					Date - 04/07/2014
					*/
					/****************************************/
					bool bRunningOnWM = false;
					OSVERSIONINFO osvi;
					memset(&osvi, 0, sizeof(OSVERSIONINFO));
					osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
					GetVersionEx(&osvi);
					bRunningOnWM = (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) ||
						(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1);		
					if(bRunningOnWM)
					{
						//for WM devices, we enter here

						/****************************************/
						/* 
						SR ID - EMBPD00128123
						Issue Description - Video Files are not getting played in MediaPlayer with Native Application on WM
						Fix Provided - Issue was reproducing because of CreateProcess Microsoft API which donot understand the path which consists of space.
						Hence we need to put an extra double inverted at the front & at the end of the string.
						Developer Name - Abhineet Agarwal
						File Name - Mediaplayer_impl.cpp
						Function Name - startvideo
						Date - 09/06/2014
						*/
						/****************************************/
						m_lpzFilename = L"\"" + m_lpzFilename + L"\"";              
					}
					else
					{
						//for CE devices we enter here
						//replace front slash with back slash if the path contains front slash
						m_lpzFilename = replaceString(m_lpzFilename, L"/", L"\\");
					}		


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


					}
					//let us wait here till some once closes the process
					HANDLE hEvents[2] = {m_hStopMediaEvent, pi.hProcess}; 					
					DWORD dwEvent = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);					  
					if(0 == dwEvent)
					{
						TerminateProcess(pi.hProcess, -1);						
					}
					LOG(INFO) + __FUNCTION__ + " windows media player is going to be terminated.";
					WaitForSingleObject(pi.hProcess, INFINITE);
					LOG(INFO) + __FUNCTION__ + " windows media player terminated.";
					CloseHandle(pi.hThread);				
					CloseHandle(pi.hProcess);
					LOG(INFO) + __FUNCTION__ + " stopping media player.";

				}
				else
				{
					LOG(INFO) + __FUNCTION__ + " file does not exist ";
				}
			}
			else
			{
				LOG(INFO) + __FUNCTION__ + " file name is empty ";
			}
		}



	}
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
				LOG(INFO) + __FUNCTION__ + " PlaySound function failed, API supports only wav extention ";
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
			// Download the audio file and store name in lFilename
			if(String_startsWith(filename, "http://") || String_startsWith(filename, "https://"))
			{
				rho::common::CRhoFile::deleteFile("download.wav");
				// Networked code
				LOG(INFO) + __FUNCTION__ + "Attempting to download the file. " + filename;
				NetRequest oNetRequest;
				Hashtable<String,String> mapHeaders;
				bool overwriteFile = true;
				bool createFolders = false;
				bool fileExists = false;
				String& newfilename = String("download.wav");

				// Call the download function with the url and new filename the temp filename to be used.
				net::CNetRequestHolder *requestHolder = new net::CNetRequestHolder();
				requestHolder->setSslVerifyPeer(false);
				NetResponse resp = getNetRequest(requestHolder).pullFile( filename, newfilename, NULL, &mapHeaders,overwriteFile,createFolders,&fileExists);

				delete requestHolder;

				if (!resp.isOK())
				{
					LOG(INFO) + __FUNCTION__ + " Could not download the file";
					return; // Don't attempt to play the file.
				}
				else
				{
					LOG(INFO) + __FUNCTION__ + " Could download the file";
					lFilename = s2ws(newfilename);
				}	
			}
			else
			{
				// Store the local filename away.
				lFilename = s2ws(filename);
			}

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
	}

	// Start playing a video file.
	virtual void startvideo( const rho::String& filename, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(INFO) + __FUNCTION__ + " start video called";
		if(false == m_bIsRunning)
		{
			//if thread not running
			szVideoFileName = filename;
			m_hMediaThread = CreateThread (NULL, 0, mediaThreadProc, this, 0, NULL);			   
		}
		else
		{
			LOG(INFO) + __FUNCTION__ + " media player already running";
		}
	}

	// Stop playing a video file.
    virtual void stopvideo(rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(INFO) + __FUNCTION__ + " stop video called.";
		if(true == m_bIsRunning)
		{
			//if thread running
			SetEvent(m_hStopMediaEvent);
			WaitForSingleObject(m_hMediaThread, INFINITE);
			ResetEvent(m_hStopMediaEvent);
			CloseHandle(m_hMediaThread);
			m_hMediaThread = NULL;
		}
		else
		{
			LOG(INFO) + __FUNCTION__ + " media player not running to stop.";
		}
		
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
HANDLE CMediaplayerSingleton::m_hMediaThread = NULL;
HANDLE CMediaplayerSingleton::m_hStopMediaEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
bool CMediaplayerSingleton::m_bIsRunning = false;

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