/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWEBVIEWSELECTIONSUPPRESSOR_H
#define QWEBVIEWSELECTIONSUPPRESSOR_H

#include <QtWebKit/qwebview.h>
#include <QtGui/qevent.h>
#include <QDebug>

class QWebViewSelectionSuppressor : public QObject
{
    Q_OBJECT
public:
    QWebViewSelectionSuppressor(QWebView *v)
        : QObject(v), view(v), enabled(false), mousePressed(false)
    {
        Q_ASSERT(view);
        enable();
    }

    inline void enable()
    {
        if (enabled)
            return;
        view->installEventFilter(this);
        enabled = true;
    }
 
    inline void disable()
    {
        if (!enabled)
            return;
        view->removeEventFilter(this);
        enabled = false;
    }

    inline bool isEnabled() const
    {
        return enabled;
    }

protected:
    inline bool eventFilter(QObject *, QEvent *e);

private:
    QWebView *view;
    bool enabled;
    bool mousePressed;
};

bool QWebViewSelectionSuppressor::eventFilter(QObject *, QEvent *e)
{
    switch (e->type()) {
    case QEvent::MouseButtonPress:
        if (static_cast<QMouseEvent *>(e)->button() == Qt::LeftButton)
            mousePressed = true;
        break;
    case QEvent::MouseButtonRelease:
        if (static_cast<QMouseEvent *>(e)->button() == Qt::LeftButton)
            mousePressed = false;
        break;
    case QEvent::MouseMove:
        if (mousePressed)
            return true;
        break;
    default:
        break;
    }
    return false;
}

#endif
