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

#ifndef QTWEBINSPECTOR_H
#define QTWEBINSPECTOR_H

#include <QWidget>
#include <QWebInspector>

namespace Ui {
    class QtWebInspector;
}

class QtWebInspector : public QWidget
{
    Q_OBJECT

public:
    explicit QtWebInspector(QWidget *parent = 0);
    ~QtWebInspector();
    QWebInspector* webInspector(void);
    void addWebInspector(QWebInspector* wi);
    void removeWebInspector(QWebInspector* wi);
    void showWebInspector(QWebInspector* wi);
    void hideWebInspector(QWebInspector* wi);
private:
    Ui::QtWebInspector *ui;
};

#endif // QTWEBINSPECTOR_H
