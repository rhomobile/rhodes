#include <string.h>

#include "defs.h"
#include "llist.h"
#include "rhoruby.h"

#ifdef __SYMBIAN32__      
      extern int g_need_launch_gc;
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

void* rho_dispatch(struct conn *c, const char* path) {
  RouteRef route;

  if ( _shttpd_match_extension(c->uri,"css,js,html,htm,png,bmp,jpg") )
    return NULL;

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
          printf("Run controller on this url: %s\n", c->uri);
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
    char* data;		 
    size_t	nDataLen; /* Content-Length	*/
    size_t	nRead; /* Number of bytes read	*/
};

void rho_write_data(struct shttpd_arg *arg)
{
    struct rho_write_state* state = arg->state;

    if ( state->nRead < state->nDataLen )
        state->nRead += shttpd_printf(arg, "%s", state->data + state->nRead);

    if ( state->nRead >= state->nDataLen )
    {
        arg->flags |= SHTTPD_END_OF_OUTPUT;
        free(arg->state);
        arg->state = NULL;
    }
}

void rho_create_write_state(struct shttpd_arg *arg, char* data)
{
    struct rho_write_state* state = 0;
	arg->state = state = calloc(1, sizeof(struct rho_write_state));

    state->data = data;
    state->nDataLen = strlen(data);
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
            }

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
        _free_route(arg->user_data);
        arg->user_data = NULL;
        free(state);
        //arg->flags |= SHTTPD_END_OF_OUTPUT;

        rho_create_write_state(arg,callFramework(req));
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
        if (arg->state)
	        free(arg->state);
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
