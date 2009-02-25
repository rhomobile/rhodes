/* system.i */
%module Camera
%{
extern void take_picture(char* callback_url);
extern void choose_picture(char* callback_url);
%}

extern void take_picture(char* callback_url);
extern void choose_picture(char* callback_url);
