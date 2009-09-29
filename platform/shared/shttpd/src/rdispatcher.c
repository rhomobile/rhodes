#include <string.h>

#include "defs.h"
#include "llist.h"
#include "ruby/ext/rho/rhoruby.h"

#include "logging/RhoLog.h"

#include "common/RhoMutexLock.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RDispatcher"

#if defined(__SYMBIAN32__) || defined(OS_WINDOWS) || defined(OS_WINCE)
extern char* get_home_url();
#else
static char* localhost = "http://localhost:8080";
#endif

#ifdef __SYMBIAN32__      
extern int g_need_launch_gc;

char* g_current_url = NULL;
RHO_INIT_LOCK(change_url);

//forward declaration
void webview_set_current_location(char* url);      
#endif


typedef struct __Route {
  char* _url;
	char* _application;
	char* _model;
	char* _id;
	char* _action;
} Route, * RouteRef;

struct data_chunk {
	struct llhead	link;
  size_t size; 
	void  *data;
};

static char *trim(char *str)
{
	char *ibuf = str, *obuf = str;
  int i;

	if (str) {
		// Remove leading spaces
		for(ibuf = str; *ibuf && isspace(*ibuf); ++ibuf);
		if (str != ibuf)
			memmove(str, ibuf, ibuf - str);

		// Remove trailing spaces
		i = strlen(str);
		while (--i >= 0) {
			if (!(isspace(obuf[i])||obuf[i]==':'))
				break;
		}
		obuf[++i] = 0;
	}
	return str;
}

static int _isid(char* str) {
	if (str!=NULL) {
		int l = strlen(str);
		if ( (l>2) && (str[0]=='{') && (str[l-1]=='}') ) 
			return 1;
	}
	return 0;
}

static char* _tok(char* t) {
  char* s = NULL;
  if (t) {
    s = strchr(t,'/');
    if ( s != NULL ) {
      s[0] = '\0'; s++;
    }
  }
  return s;
}

static int 
_parse_route(RouteRef route) {
	char *actionorid,*next;
  char* url = route->_url;

  if (url[0]=='/') url++;
	
	route->_application = url;	
	if ((route->_model = _tok(url)) == NULL)
		return 0;
	
	if ((actionorid = _tok(route->_model)) == NULL)
		return 1;
	
	next = _tok(actionorid);
	
	if (_isid(actionorid)) {
		route->_id = actionorid;
		route->_action = next;
	} else {
		route->_id = next;
		route->_action = actionorid;
	}
	_tok(next);
	
	return 1;
}

static RouteRef _alloc_route(char* uri) {
  RouteRef route; char *url;
  if ( (route = malloc(sizeof(Route))) != NULL) {
    if ((url = _shttpd_strdup(uri)) != NULL) {
      memset(route, 0, sizeof(route[0]));
      route->_url = url;
      return route;
    }
    free(route);
  }
  return NULL;
}

static void _free_route(RouteRef route) {
  if (route) {
    if (route->_url) free(route->_url);
    free(route);
  }
}

static VALUE 
_create_request_hash(struct conn *c, RouteRef route, const char* body, int bodylen) {
  const char *method, *uri, *query;
  VALUE hash_headers;
  struct parsed_header* h;
  int i;

	VALUE hash = createHash();

	addStrToHash(hash, "application", 
    route->_application, strlen(route->_application));

	addStrToHash(hash, "model", 
    route->_model, strlen(route->_model));

	if (route->_action!=NULL) {
		const char* actionName = route->_action;
		addStrToHash(hash, "action", actionName, strlen(actionName));
	}
	
	if (route->_id!=NULL) {
		const char* _id = route->_id;
		addStrToHash(hash, "id", _id, strlen(_id));
	}
	
  method = _shttpd_known_http_methods[c->method].ptr;
	addStrToHash(hash, "request-method", method, strlen(method));
		
	uri = c->uri;
	addStrToHash(hash, "request-uri", uri, strlen(uri));
	
	query = c->query == NULL ? "" : c->query;
	addStrToHash(hash, "request-query", query, strlen(query));
	
	hash_headers = createHash();
  h = &c->ch.cl;
	for (i = 0; i < sizeof(struct headers)/sizeof(struct parsed_header); i++) {
		if (h->_name) {
			char* name = trim(_shttpd_strdup(h->_name));
			if (h->_type == HDR_STRING) {
				addStrToHash(hash_headers,name,h->_v.v_vec.ptr,h->_v.v_vec.len);
			} else if (h->_type == HDR_INT) {
				addIntToHash(hash_headers, name, h->_v.v_big_int);
			} else if (h->_type == HDR_DATE) {
				addTimeToHash(hash_headers, name, h->_v.v_time);
			}
			free(name);
		}
		h++;
	}
	addHashToHash(hash,"headers",hash_headers);
	
  if (bodylen > 0) {
		addStrToHash(hash, "request-body", body, bodylen);
	}
	
	return hash;
}

