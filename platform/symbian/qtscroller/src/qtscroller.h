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

#ifndef QTSCROLLER_H
#define QTSCROLLER_H

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include "qtscrollerproperties.h"

class QWidget;
class QtScrollerPrivate;
class QtScrollerProperties;
class QtFlickGestureRecognizer;

class QtScroller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QtScrollerProperties scrollerProperties READ scrollerProperties WRITE setScrollerProperties NOTIFY scrollerPropertiesChanged)
    Q_ENUMS(State)

public:
    enum State
    {
        Inactive,
        Pressed,
        Dragging,
        Scrolling
    };

    enum ScrollerGestureType
    {
        TouchGesture,
        LeftMouseButtonGesture,
        RightMouseButtonGesture,
        MiddleMouseButtonGesture
    };

    enum Input
    {
        InputPress = 1,
        InputMove,
        InputRelease
    };

    static bool hasScroller(QObject *target);

    static QtScroller *scroller(QObject *target);
    static const QtScroller *scroller(const QObject *target);

    static Qt::GestureType grabGesture(QObject *target, ScrollerGestureType gestureType = TouchGesture);
    static Qt::GestureType grabbedGesture(QObject *target);
    static void ungrabGesture(QObject *target);

    static QList<QtScroller *> activeScrollers();

    QObject *target() const;

    State state() const;

    bool handleInput(Input input, const QPointF &position, qint64 timestamp = 0);

    void stop();
    QPointF velocity() const;
    QPointF finalPosition() const;
    QPointF pixelPerMeter() const;

    QtScrollerProperties scrollerProperties() const;

    void setSnapPositionsX( const QList<qreal> &positions );
    void setSnapPositionsX( qreal first, qreal interval );
    void setSnapPositionsY( const QList<qreal> &positions );
    void setSnapPositionsY( qreal first, qreal interval );

public Q_SLOTS:
    void setScrollerProperties(const QtScrollerProperties &prop);
    void scrollTo(const QPointF &pos);
    void scrollTo(const QPointF &pos, int scrollTime);
    void ensureVisible(const QRectF &rect, qreal xmargin, qreal ymargin);
    void ensureVisible(const QRectF &rect, qreal xmargin, qreal ymargin, int scrollTime);
    void resendPrepareEvent();

Q_SIGNALS:
    void stateChanged(QtScroller::State newstate);
    void scrollerPropertiesChanged(const QtScrollerProperties &);

private:
    QtScrollerPrivate* d_ptr;

    QtScroller(QObject *target);
    virtual ~QtScroller();

    Q_DISABLE_COPY(QtScroller)
    Q_DECLARE_PRIVATE(QtScroller)

    friend class QtFlickGestureRecognizer;
};

#endif // QTSCROLLER_H
