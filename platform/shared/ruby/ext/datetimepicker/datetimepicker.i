/* datetimepicker.i */
%module DateTimePicker
%{
	extern void choose_datetime(char* callback, char* title, int initial_time, char* format, char* opaque);
	#define choose choose_datetime
%}

%typemap(default) char* opaque {
 $1 = NULL;
}

extern void choose(char* callback, char* title, int initial_time, char* format, char* opaque);
