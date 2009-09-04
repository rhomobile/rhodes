/* datetimepicker.i */
%module DateTimePicker
%{
	extern void choose_datetime(char* callback, char* title, long initial_time, int format, char* data);
	#define choose choose_datetime
%}

%typemap(in) long initial_time {
	VALUE t;
	long val;
	VALUE class;
	VALUE class_str;
	char *class_val;
	
	class = rb_funcall($input, rb_intern("class"), 0);
	class_str = rb_funcall(class, rb_intern("to_s"), 0);
	class_val = StringValuePtr(class_str);

	if (!strcmp(class_val, "Time") == 0) {
		rb_raise(SWIG_Ruby_ErrorType(SWIG_AttributeError), " of type '" "Time""'");
	}
	t = rb_funcall($input, rb_intern("to_i"), 0);
	val = NUM2INT(t);
	$1 = (long)val;
}

%typemap(default) char* data {
 	$1 = NULL;
}

extern void choose(char* callback, char* title, long initial_time, int format, char* data);
