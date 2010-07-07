//
//  ImageProvider.h
//  rhorunner
//
//  Created by Dmitry Soldatenkov on 7/2/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//


class CImageProvider {
public:
	
	static void rho_platform_image_load_grayscale(const char* url, void** image_pixels, int* pwidth, int* pheight);
	static void rho_platform_image_free(void* image_pixels);
};