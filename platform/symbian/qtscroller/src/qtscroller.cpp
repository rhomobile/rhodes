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

#include "qevent.h"
#include "qwidget.h"
#include "qtscroller.h"
#include "qtflickgesture_p.h"
#include "qtscroller_p.h"
#include "qtscrollerproperties.h"
#include "qtscrollerproperties_p.h"

#include "math.h"

#include <QTime>
#include <QMap>
#include <QApplication>
#include <QAbstractScrollArea>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QtCore/qmath.h>
#include <QtGui/qevent.h>
#include <qnumeric.h>

// completely optional warning, but you can go nuts if you forget to set the scroll mode right
#define ITEMVIEW_SCROLLMODE_WARNING 1
#ifdef ITEMVIEW_SCROLLMODE_WARNING
#  include <QAbstractItemView>
#endif

#include <QtDebug>

// vvv QScroller Solution only
#include "qtscrollerfilter_p.h"
#include "qtscrollevent.h"
// ^^^ QScroller Solution only

#if defined(Q_WS_X11)
#  include <QX11Info>
#  include <QLibrary>
#endif


bool qt_sendSpontaneousEvent(QObject *receiver, QEvent *event);

//#define QSCROLLER_DEBUG

#ifdef QSCROLLER_DEBUG
#  define qScrollerDebug  qDebug
#else
#  define qScrollerDebug  while (false) qDebug
#endif

QDebug &operator<<(QDebug &dbg, const QtScrollerPrivate::ScrollSegment &s)
{
    dbg << "\n  Time: start:" << s.startTime << " duration:" << s.deltaTime << " stop progress:" << s.stopProgress;
    dbg << "\n  Pos: start:" << s.startPos << " delta:" << s.deltaPos << " stop:" << s.stopPos;
    dbg << "\n  Curve: type:" << s.curve.type() << "\n";
    return dbg;
}


// a few helper operators to make the code below a lot more readable:
// otherwise a lot of ifs would have to be multi-line to check both the x
// and y coordinate separately.

// returns true only if the abs. value of BOTH x and y are <= f
inline bool operator<=(const QPointF &p, qreal f)
{
    return (qAbs(p.x()) <= f) && (qAbs(p.y()) <= f);
}

// returns true only if the abs. value of BOTH x and y are < f
inline bool operator<(const QPointF &p, qreal f)
{
    return (qAbs(p.x()) < f) && (qAbs(p.y()) < f);
}

// returns true if the abs. value of EITHER x or y are >= f
inline bool operator>=(const QPointF &p, qreal f)
{
    return (qAbs(p.x()) >= f) || (qAbs(p.y()) >= f);
}

// returns true if the abs. value of EITHER x or y are > f
inline bool operator>(const QPointF &p, qreal f)
{
    return (qAbs(p.x()) > f) || (qAbs(p.y()) > f);
}

// returns a new point with both coordinates having the abs. value of the original one
inline QPointF qAbs(const QPointF &p)
{
    return QPointF(qAbs(p.x()), qAbs(p.y()));
}

// returns a new point with all components of p1 multiplied by the corresponding components of p2
inline QPointF operator*(const QPointF &p1, const QPointF &p2)
{
    return QPointF(p1.x() * p2.x(), p1.y() * p2.y());
}

// returns a new point with all components of p1 divided by the corresponding components of p2
inline QPointF operator/(const QPointF &p1, const QPointF &p2)
{
    return QPointF(p1.x() / p2.x(), p1.y() / p2.y());
}

inline QPointF clampToRect(const QPointF &p, const QRectF &rect)
{
    qreal x = qBound(rect.left(), p.x(), rect.right());
    qreal y = qBound(rect.top(), p.y(), rect.bottom());
    return QPointF(x, y);
}

// returns -1, 0 or +1 according to r being <0, ==0 or >0
inline int qSign(qreal r)
{
    return (r < 0) ? -1 : ((r > 0) ? 1 : 0);
}

// this version is not mathematically exact, but it just works for every
// easing curve type (even custom ones)

static qreal differentialForProgress(const QEasingCurve &curve, qreal pos)
{
    const qreal dx = 0.01;
    qreal left = (pos < qreal(0.5)) ? pos : pos - qreal(dx);
    qreal right = (pos >= qreal(0.5)) ? pos : pos + qreal(dx);
    qreal d = (curve.valueForProgress(right) - curve.valueForProgress(left)) / qreal(dx);

    //qScrollerDebug() << "differentialForProgress(type: " << curve.type() << ", pos: " << pos << ") = " << d;

    return d;
}

// this version is not mathematically exact, but it just works for every
// easing curve type (even custom ones)

static qreal progressForValue(const QEasingCurve &curve, qreal value)
{
    if (curve.type() >= QEasingCurve::InElastic &&
        curve.type() < QEasingCurve::Custom) {
        qWarning("progressForValue(): QEasingCurves of type %d do not have an inverse, since they are not injective.", curve.type());
        return value;
    }
    if (value < qreal(0) || value > qreal(1))
        return value;

    qreal progress = value, left(0), right(1);
    for (int iterations = 6; iterations; --iterations) {
        qreal v = curve.valueForProgress(progress);
        if (v < value)
            left = progress;
        else if (v > value)
            right = progress;
        else
            break;
        progress = (left + right) / qreal(2);
    }
    return progress;
}


class QScrollTimer : public QAbstractAnimation
{
public:
    QScrollTimer(QtScrollerPrivate *_d)
        : d(_d), ignoreUpdate(false), skip(0)
    { }

    int duration() const
    {
        return -1;
    }

    void start()
    {
        // QAbstractAnimation::start() will immediately call
        // updateCurrentTime(), but our state is not set correctly yet
        ignoreUpdate = true;
        QAbstractAnimation::start();
        ignoreUpdate = false;
        skip = 0;
    }

protected:
    void updateCurrentTime(int /*currentTime*/)
    {
        if (!ignoreUpdate) {
            if (++skip >= d->frameRateSkip()) {
                skip = 0;
                d->timerTick();
            }
        }
    }

private:
    QtScrollerPrivate *d;
    bool ignoreUpdate;
    int skip;
};

/*!
    \class QScroller
    \brief The QScroller class enables kinetic scrolling for any scrolling widget or graphics item.
    \class QtScroller
    \brief The QtScroller class enables kinetic scrolling for any scrolling widget or graphics item.
    \since 4.8

    With kinetic scrolling, the user can push the widget in a given
    direction and it will continue to scroll in this direction until it is
    stopped either by the user or by friction.  Aspects of inertia, friction
    and other physical concepts can be changed in order to fine-tune an
    intuitive user experience.

    The QtScroller object is the object that stores the current position and
    scrolling speed and takes care of updates.
    QtScroller can be triggered by a flick gesture

    \code
        QWidget *w = ...;
        QtScroller::grabGesture(w, QtScroller::LeftMouseButtonGesture);
    \endcode

    or directly like this:

    \code
        QWidget *w = ...;
        QtScroller *scroller = QtScroller::scroller(w);
        scroller->scrollTo(QPointF(100, 100));
    \endcode

    The scrolled QObjects receive a QScrollPrepareEvent whenever the scroller needs to
    update its geometry information and a QScrollEvent whenever the content of the object should
    actually be scrolled.

    The scroller uses the global QAbstractAnimation timer to generate its QScrollEvents. This
    can be changed with QtScrollerProperties::FrameRate on a per-QtScroller basis.

    Several examples in the \c scroller examples directory show how QtScroller,
    QScrollEvent and the scroller gesture can be used.

    Even though this kinetic scroller has a large number of settings available via
    QtScrollerProperties, we recommend that you leave them all at their default, platform optimized
    values. Before changing them you can experiment with the \c plot example in
    the \c scroller examples directory.

    \sa QtScrollEvent, QtScrollPrepareEvent, QtScrollerProperties
*/


QMap<QObject *, QtScroller *> QtScrollerPrivate::allScrollers;
QSet<QtScroller *> QtScrollerPrivate::activeScrollers;

/*!
    Returns \c true if a QtScroller object was already created for \a target; \c false otherwise.

    \sa scroller()
*/
bool QtScroller::hasScroller(QObject *target)
{
    return (QtScrollerPrivate::allScrollers.value(target));
}

/*!
    Returns the scroller for the given \a target.
    As long as the object exists this function will always return the same QtScroller instance.
    If no QtScroller exists for the \a target, one will implicitly be created.
    At no point more than one QtScroller will be active on an object.

    \sa hasScroller(), target()
*/
QtScroller *QtScroller::scroller(QObject *target)
{
    if (!target) {
        qWarning("QtScroller::scroller() was called with a null target.");
        return 0;
    }

    if (QtScrollerPrivate::allScrollers.contains(target))
        return QtScrollerPrivate::allScrollers.value(target);

    QtScroller *s = new QtScroller(target);
    QtScrollerPrivate::allScrollers.insert(target, s);
    return s;
}

/*!
    \overload
    This is the const version of scroller().
*/
const QtScroller *QtScroller::scroller(const QObject *target)
{
    return scroller(const_cast<QObject*>(target));
}

/*!
    Returns an application wide list of currently active QtScroller objects.
    Active QtScroller objects are in a state() that is not QtScroller::Inactive.
    This function is useful when writing your own gesture recognizer.
*/
QList<QtScroller *> QtScroller::activeScrollers()
{
    return QtScrollerPrivate::activeScrollers.toList();
}

/*!
    Returns the target object of this scroller.
    \sa hasScroller(), scroller()
 */
