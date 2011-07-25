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

#include "qgesture.h"
#include "qapplication.h"
#include "qevent.h"
#include "qwidget.h"
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"
#include "qgraphicssceneevent.h"
#include "qgraphicsview.h"
#if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
#  include <QTime>
typedef QTime QElapsedTimer;
#else
#  include <QElapsedTimer>
#endif
#include "qtscroller.h"
#include "qtflickgesture_p.h"
#include "qdebug.h"

#ifndef QT_NO_GESTURES

//#define QFLICKGESTURE_DEBUG

#ifdef QFLICKGESTURE_DEBUG
#  define qFGDebug  qDebug
#else
#  define qFGDebug  while (false) qDebug
#endif

extern bool qt_sendSpontaneousEvent(QObject *receiver, QEvent *event);

static QMouseEvent *copyMouseEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove: {
        QMouseEvent *me = static_cast<QMouseEvent *>(e);
        return new QMouseEvent(me->type(), QPoint(0, 0), me->globalPos(), me->button(), me->buttons(), me->modifiers());
    }
    case QEvent::GraphicsSceneMousePress:
    case QEvent::GraphicsSceneMouseRelease:
    case QEvent::GraphicsSceneMouseMove: {
        QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent *>(e);
#if 1
        QEvent::Type met = me->type() == QEvent::GraphicsSceneMousePress ? QEvent::MouseButtonPress :
                           (me->type() == QEvent::GraphicsSceneMouseRelease ? QEvent::MouseButtonRelease : QEvent::MouseMove);
        return new QMouseEvent(met, QPoint(0, 0), me->screenPos(), me->button(), me->buttons(), me->modifiers());
#else
        QGraphicsSceneMouseEvent *copy = new QGraphicsSceneMouseEvent(me->type());
        copy->setPos(me->pos());
        copy->setScenePos(me->scenePos());
        copy->setScreenPos(me->screenPos());
        for (int i = 0x1; i <= 0x10; i <<= 1) {
            Qt::MouseButton button = Qt::MouseButton(i);
            copy->setButtonDownPos(button, me->buttonDownPos(button));
            copy->setButtonDownScenePos(button, me->buttonDownScenePos(button));
            copy->setButtonDownScreenPos(button, me->buttonDownScreenPos(button));
        }
        copy->setLastPos(me->lastPos());
        copy->setLastScenePos(me->lastScenePos());
        copy->setLastScreenPos(me->lastScreenPos());
        copy->setButtons(me->buttons());
        copy->setButton(me->button());
        copy->setModifiers(me->modifiers());
        return copy;
#endif
    }
    default:
        return 0;
    }
}

class PressDelayHandler : public QObject
{
private:
    PressDelayHandler(QObject *parent = 0)
        : QObject(parent)
        , pressDelayTimer(0)
        , sendingEvent(false)
        , mouseButton(Qt::NoButton)
        , mouseTarget(0)
    { }

    static PressDelayHandler *inst;

public:
    enum {
        UngrabMouseBefore = 1,
        RegrabMouseAfterwards = 2
    };

    static PressDelayHandler *instance()
    {
        static PressDelayHandler *inst = 0;
        if (!inst)
            inst = new PressDelayHandler(QCoreApplication::instance());
        return inst;
    }

    bool shouldEventBeIgnored(QEvent *) const
    {
        return sendingEvent;
    }

    bool isDelaying() const
    {
        return !pressDelayEvent.isNull();
    }

    void pressed(QEvent *e, int delay)
    {
        if (!pressDelayEvent) {
            pressDelayEvent.reset(copyMouseEvent(e));
            pressDelayTimer = startTimer(delay);
            mouseTarget = QApplication::widgetAt(pressDelayEvent->globalPos());
            mouseButton = pressDelayEvent->button();
            qFGDebug() << "QFG: consuming/delaying mouse press";
        } else {
            qFGDebug() << "QFG: NOT consuming/delaying mouse press";
        }
        e->setAccepted(true);
    }

