#include "VVEngine.h"

#include <string.h>

#include "vocoders_api/vocoders_api.h"
#include "media_manager/MediaManager.h"
#include "ipconfig/ipconfig.h"

#define TEMP_BUFF_LEN							(200U)

VVEngine* VVEngine::m_vvEngine = NULL;

VVEngine::VVEngine()
: m_localAddr(NULL)
, m_aec(NULL)
, m_voiceCodec(NULL)
, m_videoCodec(NULL)
, m_callStarted(false)
, m_video(false)
, m_endStart(false)
{
	memset(m_aecList, 0, sizeof(m_aecList));
	memset(m_voiceCodecList, 0, sizeof(m_voiceCodecList));
	memset(m_videoCodecList, 0, sizeof(m_videoCodecList));

	m_mediaMgr = new CMediaManager();
	assert(m_mediaMgr);

	if(!m_mediaMgr->Create())
	{
		assert(0); // <TODO> media manager creation failed!!
		delete m_mediaMgr;
		m_mediaMgr = NULL;
	}

}

VVEngine::~VVEngine()
{
	assert(m_mediaMgr);
	m_mediaMgr->Destroy();
	delete m_mediaMgr;

	free(m_localAddr);			// allocated by _strdup
	free(m_aec);				// allocated by _strdup
	free(m_voiceCodec);			// allocated by _strdup
	free(m_videoCodec);			// allocated by _strdup
}

void VVEngine::create()
{
	assert(NULL == m_vvEngine);
	m_vvEngine = new VVEngine();
	assert(m_vvEngine);
}

void VVEngine::destroy()
{
	if(NULL != m_vvEngine)
	{
		delete m_vvEngine;
		m_vvEngine = NULL;
	}
}

char* VVEngine::getLocalAddresses()
{
	if(NULL == m_localAddr)
	{
		CIpConfig ipconfig;
		char tempAddr[TEMP_BUFF_LEN] = {0};

		if(ipconfig.Update() && ipconfig.IsExist()){
			int i = 0;
			while(1){
				CString str2=ipconfig.Next();
				CStringA  strA;
				strA.Format("%S", str2);
				strcat(tempAddr,strA.GetBuffer());

				if(!ipconfig.IsExist())
					break;
				strcat(tempAddr,"^");		
			}
		}

		m_localAddr = _strdup(tempAddr);
	}

	assert(m_localAddr);
	return m_localAddr;
}

char* VVEngine::getAEC()
{
	if(NULL == m_aec)
	{
		char tempAEC[TEMP_BUFF_LEN] = {0};

		const char ** ppAECMode = CMediaManager::GetAECModeList();
		unsigned i = 0;	
		while(1){
			strcpy(m_aecList[i], ppAECMode[i]);
			strcat(tempAEC, ppAECMode[i]);

			++i;
			if(!ppAECMode[i])
				break;
			strcat(tempAEC,"^");		

			assert(MAX_AECS >= i);
		}
		m_aec = _strdup(tempAEC);
	}

	assert(m_aec);
	return m_aec;
}

char* VVEngine::getVoiceCodec()
{
	if(NULL == m_voiceCodec)
	{
		char tempCodec[TEMP_BUFF_LEN] = {0};	
		const tAudioCodecDescSettings* pACDS = CVoiceChannel::GetCodecList();
		unsigned i = 0;
		while(1){		
			strcpy(m_voiceCodecList[i], pACDS[i].pVoiceCodec->pFriendlyName);
			strcat(tempCodec, pACDS[i].pVoiceCodec->pFriendlyName);
			
			++i;
			if(!pACDS || !pACDS[i].pVoiceCodec)
				break;
			strcat(tempCodec, "^");

			assert(MAX_VOICE_CODECS >= i);
		}

		m_voiceCodec = _strdup(tempCodec);
	}

	assert(m_voiceCodec);
	return m_voiceCodec;
}

char* VVEngine::getVideoCodec()
{
	if(NULL == m_videoCodec)
	{
		char tempCodec[TEMP_BUFF_LEN] = {0};
		const tVideoCodecDescSettings* pVCDS = CVideoChannel::GetCodecList();	
		unsigned i = 0;	
		while(1){
			strcpy(m_videoCodecList[i], pVCDS[i].pVideoCodec->pFriendlyName);
			strcat(tempCodec, pVCDS[i].pVideoCodec->pFriendlyName);		
			++i;

			if(!pVCDS || !pVCDS[i].pVideoCodec)
				break;
			strcat(tempCodec, "^");

			assert(MAX_VIDEO_CODECS >= i);			
		}
		m_videoCodec = _strdup(tempCodec);
	}

	assert(m_videoCodec);
	return m_videoCodec;
}

bool VVEngine::startCall(const char* local_ip,
				const char* destination,
				int aec, /* Index of AEC returned by get_aec()*/
				int voice_codec, /* Index of voice codec returned by get_voice_codec()*/
				int video_codec, /* Index of video codec returned by get_video_codec()*/
				int main_video_x,
				int main_video_y,
				int main_video_width,
				int main_video_height,
				int local_video_x,
				int local_video_y,
				int local_video_width,
				int local_video_height,
				int camera)
{
	assert(false == m_callStarted);
	assert(m_mediaMgr);

	if(m_callStarted)
		return false;

	m_callStarted = m_video = true;

	m_mediaMgr->SetLocalIp(local_ip);
	m_mediaMgr->SetRemoteIp(destination);

	assert(m_aecList[aec]);
	m_mediaMgr->SetAECMode(m_aecList[aec]);

	assert(m_voiceCodecList[voice_codec]);
	m_mediaMgr->SetVoiceCodec(m_voiceCodecList[voice_codec]);

	assert(m_videoCodecList[video_codec]);
	m_mediaMgr->SetVideoCodec(m_videoCodecList[video_codec]);

	m_mediaMgr->SetCamera(camera + 1); 
	m_mediaMgr->SetLocalWindow(local_video_x, local_video_y, local_video_width, local_video_height);
	m_mediaMgr->SetRemoteWindow(main_video_x, main_video_y, main_video_width, main_video_height);

	bool endStart = true;
	bool res = m_mediaMgr->Start(m_video);
	{
		tLock lock(m_csLock);
		m_endStart = endStart;
	}

	assert(res);
	return res;
}

bool VVEngine::stop()
{
	assert(m_callStarted);
	assert(m_mediaMgr);

	bool res = m_mediaMgr->Stop();

	assert(res);
	m_callStarted = false;
	m_video = false;

	return res;
}

void VVEngine::mute()
{
	assert(m_callStarted);
	assert(m_mediaMgr);

	m_mediaMgr->Mute();
}

void VVEngine::unmute()
{
	assert(m_callStarted);
	assert(m_mediaMgr);
	m_mediaMgr->Unmute();
}

void VVEngine::pauseVideo()
{
	assert(m_callStarted);
	assert(m_mediaMgr);

	m_mediaMgr->PauseVideo();
}

void VVEngine::resumeVideo()
{
	assert(m_callStarted);
	assert(m_mediaMgr);

	m_mediaMgr->ResumeVideo();
}

// End of file
