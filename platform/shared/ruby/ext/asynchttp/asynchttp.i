/* asynchttp.i */
%module "Rho::AsyncHttp"
%{
/* Put header files here or function declarations like below */
	extern void rho_asynchttp_get(const char* url, VALUE headers, const char* callback, const char* callback_params, int ssl_verify_peer);
	#define do_get rho_asynchttp_get
	
	extern void rho_asynchttp_post(const char* url, VALUE headers, const char* body, const char* callback, const char* callback_params, int ssl_verify_peer);
	#define do_post rho_asynchttp_post

	extern void rho_asynchttp_downloadfile(const char* url, VALUE headers, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
	#define do_downloadfile rho_asynchttp_downloadfile

	extern void rho_asynchttp_uploadfile(const char* url, VALUE headers, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
	#define do_uploadfile rho_asynchttp_uploadfile

	extern void rho_asynchttp_cancel(const char* cancel_callback);
	#define cancel rho_asynchttp_cancel

	extern void rho_asynchttp_set_threaded_mode(int b);
	#define set_threaded_mode rho_asynchttp_set_threaded_mode
	
%}

%typemap(default) const char* cancel_callback {
 $1 = "*";
}

extern void do_get(const char* url, VALUE headers, const char* callback, const char* callback_params, int ssl_verify_peer);
extern void do_post(const char* url, VALUE headers, const char* body, const char* callback, const char* callback_params, int ssl_verify_peer);
extern void do_downloadfile(const char* url, VALUE headers, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
extern void do_uploadfile(const char* url, VALUE headers, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
extern void cancel(const char* cancel_callback);
extern void set_threaded_mode(int b);