/*
static char* 
_rho_resolve_index(char* url,char* path,const char *index_names) {
	char file[FILENAME_MAX];
	char indexfile[128];
	struct stat	st;
	char* resolved_url;
	const char* slash = path[strlen(path)-1] == '/' ? "" : "/";
	int full_len,url_len,len;

	FOR_EACH_WORD_IN_LIST(index_names, len) {
		strncpy(indexfile,index_names,len);
		_shttpd_snprintf(file, sizeof(file), "%s%s%s", path, slash, indexfile);
		if ( (_shttpd_stat(file, &st) == 0) && (!S_ISDIR(st.st_mode)) ) {
			
			//if ( i == 0 ) {// there is a controller in this folder
			//	return url;
			//}
			
			url_len = strlen(url);
			slash = url[strlen(url)-1] == '/' ? "" : "/";

			full_len = url_len + strlen(slash)+len+1;
			resolved_url = malloc(full_len);
			_shttpd_snprintf(resolved_url, full_len, "%s%s%s", url, slash, indexfile);	
			free(url);
			
			return resolved_url;
		}
	}
	return url;
}
*/

static char* 
_rho_resolve_index(char* url,char* path,const char *index_names) {
	char filename[FILENAME_MAX];
	struct stat	st;

	//check if there is controller.rb
	int len = strlen(path);
	char* slash = path[len-1] == '\\' || path[len-1] == '/' ? "" : "/";
	_shttpd_snprintf(filename,sizeof(filename),"%s%s%s",path,slash,"controller.iseq");
	if ((_shttpd_stat(filename, &st) == 0)&&(!S_ISDIR(st.st_mode))) {
	  return url;
	}
	
	len = strlen(url);
	if (url[len-1]!='/') {
		char* tmp_url = malloc(len+2);
		_shttpd_snprintf(tmp_url,len+2,"%s/",url);
		free(url);
		return tmp_url;
	} 

	return url;				
}

char* rho_resolve_url(char* url, const char* root,const char *index_names) {
	char path[URI_MAX];
	struct stat	st;
	char* tmp_url;
	char* ret;
	int full_len;

	char* full_path = strstr(url,"http://");
	if (full_path) {
		return full_path;
	}

	if (strlen(url) + strlen(root) >= sizeof(path)) {
		tmp_url = url;
	} else {
		_shttpd_snprintf(path, sizeof(path), "%s%s", root, url);
		if ( _shttpd_stat(path, &st) == -1 ) {
			tmp_url = url;				
		} else if ( S_ISDIR(st.st_mode) ) {
			tmp_url = _rho_resolve_index(url,path,index_names);
		} else {
			tmp_url = url;				
		}
	}
	
#if defined(__SYMBIAN32__) || defined(OS_WINDOWS) || defined(OS_WINCE)	
	full_len = strlen(get_home_url())+strlen(tmp_url)+1;
	ret = malloc(full_len);
	_shttpd_snprintf(ret, full_len, "%s%s", get_home_url(), tmp_url);	
#else		
	full_len = strlen(localhost)+strlen(tmp_url)+1;
	ret = malloc(full_len);
	_shttpd_snprintf(ret, full_len, "%s%s", localhost, tmp_url);	
#endif	

	free(tmp_url);
	
	return ret;
}

