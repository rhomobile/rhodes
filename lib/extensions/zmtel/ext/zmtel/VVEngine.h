#ifndef __VVENGINE_H__
#define __VVENGINE_H__

#include <afx.h>
#include <assert.h>

#include "system/spirit_system.h"

#define MAX_BUFF_LEN			(200U)
#define MAX_AECS				(10U)
#define MAX_VOICE_CODECS		(10U)
#define MAX_VIDEO_CODECS		(10U)

class CMediaManager;

class VVEngine {

public: // Public static methods
	static VVEngine* instance()
	{
		assert(m_vvEngine);
		return m_vvEngine;
	}

	static void create();
	static void destroy();

public: // Public methods
	char* getLocalAddresses();

	char* getAEC();

	char* getVoiceCodec();
 
	char* getVideoCodec();

	bool startCall(const char* local_ip,
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
					int camera);

	bool stop();

	void mute();
	void unmute();
	void pauseVideo();
	void resumeVideo();

private: // Constructors & destructor
	VVEngine();
	~VVEngine();

private: // Data members
	static VVEngine*			m_vvEngine;		// Single instance
	CMediaManager*				m_mediaMgr;
	
	char*						m_localAddr;
	char*						m_aec;
	char						m_aecList[MAX_AECS][MAX_BUFF_LEN];

	char*						m_voiceCodec;
	char						m_voiceCodecList[MAX_VOICE_CODECS][MAX_BUFF_LEN];

	char*						m_videoCodec;
	char						m_videoCodecList[MAX_VIDEO_CODECS][MAX_BUFF_LEN];

	bool						m_callStarted;
	bool						m_video;
	bool						m_endStart;

	tCritSec					m_csLock;
};

#endif // __VVENGINE_H__ 

// End of file
