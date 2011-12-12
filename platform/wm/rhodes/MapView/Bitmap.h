/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#pragma once

#include "stdafx.h"

#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>

#if defined(_WIN32_WCE)
#include <imaging.h>
#else
struct IImage;
#endif

#include <string>
#include <string>
#include "common/StringConverter.h"

class WMAlphaBitmap 
{
public:
	WMAlphaBitmap(IImage* img);
	virtual ~WMAlphaBitmap();
	virtual int width() const {return mWidth;}
	virtual int height() const {return mHeight;}
	
	virtual void draw(unsigned short int *buf, int lineSizeInBytes);

private:
	unsigned int *mImgBuf;
	int mWidth;
	int mHeight;
};

class WMBitmap 
{
public:
	WMBitmap(IImage* img, bool useAlpha);
	virtual public ~WMBitmap();

	virtual void draw(HDC hdc, int x, int y);

	virtual void addRef();
	virtual void release();

	virtual int width() const {return mWidth;}
	virtual int height() const {return mHeight;}

private:
//	HDC mMemoryDC;
	HBITMAP mMemoryBitmap;
	unsigned short* mBuf; // do not delete it - it controlled by Bitmap
	int mWidth;
	int mHeight;
	int mRowByteSize;
	int mReferenceCount;
	WMAlphaBitmap* mAlphaBitmap;
};


