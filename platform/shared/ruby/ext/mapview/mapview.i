/* mapview.i */
%module MapView
%{
extern void mapview_create(int nparams, char** params, int nannotations, char** annotation);
#define create mapview_create
%}

%typemap(in) (int nparams, char** params, int nannotations, char** annotation) {
	VALUE input_arr = $input;
	int i, j, len = 0;
	char **ret_val, **settings;
	ret_val = settings = NULL;
	$1 = $3 = 0;
	if (input_arr) {
		VALUE main_keys_arr = rb_funcall($input, rb_intern("keys"), 0, NULL);
		int mk,main_keys_len = RARRAY_LEN(main_keys_arr);
		for(mk=0; mk<main_keys_len; mk++) {
			VALUE main_key = rb_ary_entry(main_keys_arr, mk);
			VALUE main_data = rb_hash_aref($input,main_key);
			char *main_key_str = StringValuePtr(main_key);
			if (!strcmp(main_key_str,"settings")) {
				int arr_len = 0;
				VALUE settings_keys_arr = rb_funcall(main_data, rb_intern("keys"), 0, NULL);
				int len = RARRAY_LEN(settings_keys_arr);
				settings = (char **)malloc(2*len*sizeof(char*));
				for(j=0; j<len; j++) {
					VALUE val;
					char *tmp;
					VALUE key = rb_ary_entry(settings_keys_arr, j);
					VALUE data = rb_hash_aref(main_data,key);
					char *key_str = StringValuePtr(key);
					switch(TYPE(data)) {
						case T_STRING:
							tmp = StringValuePtr(data);
							break;
						case T_ARRAY:
							{
								int alen = RARRAY_LEN(data);
								char **arr = (char **)malloc((alen+1)*sizeof(char*));
								for(i=0;i<alen;i++) {
									VALUE item = rb_ary_entry(data,i);
									if (TYPE(item) != T_STRING)
										item = rb_funcall(item, rb_intern("to_s"), 0, NULL);
									tmp = StringValuePtr(item);
									arr[i] = tmp;
								}
								arr[alen] = NULL;
								tmp = (char*)arr;
							}
							break;
						default:
							val = rb_funcall(data, rb_intern("to_s"), 0, NULL);
							tmp = StringValuePtr(val);
					}
					settings[arr_len++] = key_str;
					settings[arr_len++] = tmp;
				}
				
				$1 = arr_len;
				$2 = settings;
			} else if (!strcmp(main_key_str,"annotations")) {
				int arr_len = 0;
				len = RARRAY_LEN(main_data);
				ret_val = (char **)malloc(6*len*sizeof(char*));
				for(i=0; i<len; i++) {
					char *lat, *lon, *addr, *title, *subtitle, *url;
					VALUE hash = rb_ary_entry(main_data,i);
					VALUE keys_arr = rb_funcall(hash, rb_intern("keys"), 0, NULL);
					int keys_len = RARRAY_LEN(keys_arr);
					lat=lon="10000";
					addr=title=subtitle=url="";
					for(j=0; j<keys_len; j++) {
						VALUE val;
						char *tmp;
						VALUE key = rb_ary_entry(keys_arr, j);
						VALUE data = rb_hash_aref(hash,key);
						char *key_str = StringValuePtr(key);
						if (TYPE(data) != T_STRING)
							data = rb_funcall(data, rb_intern("to_s"), 0, NULL);
						tmp = StringValuePtr(data);
						if (!strcmp(key_str,"latitude")) {
							lat = tmp;
						} else if (!strcmp(key_str, "longitude")) {
							lon = tmp;
						} else if (!strcmp(key_str, "street_address")) {
							addr = tmp;
						} else if (!strcmp(key_str, "title")) {
							title = tmp;
						} else if (!strcmp(key_str, "subtitle")) {
							subtitle = tmp;
						} else if (!strcmp(key_str, "url")) {
							url = tmp;
						}  
					}
					ret_val[arr_len++] = lat;
					ret_val[arr_len++] = lon;
					ret_val[arr_len++] = addr;
					ret_val[arr_len++] = title;
					ret_val[arr_len++] = subtitle;
					ret_val[arr_len++] = url;
				} /* for(i=0; i<len; i++) */
				
				$3 = arr_len;
				$4 = ret_val;
			} /* if (!strcmp(key_str,"annotations")) */
		} /* for(mk=0; mk<main_keys_len; mk++) */
	} /* if (input_arr) */
	

}

%typemap(freearg) (int nparams, char** params, int nannotations, char** annotation) {
 free((void *) $2);
 free((void *) $4);
}

extern void create(int nparams, char** params, int nannotations, char** annotation);
