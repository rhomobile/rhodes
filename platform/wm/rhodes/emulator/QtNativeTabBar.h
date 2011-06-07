#include <QTabBar>
#include <QStylePainter>
#include <QStyleOptionTabV3>
#include "QtMainWindow.h"

class QtNativeTabBar : public QTabBar
{
public:
    explicit QtNativeTabBar(QWidget* parent=0) : QTabBar(parent)
    {
        setIconSize(QSize(58, 58));
        setExpanding(true);
    }
    //virtual ~QtNativeTabBar () {}
protected:
    QSize tabSizeHint(int) const
    {
        return QSize(80, 80);
    }

    void paintEvent(QPaintEvent *)
    {
        QStylePainter painter(this);
        for (int i = 0; i < this->count(); ++i) {
            //QtMainWindow::QTabBarRuntimeParams tbrp = this->tabData(i).toHash();
            QStyleOptionTabV3 tab;
            initStyleOption(&tab, i);
            // drawing tab background
            painter.drawControl(QStyle::CE_TabBarTabShape, tab);
            QSize iSize = this->iconSize();
            // icon is aligned to top
            QRect iconRect = QRect(tab.rect);
            iconRect.setHeight(iSize.height());
            // text occupies all the rest
            QRect labelRect = QRect(tab.rect);
            labelRect.setTop(labelRect.top()+iSize.height());
            // drawing
            painter.drawItemPixmap(iconRect, Qt::AlignCenter, tab.icon.pixmap(iSize));
            painter.drawItemText(labelRect, Qt::AlignHCenter, palette(), 1, tab.text);
        }
    }
};