QObject *QtScroller::target() const
{
    Q_D(const QtScroller);
    return d->target;
}

/*!
    \fn QtScroller::scrollerPropertiesChanged(const QtScrollerProperties &newProperties);

    QtScroller emits this signal whenever its scroller properties change.
    \a newProperties are the new scroller properties.

    \sa scrollerProperties
*/


/*! \property QtScroller::scrollerProperties
    \brief The scroller properties of this scroller.
    The properties are used by the QtScroller to determine its scrolling behavior.
*/
QtScrollerProperties QtScroller::scrollerProperties() const
{
    Q_D(const QtScroller);
    return d->properties;
}

void QtScroller::setScrollerProperties(const QtScrollerProperties &sp)
{
    Q_D(QtScroller);
    if (d->properties != sp) {
        d->properties = sp;
        emit scrollerPropertiesChanged(sp);

        // we need to force the recalculation here, since the overshootPolicy may have changed and
        // existing segments may include an overshoot animation.
        d->recalcScrollingSegments(true);
    }
}


/*!
    Registers a custom scroll gesture recognizer, grabs it for the \a
    target and returns the resulting gesture type.  If \a scrollGestureType is
    set to TouchGesture the gesture triggers on touch events. If it is set to
    one of LeftMouseButtonGesture, RightMouseButtonGesture or
    MiddleMouseButtonGesture it triggers on mouse events of the
    corresponding button.

    Only one scroll gesture can be active on a single object at the same
    time. If you call this function twice on the same object, it will
    ungrab the existing gesture before grabbing the new one.

    \note To avoid unwanted side-effects, mouse events are consumed while
    the gesture is triggered. Since the initial mouse press event is
    not consumed, the gesture sends a fake mouse release event
    at the global position \c{(INT_MIN, INT_MIN)}. This ensures that
    internal states of the widget that received the original mouse press
    are consistent.

    \sa ungrabGesture, grabbedGesture
*/
Qt::GestureType QtScroller::grabGesture(QObject *target, ScrollerGestureType scrollGestureType)
{
    // ensure that a scroller for target is created
    QtScroller *s = scroller(target);
    if (!s)
        return Qt::GestureType(0);

    QtScrollerPrivate *sp = s->d_ptr;
    if (sp->recognizer)
        ungrabGesture(target); // ungrab the old gesture

    Qt::MouseButton button;
    switch (scrollGestureType) {
    case LeftMouseButtonGesture  : button = Qt::LeftButton; break;
    case RightMouseButtonGesture : button = Qt::RightButton; break;
    case MiddleMouseButtonGesture: button = Qt::MidButton; break;
    default                      :
    case TouchGesture            : button = Qt::NoButton; break; // NoButton == Touch
    }

    sp->recognizer = new QtFlickGestureRecognizer(button);
    sp->recognizerType = QGestureRecognizer::registerRecognizer(sp->recognizer);
    if (target->isWidgetType()) {
        QWidget *widget = static_cast<QWidget *>(target);
        widget->grabGesture(sp->recognizerType);
        if (scrollGestureType == TouchGesture)
            widget->setAttribute(Qt::WA_AcceptTouchEvents);
#if ITEMVIEW_SCROLLMODE_WARNING
        QAbstractItemView *view = 0;
        if ((view = qobject_cast<QAbstractItemView *>(widget->parentWidget())) &&
            (view->viewport() == widget)) {
            if (view->verticalScrollMode() != QAbstractItemView::ScrollPerPixel)
                qWarning("QtScroller::grabGesture() was called on a QAbstractItemView with verticaScrollMode not set to ScrollPerPixel");
            if (view->horizontalScrollMode() != QAbstractItemView::ScrollPerPixel)
                qWarning("QtScroller::grabGesture() was called on a QAbstractItemView with horizontalScrollMode not set to ScrollPerPixel");
        }
#endif // ITEMVIEW_SCROLLMODE_WARNING
    } else if (QGraphicsObject *go = qobject_cast<QGraphicsObject*>(target)) {
        if (scrollGestureType == TouchGesture)
            go->setAcceptTouchEvents(true);
        go->grabGesture(sp->recognizerType);
    }
    
    // vvv QScroller Solution only
    QtScrollerFilter::instance()->add(target);
    // ^^^ QScroller Solution only
    
    return sp->recognizerType;
}

/*!
    Returns the gesture type currently grabbed for the \a target or 0 if no
    gesture is grabbed.

    \sa grabGesture, ungrabGesture
*/
Qt::GestureType QtScroller::grabbedGesture(QObject *target)
{
    QtScroller *s = scroller(target);
    if (s && s->d_ptr)
        return s->d_ptr->recognizerType;
    else
        return Qt::GestureType(0);
}

/*!
    Ungrabs the gesture for the \a target.
    Does nothing if no gesture is grabbed.

    \sa grabGesture, grabbedGesture
*/
void QtScroller::ungrabGesture(QObject *target)
{
    QtScroller *s = scroller(target);
    if (!s)
        return;

    QtScrollerPrivate *sp = s->d_ptr;
    if (!sp->recognizer)
        return; // nothing to do

    if (target->isWidgetType()) {
        QWidget *widget = static_cast<QWidget *>(target);
        widget->ungrabGesture(sp->recognizerType);

    } else if (QGraphicsObject *go = qobject_cast<QGraphicsObject*>(target)) {
        go->ungrabGesture(sp->recognizerType);
    }

    QGestureRecognizer::unregisterRecognizer(sp->recognizerType);
    // do not delete the recognizer. The QGestureManager is doing this.
    sp->recognizer = 0;

    // vvv QScroller Solution only
    QtScrollerFilter::instance()->remove(target);
    // ^^^ QScroller Solution only
}

/*!
    \internal
*/
QtScroller::QtScroller(QObject *target)
    : d_ptr(new QtScrollerPrivate(this, target))
{
    Q_ASSERT(target); // you can't create a scroller without a target in any normal way
    Q_D(QtScroller);
    d->init();
}

/*!
    \internal
*/
QtScroller::~QtScroller()
{
    Q_D(QtScroller);
    QGestureRecognizer::unregisterRecognizer(d->recognizerType);
    // do not delete the recognizer. The QGestureManager is doing this.
    d->recognizer = 0;
    QtScrollerPrivate::allScrollers.remove(d->target);
    QtScrollerPrivate::activeScrollers.remove(this);

    delete d_ptr;
}


/*!
    \fn QtScroller::stateChanged(QtScroller::State newState);

    QtScroller emits this signal whenever the state changes. \a newState is the new State.

    \sa state
*/

/*!
    \property QtScroller::state
    \brief the state of the scroller

    \sa QtScroller::State
*/
QtScroller::State QtScroller::state() const
{
    Q_D(const QtScroller);
    return d->state;
}

/*!
    Stops the scroller and resets its state back to Inactive.
*/
void QtScroller::stop()
{
    Q_D(QtScroller);
    if (d->state != Inactive) {
        QPointF here = clampToRect(d->contentPosition, d->contentPosRange);
        qreal snapX = d->nextSnapPos(here.x(), 0, Qt::Horizontal);
        qreal snapY = d->nextSnapPos(here.y(), 0, Qt::Vertical);
        QPointF snap = here;
        if (!qIsNaN(snapX))
            snap.setX(snapX);
        if (!qIsNaN(snapY))
            snap.setY(snapY);
        d->contentPosition = snap;
        d->overshootPosition = QPointF(0, 0);

        d->setState(Inactive);
    }
}

/*!
    Returns the pixel per meter metric for the scrolled widget.

    The value is reported for both the x and y axis separately by using a QPointF.

    \note Please note that this value should be physically correct. The actual DPI settings
    that Qt returns for the display may be reported wrongly on purpose by the underlying
    windowing system, for example on Mac OS X or Maemo 5.
*/
QPointF QtScroller::pixelPerMeter() const
{
    Q_D(const QtScroller);
    QPointF ppm = d->pixelPerMeter;

    if (QGraphicsObject *go = qobject_cast<QGraphicsObject *>(d->target)) {
        QTransform viewtr;
        //TODO: the first view isn't really correct - maybe use an additional field in the prepare event?
        if (go->scene() && !go->scene()->views().isEmpty())
            viewtr = go->scene()->views().first()->viewportTransform();
        QTransform tr = go->deviceTransform(viewtr);
        if (tr.isScaling()) {
            QPointF p0 = tr.map(QPointF(0, 0));
            QPointF px = tr.map(QPointF(1, 0));
            QPointF py = tr.map(QPointF(0, 1));
            ppm.rx() /= QLineF(p0, px).length();
            ppm.ry() /= QLineF(p0, py).length();
        }
    }
    return ppm;
}

/*!
    Returns the current scrolling velocity in meter per second when the state is Scrolling or Dragging.
    Returns a zero velocity otherwise.

    The velocity is reported for both the x and y axis separately by using a QPointF.

    \sa pixelPerMeter()
*/
QPointF QtScroller::velocity() const
{
    Q_D(const QtScroller);
    const QtScrollerPropertiesPrivate *sp = d->properties.d.data();

     switch (state()) {
     case Dragging:
        return d->releaseVelocity;
     case Scrolling: {
        QPointF vel;
        qint64 now = d->monotonicTimer.elapsed();

        if (!d->xSegments.isEmpty()) {
            const QtScrollerPrivate::ScrollSegment &s = d->xSegments.head();
            qreal progress = qreal(now - s.startTime) / qreal(s.deltaTime);
            qreal v = qSign(s.deltaPos) * qreal(s.deltaTime) / qreal(1000) * sp->decelerationFactor * qreal(0.5) * differentialForProgress(s.curve, progress);
            vel.setX(v);
        }

        if (!d->ySegments.isEmpty()) {
            const QtScrollerPrivate::ScrollSegment &s = d->ySegments.head();
            qreal progress = qreal(now - s.startTime) / qreal(s.deltaTime);
            qreal v = qSign(s.deltaPos) * qreal(s.deltaTime) / qreal(1000) * sp->decelerationFactor * qreal(0.5) * differentialForProgress(s.curve, progress);
            vel.setY(v);
        }
        return vel;
     }
     default:
         return QPointF(0, 0);
     }
}

