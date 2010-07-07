
/*
Image Provider functions must be implement on each platform !!!
*/


#ifdef __cplusplus
extern "C" {
#endif

void rho_platform_image_load_grayscale(const char* url, void** image_pixels, int* pwidth, int* pheight);
void rho_platform_image_free(void* image_pixels);

#ifdef __cplusplus
}
#endif
