#ifndef QSCROLLEVENT_P_H
#define QSCROLLEVENT_P_H

#include "qtscrollevent.h"

class QtScrollPrepareEventPrivate
{
public:
    inline QtScrollPrepareEventPrivate()
        : target(0)
    {
    }

    QObject* target;
    QPointF startPos;
    QSizeF viewportSize;
    QRectF contentPosRange;
    QPointF contentPos;
};

class QtScrollEventPrivate
{
public:
    inline QtScrollEventPrivate()
    {
    }

    QPointF contentPos;
    QPointF overshoot;
    QtScrollEvent::ScrollState state;
};

#endif // QSCROLLEVENT_P_H
