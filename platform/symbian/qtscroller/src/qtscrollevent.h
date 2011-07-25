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

#ifndef QTSCROLLEVENT_H
#define QTSCROLLEVENT_H

#include <QEvent>
#include <QPointF>
#include <QSizeF>
#include <QRectF>
#include <QScopedPointer>


class QtScrollPrepareEventPrivate;
class QtScrollPrepareEvent : public QEvent
{
public:
    enum { ScrollPrepare = 15056 }; // random number

    QtScrollPrepareEvent(const QPointF &startPos);
    ~QtScrollPrepareEvent();

    QPointF startPos() const;

    QSizeF viewportSize() const;
    QRectF contentPosRange() const;
    QPointF contentPos() const;

    void setViewportSize(const QSizeF &size);
    void setContentPosRange(const QRectF &rect);
    void setContentPos(const QPointF &pos);

private:
    QScopedPointer<QtScrollPrepareEventPrivate> d;
};


class QtScrollEventPrivate;
class QtScrollEvent : public QEvent
{
public:
    enum { Scroll = 15057 }; // random number

    enum ScrollState
    {
        ScrollStarted,
        ScrollUpdated,
        ScrollFinished
    };

    QtScrollEvent(const QPointF &contentPos, const QPointF &overshoot, ScrollState scrollState);
    ~QtScrollEvent();

    QPointF contentPos() const;
    QPointF overshootDistance() const;
    ScrollState scrollState() const;

private:
    QScopedPointer<QtScrollEventPrivate> d;
};

#endif // QTSCROLLEVENT_H
