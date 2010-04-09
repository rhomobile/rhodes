// #include "stdafx.h"
#include "zmtel.h"

#include "VVEngine.h"
#include "ConnManager.h"
#include "Phone.h"

/*
 * Connection Manager
 */
char* getWirelessNetworks(void)
{
    // <TODO> : Implementation
    return 0;
}

char* getDeviceID(void)
{
    return ConnManager::instance()->getDeviceID();
}

char* getPhoneNumber(int lineNumber)
{
    return ConnManager::instance()->getPhoneNumber(lineNumber);
}

char* getConnectedNetworks(void)
{
    // <TODO> : Implementation
    return 0;
}

char* getIMEI(void)
{
    // <TODO> : Implementation
    return 0;
}

void sendDTMF(char key)
{
    PhoneSendDTMFStart(key);
}

void makePhoneCall(char *phoneNum)
{
    // <TODO> : Implementation
}


/*
 * Video conferencing 
 */

void init()
{
    VVEngine::create();
}

void deinit()
{
    VVEngine::destroy();
}

char* get_local_address()
{
    return VVEngine::instance()->getLocalAddresses();
}

char* get_aec()
{
    return VVEngine::instance()->getAEC();
}

char* get_voice_codec()
{
    return VVEngine::instance()->getVoiceCodec();
}

char* get_video_codec()
{
    return VVEngine::instance()->getVideoCodec();
}

void call_start(const char* local_ip, const char* destination, int aec, int voice_codec, int video_codec,
                int main_video_x, int main_video_y, int main_video_width, int main_video_height,
                int local_video_x, int local_video_y, int local_video_width, int local_video_height,
                int camera)
{
    VVEngine::instance()->startCall(local_ip, destination, aec, voice_codec, video_codec, 
                                        main_video_x, main_video_y, main_video_width, main_video_height,
                                        local_video_x, local_video_y, local_video_width, local_video_height,
                                        camera);
}

void call_end()
{
    VVEngine::instance()->stop();
}

void mute()
{
    // <TODO> : Implementation
}

void unmute()
{
    // <TODO> : Implementation
}

void pause_video()
{
    // <TODO> : Implementation
}

void resume_video()
{
    // <TODO> : Implementation
}


/*
 * Others
 */
void storeEvent(char *event)
{
}

// End of file
