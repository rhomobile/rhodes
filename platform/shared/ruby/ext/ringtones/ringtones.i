/* ringtones.i */
%module RingtoneManager
%{
extern VALUE ringtone_manager_get_all();
#define get_all_ringtones ringtone_manager_get_all 
extern void ringtone_manager_stop();
#define stop ringtone_manager_stop
extern void ringtone_manager_play(char* file_name);
#define play ringtone_manager_play
%}

extern VALUE get_all_ringtones();
extern void stop();
extern void play(char* file_name);
