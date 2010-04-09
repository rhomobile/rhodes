

#ifndef VIDEO_SETTINGS_H_INCLUDED
#define VIDEO_SETTINGS_H_INCLUDED

#include "media_manager_api/spirit_media_manager_types.h"
//outgoing video resolution
//In most cases CIF (352x288) or QCIF (176x144) should be used 
const int g_nVideoWidth	 = 176;
const int g_nVideoHeight = 144;

//outgoing video frame rate defined as g_nVideoFramerateNum	/ g_nVideoFramerateDenom
const int g_nVideoFramerateNum		= 15;
const int g_nVideoFramerateDenom	=  1;

//outgoing video bitrate, bits per second
const int g_nVideoBitrate	= 64*1000;

//Channel 1 remote video size and position
sSpiritVideoRect g_rectRemoteVideo1 = {30,150,g_nVideoWidth,g_nVideoHeight};

//Channel 2 remote video size and position
sSpiritVideoRect g_rectRemoteVideo2 = {30,g_nVideoHeight+150,g_nVideoWidth,g_nVideoHeight};

//Local Video Preview Rect 
sSpiritVideoRect g_rectPreview = {g_nVideoWidth,150,g_nVideoWidth/2,g_nVideoHeight/2};


#endif
