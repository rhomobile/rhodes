#ifndef OS_SAILFISH
#ifndef QTCUSTOMSTYLE_H
#define QTCUSTOMSTYLE_H

#include <QProxyStyle>
#include <QStyleOption>

class QtCustomStyle : public QProxyStyle
{
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = 0) const
    {
        if (element == QStyle::CE_CheckBox || 
            element == QStyle::CE_RadioButton) {
            option->styleObject->setProperty("_q_no_animation", true);                          
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }
};

#endif //QTCUSTOMSTYLE_H
#endif