    bool released(QEvent *e, bool scrollerWasActive, bool scrollerIsActive)
    {
        // consume this event if the scroller was or is active
        bool result = scrollerWasActive || scrollerIsActive;

        // stop the timer
        if (pressDelayTimer) {
            killTimer(pressDelayTimer);
            pressDelayTimer = 0;
        }
        // we still haven't even sent the press, so do it now
        if (pressDelayEvent && mouseTarget && !scrollerIsActive) {
            QScopedPointer<QMouseEvent> releaseEvent(copyMouseEvent(e));

            qFGDebug() << "QFG: re-sending mouse press (due to release) for " << mouseTarget;
            sendMouseEvent(pressDelayEvent.data(), UngrabMouseBefore);

            qFGDebug() << "QFG: faking mouse release (due to release) for " << mouseTarget;
            sendMouseEvent(releaseEvent.data());
            result = true; // consume this event
        } else if (mouseTarget && scrollerIsActive) {
            // we grabbed the mouse expicitly when the scroller became active, so undo that now
            sendMouseEvent(0, UngrabMouseBefore);
        }
        pressDelayEvent.reset(0);
        mouseTarget = 0;
        return result;
    }

    void scrollerWasIntercepted()
    {
        qFGDebug() << "QFG: deleting delayed mouse press, since scroller was only intercepted";
        if (pressDelayEvent) {
            // we still haven't even sent the press, so just throw it away now
            if (pressDelayTimer) {
                killTimer(pressDelayTimer);
                pressDelayTimer = 0;
            }
            pressDelayEvent.reset(0);
        }
        mouseTarget = 0;
    }

    void scrollerBecameActive()
    {
        if (pressDelayEvent) {
            // we still haven't even sent the press, so just throw it away now
            qFGDebug() << "QFG: deleting delayed mouse press, since scroller is active now";
            if (pressDelayTimer) {
                killTimer(pressDelayTimer);
                pressDelayTimer = 0;
            }
            pressDelayEvent.reset(0);
            mouseTarget = 0;
        } else if (mouseTarget) {
            // we did send a press, so we need to fake a release now
            Qt::MouseButtons mouseButtons = QApplication::mouseButtons();

            // release all pressed mouse buttons
            /*for (int i = 0; i < 32; ++i) {
                if (mouseButtons & (1 << i)) {
                    Qt::MouseButton b = static_cast<Qt::MouseButton>(1 << i);
                    mouseButtons &= ~b;
                    QPoint farFarAway(-QWIDGETSIZE_MAX, -QWIDGETSIZE_MAX);

                    qFGDebug() << "QFG: sending a fake mouse release at far-far-away to " << mouseTarget;
                    QMouseEvent re(QEvent::MouseButtonRelease, QPoint(), farFarAway,
                                   b, mouseButtons, QApplication::keyboardModifiers());
                    sendMouseEvent(&re);
                }
            }*/

            QPoint farFarAway(-QWIDGETSIZE_MAX, -QWIDGETSIZE_MAX);

            qFGDebug() << "QFG: sending a fake mouse release at far-far-away to " << mouseTarget;
            QMouseEvent re(QEvent::MouseButtonRelease, QPoint(), farFarAway,
                           mouseButton, QApplication::mouseButtons() & ~mouseButton,
                           QApplication::keyboardModifiers());
            sendMouseEvent(&re, RegrabMouseAfterwards);
            // don't clear the mouseTarget just yet, since we need to explicitly ungrab the mouse on release!
        }
    }

protected:
    void timerEvent(QTimerEvent *e)
    {
        if (e->timerId() == pressDelayTimer) {
            if (pressDelayEvent && mouseTarget) {
                qFGDebug() << "QFG: timer event: re-sending mouse press to " << mouseTarget;
                sendMouseEvent(pressDelayEvent.data(), UngrabMouseBefore);
            }
            pressDelayEvent.reset(0);

            if (pressDelayTimer) {
                killTimer(pressDelayTimer);
                pressDelayTimer = 0;
            }
        }
    }

