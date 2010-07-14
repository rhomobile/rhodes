//#include "rhodes/JNIRhodes.h"
#include <rhodes.h>
#include <android/log.h>
#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>


#include <stdlib.h>

#define IP_PORTION_COUNT 32 


RHO_GLOBAL void rho_platform_image_load_grayscale(const char* url, void** image_pixels, int* pwidth, int* pheight) {
	*image_pixels = 0;
	*pwidth = 0;
	*pheight = 0;


	//__android_log_write(ANDROID_LOG_INFO, "APP", "$$$$$$$$$$$$$$$$$$$$$   [ 1 ]");

	JNIEnv *env = jnienv();
	jclass bitmapf_class = rho_find_class(env, "android/graphics/BitmapFactory");
	if (!bitmapf_class) {
		RAWLOG_ERROR("rho_platform_image_load_grayscale() : can not found Bitmap class");
		return;
	}

  jclass rhofileapi_class = rho_find_class(env, "com/rhomobile/rhodes/file/RhoFileApi");
  if (!rhofileapi_class) {
    RAWLOG_ERROR("rho_platform_image_load_grayscale() : can not find RhoFileApi class");
    return;
  }

	if (url == 0) {
		return;
	}

	jstring jstrFileUrl = rho_cast<jstring>(url);
	if (!jstrFileUrl) {
		RAWLOG_ERROR("rho_platform_image_load_grayscale() : invalid URL !");
		return;
	}

  jmethodID fopen_mid = env->GetStaticMethodID(rhofileapi_class, "open", "(Ljava/lang/String;)Ljava/io/InputStream;");
  if (!fopen_mid) {
    RAWLOG_ERROR("rho_platform_image_load_grayscale() : can not find 'open' method in RhoFileApi");
    return;
  }

  jmethodID fclose_mid = env->GetStaticMethodID(rhofileapi_class, "close", "(Ljava/io/InputStream;)V");
  if (!fclose_mid) {
    RAWLOG_ERROR("rho_platform_image_load_grayscale() : can not find 'close' method in RhoFileApi");
    return;
  }

  
	jmethodID bf_mid = env->GetStaticMethodID(bitmapf_class, "decodeStream", "(Ljava/io/InputStream;)Landroid/graphics/Bitmap;");
	if (!bf_mid) {
		RAWLOG_ERROR("rho_platform_image_load_grayscale() : not identify decodeStream() !");
		return;
	}

  jobject is = env->CallStaticObjectMethod(rhofileapi_class, fopen_mid, jstrFileUrl);
  if (!is) {
    RAWLOG_ERROR("rho_platform_image_load_grayscale() : can not open stream");
    return;
  }

	jobject bitmap = env->CallStaticObjectMethod(bitmapf_class, bf_mid, is);
  env->CallStaticVoidMethod(rhofileapi_class, fclose_mid, is);
	if (!bitmap) {
		RAWLOG_ERROR("rho_platform_image_load_grayscale() : Bitmap not produced !");
		return;
	}

	jclass bitmap_class = env->GetObjectClass(bitmap);
	if (!bitmap_class) {
		RAWLOG_ERROR("rho_platform_image_load_grayscale() : Bitmap class not detected !");
		return;
	}

	jmethodID b_getPixels_mid = env->GetMethodID( bitmap_class, "getPixels", "([IIIIIII)V");
	jmethodID b_getWidth_mid = env->GetMethodID( bitmap_class, "getWidth", "()I");
	jmethodID b_getHeight_mid = env->GetMethodID( bitmap_class, "getHeight", "()I");

	if ((!b_getPixels_mid) || (!b_getWidth_mid) || (!b_getHeight_mid)) {
		RAWLOG_ERROR("rho_platform_image_load_grayscale() : Bitmap methods not recognized !");
		return;
	}

	jint width = env->CallIntMethod(bitmap, b_getWidth_mid);
	jint height = env->CallIntMethod(bitmap, b_getHeight_mid);

	unsigned char* gray_buf = new unsigned char[width*height];

	if (!gray_buf) {
		RAWLOG_ERROR2("rho_platform_image_load_grayscale() : gray buffer not created [ %d x %d ]!", width, height);
		env->DeleteLocalRef(bitmap);
		env->DeleteLocalRef(jstrFileUrl);
		return;
	}

	int portion_size = height / IP_PORTION_COUNT;
	int current_y = 0;

	jintArray bufARGB_j = env->NewIntArray(width*portion_size);
	if (!bufARGB_j) {
		RAWLOG_ERROR2("rho_platform_image_load_grayscale() : int array not created [ %d x %d ]!", width, portion_size);
		delete gray_buf;
		env->DeleteLocalRef(bitmap);
		env->DeleteLocalRef(jstrFileUrl);
		return;
	}

	while (current_y < height) {
		int current_size = portion_size;
		if ((current_y + current_size) > height) {
			current_size = height - current_y;
		}
		env->CallVoidMethod(bitmap, b_getPixels_mid, bufARGB_j, 0, width, 0, current_y, width, current_size);

		jint* bufARGB = env->GetIntArrayElements(bufARGB_j, 0);
		if (!bufARGB) {
			RAWLOG_ERROR("rho_platform_image_load_grayscale() : int array not locked for access !");
			delete gray_buf;
			env->DeleteLocalRef(bufARGB_j);
			env->DeleteLocalRef(bitmap);
			env->DeleteLocalRef(jstrFileUrl);
			return;
		}

		// now we have image in int_32(ARGB) array
		{
			// make gray image
			unsigned char* dst = gray_buf + current_y*width;
			int* src = bufARGB;
			int tk = (1<<16)/3;
			int count = width*current_size;
			int i;
			for (i = count; i > 0; i--) {
				int c = *src++;
				//dst = (R+G+B)/3;
				*dst++ = (unsigned char)((((c & 0xFF) + ((c & 0xFF00)>>8)	+ ((c & 0xFF0000)>>16))*tk)>>16);
			}
		}
		env->ReleaseIntArrayElements(bufARGB_j, bufARGB, 0); 
		current_y += current_size;
	}

	*image_pixels = gray_buf;
	*pwidth = width;
	*pheight = height;

	env->DeleteLocalRef(bufARGB_j);
	env->DeleteLocalRef(bitmap);
	env->DeleteLocalRef(jstrFileUrl);

}

RHO_GLOBAL void rho_platform_image_free(void* image_pixels) {
  if (image_pixels != 0) {
    delete image_pixels;
  }
}
