/* alert.i */
%module Alert
%{
extern void alert_show_popup(char* message);
#define show_popup alert_show_popup 
extern void alert_vibrate(void*);
#define vibrate alert_vibrate
extern void alert_play_file(char* file_name, ...);
#define play_file alert_play_file 
%}

extern void show_popup(char* message);
extern void vibrate(...);
extern void play_file(char* file_name, ...);
