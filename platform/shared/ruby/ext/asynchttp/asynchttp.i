/* asynchttp.i */
%module AsyncHttp
%{
/* Put header files here or function declarations like below */
	extern void rho_asynchttp_get(const char* url, VALUE headers, const char* callback, const char* callback_params);
	#define do_get rho_asynchttp_get
	
	extern void rho_asynchttp_post(const char* url, VALUE headers, const char* body, const char* callback, const char* callback_params);
	#define do_post rho_asynchttp_post

	extern void rho_asynchttp_cancel(const char* cancel_callback);
	#define cancel rho_asynchttp_cancel

	extern void rho_asynchttp_set_threaded_mode(int b);
	#define set_threaded_mode rho_asynchttp_set_threaded_mode
	
%}

%typemap(default) const char* cancel_callback {
 $1 = "*";
}

extern void do_get(const char* url, VALUE headers, const char* callback, const char* callback_params);
extern void do_post(const char* url, VALUE headers, const char* body, const char* callback, const char* callback_params);
extern void cancel(const char* cancel_callback);
extern void set_threaded_mode(int b);