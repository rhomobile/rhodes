#ifndef __ZMTEL_H__
#define __ZMTEL_H__

#ifdef __cplusplus
extern "C"{
#endif 

    /*
     * Connection Manager
     */
    char* getWirelessNetworks(void);
    char* getDeviceID(void);
    char* getPhoneNumber(int lineNumber);
    char* getConnectedNetworks(void);
    char* getIMEI(void);
    void sendDTMF(char key);
    void makePhoneCall(char *phoneNum);


    /*
     * Video conferencing
     */

    void init();
    void deinit();        

    char* get_local_address();
    char* get_aec();
    char* get_voice_codec();
    char* get_video_codec();

    void call_start(const char* local_ip,
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
                    int camera); /* Index of camera type */

    void call_end();

    void mute();
    void unmute();

    void pause_video();
    void resume_video();
	
	/*
	 * Others
	 */
	void storeEvent(char *event);

#ifdef __cplusplus
}; /* extern "C" */
#endif

#endif /* __ZMTEL_H__ */

/* End of file */
