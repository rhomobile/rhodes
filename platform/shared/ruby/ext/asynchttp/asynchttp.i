/* asynchttp.i */
%module "Rho::AsyncHttp"
%{
/* Put header files here or function declarations like below */
	extern VALUE rho_asynchttp_get(const char* url, VALUE headers, const char* callback, const char* callback_params, int ssl_verify_peer);
	#define do_get rho_asynchttp_get
	
	extern VALUE rho_asynchttp_post(const char* url, VALUE headers, const char* body, const char* callback, const char* callback_params, int ssl_verify_peer);
	#define do_post rho_asynchttp_post

	extern VALUE rho_asynchttp_downloadfile(const char* url, VALUE headers, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
	#define do_downloadfile rho_asynchttp_downloadfile

	extern VALUE rho_asynchttp_uploadfile(const char* url, VALUE headers, const char* body, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
	#define do_uploadfile rho_asynchttp_uploadfile

	extern void rho_asynchttp_cancel(const char* cancel_callback);
	#define cancel rho_asynchttp_cancel

%}

%typemap(default) const char* cancel_callback {
 $1 = "*";
}

extern VALUE do_get(const char* url, VALUE headers, const char* callback, const char* callback_params, int ssl_verify_peer);
extern VALUE do_post(const char* url, VALUE headers, const char* body, const char* callback, const char* callback_params, int ssl_verify_peer);
extern VALUE do_downloadfile(const char* url, VALUE headers, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
extern VALUE do_uploadfile(const char* url, VALUE headers, const char* body, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
extern void cancel(const char* cancel_callback);