/*!
    Returns the estimated final position for the current scroll movement.
    Returns the current position if the scroller state is not Scrolling.
    The result is undefined when the scroller state is Inactive.

    The target position is in pixel.

    \sa pixelPerMeter(), scrollTo()
*/
QPointF QtScroller::finalPosition() const
{
    Q_D(const QtScroller);
    return QPointF(d->scrollingSegmentsEndPos(Qt::Horizontal),
                   d->scrollingSegmentsEndPos(Qt::Vertical));
}

/*!
    Starts scrolling the widget so that point \a pos is at the top-left position in
    the viewport.

    The behaviour when scrolling outside the valid scroll area is undefined.
    In this case the scroller might or might not overshoot.

    The scrolling speed will be calculated so that the given position will
    be reached after a platform-defined time span.

    \a pos is given in viewport coordinates.

    \sa ensureVisible()
*/
void QtScroller::scrollTo(const QPointF &pos)
{
    // we could make this adjustable via QtScrollerProperties
    scrollTo(pos, 300);
}

/*! \overload

    This version will reach its destination position in \a scrollTime milliseconds.
*/
void QtScroller::scrollTo(const QPointF &pos, int scrollTime)
{
    Q_D(QtScroller);

    if (d->state == Pressed || d->state == Dragging )
        return;

    // no need to resend a prepare event if we are already scrolling
    if (d->state == Inactive && !d->prepareScrolling(QPointF()))
        return;

    QPointF newpos = clampToRect(pos, d->contentPosRange);
    qreal snapX = d->nextSnapPos(newpos.x(), 0, Qt::Horizontal);
    qreal snapY = d->nextSnapPos(newpos.y(), 0, Qt::Vertical);
    if (!qIsNaN(snapX))
        newpos.setX(snapX);
    if (!qIsNaN(snapY))
        newpos.setY(snapY);

    qScrollerDebug() << "QtScroller::scrollTo(req:" << pos << " [pix] / snap:" << newpos << ", " << scrollTime << " [ms])";

    if (newpos == d->contentPosition + d->overshootPosition)
        return;

    QPointF vel = velocity();

    if (scrollTime < 0)
        scrollTime = 0;
    qreal time = qreal(scrollTime) / 1000;

    d->createScrollToSegments(vel.x(), time, newpos.x(), Qt::Horizontal, QtScrollerPrivate::ScrollTypeScrollTo);
    d->createScrollToSegments(vel.y(), time, newpos.y(), Qt::Vertical, QtScrollerPrivate::ScrollTypeScrollTo);

    if (!scrollTime)
        d->setContentPositionHelperScrolling();
    d->setState(scrollTime ? Scrolling : Inactive);
}

/*!
    Starts scrolling so that the rectangle \a rect is visible inside the
    viewport with additional margins specified in pixels by \a xmargin and \a ymargin around
    the rect.

    In cases where it is not possible to fit the rect plus margins inside the viewport the contents
    are scrolled so that as much as possible is visible from \a rect.

    The scrolling speed is calculated so that the given position is reached after a platform-defined
    time span.

    This function performs the actual scrolling by calling scrollTo().

    \sa scrollTo
*/
void QtScroller::ensureVisible(const QRectF &rect, qreal xmargin, qreal ymargin)
{
    // we could make this adjustable via QtScrollerProperties
    ensureVisible(rect, xmargin, ymargin, 1000);
}

/*! \overload

    This version will reach its destination position in \a scrollTime milliseconds.
*/
void QtScroller::ensureVisible(const QRectF &rect, qreal xmargin, qreal ymargin, int scrollTime)
{
    Q_D(QtScroller);

    if (d->state == Pressed || d->state == Dragging)
        return;

    if (d->state == Inactive && !d->prepareScrolling(QPointF()))
        return;

    // -- calculate the current pos (or the position after the current scroll)
    QPointF startPos = d->contentPosition + d->overshootPosition;
    startPos = QPointF(d->scrollingSegmentsEndPos(Qt::Horizontal),
                       d->scrollingSegmentsEndPos(Qt::Vertical));

    QRectF marginRect(rect.x() - xmargin, rect.y() - ymargin,
                      rect.width() + 2 * xmargin, rect.height() + 2 * ymargin);

    QSizeF visible = d->viewportSize;
    QRectF visibleRect(startPos, visible);

    qScrollerDebug() << "QtScroller::ensureVisible(" << rect << " [pix], " << xmargin << " [pix], " << ymargin << " [pix], " << scrollTime << "[ms])";
    qScrollerDebug() << "  --> content position:" << d->contentPosition;

    if (visibleRect.contains(marginRect))
        return;

    QPointF newPos = startPos;

    if (visibleRect.width() < rect.width()) {
        // at least try to move the rect into view
        if (rect.left() > visibleRect.left())
            newPos.setX(rect.left());
        else if (rect.right() < visibleRect.right())
            newPos.setX(rect.right() - visible.width());

    } else if (visibleRect.width() < marginRect.width()) {
        newPos.setX(rect.center().x() - visibleRect.width() / 2);
    } else if (marginRect.left() > visibleRect.left()) {
        newPos.setX(marginRect.left());
    } else if (marginRect.right() < visibleRect.right()) {
        newPos.setX(marginRect.right() - visible.width());
    }

    if (visibleRect.height() < rect.height()) {
        // at least try to move the rect into view
        if (rect.top() > visibleRect.top())
            newPos.setX(rect.top());
        else if (rect.bottom() < visibleRect.bottom())
            newPos.setX(rect.bottom() - visible.height());

    } else if (visibleRect.height() < marginRect.height()) {
        newPos.setY(rect.center().y() - visibleRect.height() / 2);
    } else if (marginRect.top() > visibleRect.top()) {
        newPos.setY(marginRect.top());
    } else if (marginRect.bottom() < visibleRect.bottom()) {
        newPos.setY(marginRect.bottom() - visible.height());
    }

    // clamp to maximum content position
    newPos = clampToRect(newPos, d->contentPosRange);
    if (newPos == startPos)
        return;

    scrollTo(newPos, scrollTime);
}

/*! This function resends the QScrollPrepareEvent.
    Calling resendPrepareEvent triggers a QScrollPrepareEvent from the scroller.
    This allows the receiver to re-set content position and content size while
    scrolling.
    Calling this function while in the Inactive state is useless as the prepare event
    is sent again before scrolling starts.
 */
void QtScroller::resendPrepareEvent()
{
    Q_D(QtScroller);
    d->prepareScrolling(d->pressPosition);
}

/*! Set the snap positions for the horizontal axis to a list of \a positions.
    This overwrites all previously set snap positions and also a previously
    set snapping interval.
    Snapping can be deactivated by setting an empty list of positions.
 */
void QtScroller::setSnapPositionsX(const QList<qreal> &positions)
{
    Q_D(QtScroller);
    d->snapPositionsX = positions;
    d->snapIntervalX = 0.0;

    d->recalcScrollingSegments();
}

/*! Set the snap positions for the horizontal axis to regular spaced intervals.
    The first snap position is at \a first. The next at \a first + \a interval.
    This can be used to implement a list header.
    This overwrites all previously set snap positions and also a previously
    set snapping interval.
    Snapping can be deactivated by setting an interval of 0.0
 */
void QtScroller::setSnapPositionsX(qreal first, qreal interval)
{
    Q_D(QtScroller);
    d->snapFirstX = first;
    d->snapIntervalX = interval;
    d->snapPositionsX.clear();

    d->recalcScrollingSegments();
}

/*! Set the snap positions for the vertical axis to a list of \a positions.
    This overwrites all previously set snap positions and also a previously
    set snapping interval.
    Snapping can be deactivated by setting an empty list of positions.
 */
void QtScroller::setSnapPositionsY(const QList<qreal> &positions)
{
    Q_D(QtScroller);
    d->snapPositionsY = positions;
    d->snapIntervalY = 0.0;

    d->recalcScrollingSegments();
}

/*! Set the snap positions for the vertical axis to regular spaced intervals.
    The first snap position is at \a first. The next at \a first + \a interval.
    This overwrites all previously set snap positions and also a previously
    set snapping interval.
    Snapping can be deactivated by setting an interval of 0.0
 */
void QtScroller::setSnapPositionsY(qreal first, qreal interval)
{
    Q_D(QtScroller);
    d->snapFirstY = first;
    d->snapIntervalY = interval;
    d->snapPositionsY.clear();

    d->recalcScrollingSegments();
}



// -------------- private ------------

QtScrollerPrivate::QtScrollerPrivate(QtScroller *q, QObject *_target)
    : target(_target)
    , recognizer(0)
    , recognizerType(Qt::CustomGesture)
    , state(QtScroller::Inactive)
    , firstScroll(true)
    , pressTimestamp(0)
    , lastTimestamp(0)
    , snapFirstX(-1.0)
    , snapIntervalX(0.0)
    , snapFirstY(-1.0)
    , snapIntervalY(0.0)
    , scrollTimer(new QScrollTimer(this))
    , q_ptr(q)
{
    connect(target, SIGNAL(destroyed(QObject*)), this, SLOT(targetDestroyed()));
}

