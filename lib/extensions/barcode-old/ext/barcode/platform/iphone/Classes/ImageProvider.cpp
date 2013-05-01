
#include "ImageProvider.h"




extern "C"
{

void rho_platform_image_load_grayscale(const char* url, void** image_pixels, int* pwidth, int* pheight) {
	*image_pixels = 0;
	*pwidth = 0;
	*pheight = 0;
	CImageProvider::rho_platform_image_load_grayscale(url, image_pixels, pwidth, pheight);
}

void rho_platform_image_free(void* image_pixels) {
	CImageProvider::rho_platform_image_free(image_pixels);	
}



};


