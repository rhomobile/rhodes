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

#ifndef QTSCROLLER_P_H
#define QTSCROLLER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QObject>
#include <QPointer>
#include <QQueue>
#include <QSet>
#include <QEasingCurve>
#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
#  include <QTime>
typedef QTime QElapsedTimer;
#else
#  include <QElapsedTimer>
#endif
#include <QSizeF>
#include <QPointF>
#include <QRectF>
#include "qtscroller.h"
#include "qtscrollerproperties.h"
#include "qtscrollerproperties_p.h"
#include <QAbstractAnimation>

class QtFlickGestureRecognizer;

class QScrollTimer;

class QtScrollerPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QtScroller)

public:
    QtScrollerPrivate(QtScroller *q, QObject *target);
    void init();

    void sendEvent(QObject *o, QEvent *e);

    void setState(QtScroller::State s);

    enum ScrollType {
        ScrollTypeFlick = 0,
        ScrollTypeScrollTo,
        ScrollTypeOvershoot
    };

    struct ScrollSegment {
        qint64 startTime;
        qint64 deltaTime;
        qreal startPos;
        qreal deltaPos;
        QEasingCurve curve;
        qreal stopProgress; // whatever is..
        qreal stopPos;      // ..reached first
        ScrollType type;
    };

    bool pressWhileInactive(const QPointF &position, qint64 timestamp);
    bool moveWhilePressed(const QPointF &position, qint64 timestamp);
    bool releaseWhilePressed(const QPointF &position, qint64 timestamp);
    bool moveWhileDragging(const QPointF &position, qint64 timestamp);
    bool releaseWhileDragging(const QPointF &position, qint64 timestamp);
    bool pressWhileScrolling(const QPointF &position, qint64 timestamp);

    void timerTick();
    void timerEventWhileDragging();
    void timerEventWhileScrolling();

    bool prepareScrolling(const QPointF &position);
    void handleDrag(const QPointF &position, qint64 timestamp);

    QPointF realDpi(int screen);
    QPointF dpi() const;
    void setDpi(const QPointF &dpi);
    void setDpiFromWidget(QWidget *widget);

    void updateVelocity(const QPointF &deltaPixelRaw, qint64 deltaTime);
    void pushSegment(ScrollType type, qreal deltaTime, qreal stopProgress, qreal startPos, qreal deltaPos, qreal stopPos, QEasingCurve::Type curve, Qt::Orientation orientation);
    void recalcScrollingSegments(bool force = false);
    qreal scrollingSegmentsEndPos(Qt::Orientation orientation) const;
    bool scrollingSegmentsValid(Qt::Orientation orientation);
    void createScrollToSegments(qreal v, qreal deltaTime, qreal endPos, Qt::Orientation orientation, ScrollType type);
    void createScrollingSegments(qreal v, qreal startPos, qreal ppm, Qt::Orientation orientation);

    void setContentPositionHelperDragging(const QPointF &deltaPos);
    void setContentPositionHelperScrolling();

    qreal nextSnapPos(qreal p, int dir, Qt::Orientation orientation);
    static qreal nextSegmentPosition(QQueue<ScrollSegment> &segments, qint64 now, qreal oldPos);

    inline int frameRateSkip() const { return properties.d.data()->frameRate; }

    static const char *stateName(QtScroller::State state);
    static const char *inputName(QtScroller::Input input);

public slots:
    void targetDestroyed();

public:
    // static
    static QMap<QObject *, QtScroller *> allScrollers;
    static QSet<QtScroller *> activeScrollers;

    // non static
    QObject *target;
    QtScrollerProperties properties;
    QtFlickGestureRecognizer *recognizer;
    Qt::GestureType recognizerType;

    // scroller state:

    // QPointer<QObject> scrollTarget;
    QSizeF viewportSize;
    QRectF contentPosRange;
    QPointF contentPosition;
    QPointF overshootPosition; // the number of pixels we are overshooting (before overshootDragResistanceFactor)

    // state

    bool enabled;
    QtScroller::State state;
    bool firstScroll; // true if we haven't already send a scroll event

    QPointF oldVelocity; // the release velocity of the last drag

    QPointF pressPosition;
    QPointF lastPosition;
    qint64  pressTimestamp;
    qint64  lastTimestamp;

    QPointF dragDistance; // the distance we should move during the next drag timer event

    QQueue<ScrollSegment> xSegments;
    QQueue<ScrollSegment> ySegments;

    // snap positions
    QList<qreal> snapPositionsX;
    qreal snapFirstX;
    qreal snapIntervalX;
    QList<qreal> snapPositionsY;
    qreal snapFirstY;
    qreal snapIntervalY;

    QPointF pixelPerMeter;

    QElapsedTimer monotonicTimer;

    QPointF releaseVelocity; // the starting velocity of the scrolling state
    QScrollTimer *scrollTimer;

    QtScroller *q_ptr;
};

#endif // QTSCROLLER_P_H