void QtScrollerPrivate::init()
{
    setDpiFromWidget(0);
    monotonicTimer.start();
}

void QtScrollerPrivate::sendEvent(QObject *o, QEvent *e)
{
    qt_sendSpontaneousEvent(o, e);
}

const char *QtScrollerPrivate::stateName(QtScroller::State state)
{
    switch (state) {
    case QtScroller::Inactive:  return "inactive";
    case QtScroller::Pressed:   return "pressed";
    case QtScroller::Dragging:  return "dragging";
    case QtScroller::Scrolling: return "scrolling";
    default:                    return "(invalid)";
    }
}

const char *QtScrollerPrivate::inputName(QtScroller::Input input)
{
    switch (input) {
    case QtScroller::InputPress:   return "press";
    case QtScroller::InputMove:    return "move";
    case QtScroller::InputRelease: return "release";
    default:                       return "(invalid)";
    }
}

void QtScrollerPrivate::targetDestroyed()
{
    scrollTimer->stop();
    delete q_ptr;
}

void QtScrollerPrivate::timerTick()
{
    struct timerevent {
        QtScroller::State state;
        typedef void (QtScrollerPrivate::*timerhandler_t)();
        timerhandler_t handler;
    };

    timerevent timerevents[] = {
        { QtScroller::Dragging, &QtScrollerPrivate::timerEventWhileDragging },
        { QtScroller::Scrolling, &QtScrollerPrivate::timerEventWhileScrolling },
    };

    for (int i = 0; i < int(sizeof(timerevents) / sizeof(*timerevents)); ++i) {
        timerevent *te = timerevents + i;

        if (state == te->state) {
            (this->*te->handler)();
            return;
        }
    }

    scrollTimer->stop();
}

/*!
    This function is used by gesture recognizers to inform the scroller about a new input event.
    The scroller changes its internal state() according to the input event and its attached
    scroller properties. The scroller doesn't distinguish between the kind of input device the
    event came from. Therefore the event needs to be split into the \a input type, a \a position and a
    milli-second \a timestamp.  The \a position needs to be in the target's coordinate system.

    The return value is \c true if the event should be consumed by the calling filter or \c false
    if the event should be forwarded to the control.

    \note Using grabGesture() should be sufficient for most use cases.
*/
bool QtScroller::handleInput(Input input, const QPointF &position, qint64 timestamp)
{
    Q_D(QtScroller);

    qScrollerDebug() << "QtScroller::handleInput(" << input << ", " << d->stateName(d->state) << ", " << position << ", " << timestamp << ")";
    struct statechange {
        State state;
        Input input;
        typedef bool (QtScrollerPrivate::*inputhandler_t)(const QPointF &position, qint64 timestamp);
        inputhandler_t handler;
    };

    statechange statechanges[] = {
        { QtScroller::Inactive,  InputPress,   &QtScrollerPrivate::pressWhileInactive },
        { QtScroller::Pressed,   InputMove,    &QtScrollerPrivate::moveWhilePressed },
        { QtScroller::Pressed,   InputRelease, &QtScrollerPrivate::releaseWhilePressed },
        { QtScroller::Dragging,  InputMove,    &QtScrollerPrivate::moveWhileDragging },
        { QtScroller::Dragging,  InputRelease, &QtScrollerPrivate::releaseWhileDragging },
        { QtScroller::Scrolling, InputPress,   &QtScrollerPrivate::pressWhileScrolling }
    };

    for (int i = 0; i < int(sizeof(statechanges) / sizeof(*statechanges)); ++i) {
        statechange *sc = statechanges + i;

         if (d->state == sc->state && input == sc->input)
             return (d->*sc->handler)(position - d->overshootPosition, timestamp);
    }
    return false;
}

#if !defined(Q_WS_MAC)
// the Mac version is implemented in qtscroller_mac.mm

QPointF QtScrollerPrivate::realDpi(int screen)
{
#  ifdef Q_WS_MAEMO_5
    Q_UNUSED(screen);

    // The DPI value is hardcoded to 96 on Maemo5:
    // https://projects.maemo.org/bugzilla/show_bug.cgi?id=152525
    // This value (260) is only correct for the N900 though, but
    // there's no way to get the real DPI at run time.
    return QPointF(260, 260);

#  elif defined(Q_WS_X11) && !defined(QT_NO_XRANDR)
    // Avoid including the libXRandR header for a very simple struct
    struct structXRRScreenSize
    {
        int   width, height;
        int   mwidth, mheight;
    };
    typedef structXRRScreenSize *(*PtrXRRSizes)(Display *, int, int *);
    typedef int (*PtrXRRRootToScreen)(Display *, Qt::HANDLE);
    static PtrXRRSizes ptrXRRSizes = 0;
    static PtrXRRRootToScreen ptrXRRRootToScreen = 0;
    static bool resolvedXRR = false;

    if (!resolvedXRR) {
        QLibrary xrandrLib(QLatin1String("Xrandr"), 2);
        if (!xrandrLib.load()) { // try without the version number
            xrandrLib.setFileName(QLatin1String("Xrandr"));
            xrandrLib.load();
        }
        if (xrandrLib.isLoaded()) {
            ptrXRRSizes = (PtrXRRSizes) xrandrLib.resolve("XRRSizes");
            ptrXRRRootToScreen = (PtrXRRRootToScreen) xrandrLib.resolve("XRRRootToScreen");
        }
        resolvedXRR = true;
    }

    if (ptrXRRSizes && ptrXRRRootToScreen) {
        int nsizes = 0;
        // QDesktopWidget is based on Xinerama screens, which do not always
        // correspond to RandR screens: NVidia's TwinView e.g.  will show up
        // as 2 screens in QDesktopWidget, but libXRandR will only see 1 screen.
        // (although with the combined size of the Xinerama screens).
        // Additionally, libXrandr will simply crash when calling XRRSizes
        // for (the non-existant) screen 1 in this scenario.
        Qt::HANDLE root = QX11Info::appRootWindow(screen == -1 ? QX11Info::appScreen() : screen);
        int randrscreen = (root != 0) ? ptrXRRRootToScreen(QX11Info::display(), root) : -1;

        structXRRScreenSize *sizes = ptrXRRSizes(QX11Info::display(), randrscreen == -1 ? 0 : randrscreen, &nsizes);

        if (nsizes > 0 && sizes && sizes->width && sizes->height && sizes->mwidth && sizes->mheight) {
            qScrollerDebug() << "XRandR DPI:" << QPointF(qreal(25.4) * qreal(sizes->width) / qreal(sizes->mwidth),
                                                         qreal(25.4) * qreal(sizes->height) / qreal(sizes->mheight));
            return QPointF(qreal(25.4) * qreal(sizes->width) / qreal(sizes->mwidth),
                           qreal(25.4) * qreal(sizes->height) / qreal(sizes->mheight));
        }
    }
#  endif

    QWidget *w = QApplication::desktop()->screen(screen);
    return QPointF(w->physicalDpiX(), w->physicalDpiY());
}

#endif // !Q_WS_MAC


/*! \internal
    Returns the resolution of the used screen.
*/
QPointF QtScrollerPrivate::dpi() const
{
    return pixelPerMeter * qreal(0.0254);
}

/*! \internal
    Sets the resolution used for scrolling.
    This resolution is only used by the kinetic scroller. If you change this
    then the scroller will behave quite different as a lot of the values are
    given in physical distances (millimeter).
*/
void QtScrollerPrivate::setDpi(const QPointF &dpi)
{
    pixelPerMeter = dpi / qreal(0.0254);
}

/*! \internal
    Sets the dpi used for scrolling to the value of the widget.
*/
void QtScrollerPrivate::setDpiFromWidget(QWidget *widget)
{
    QDesktopWidget *dw = QApplication::desktop();
    setDpi(realDpi(widget ? dw->screenNumber(widget) : dw->primaryScreen()));
}

/*! \internal
    Updates the velocity during dragging.
    Sets releaseVelocity.
*/
void QtScrollerPrivate::updateVelocity(const QPointF &deltaPixelRaw, qint64 deltaTime)
{
    if (deltaTime <= 0)
        return;

    Q_Q(QtScroller);
    QPointF ppm = q->pixelPerMeter();
    const QtScrollerPropertiesPrivate *sp = properties.d.data();
    QPointF deltaPixel = deltaPixelRaw;

    qScrollerDebug() << "QtScroller::updateVelocity(" << deltaPixelRaw << " [delta pix], " << deltaTime << " [delta ms])";

    // faster than 2.5mm/ms seems bogus (that would be a screen height in ~20 ms)
    if (((deltaPixelRaw / qreal(deltaTime)).manhattanLength() / ((ppm.x() + ppm.y()) / 2) * 1000) > qreal(2.5))
        deltaPixel = deltaPixelRaw * qreal(2.5) * ppm / 1000 / (deltaPixelRaw / qreal(deltaTime)).manhattanLength();

    QPointF newv = -deltaPixel / qreal(deltaTime) * qreal(1000) / ppm;
    // around 95% of all updates are in the [1..50] ms range, so make sure
    // to scale the smoothing factor over that range: this way a 50ms update
    // will have full impact, while 5ms update will only have a 10% impact.
    qreal smoothing = sp->dragVelocitySmoothingFactor * qMin(qreal(deltaTime), qreal(50)) / qreal(50);

    // only smooth if we already have a release velocity and only if the
    // user hasn't stopped to move his finger for more than 100ms
    if ((releaseVelocity != QPointF(0, 0)) && (deltaTime < 100)) {
        qScrollerDebug() << "SMOOTHED from " << newv << " to " << newv * smoothing + releaseVelocity * (qreal(1) - smoothing);
        // smooth x or y only if the new velocity is either 0 or at least in
        // the same direction of the release velocity
        if (!newv.x() || (qSign(releaseVelocity.x()) == qSign(newv.x())))
            newv.setX(newv.x() * smoothing + releaseVelocity.x() * (qreal(1) - smoothing));
        if (!newv.y() || (qSign(releaseVelocity.y()) == qSign(newv.y())))
            newv.setY(newv.y() * smoothing + releaseVelocity.y() * (qreal(1) - smoothing));
    } else
        qScrollerDebug() << "NO SMOOTHING to " << newv;

    releaseVelocity.setX(qBound(-sp->maximumVelocity, newv.x(), sp->maximumVelocity));
    releaseVelocity.setY(qBound(-sp->maximumVelocity, newv.y(), sp->maximumVelocity));

    qScrollerDebug() << "  --> new velocity:" << releaseVelocity;
}

