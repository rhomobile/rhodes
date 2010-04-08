%module zmtel
%{
extern char* getWirelessNetworks(void);
extern char* getDeviceID(void);
extern char* getPhoneNumber(int lineNumber);
extern char* getConnectedNetworks(void);
extern char* getIMEI(void);
extern void sendDTMF(char key);
extern void makePhoneCall(char *phoneNum);
extern void storeEvent(char *event);

extern void init();
extern void deinit();		
extern char* get_local_address();
extern char* get_aec();
extern char* get_voice_codec();
extern char* get_video_codec();
extern void call_start(const char* local_ip, const char* destination, int aec, int voice_codec, int video_codec, int main_video_x, int main_video_y, int main_video_width, int main_video_height, int local_video_x, int local_video_y, int local_video_width, int local_video_height, int camera);
extern void call_end();
extern void mute();
extern void unmute();
extern void pause_video();
extern void resume_video();
%}
extern char* getWirelessNetworks(void);
extern char* getDeviceID(void);
extern char* getPhoneNumber(int lineNumber);
extern char* getConnectedNetworks(void);
extern char* getIMEI(void);
extern void sendDTMF(char key);
extern void makePhoneCall(char *phoneNum);
extern void storeEvent(char *event);

extern void init();
extern void deinit();       
extern char* get_local_address();
extern char* get_aec();
extern char* get_voice_codec();
extern char* get_video_codec();
extern void call_start(const char* local_ip, const char* destination, int aec, int voice_codec, int video_codec, int main_video_x, int main_video_y, int main_video_width, int main_video_height, int local_video_x, int local_video_y, int local_video_width, int local_video_height, int camera);
extern void call_end();
extern void mute();
extern void unmute();
extern void pause_video();
extern void resume_video();
