#include "config.h"
#include "zbar/include/zbar.h"

int zbar_video_init(	zbar_video_t *video,
					unsigned long format) {
	return 0;
}

int zbar_processor_set_config(zbar_processor_t *processor,
							  zbar_symbol_type_t symbology,
							  zbar_config_t config,
							  int value) {
	return 0;
}


#ifdef RHO_NEED_FAKE_SNPRINTF
int snprintf( char *buffer, int buff_size, const char *format, ... ){
	return 0;
}
#endif

#ifdef _WIN32_WCE

char* strdup (const char *src) {
	return 0;
}

#endif