void QtScrollerPrivate::pushSegment(ScrollType type, qreal deltaTime, qreal stopProgress, qreal startPos, qreal deltaPos, qreal stopPos, QEasingCurve::Type curve, Qt::Orientation orientation)
{
    if (startPos == stopPos || deltaPos == 0)
        return;

    ScrollSegment s;
    if (orientation == Qt::Horizontal && !xSegments.isEmpty())
        s.startTime = xSegments.last().startTime + xSegments.last().deltaTime * xSegments.last().stopProgress;
    else if (orientation == Qt::Vertical && !ySegments.isEmpty())
        s.startTime = ySegments.last().startTime + ySegments.last().deltaTime * ySegments.last().stopProgress;
    else
        s.startTime = monotonicTimer.elapsed();

    s.startPos = startPos;
    s.deltaPos = deltaPos;
    s.stopPos = stopPos;
    s.deltaTime = deltaTime * 1000;
    s.stopProgress = stopProgress;
    s.curve.setType(curve);
    s.type = type;

    if (orientation == Qt::Horizontal)
        xSegments.enqueue(s);
    else
        ySegments.enqueue(s);

    qScrollerDebug() << "+++ Added a new ScrollSegment: " << s;
}


/*! \internal
    Clears the old segments and recalculates them if the current segments are not longer valid
*/
void QtScrollerPrivate::recalcScrollingSegments(bool forceRecalc)
{
    Q_Q(QtScroller);
    QPointF ppm = q->pixelPerMeter();

    releaseVelocity = q->velocity();

    if (forceRecalc || !scrollingSegmentsValid(Qt::Horizontal))
        createScrollingSegments(releaseVelocity.x(), contentPosition.x() + overshootPosition.x(), ppm.x(), Qt::Horizontal);

    if (forceRecalc || !scrollingSegmentsValid(Qt::Vertical))
        createScrollingSegments(releaseVelocity.y(), contentPosition.y() + overshootPosition.y(), ppm.y(), Qt::Vertical);
}

/*! \internal
    Returns the end position after the current scroll has finished.
*/
qreal QtScrollerPrivate::scrollingSegmentsEndPos(Qt::Orientation orientation) const
{
    if (orientation == Qt::Horizontal) {
        if (xSegments.isEmpty())
            return contentPosition.x() + overshootPosition.x();
        else
            return xSegments.last().stopPos;
    } else {
        if (ySegments.isEmpty())
            return contentPosition.y() + overshootPosition.y();
        else
            return ySegments.last().stopPos;
    }
}

/*! \internal
    Checks if the scroller segment end in a valid position.
*/
bool QtScrollerPrivate::scrollingSegmentsValid(Qt::Orientation orientation)
{
    QQueue<ScrollSegment> *segments;
    qreal minPos;
    qreal maxPos;

    if (orientation == Qt::Horizontal) {
        segments = &xSegments;
        minPos = contentPosRange.left();
        maxPos = contentPosRange.right();
    } else {
        segments = &ySegments;
        minPos = contentPosRange.top();
        maxPos = contentPosRange.bottom();
    }

    if (segments->isEmpty())
        return true;

    const ScrollSegment &last = segments->last();
    qreal stopPos = last.stopPos;

    if (last.type == ScrollTypeScrollTo)
        return true; // scrollTo is always valid

    if (last.type == ScrollTypeOvershoot &&
        (stopPos != minPos && stopPos != maxPos))
        return false;

    if (stopPos < minPos || stopPos > maxPos)
        return false;

    if (stopPos == minPos || stopPos == maxPos) // the begin and the end of the list are always ok
        return true;

    qreal nextSnap = nextSnapPos(stopPos, 0, orientation);
    if (!qIsNaN(nextSnap) && stopPos != nextSnap)
        return false;

    return true;
}

/*! \internal
   Creates the sections needed to scroll to the specific \a endPos to the segments queue.
*/
void QtScrollerPrivate::createScrollToSegments(qreal v, qreal deltaTime, qreal endPos, Qt::Orientation orientation, ScrollType type)
{
    Q_UNUSED(v);

    if (orientation == Qt::Horizontal)
        xSegments.clear();
    else
        ySegments.clear();

    qScrollerDebug() << "+++ createScrollToSegments: t:" << deltaTime << "ep:" << endPos << "o:" << int(orientation);

    const QtScrollerPropertiesPrivate *sp = properties.d.data();

    qreal startPos = (orientation == Qt::Horizontal) ? contentPosition.x() + overshootPosition.x()
                                                     : contentPosition.y() + overshootPosition.y();
    qreal deltaPos = (endPos - startPos) / 2;

    pushSegment(type, deltaTime * qreal(0.3), qreal(1.0), startPos, deltaPos, startPos + deltaPos, QEasingCurve::InQuad, orientation);
    pushSegment(type, deltaTime * qreal(0.7), qreal(1.0), startPos + deltaPos, deltaPos, endPos, sp->scrollingCurve.type(), orientation);
}

