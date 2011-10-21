/* ringtones.i */
%module "Rho::RingtoneManager"
%{
extern VALUE rho_ringtone_manager_get_all();
#define get_all_ringtones rho_ringtone_manager_get_all
extern void rho_ringtone_manager_stop();
#define stop rho_ringtone_manager_stop
extern void rho_ringtone_manager_play(char* file_name);
#define play rho_ringtone_manager_play
%}

extern VALUE get_all_ringtones();
extern void stop();
extern void play(char* file_name);
