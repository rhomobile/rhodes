


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


void jxcoretau_init();

void jxcoretau_start(const char* main_filename, const char* xjcore_folderpath, const char* user_folderpath);

void jxcoretau_start_app(const char* filename);

bool jxcoretau_is_http_server_started();
    
    
#ifdef __cplusplus
};
#endif //__cplusplus
    