/*! \internal
*/
void QtScrollerPrivate::createScrollingSegments(qreal v, qreal startPos, qreal ppm, Qt::Orientation orientation)
{
    const QtScrollerPropertiesPrivate *sp = properties.d.data();

    QtScrollerProperties::OvershootPolicy policy;
    qreal minPos;
    qreal maxPos;
    qreal viewSize;

    if (orientation == Qt::Horizontal) {
        xSegments.clear();
        policy = sp->hOvershootPolicy;
        minPos = contentPosRange.left();
        maxPos = contentPosRange.right();
        viewSize = viewportSize.width();
    } else {
        ySegments.clear();
        policy = sp->vOvershootPolicy;
        minPos = contentPosRange.top();
        maxPos = contentPosRange.bottom();
        viewSize = viewportSize.height();
    }

    bool alwaysOvershoot = (policy == QtScrollerProperties::OvershootAlwaysOn);
    bool noOvershoot = (policy == QtScrollerProperties::OvershootAlwaysOff) || !sp->overshootScrollDistanceFactor;
    bool canOvershoot = !noOvershoot && (alwaysOvershoot || maxPos);

    qScrollerDebug() << "+++ createScrollingSegments: s:" << startPos << "maxPos:" << maxPos << "o:" << int(orientation);

    qScrollerDebug() << "v = " << v << ", decelerationFactor = " << sp->decelerationFactor << ", curveType = " << sp->scrollingCurve.type();

    // This is only correct for QEasingCurve::OutQuad (linear velocity,
    // constant deceleration), but the results look and feel ok for OutExpo
    // and OutSine as well

    // v(t) = deltaTime * a * 0.5 * differentialForProgress(t / deltaTime)
    // v(0) = vrelease
    // v(deltaTime) = 0
    // deltaTime = (2 * vrelease) / (a * differntial(0))

    // pos(t) = integrate(v(t)dt)
    // pos(t) = vrelease * t - 0.5 * a * t * t
    // pos(t) = deltaTime * a * 0.5 * progress(t / deltaTime) * deltaTime
    // deltaPos = pos(deltaTime)

    qreal deltaTime = (qreal(2) * qAbs(v)) / (sp->decelerationFactor * differentialForProgress(sp->scrollingCurve, 0));
    qreal deltaPos = qSign(v) * deltaTime * deltaTime * qreal(0.5) * sp->decelerationFactor * ppm;
    qreal endPos = startPos + deltaPos;

    qScrollerDebug() << "  Real Delta:" << deltaPos;

    // -- determine snap points
    qreal nextSnap = nextSnapPos(endPos, 0, orientation);
    qreal lowerSnapPos = nextSnapPos(startPos, -1, orientation);
    qreal higherSnapPos = nextSnapPos(startPos, 1, orientation);

    qScrollerDebug() << "  Real Delta:" << lowerSnapPos <<"-"<<nextSnap <<"-"<<higherSnapPos;

    // - check if we can reach another snap point
    if (nextSnap > higherSnapPos || qIsNaN(higherSnapPos))
        higherSnapPos = nextSnap;
    if (nextSnap < lowerSnapPos || qIsNaN(lowerSnapPos))
        lowerSnapPos = nextSnap;

    // -- check if are in overshoot and end in overshoot
    if ((startPos < minPos && endPos < minPos) ||
        (startPos > maxPos && endPos > maxPos)) {
        qreal stopPos = endPos < minPos ? minPos : maxPos;
        qreal oDeltaTime = sp->overshootScrollTime;

        pushSegment(ScrollTypeOvershoot, oDeltaTime * qreal(0.7), qreal(1.0), startPos, stopPos - startPos, stopPos, sp->scrollingCurve.type(), orientation);
        return;
    }

    if (qAbs(v) < sp->minimumVelocity) {

        qScrollerDebug() << "### below minimum Vel" << orientation;

        // - no snap points or already at one
        if (qIsNaN(nextSnap) || nextSnap == startPos)
            return; // nothing to do, no scrolling needed.

        // - decide which point to use

        qreal snapDistance = higherSnapPos - lowerSnapPos;

        qreal pressDistance = (orientation == Qt::Horizontal) ?
            lastPosition.x() - pressPosition.x() :
            lastPosition.y() - pressPosition.y();

        // if not dragged far enough, pick the next snap point.
        if (sp->snapPositionRatio == 0.0 || qAbs(pressDistance / sp->snapPositionRatio) > snapDistance)
            endPos = nextSnap;
        else if (pressDistance < 0.0)
            endPos = lowerSnapPos;
        else
            endPos = higherSnapPos;

        deltaPos = endPos - startPos;
        qreal midPos = startPos + deltaPos * qreal(0.3);
        pushSegment(ScrollTypeFlick, sp->snapTime * qreal(0.3), qreal(1.0), startPos, midPos - startPos, midPos, QEasingCurve::InQuad, orientation);
        pushSegment(ScrollTypeFlick, sp->snapTime * qreal(0.7), qreal(1.0), midPos, endPos - midPos, endPos, sp->scrollingCurve.type(), orientation);
        return;
    }

    // - go to the next snappoint if there is one
    if (v > 0 && !qIsNaN(higherSnapPos)) {
        // change the time in relation to the changed end position
        if (endPos - startPos)
            deltaTime *= qAbs((higherSnapPos - startPos) / (endPos - startPos));
        if (deltaTime > sp->snapTime)
            deltaTime = sp->snapTime;
        endPos = higherSnapPos;

    } else if (v < 0 && !qIsNaN(lowerSnapPos)) {
        // change the time in relation to the changed end position
        if (endPos - startPos)
            deltaTime *= qAbs((lowerSnapPos - startPos) / (endPos - startPos));
        if (deltaTime > sp->snapTime)
            deltaTime = sp->snapTime;
        endPos = lowerSnapPos;

    // -- check if we are overshooting
    } else if (endPos < minPos || endPos > maxPos) {
        qreal stopPos = endPos < minPos ? minPos : maxPos;

        qScrollerDebug() << "Overshoot: delta:" << (stopPos - startPos);

        qreal stopProgress = progressForValue(sp->scrollingCurve, qAbs((stopPos - startPos) / deltaPos));

        if (!canOvershoot) {
            qScrollerDebug() << "Overshoot stopp:" << stopProgress;

            pushSegment(ScrollTypeFlick, deltaTime, stopProgress, startPos, endPos, stopPos, sp->scrollingCurve.type(), orientation);
        } else {
            qreal oDeltaTime = sp->overshootScrollTime;
            qreal oStopProgress = qMin(stopProgress + oDeltaTime * qreal(0.3) / deltaTime, qreal(1));
            qreal oDistance = startPos + deltaPos * sp->scrollingCurve.valueForProgress(oStopProgress) - stopPos;
            qreal oMaxDistance = qSign(oDistance) * (viewSize * sp->overshootScrollDistanceFactor);

            qScrollerDebug() << "1 oDistance:" << oDistance << "Max:" << oMaxDistance << "stopP/oStopP" << stopProgress << oStopProgress;

            if (qAbs(oDistance) > qAbs(oMaxDistance)) {
                oStopProgress = progressForValue(sp->scrollingCurve, qAbs((stopPos + oMaxDistance - startPos) / deltaPos));
                oDistance = oMaxDistance;
                qScrollerDebug() << "2 oDistance:" << oDistance << "Max:" << oMaxDistance << "stopP/oStopP" << stopProgress << oStopProgress;
            }

            pushSegment(ScrollTypeFlick, deltaTime, oStopProgress, startPos, deltaPos, stopPos + oDistance, sp->scrollingCurve.type(), orientation);
            pushSegment(ScrollTypeOvershoot, oDeltaTime * qreal(0.7), qreal(1.0), stopPos + oDistance, -oDistance, stopPos, sp->scrollingCurve.type(), orientation);
        }
        return;
    }

    pushSegment(ScrollTypeFlick, deltaTime, qreal(1.0), startPos, deltaPos, endPos, sp->scrollingCurve.type(), orientation);
}


/*! \internal
    Prepares scrolling by sending a QScrollPrepareEvent to the receiver widget.
    Returns true if the scrolling was accepted and a target was returned.
*/
bool QtScrollerPrivate::prepareScrolling(const QPointF &position)
{
    QtScrollPrepareEvent spe(position);
    spe.ignore();
    sendEvent(target, &spe);

    qScrollerDebug() << "QScrollPrepareEvent returned from" << target << "with" << spe.isAccepted() << "mcp:" << spe.contentPosRange() << "cp:" << spe.contentPos();
    if (spe.isAccepted()) {
        QPointF oldContentPos = contentPosition + overshootPosition;
        QPointF contentDelta = spe.contentPos() - oldContentPos;

        viewportSize = spe.viewportSize();
        contentPosRange = spe.contentPosRange();
        if (contentPosRange.width() < 0)
            contentPosRange.setWidth(0);
        if (contentPosRange.height() < 0)
            contentPosRange.setHeight(0);
        contentPosition = clampToRect(spe.contentPos(), contentPosRange);
        overshootPosition = spe.contentPos() - contentPosition;

        // - check if the content position was moved
        if (contentDelta != QPointF(0, 0)) {
            // need to correct all segments
            for (int i = 0; i < xSegments.count(); i++)
                xSegments[i].startPos -= contentDelta.x();

            for (int i = 0; i < ySegments.count(); i++)
                ySegments[i].startPos -= contentDelta.y();
        }

        if (QWidget *w = qobject_cast<QWidget *>(target))
            setDpiFromWidget(w);
        if (QGraphicsObject *go = qobject_cast<QGraphicsObject *>(target)) {
            //TODO: the first view isn't really correct - maybe use an additional field in the prepare event?
            if (go->scene() && !go->scene()->views().isEmpty())
                setDpiFromWidget(go->scene()->views().first());
        }

        if (state == QtScroller::Scrolling) {
            recalcScrollingSegments();
        }
        return true;
    }

    return false;
}

void QtScrollerPrivate::handleDrag(const QPointF &position, qint64 timestamp)
{
    const QtScrollerPropertiesPrivate *sp = properties.d.data();

    QPointF deltaPixel = position - lastPosition;
    qint64 deltaTime = timestamp - lastTimestamp;

    if (sp->axisLockThreshold) {
        int dx = qAbs(deltaPixel.x());
        int dy = qAbs(deltaPixel.y());
        if (dx || dy) {
            bool vertical = (dy > dx);
            qreal alpha = qreal(vertical ? dx : dy) / qreal(vertical ? dy : dx);
            //qScrollerDebug() << "QtScroller::handleDrag() -- axis lock:" << alpha << " / " << axisLockThreshold << "- isvertical:" << vertical << "- dx:" << dx << "- dy:" << dy;
            if (alpha <= sp->axisLockThreshold) {
                if (vertical)
                    deltaPixel.setX(0);
                else
                    deltaPixel.setY(0);
            }
        }
    }

    // calculate velocity (if the user would release the mouse NOW)
    updateVelocity(deltaPixel, deltaTime);

    // restrict velocity, if content is not scrollable
    QRectF max = contentPosRange;
    bool canScrollX = (max.width() > 0) || (sp->hOvershootPolicy == QtScrollerProperties::OvershootAlwaysOn);
    bool canScrollY = (max.height() > 0) || (sp->vOvershootPolicy == QtScrollerProperties::OvershootAlwaysOn);

    if (!canScrollX) {
        deltaPixel.setX(0);
        releaseVelocity.setX(0);
    }
    if (!canScrollY) {
        deltaPixel.setY(0);
        releaseVelocity.setY(0);
    }

//    if (firstDrag) {
//        // Do not delay the first drag
//        setContentPositionHelper(q->contentPosition() - overshootDistance - deltaPixel);
//        dragDistance = QPointF(0, 0);
//    } else {
    dragDistance += deltaPixel;
//    }
//qScrollerDebug() << "######################" << deltaPixel << position.y() << lastPosition.y();
    if (canScrollX)
        lastPosition.setX(position.x());
    if (canScrollY)
        lastPosition.setY(position.y());
    lastTimestamp = timestamp;
}

bool QtScrollerPrivate::pressWhileInactive(const QPointF &position, qint64 timestamp)
{
    if (prepareScrolling(position)) {
        const QtScrollerPropertiesPrivate *sp = properties.d.data();

        if (!contentPosRange.isNull() ||
            (sp->hOvershootPolicy == QtScrollerProperties::OvershootAlwaysOn) ||
            (sp->vOvershootPolicy == QtScrollerProperties::OvershootAlwaysOn)) {

            lastPosition = pressPosition = position;
            lastTimestamp = pressTimestamp = timestamp;
            setState(QtScroller::Pressed);
        }
    }
    return false;
}

bool QtScrollerPrivate::releaseWhilePressed(const QPointF &, qint64)
{
    if (overshootPosition != QPointF(0.0, 0.0)) {
        setState(QtScroller::Scrolling);
        return true;
    } else {
        setState(QtScroller::Inactive);
        return false;
    }
}