    void sendMouseEvent(QMouseEvent *me, int flags = 0)
    {
        if (mouseTarget) {
            sendingEvent = true;

            QGraphicsItem *grabber = 0;
            if (mouseTarget->parentWidget()) {
                if (QGraphicsView *gv = qobject_cast<QGraphicsView *>(mouseTarget->parentWidget())) {
                    if (gv->scene())
                        grabber = gv->scene()->mouseGrabberItem();
                }
            }

            if (grabber && (flags & UngrabMouseBefore)) {
                // GraphicsView Mouse Handling Workaround #1:
                // we need to ungrab the mouse before re-sending the press,
                // since the scene had already set the mouse grabber to the
                // original (and consumed) event's receiver
                qFGDebug() << "QFG: ungrabbing" << grabber;
                grabber->ungrabMouse();
            }

            if (me) {
                QMouseEvent copy(me->type(), mouseTarget->mapFromGlobal(me->globalPos()), me->globalPos(), me->button(), me->buttons(), me->modifiers());
                qt_sendSpontaneousEvent(mouseTarget, &copy);
            }

            if (grabber && (flags & RegrabMouseAfterwards)) {
                // GraphicsView Mouse Handling Workaround #2:
                // we need to re-grab the mouse after sending a faked mouse
                // release, since we still need the mouse moves for the gesture
                // (the scene will clear the item's mouse grabber status on
                // release).
                qFGDebug() << "QFG: re-grabbing" << grabber;
                grabber->grabMouse();
            }
            sendingEvent = false;
        }
    }


private:
    int pressDelayTimer;
    QScopedPointer<QMouseEvent> pressDelayEvent;
    bool sendingEvent;
    Qt::MouseButton mouseButton;
    QPointer<QWidget> mouseTarget;
};


/*!
    \internal
    \class QFlickGesture
    \since 4.8
    \brief The QFlickGesture class describes a flicking gesture made by the user.
    \ingroup gestures
    The QFlickGesture is more complex than the QPanGesture that uses QScroller and QScrollerProperties
    to decide if it is triggered.
    This gesture is reacting on touch event as compared to the QMouseFlickGesture.

    \sa {Gestures Programming}, QScroller, QScrollerProperties, QMouseFlickGesture
*/

/*!
    \internal
*/
QtFlickGesture::QtFlickGesture(QObject *receiver_, Qt::MouseButton button, QObject *parent)
    : QGesture(parent), receiver(receiver_), receiverScroller(0), button(button),
      macIgnoreWheel(false)
{
    receiverScroller = (receiver && QtScroller::hasScroller(receiver)) ? QtScroller::scroller(receiver) : 0;
}

QtFlickGesture::~QtFlickGesture()
{ }

bool QtFlickGesture::eventFilter(QObject *o, QEvent *e)
{
    if ((e->type() == QEvent::Move) && o && o == receiverWindow) {
        receiverWindowPos = receiverWindow->geometry().topLeft();
    }
    return QGesture::eventFilter(o, e);
}

//
// QFlickGestureRecognizer
//


QtFlickGestureRecognizer::QtFlickGestureRecognizer(Qt::MouseButton button)
{
    this->button = button;
}

/*! \reimp
 */
QGesture *QtFlickGestureRecognizer::create(QObject *target)
{
    QGraphicsObject *go = qobject_cast<QGraphicsObject*>(target);
    if (go && button == Qt::NoButton) {
        go->setAcceptTouchEvents(true);
    }
    return new QtFlickGesture(target, button);
}

/*! \internal
    The recognize function detects a touch event suitable to start the attached QScroller.
    The QFlickGesture will be triggered as soon as the scroller is no longer in the state
    QScroller::Inactive or QScroller::Pressed. It will be finished or canceled
    at the next QEvent::TouchEnd.
    Note that the QScroller might continue scrolling (kinetically) at this point.
 */
