#include "stdafx.h"
#ifndef WINCE
#include "zmtel.h"
#endif //WINCE

#include <stdlib.h>
/*
 * Connection Manager
 */
char* getWirelessNetworks(void)
{
    // dummy implementation
    return NULL;
}

char* getDeviceID(void)
{
    // dummy implementation
    return NULL;
}

char* getPhoneNumber(int lineNumber)
{
    // dummy implementation
    return NULL;
}

char* getConnectedNetworks(void)
{
    // dummy implementation
    return NULL;
}

char* getIMEI(void)
{
    // dummy implementation
    return NULL;
}

void sendDTMF(char key)
{
    // dummy implementation
}

void makePhoneCall(char *phoneNum)
{
    // dummy implementation
}


/*
 * Video conferencing 
 */

void init()
{
    // dummy implementation
}

void deinit()
{
    // dummy implementation
}

char* get_local_address()
{
    // dummy implementation
    return NULL;
}

char* get_aec()
{
    // dummy implementation
    return NULL;
}

char* get_voice_codec()
{
    // dummy implementation
    return NULL;
}

char* get_video_codec()
{
    // dummy implementation
    return NULL;
}

void call_start(const char* /* local_ip */, const char* /* destination */, int /* aec */, int /* voice_codec */, int /* video_codec */,
                int /* main_video_x */, int /* main_video_y */, int /* main_video_width */, int /* main_video_height */,
                int /* local_video_x */, int /* local_video_y */, int /* local_video_width */, int /* local_video_height */,
                int /* camera */)
{
    // dummy implementation
}

void call_end()
{
    // dummy implementation
}

void mute()
{
    // dummy implementation
}

void unmute()
{
    // dummy implementation
}

void pause_video()
{
    // dummy implementation
}

void resume_video()
{
    // dummy implementation
}

// End of file