bool QtScrollerPrivate::moveWhilePressed(const QPointF &position, qint64 timestamp)
{
    Q_Q(QtScroller);
    const QtScrollerPropertiesPrivate *sp = properties.d.data();
    QPointF ppm = q->pixelPerMeter();

    QPointF deltaPixel = position - pressPosition;

    bool moveAborted = false;
    bool moveStarted = (((deltaPixel / ppm).manhattanLength()) > sp->dragStartDistance);

    // check the direction of the mouse drag and abort if it's too much in the wrong direction.
    if (moveStarted) {
        QRectF max = contentPosRange;
        bool canScrollX = (max.width() > 0);
        bool canScrollY = (max.height() > 0);

        if (sp->hOvershootPolicy == QtScrollerProperties::OvershootAlwaysOn)
            canScrollX = true;
        if (sp->vOvershootPolicy == QtScrollerProperties::OvershootAlwaysOn)
            canScrollY = true;

        if (qAbs(deltaPixel.x() / ppm.x()) < qAbs(deltaPixel.y() / ppm.y())) {
            if (!canScrollY)
                moveAborted = true;
        } else {
            if (!canScrollX)
                moveAborted = true;
        }
    }

    if (moveAborted) {
        setState(QtScroller::Inactive);
        moveStarted = false;

    } else if (moveStarted) {
        setState(QtScroller::Dragging);

        // subtract the dragStartDistance
        deltaPixel = deltaPixel - deltaPixel * (sp->dragStartDistance / deltaPixel.manhattanLength());

        if (deltaPixel != QPointF(0, 0)) {
            // handleDrag updates lastPosition, lastTimestamp and velocity
            handleDrag(pressPosition + deltaPixel, timestamp);
        }
    }
    return moveStarted;
}

bool QtScrollerPrivate::moveWhileDragging(const QPointF &position, qint64 timestamp)
{
    // handleDrag updates lastPosition, lastTimestamp and velocity
    handleDrag(position, timestamp);
    return true;
}

void QtScrollerPrivate::timerEventWhileDragging()
{
    if (dragDistance != QPointF(0, 0)) {
        qScrollerDebug() << "QtScroller::timerEventWhileDragging() -- dragDistance:" << dragDistance;

        setContentPositionHelperDragging(-dragDistance);
        dragDistance = QPointF(0, 0);
    }
}

bool QtScrollerPrivate::releaseWhileDragging(const QPointF &position, qint64 timestamp)
{
    Q_Q(QtScroller);
    const QtScrollerPropertiesPrivate *sp = properties.d.data();

    // handleDrag updates lastPosition, lastTimestamp and velocity
    handleDrag(position, timestamp);

    // check if we moved at all - this can happen if you stop a running
    // scroller with a press and release shortly afterwards
    QPointF deltaPixel = position - pressPosition;
    if (((deltaPixel / q->pixelPerMeter()).manhattanLength()) > sp->dragStartDistance) {

        // handle accelerating flicks
        if ((oldVelocity != QPointF(0, 0)) && sp->acceleratingFlickMaximumTime &&
            ((timestamp - pressTimestamp) < qint64(sp->acceleratingFlickMaximumTime * 1000))) {

            // - determine if the direction was changed
            int signX = 0, signY = 0;
            if (releaseVelocity.x())
                signX = (releaseVelocity.x() > 0) == (oldVelocity.x() > 0) ? 1 : -1;
            if (releaseVelocity.y())
                signY = (releaseVelocity.y() > 0) == (oldVelocity.y() > 0) ? 1 : -1;

            if (signX > 0)
                releaseVelocity.setX(qBound(-sp->maximumVelocity,
                                            oldVelocity.x() * sp->acceleratingFlickSpeedupFactor,
                                            sp->maximumVelocity));
            if (signY > 0)
                releaseVelocity.setY(qBound(-sp->maximumVelocity,
                                            oldVelocity.y() * sp->acceleratingFlickSpeedupFactor,
                                            sp->maximumVelocity));
        }
    }

    QPointF ppm = q->pixelPerMeter();
    createScrollingSegments(releaseVelocity.x(), contentPosition.x() + overshootPosition.x(), ppm.x(), Qt::Horizontal);
    createScrollingSegments(releaseVelocity.y(), contentPosition.y() + overshootPosition.y(), ppm.y(), Qt::Vertical);

    qScrollerDebug() << "QtScroller::releaseWhileDragging() -- velocity:" << releaseVelocity << "-- minimum velocity:" << sp->minimumVelocity << "overshoot" << overshootPosition;

    if (xSegments.isEmpty() && ySegments.isEmpty())
        setState(QtScroller::Inactive);
    else
        setState(QtScroller::Scrolling);

    return true;
}

void QtScrollerPrivate::timerEventWhileScrolling()
{
    qScrollerDebug() << "QtScroller::timerEventWhileScrolling()";

    setContentPositionHelperScrolling();
    if (xSegments.isEmpty() && ySegments.isEmpty())
        setState(QtScroller::Inactive);
}

bool QtScrollerPrivate::pressWhileScrolling(const QPointF &position, qint64 timestamp)
{
    Q_Q(QtScroller);

    if ((q->velocity() <= properties.d->maximumClickThroughVelocity) &&
        (overshootPosition == QPointF(0.0, 0.0))) {
        setState(QtScroller::Inactive);
        return false;
    } else {
        lastPosition = pressPosition = position;
        lastTimestamp = pressTimestamp = timestamp;
        setState(QtScroller::Pressed);
        setState(QtScroller::Dragging);
        return true;
    }
}

/*! \internal
    This function handles all state changes of the scroller.
*/
void QtScrollerPrivate::setState(QtScroller::State newstate)
{
    Q_Q(QtScroller);
    bool sendLastScroll = false;

    if (state == newstate)
        return;

    qScrollerDebug() << q << "QtScroller::setState(" << stateName(newstate) << ")";

    switch (newstate) {
    case QtScroller::Inactive:
        scrollTimer->stop();

        // send the last scroll event (but only after the current state change was finished)
        if (!firstScroll)
            sendLastScroll = true;

        releaseVelocity = QPointF(0, 0);
        break;

    case QtScroller::Pressed:
        scrollTimer->stop();

        oldVelocity = releaseVelocity;
        releaseVelocity = QPointF(0, 0);
        break;

    case QtScroller::Dragging:
        dragDistance = QPointF(0, 0);
        if (state == QtScroller::Pressed)
            scrollTimer->start();
        break;

    case QtScroller::Scrolling:
        scrollTimer->start();
        break;
    }

    qSwap(state, newstate);

    if (sendLastScroll) {
        QtScrollEvent se(contentPosition, overshootPosition, QtScrollEvent::ScrollFinished);
        sendEvent(target, &se);
        firstScroll = true;
    }
    if (state == QtScroller::Dragging || state == QtScroller::Scrolling)
        activeScrollers.insert(q);
    else
        activeScrollers.remove(q);
    emit q->stateChanged(state);
}


/*! \internal
    Helps when setting the content position.
    It will try to move the content by the requested delta but stop in case
    when we are coming back from an overshoot or a scrollTo.
    It will also indicate a new overshooting condition by the overshootX and oversthootY flags.

    In this cases it will reset the velocity variables and other flags.

    Also keeps track of the current over-shooting value in overshootPosition.

    \a deltaPos is the amount of pixels the current content position should be moved
*/
void QtScrollerPrivate::setContentPositionHelperDragging(const QPointF &deltaPos)
{
    Q_Q(QtScroller);
    QPointF ppm = q->pixelPerMeter();
    const QtScrollerPropertiesPrivate *sp = properties.d.data();
    QPointF v = q->velocity();

    if (sp->overshootDragResistanceFactor)
        overshootPosition /= sp->overshootDragResistanceFactor;

    QPointF oldPos = contentPosition + overshootPosition;
    QPointF newPos = oldPos + deltaPos;

    qScrollerDebug() << "QtScroller::setContentPositionHelperDragging(" << deltaPos << " [pix])";
    qScrollerDebug() << "  --> overshoot:" << overshootPosition << "- old pos:" << oldPos << "- new pos:" << newPos;

    QPointF oldClampedPos = clampToRect(oldPos, contentPosRange);
    QPointF newClampedPos = clampToRect(newPos, contentPosRange);

    // --- handle overshooting and stop if the coordinate is going back inside the normal area
    bool alwaysOvershootX = (sp->hOvershootPolicy == QtScrollerProperties::OvershootAlwaysOn);
    bool alwaysOvershootY = (sp->vOvershootPolicy == QtScrollerProperties::OvershootAlwaysOn);
    bool noOvershootX = (sp->hOvershootPolicy == QtScrollerProperties::OvershootAlwaysOff) ||
                        ((state == QtScroller::Dragging) && !sp->overshootDragResistanceFactor) ||
                        !sp->overshootDragDistanceFactor;
    bool noOvershootY = (sp->vOvershootPolicy == QtScrollerProperties::OvershootAlwaysOff) ||
                        ((state == QtScroller::Dragging) && !sp->overshootDragResistanceFactor) ||
                        !sp->overshootDragDistanceFactor;
    bool canOvershootX = !noOvershootX && (alwaysOvershootX || contentPosRange.width());
    bool canOvershootY = !noOvershootY && (alwaysOvershootY || contentPosRange.height());

    qreal oldOvershootX = (canOvershootX) ? oldPos.x() - oldClampedPos.x() : 0;
    qreal oldOvershootY = (canOvershootY) ? oldPos.y() - oldClampedPos.y() : 0;

    qreal newOvershootX = (canOvershootX) ? newPos.x() - newClampedPos.x() : 0;
    qreal newOvershootY = (canOvershootY) ? newPos.y() - newClampedPos.y() : 0;

    qreal maxOvershootX = viewportSize.width() * sp->overshootDragDistanceFactor;
    qreal maxOvershootY = viewportSize.height() * sp->overshootDragDistanceFactor;

    qScrollerDebug() << "  --> noOs:" << noOvershootX << "drf:" << sp->overshootDragResistanceFactor << "mdf:" << sp->overshootScrollDistanceFactor << "ossP:"<<sp->hOvershootPolicy;
    qScrollerDebug() << "  --> canOS:" << canOvershootX << "newOS:" << newOvershootX << "maxOS:" << maxOvershootX;

    if (sp->overshootDragResistanceFactor) {
        oldOvershootX *= sp->overshootDragResistanceFactor;
        oldOvershootY *= sp->overshootDragResistanceFactor;
        newOvershootX *= sp->overshootDragResistanceFactor;
        newOvershootY *= sp->overshootDragResistanceFactor;
    }

    // -- stop at the maximum overshoot distance

    newOvershootX = qBound(-maxOvershootX, newOvershootX, maxOvershootX);
    newOvershootY = qBound(-maxOvershootY, newOvershootY, maxOvershootY);

    overshootPosition.setX(newOvershootX);
    overshootPosition.setY(newOvershootY);
    contentPosition = newClampedPos;

    QtScrollEvent se(contentPosition, overshootPosition, firstScroll ? QtScrollEvent::ScrollStarted : QtScrollEvent::ScrollUpdated);
    sendEvent(target, &se);
    firstScroll = false;

    qScrollerDebug() << "  --> new position:" << newClampedPos << "- new overshoot:" << overshootPosition <<
                        "- overshoot x/y?:" << overshootPosition;
}