QGestureRecognizer::Result QtFlickGestureRecognizer::recognize(QGesture *state,
                                                              QObject *watched,
                                                              QEvent *event)
{
    Q_UNUSED(watched);

    static QElapsedTimer monotonicTimer;
    if (!monotonicTimer.isValid())
        monotonicTimer.start();

    QtFlickGesture *q = static_cast<QtFlickGesture *>(state);
    QtFlickGesture *d = q; // for source compatiblitly with the 4.8 QScroller

    QtScroller *scroller = d->receiverScroller;
    if (!scroller)
        return Ignore; // nothing to do without a scroller?

    QWidget *receiverWidget = qobject_cast<QWidget *>(d->receiver);
    QGraphicsObject *receiverGraphicsObject = qobject_cast<QGraphicsObject *>(d->receiver);

    // this is only set for events that we inject into the event loop via sendEvent()
    if (PressDelayHandler::instance()->shouldEventBeIgnored(event)) {
        //qFGDebug() << state << "QFG: ignored event: " << event->type();
        return Ignore;
    }

    const QMouseEvent *me = 0;
    const QGraphicsSceneMouseEvent *gsme = 0;
    const QTouchEvent *te = 0;
    QPoint globalPos;

    // qFGDebug() << "FlickGesture "<<state<<"watched:"<<watched<<"receiver"<<d->receiver<<"event"<<event->type()<<"button"<<button;

    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
        if (!receiverWidget)
            return Ignore;
        if (button != Qt::NoButton) {
            me = static_cast<const QMouseEvent *>(event);
            globalPos = me->globalPos();
        }
        break;
    case QEvent::GraphicsSceneMousePress:
    case QEvent::GraphicsSceneMouseRelease:
    case QEvent::GraphicsSceneMouseMove:
        if (!receiverGraphicsObject)
            return Ignore;
        if (button != Qt::NoButton) {
            gsme = static_cast<const QGraphicsSceneMouseEvent *>(event);
            globalPos = gsme->screenPos();
        }
        break;
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchUpdate:
        if (button == Qt::NoButton) {
            te = static_cast<const QTouchEvent *>(event);
            if (!te->touchPoints().isEmpty())
                globalPos = te->touchPoints().at(0).screenPos().toPoint();
        }
        break;

#if defined(Q_WS_MAC)
    // the only way to distinguish between real mouse wheels and wheel
    // events generated by the native 2 finger swipe gesture is to listen
    // for these events (according to Apple's Cocoa Event-Handling Guide)

    case QEvent::NativeGesture: {
#if 0 // QNativeGestureEvent is in qevent_p.h, which is of no use for this solution
        QNativeGestureEvent *nge = static_cast<QNativeGestureEvent *>(event);
        if (nge->gestureType == QNativeGestureEvent::GestureBegin)
            d->macIgnoreWheel = true;
        else if (nge->gestureType == QNativeGestureEvent::GestureEnd)
            d->macIgnoreWheel = false;
#else // This is a very evil hack, but it only has to work for Qt 4.6/4.7 anyway
        int type = *reinterpret_cast<int *>(event + 1);
        if (type == 1)
            d->macIgnoreWheel = true;
        else if (type == 2)
            d->macIgnoreWheel = false;
#endif
        break;
    }
#endif

    // consume all wheel events if the scroller is active
    case QEvent::Wheel:
        if (d->macIgnoreWheel || (scroller->state() != QtScroller::Inactive))
            return Ignore | ConsumeEventHint;
        break;

    // consume all dbl click events if the scroller is active
    case QEvent::MouseButtonDblClick:
        if (scroller->state() != QtScroller::Inactive)
            return Ignore | ConsumeEventHint;
        break;

    default:
        break;
    }

    if (!me && !gsme && !te) // Neither mouse nor touch
        return Ignore;
    // get the current pointer position in local coordinates.
    QPointF point;
    QtScroller::Input inputType = (QtScroller::Input) 0;

    switch (event->type()) {
    case QEvent::MouseButtonPress:
        if (me && me->button() == button && me->buttons() == button) {
            point = me->globalPos();
            inputType = QtScroller::InputPress;
        } else if (me) {
            scroller->stop();
            return CancelGesture;
        }
        break;
    case QEvent::MouseButtonRelease:
        if (me && me->button() == button) {
            point = me->globalPos();
            inputType = QtScroller::InputRelease;
        }
        break;
    case QEvent::MouseMove:
#ifdef Q_OS_SYMBIAN
        // Qt on Symbian tracks the button state internally, while Qt on Win/Mac/Unix
        // relies on the windowing system to report the current buttons state.
        if (me && (me->buttons() == button || !me->buttons())) {
#else
        if (me && me->buttons() == button) {
#endif
            point = me->globalPos();
            inputType = QtScroller::InputMove;
        }
        break;

    case QEvent::GraphicsSceneMousePress:
        if (gsme && gsme->button() == button && gsme->buttons() == button) {
            point = gsme->scenePos();
            inputType = QtScroller::InputPress;
        } else if (gsme) {
            scroller->stop();
            return CancelGesture;
        }
        break;
    case QEvent::GraphicsSceneMouseRelease:
        if (gsme && gsme->button() == button) {
            point = gsme->scenePos();
            inputType = QtScroller::InputRelease;
        }
        break;
    case QEvent::GraphicsSceneMouseMove:
#ifdef Q_OS_SYMBIAN
        // Qt on Symbian tracks the button state internally, while Qt on Win/Mac/Unix
        // relies on the windowing system to report the current buttons state.
        if (gsme && (gsme->buttons() == button || !gsme->buttons())) {
#else
        if (gsme && gsme->buttons() == button) {
#endif
            point = gsme->scenePos();
            inputType = QtScroller::InputMove;
        }
        break;

    case QEvent::TouchBegin:
        inputType = QtScroller::InputPress;
        // fall through
    case QEvent::TouchEnd:
        if (!inputType)
            inputType = QtScroller::InputRelease;
        // fallthrough
    case QEvent::TouchUpdate:
        if (!inputType)
            inputType = QtScroller::InputMove;

        if (te->deviceType() == QTouchEvent::TouchPad) {
            if (te->touchPoints().count() != 2)  // 2 fingers on pad
                return Ignore;

            point = te->touchPoints().at(0).startScenePos() +
                    ((te->touchPoints().at(0).scenePos() - te->touchPoints().at(0).startScenePos()) +
                     (te->touchPoints().at(1).scenePos() - te->touchPoints().at(1).startScenePos())) / 2;
        } else { // TouchScreen
            if (te->touchPoints().count() != 1) // 1 finger on screen
                return Ignore;

            point = te->touchPoints().at(0).scenePos();
        }
        break;

    default:
        break;
    }

    // Check for an active scroller at globalPos
    if (inputType == QtScroller::InputPress) {
        foreach (QtScroller *as, QtScroller::activeScrollers()) {
            if (as != scroller) {
                QRegion scrollerRegion;

                if (QWidget *w = qobject_cast<QWidget *>(as->target())) {
                    scrollerRegion = QRect(w->mapToGlobal(QPoint(0, 0)), w->size());
                } else if (QGraphicsObject *go = qobject_cast<QGraphicsObject *>(as->target())) {
                    if (go->scene() && !go->scene()->views().isEmpty()) {
                        foreach (QGraphicsView *gv, go->scene()->views())
                            scrollerRegion |= gv->mapFromScene(go->mapToScene(go->boundingRect()))
                                              .translated(gv->mapToGlobal(QPoint(0, 0)));
                    }
                }
                // active scrollers always have priority
                if (scrollerRegion.contains(globalPos)) {
                    qFGDebug() << "QFG: ignoring press event at" << globalPos << "since another scroller is active there";
                    return Ignore;
                }
            }
        }
    }

    bool scrollerWasDragging = (scroller->state() == QtScroller::Dragging);
    bool scrollerWasScrolling = (scroller->state() == QtScroller::Scrolling);

    if (inputType) {
        // QWidget::mapFromGlobal is very expensive on X11, so we cache the global position of the widget
        if (receiverWidget) {
            if (receiverWidget->window() != d->receiverWindow) {
                if (d->receiverWindow)
                    d->receiverWindow->removeEventFilter(q);
                d->receiverWindow = receiverWidget->window();
                d->receiverWindowPos = d->receiverWindow->geometry().topLeft();
                d->receiverWindow->installEventFilter(q);
            }
            point = receiverWidget->mapFrom(d->receiverWindow, point.toPoint() - d->receiverWindowPos);
        } else if (receiverGraphicsObject) {
            point = receiverGraphicsObject->mapFromScene(point);
        }
        // inform the scroller about the new event
        scroller->handleInput(inputType, point, monotonicTimer.elapsed());
    }

    // depending on the scroller state return the gesture state
    Result result(0);
    bool scrollerIsActive = (scroller->state() == QtScroller::Dragging ||
                             scroller->state() == QtScroller::Scrolling);

    // Consume all mouse events while dragging or scrolling to avoid nasty
    // side effects with Qt's standard widgets.
    if ((me || gsme) && scrollerIsActive)
        result |= ConsumeEventHint;

    // The only problem with this approach is that we consume the
    // MouseRelease when we start the scrolling with a flick gesture, so we
    // have to fake a MouseRelease "somewhere" to not mess with the internal
    // states of Qt's widgets (a QPushButton would stay in 'pressed' state
    // forever, if it doesn't receive a MouseRelease).
    if (me || gsme) {
        if (!scrollerWasDragging && !scrollerWasScrolling && scrollerIsActive)
            PressDelayHandler::instance()->scrollerBecameActive();
        else if (scrollerWasScrolling && (scroller->state() == QtScroller::Dragging || scroller->state() == QtScroller::Inactive))
            PressDelayHandler::instance()->scrollerWasIntercepted();
    }

    if (!inputType) {
        result |= Ignore;
    } else {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::GraphicsSceneMousePress:
            if (scroller->state() == QtScroller::Pressed) {
                int pressDelay = int(1000 * scroller->scrollerProperties().scrollMetric(QtScrollerProperties::MousePressEventDelay).toReal());
                if (pressDelay > 0) {
                    result |= ConsumeEventHint;

                    PressDelayHandler::instance()->pressed(event, pressDelay);
                    event->accept();
                }
            }
            // fall through
        case QEvent::TouchBegin:
            q->setHotSpot(globalPos);
            result |= scrollerIsActive ? TriggerGesture : MayBeGesture;
            break;


        case QEvent::MouseMove:
        case QEvent::GraphicsSceneMouseMove:
            if (PressDelayHandler::instance()->isDelaying())
                result |= ConsumeEventHint;
            // fall through
        case QEvent::TouchUpdate:
            result |= scrollerIsActive ? TriggerGesture : Ignore;
            break;

        case QEvent::GraphicsSceneMouseRelease:
        case QEvent::MouseButtonRelease:
            if (PressDelayHandler::instance()->released(event, scrollerWasDragging || scrollerWasScrolling, scrollerIsActive))
                result |= ConsumeEventHint;
            // fall through
        case QEvent::TouchEnd:
            result |= scrollerIsActive ? FinishGesture : CancelGesture;
            break;

        default:
            result |= Ignore;
            break;
        }
    }
    return result;
}


/*! \reimp
 */
void QtFlickGestureRecognizer::reset(QGesture *state)
{
    QGestureRecognizer::reset(state);
}

#endif // QT_NO_GESTURES
