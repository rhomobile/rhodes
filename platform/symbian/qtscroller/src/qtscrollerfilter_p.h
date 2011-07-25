/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#ifndef QTSCROLLERFILTER_P_H
#define QTSCROLLERFILTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QObject>
#include <QItemSelection>
#include <QMap>
#include <QPoint>
#include "qtscroller.h"

class QAbstractScrollArea;
class QAbstractItemView;
class QWebView;
class QWebFrame;

class QtScrollerFilter : public QObject {
    Q_OBJECT

public:
    static QtScrollerFilter *instance();

    void add(QObject *);
    void remove(QObject *);

protected:
    QtScrollerFilter();

    bool eventFilter(QObject *o, QEvent *e);

#ifndef QTSCROLLER_NO_WEBKIT
    //+++ QWebView
    bool eventFilter_QWebView(QWebView *web, QEvent *event);
    QWebFrame *scrollingFrameAt_QWebView(QWebView *web, const QPoint &pos) const;
    //WebCore::VisibleSelection oldWebSelection;
    QMap<QWebView *, QWebFrame *> scrollingFrames;
#endif

    //+++ QAbstractScrollArea
    bool eventFilter_QAbstractScrollArea(QAbstractScrollArea *area, QEvent *event);
    bool canStartScrollingAt_QAbstractScrollArea(QAbstractScrollArea *area, const QPoint &pos) const;
    QMap<QAbstractScrollArea *, QPoint> overshoot;
    bool ignoreMove;

    //+++ QAbstractItemView
    bool eventFilter_QAbstractItemView(QAbstractItemView *view, QEvent *event);
    void stateChanged_QAbstractItemView(QAbstractItemView *view, QtScroller::State state);

    // the selection before the last mouse down. In case we have to restore it for scrolling    
    QItemSelection oldSelection; 
    QModelIndex oldCurrent;

protected slots:
    void stateChanged(QtScroller::State state);
    
private:
    static QtScrollerFilter *inst;
};

#endif // QTSCROLLERFILTER_P_H

