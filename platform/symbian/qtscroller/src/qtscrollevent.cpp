#include <qtscrollevent.h>
#include <qtscrollevent_p.h>

/*!
    \class QtScrollPrepareEvent
    \since 4.8
    \ingroup events

    \brief The QtScrollPrepareEvent class is send in preparation of a scrolling.

    The scroll prepare event is send before scrolling (usually by QtScroller) is started.
    The object receiving this event should set viewportSize, maxContentPos and contentPos.
    It also should accept this event to indicate that scrolling should be started.

    It is not guaranteed that a QtScrollEvent will be send after an acceepted
    QtScrollPrepareEvent, e.g. in a case where the maximum content position is (0,0).

    \sa QScrollEvent, QScroller
*/

/*!
    Creates new QScrollPrepareEvent
    The \a startPos is the position of a touch or mouse event that started the scrolling.
*/
QtScrollPrepareEvent::QtScrollPrepareEvent(const QPointF &startPos)
    : QEvent(static_cast<QEvent::Type>(ScrollPrepare)),
      d(new QtScrollPrepareEventPrivate())
{
    d->startPos = startPos;
}

/*!
    Destroys QtScrollPrepareEvent.
*/
QtScrollPrepareEvent::~QtScrollPrepareEvent()
{
}

/*!
    Returns the position of the touch or mouse event that started the scrolling.
*/
QPointF QtScrollPrepareEvent::startPos() const
{
    return d->startPos;
}

/*!
    Returns size of the area that is to be scrolled as set by setViewportSize

    \sa setViewportSize()
*/
QSizeF QtScrollPrepareEvent::viewportSize() const
{
    return d->viewportSize;
}

/*!
    Returns maximum coordinates for the content as set by setMaximumContentRect.

    \sa setMaximumContentRect()
*/
QRectF QtScrollPrepareEvent::contentPosRange() const
{
    return d->contentPosRange;
}

/*!
    Returns the current position of the content as set by setContentPos.
*/
QPointF QtScrollPrepareEvent::contentPos() const
{
    return d->contentPos;
}


/*!
    Sets the size of the area that is to be scrolled to \a size.

    \sa viewportSize()
*/
void QtScrollPrepareEvent::setViewportSize(const QSizeF &size)
{
    d->viewportSize = size;
}

/*!
    Sets the maximum content coordinates to \a rect.

    \sa contentPosRange
*/
void QtScrollPrepareEvent::setContentPosRange(const QRectF &rect)
{
    d->contentPosRange = rect;
}

/*!
    Sets the current content position to \a pos.

    \sa contentPos()
*/
void QtScrollPrepareEvent::setContentPos(const QPointF &pos)
{
    d->contentPos = pos;
}


/*!
    \class QtScrollEvent
    \since 4.8
    \ingroup events

    \brief The QtScrollEvent class is send when scrolling.

    The scroll event is send to indicate that the receiver should be scrolled.
    Usually the receiver should be something visual like QWidget or QGraphicsObject.

    Some care should be taken that no conflicting QtScrollEvents are sent from two
    sources. Using QtScroller::scrollTo is safe however.

    \sa QtScrollPrepareEvent, QtScroller
*/

/*!
    \enum QtScrollEvent::ScrollStateFlag

    This enum describes the states a scroll event can have.

    \value ScrollStarted Set for the first scroll event of a scroll activity.

    \value ScrollUpdated Set for all but the first and the last scroll event of a scroll activity.

    \value ScrollFinished Set for the last scroll event of a scroll activity.

    \sa QtScrollEvent::scrollState
*/

/*!
    Creates a new QtScrollEvent
    \a contentPos is the new content position, \a overshootDistance is the
    new overshoot distance while \a scrollState indicates if this scroll
    event is the first one, the last one or some event in between.
*/
QtScrollEvent::QtScrollEvent(const QPointF &contentPos, const QPointF &overshootDistance, ScrollState scrollState)
    : QEvent(static_cast<QEvent::Type>(Scroll)),
      d(new QtScrollEventPrivate())
{
    d->contentPos = contentPos;
    d->overshoot= overshootDistance;
    d->state = scrollState;
}

/*!
    Destroys QtScrollEvent.
*/
QtScrollEvent::~QtScrollEvent()
{
}

/*!
    Returns the new scroll position.
*/
QPointF QtScrollEvent::contentPos() const
{
    return d->contentPos;
}

/*!
    Returns the new overshoot distance.
    See QtScroller for an explanation of the term overshoot.

    \sa QtScroller
*/
QPointF QtScrollEvent::overshootDistance() const
{
    return d->overshoot;
}

/*!
    Returns the current scroll state as a combination of ScrollStateFlag values.
    ScrollStarted (or ScrollFinished) will be set, if this scroll event is the first (or last) event in a scrolling activity.
    Please note that both values can be set at the same time, if the activity consists of a single QScrollEvent.
    All other scroll events in between will have their state set to ScrollUpdated.

    A widget could for example revert selections when scrolling is started and stopped.
    \sa isLast()
*/
QtScrollEvent::ScrollState QtScrollEvent::scrollState() const
{
    return d->state;
}

