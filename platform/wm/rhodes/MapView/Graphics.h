#pragma once

#include "stdafx.h"

#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>

#include <imaging.h>

#include <string>
#include <string>
#include "ext/rho/rhoruby.h"
#include "common/StringConverter.h"

#include <common/rhoparams.h>
#include <common/map/MapEngine.h>


using namespace rho::common;
using namespace rho::common::map;
using namespace rho;
using namespace stdext;

/*
class IDrawingImage {
public:
	virtual ~IDrawingImage(){}
	virtual int width() const = 0;
	virtual int height() const = 0;
};

class IDrawingDevice
{
public:
	virtual ~IDrawingDevice(){}

	virtual IDrawingImage* createImage(String path) = 0;
	virtual IDrawingImage* createImage(void* p, size_t s) = 0;
	virtual void destroyImage(IDrawingImage* image) = 0;

	virtual void requestRedraw() = 0;



};

class IDrawingContext {
public:
	virtual ~IDrawingContext(){}

	virtual int getWidth() = 0;
	virtual int getHeight() = 0;

	virtual void drawImage(int x, int y, IDrawingImage* image) = 0;

	virtual void drawText(int x, int y, String &text, int color) = 0;

	virtual void getTextRect(int x, int y, String &text, RECT* resultRect) = 0;

	virtual void fillRect(int x, int y, int width, int height, int color) = 0;

	virtual void drawLine(int x1, int y1, int x2, int y2, int color) = 0;

};
*/

class DrawingImageImpl : public IDrawingImage {
public:
	DrawingImageImpl(void const *p, int size);
	DrawingImageImpl(const char* path);
	DrawingImageImpl(IImage* img);
	virtual ~DrawingImageImpl();
	
	virtual void draw(HDC hdc, int x, int y);

	virtual IDrawingImage* clone();

	virtual int width() const {return mWidth;}
	virtual int height() const {return mHeight;}

private:
	void init(const char* path, void const *p, int size, IImage* img);
	IImage* mImage;
	int mWidth;
	int mHeight;
};


class DrawingContextImpl : public IDrawingContext{
public:
	DrawingContextImpl(HDC hdc, int width, int height);
	virtual int getWidth();
	virtual int getHeight();
	virtual void drawImage(int x, int y, IDrawingImage* image);
	virtual void drawText(int x, int y, String const &text, int color);
	virtual void getTextRect(int x, int y, String &text, RECT* resultRect);
	virtual void fillRect(int x, int y, int width, int height, int color);
	virtual void drawLine(int x1, int y1, int x2, int y2, int color);


private:
	HDC mHDC;
	int mWidth;
	int mHeight;
};

class DrawingDeviceImpl : public IDrawingDevice
{
public:

	virtual IDrawingImage* createImage(String const &path) {
		return new DrawingImageImpl(path.c_str());
	}
	virtual IDrawingImage* createImage(void const *p, size_t s) {
		return new DrawingImageImpl(p, s);
	}

	virtual IDrawingImage* cloneImage(IDrawingImage *image) {
		if (image == NULL) {
			return NULL;
		}
		return ((DrawingImageImpl*)image)->clone();
	}

	virtual void destroyImage(IDrawingImage* image) {
		delete image;
	};

	virtual void requestRedraw(){}
};