qreal QtScrollerPrivate::nextSegmentPosition(QQueue<ScrollSegment> &segments, qint64 now, qreal oldPos)
{
    qreal pos = oldPos;

    // check the X segments for new positions
    while (!segments.isEmpty()) {
        const ScrollSegment s = segments.head();

        if ((s.startTime + s.deltaTime * s.stopProgress) <= now) {
            segments.dequeue();
            pos = s.stopPos;
        } else if (s.startTime <= now) {
            qreal progress = qreal(now - s.startTime) / qreal(s.deltaTime);
            pos = s.startPos + s.deltaPos * s.curve.valueForProgress(progress);
            if (s.deltaPos > 0 ? pos > s.stopPos : pos < s.stopPos) {
                segments.dequeue();
                pos = s.stopPos;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    return pos;
}

void QtScrollerPrivate::setContentPositionHelperScrolling()
{
    qint64 now = monotonicTimer.elapsed();
    QPointF newPos = contentPosition + overshootPosition;

    newPos.setX(nextSegmentPosition(xSegments, now, newPos.x()));
    newPos.setY(nextSegmentPosition(ySegments, now, newPos.y()));

    // -- set the position and handle overshoot
    qScrollerDebug() << "QtScroller::setContentPositionHelperScrolling()";
    qScrollerDebug() << "  --> overshoot:" << overshootPosition << "- new pos:" << newPos;

    QPointF newClampedPos = clampToRect(newPos, contentPosRange);

    overshootPosition = newPos - newClampedPos;
    contentPosition = newClampedPos;

    QtScrollEvent se(contentPosition, overshootPosition, firstScroll ? QtScrollEvent::ScrollStarted : QtScrollEvent::ScrollUpdated);
    sendEvent(target, &se);
    firstScroll = false;

    qScrollerDebug() << "  --> new position:" << newClampedPos << "- new overshoot:" << overshootPosition;
}

/*! \internal
    Returns the next snap point in direction.
    If \a direction >0 it will return the next snap point that is larger than the current position.
    If \a direction <0 it will return the next snap point that is smaller than the current position.
    If \a direction ==0 it will return the nearest snap point (or the current position if we are already
    on a snap point.
    Returns the nearest snap position or NaN if no such point could be found.
 */
qreal QtScrollerPrivate::nextSnapPos(qreal p, int dir, Qt::Orientation orientation)
{
    qreal bestSnapPos = Q_QNAN;
    qreal bestSnapPosDist = Q_INFINITY;

    qreal minPos;
    qreal maxPos;

    if (orientation == Qt::Horizontal) {
        minPos = contentPosRange.left();
        maxPos = contentPosRange.right();
    } else {
        minPos = contentPosRange.top();
        maxPos = contentPosRange.bottom();
    }

    if (orientation == Qt::Horizontal) {
        // the snap points in the list
        foreach (qreal snapPos, snapPositionsX) {
            qreal snapPosDist = snapPos - p;
            if ((dir > 0 && snapPosDist < 0) ||
                (dir < 0 && snapPosDist > 0))
                continue; // wrong direction
            if (snapPos < minPos || snapPos > maxPos)
                continue; // invalid

            if (qIsNaN(bestSnapPos) ||
                qAbs(snapPosDist) < bestSnapPosDist) {
                bestSnapPos = snapPos;
                bestSnapPosDist = qAbs(snapPosDist);
            }
        }

        // the snap point interval
        if (snapIntervalX > 0.0) {
            qreal first = minPos + snapFirstX;
            qreal snapPos;
            if (dir > 0)
                snapPos = qCeil((p - first) / snapIntervalX) * snapIntervalX + first;
            else if (dir < 0)
                snapPos = qFloor((p - first) / snapIntervalX) * snapIntervalX + first;
            else if (p <= first)
                snapPos = first;
            else
            {
                qreal last = qFloor((maxPos - first) / snapIntervalX) * snapIntervalX + first;
                if (p >= last)
                    snapPos = last;
                else
                    snapPos = qRound((p - first) / snapIntervalX) * snapIntervalX + first;
            }

            if (snapPos >= first && snapPos <= maxPos ) {
                qreal snapPosDist = snapPos - p;

                if (qIsNaN(bestSnapPos) ||
                    qAbs(snapPosDist) < bestSnapPosDist) {
                    bestSnapPos = snapPos;
                    bestSnapPosDist = qAbs(snapPosDist);
                }
            }
        }

    } else { // (orientation == Qt::Vertical)
        // the snap points in the list
        foreach (qreal snapPos, snapPositionsY) {
            qreal snapPosDist = snapPos - p;
            if ((dir > 0 && snapPosDist < 0) ||
                (dir < 0 && snapPosDist > 0))
                continue; // wrong direction
            if (snapPos < minPos || snapPos > maxPos)
                continue; // invalid

            if (qIsNaN(bestSnapPos) ||
                qAbs(snapPosDist) < bestSnapPosDist) {
                bestSnapPos = snapPos;
                bestSnapPosDist = qAbs(snapPosDist);
            }
        }

        // the snap point interval
        if (snapIntervalY > 0.0) {
            qreal first = minPos + snapFirstY;
            qreal snapPos;
            if (dir > 0)
                snapPos = qCeil((p - first) / snapIntervalY) * snapIntervalY + first;
            else if (dir < 0)
                snapPos = qFloor((p - first) / snapIntervalY) * snapIntervalY + first;
            else if (p <= first)
                snapPos = first;
            else
            {
                qreal last = qFloor((maxPos - first) / snapIntervalY) * snapIntervalY + first;
                if (p >= last)
                    snapPos = last;
                else
                    snapPos = qRound((p - first) / snapIntervalY) * snapIntervalY + first;
            }

            if (snapPos >= first && snapPos <= maxPos ) {
                qreal snapPosDist = snapPos - p;

                if (qIsNaN(bestSnapPos) ||
                    qAbs(snapPosDist) < bestSnapPosDist) {
                    bestSnapPos = snapPos;
                    bestSnapPosDist = qAbs(snapPosDist);
                }
            }
        }
    }

    return bestSnapPos;
}

/*!
    \enum QtScroller::State

    This enum contains the different QtScroller states.

    \value Inactive The scroller is not scrolling and nothing is pressed.
    \value Pressed A touch event was received or the mouse button was pressed but the scroll area is currently not dragged.
    \value Dragging The scroll area is currently following the touch point or mouse.
    \value Scrolling The scroll area is moving on it's own.
*/

/*!
    \enum QtScroller::ScrollerGestureType

    This enum contains the different gesture types that are supported by the QtScroller gesture recognizer.

    \value TouchGesture The gesture recognizer will only trigger on touch
        events.  Specifically it will react on single touch points when using a
        touch screen and dual touch points when using a touchpad.
    \value LeftMouseButtonGesture The gesture recognizer will only trigger on left mouse button events.
    \value MiddleMouseButtonGesture The gesture recognizer will only trigger on middle mouse button events.
    \value RightMouseButtonGesture The gesture recognizer will only trigger on right mouse button events.
*/

/*!
    \enum QtScroller::Input

    This enum contains an input device agnostic view of input events that are relevant for QtScroller.

    \value InputPress   The user pressed the input device (e.g.  QEvent::MouseButtonPress,
                        QEvent::GraphicsSceneMousePress, QEvent::TouchBegin)

    \value InputMove    The user moved the input device (e.g.  QEvent::MouseMove,
                        QEvent::GraphicsSceneMouseMove, QEvent::TouchUpdate)

    \value InputRelease The user released the input device (e.g.  QEvent::MouseButtonRelease,
                        QEvent::GraphicsSceneMouseRelease, QEvent::TouchEnd)

*/

