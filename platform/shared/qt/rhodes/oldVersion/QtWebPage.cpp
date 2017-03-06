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

#include "QtWebPage.h"

IMPLEMENT_LOGCLASS(QtWebPage,"JavaScript");

void QtWebPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID)
{
	QString origin = "Console message at " + sourceID + ":" + QString::number(lineNumber);
	const QByteArray asc_origin = origin.toLatin1(); 
	rho::String str_origin = ::std::string(asc_origin.constData(), asc_origin.length());
	LOG(INFO) + str_origin.c_str();

	const QByteArray asc_message = message.toLatin1(); 
	rho::String str_message = ::std::string(asc_message.constData(), asc_message.length());
	LOG(INFO) + str_message.c_str();
}