extern void rho_sync_addobjectnotify_bysrcname(const char* szSrcName, const char* szObject);
void* rho_dispatch(struct conn *c, const char* path) {
  RouteRef route;
  
  if ( _shttpd_match_extension(c->uri,"css,js,html,htm,png,bmp,jpg") )
    return NULL;

#ifdef __SYMBIAN32__
  if ( strstr(_shttpd_known_http_methods[c->method].ptr, "GET" ) )
	  webview_set_current_location(c->uri);
#endif
  
  if ((route = _alloc_route(c->uri)) != NULL) {
    if (_parse_route(route)) {
      struct stat	st;

      //is this an actual file or folder
      if (_shttpd_stat(path, &st) != 0)
        return route;      
      //is this a folder
      if (S_ISDIR(st.st_mode)) {
        //check if there is controller.rb to run
        char	filename[FILENAME_MAX];
        int len = strlen(path);
        char* slash = path[len-1] == '\\' || path[len-1] == '/' ? "" : "/";
        _shttpd_snprintf(filename,sizeof(filename),"%s%s%s",path,slash,"controller.iseq");

        if ((_shttpd_stat(filename, &st) == 0)&&(!S_ISDIR(st.st_mode))) {
          RAWLOG_INFO1("Run controller on this url: %s", c->uri);
          return route;
        }
      }
    }
    _free_route(route);
  }

  return NULL;
}

int collect_data(struct llhead *plist, const void* data, size_t len) {
    struct data_chunk *chunk;

	if ((chunk = malloc(sizeof(*chunk))) != NULL) 
    {
        if ((chunk->data = malloc(len)) != NULL) 
        {
            memcpy(chunk->data,data,len);
            chunk->size = len;
            LL_TAIL(plist, &chunk->link);
            return len;
        }
        free(chunk);
	}
    return -1;
}

static void
collected_data_destructor(struct llhead *lp)
{
	struct data_chunk *chunk = LL_ENTRY(lp, struct data_chunk, link);
	free(chunk->data);
	free(chunk);
}

static void*
get_collected_data(struct llhead *head, int nchunks, size_t datasize) {
  struct data_chunk *chunk;
  struct llhead	*lp, *tmp;
  char* data = NULL;

  if (nchunks == 1) {
    chunk = LL_ENTRY(head->next, struct data_chunk, link);
    data = chunk->data;
    free(chunk);
  } else if (nchunks > 1) {
    char *p = data = malloc(datasize);
    LL_FOREACH_SAFE(head, lp, tmp) {
		  LL_DEL(lp);
      chunk = (struct data_chunk *)lp;
      memcpy(p, chunk->data, chunk->size);
      p += chunk->size;
		  collected_data_destructor(lp);
	  }
  }
  
  return data;
}

struct rho_write_state {
    VALUE   data;		 
    size_t	nDataLen; /* Content-Length	*/
    size_t	nRead; /* Number of bytes read	*/
};

void rho_write_data(struct shttpd_arg *arg)
{
    struct rho_write_state* state = arg->state;

    if ( state->nRead < state->nDataLen )
        state->nRead += shttpd_printf(arg, "%s", getStringFromValue(state->data) + state->nRead);

    if ( state->nRead >= state->nDataLen )
    {
        arg->flags |= SHTTPD_END_OF_OUTPUT;
        releaseValue(state->data);

        free(arg->state);
        arg->state = NULL;
    }
}

void rho_create_write_state(struct shttpd_arg *arg, VALUE data)
{
    struct rho_write_state* state = 0;
	arg->state = state = calloc(1, sizeof(struct rho_write_state));

    state->data = data;
    state->nDataLen = getStringLenFromValue(data);
#if defined(OS_WINDOWS)
	{
		char* reply = getStringFromValue(state->data);
		RAWTRACE1("Reply: [%s]",reply);
	}
#endif
}

