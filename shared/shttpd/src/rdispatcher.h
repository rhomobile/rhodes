#ifndef RDISPATCHER_HEADER_INCLUDED
#define	RDISPATCHER_HEADER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void* rho_dispatch(struct conn *c, const char* path);
void  rho_serve(struct shttpd_arg *arg);
int   isindex(struct conn *c, char* path);
void  rho_serve_index(struct shttpd_arg *arg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*RDISPATCHER_HEADER_INCLUDED*/