void rho_serve(struct shttpd_arg *arg) {
	
	//const char	*s;
    struct rho_read_state {
        size_t	cl;		 /* Content-Length	*/
        size_t	nread; /* Number of bytes read	*/
        int     nchunks; /* Number of chunks in the list */
        struct llhead	post_data;
    } *state;

	/* If the connection was broken prematurely, cleanup */
	if (arg->flags & SHTTPD_CONNECTION_ERROR) {
        if (arg->state) {

            if (arg->user_data){ //Read request
                state = arg->state;
                free_list(&state->post_data, collected_data_destructor);
            }else
                releaseValue( ((struct rho_write_state*)arg->state)->data);

            free(arg->state);
        }

        if (arg->user_data)
            _free_route(arg->user_data);

        arg->user_data = NULL;
        arg->state = NULL;

        return;
	} 

    if ( !arg->user_data ) {//Request read. Return response
        rho_write_data(arg);
        return;
    }

    if (arg->state == NULL) {
		/* New request. Allocate a state structure. */
		arg->state = state = calloc(1, sizeof(*state));
        state->cl = ((struct conn *)(arg->priv))->ch.cl._v.v_big_int;
        state->nchunks = 0;
        LL_INIT(&state->post_data);
	} 
    
	state = arg->state;

	/* Collect the POST data */
    if (arg->in.len > 0) {
      if ( collect_data(&state->post_data,arg->in.buf, arg->in.len) == -1 ) {
        shttpd_printf(arg, "HTTP/1.0 500 Out of memory\n\n");
	        arg->flags |= SHTTPD_END_OF_OUTPUT;
        return;
      }

      state->nchunks++;
      state->nread += arg->in.len;
	  /* Tell SHTTPD we have processed all data */
	  arg->in.num_bytes = arg->in.len;
    }

	/* Read all input? Call ruby framework with collected data (if any) */
	if (state->nread >= state->cl) {
        void* data = get_collected_data(&state->post_data, state->nchunks, state->nread);
        VALUE req = _create_request_hash(
            arg->priv, (RouteRef) arg->user_data, data, state->nread );
        #ifdef __SYMBIAN32__      
            g_need_launch_gc = 1;
        #endif      

        //shttpd_printf(arg, "%s", callFramework(req));
//        _free_route(arg->user_data);
//        arg->user_data = NULL;
//        free(state);
        //arg->flags |= SHTTPD_END_OF_OUTPUT;

        rho_create_write_state(arg,callFramework(req));
        {
            RouteRef route = arg->user_data;
            if ( _isid(route->_id) )
            {
              rho_sync_addobjectnotify_bysrcname(route->_model, route->_id);
            }
        }

        _free_route(arg->user_data);
        arg->user_data = NULL;
        free(state);

        rho_write_data(arg);
	}
}

int isindex(struct conn *c, char* path) {
	int pathlen = strlen(path);
	const char *s = c->ctx->options[OPT_INDEX_FILES];
	int	len;

	FOR_EACH_WORD_IN_LIST(s, len) {
		if ( len > pathlen )
			continue;
		if( strncmp(path+pathlen-len,s,len) == 0 )
			return 1;
	}
	return 0;
}

void rho_serve_index(struct shttpd_arg *arg) {

	if (arg->flags & SHTTPD_CONNECTION_ERROR) {
        if (arg->state){
            releaseValue( ((struct rho_write_state*)arg->state)->data);
	        free(arg->state);
        }
        if ( arg->user_data )
            free(arg->user_data);

        arg->state = NULL;
        arg->user_data = NULL;

        return;
	}

    if (arg->state == NULL) {
        rho_create_write_state(arg,callServeIndex(arg->user_data));

        free(arg->user_data);
        arg->user_data = NULL;
	}
    rho_write_data(arg);

	//shttpd_printf(arg, "%s", callServeIndex(arg->user_data));
	//free(arg->user_data);
	//arg->flags |= SHTTPD_END_OF_OUTPUT;
}

#ifdef __SYMBIAN32__
//this is platform specific function
extern void webview_navigate(char* url);

char* webview_current_location() {
		char* retval = NULL;
		
		RHO_LOCK(change_url);

		retval = strdup(g_current_url);
	    
	    RHO_UNLOCK(change_url);
	    
	    return retval;
	}

void webview_set_current_location(char* url) {
	
	RHO_LOCK(change_url);
	
	if ( g_current_url )
		free(g_current_url);

	g_current_url = strdup(url);
	
	RHO_UNLOCK(change_url);
}

void webview_refresh() {
	
	char* url = webview_current_location();
	if ( url )
	{
		webview_navigate(url);
		free(url);
	}
}
#endif
