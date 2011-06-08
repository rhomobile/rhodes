#include <QTabBar>
#include <QStylePainter>
#include <QStyleOptionTabV3>
#include "QtMainWindow.h"

class QtNativeTabBar : public QTabBar
{
public:
    explicit QtNativeTabBar(QWidget* parent=0) : QTabBar(parent)
    {
        this->setIconSize(QSize(58, 58));
        this->setExpanding(true);
    }
protected:
    QSize tabSizeHint(int) const
    {
        return QSize(80, 80);
    }

    void paintEvent(QPaintEvent *)
    {
        QStylePainter painter(this);
        bool isNotCustomized = true;
        for (int pass=0; pass<3; ++pass) {
            for (int i = 0; i < this->count(); ++i) {
                if (((pass==1) && (this->currentIndex()!=i)) || (this->currentIndex()==i)) {
                    QStyleOptionTabV3 tab;
                    initStyleOption(&tab, i);
                    bool enabled = (tab.state & QStyle::State_Enabled);

                    QtMainWindow::QTabBarRuntimeParams tbrp = this->tabData(i).toHash();

                    if (pass==0) {
                        // manipulating the style
                        QString background_color = tbrp["background_color"].toString();
                        QString selected_color = tbrp["selected_color"].toString();
                        QString style = "";
                        if (background_color.length()>0)
                            style += "QTabBar::tab {background:"+background_color+"}";
                        if (selected_color.length()>0)
                            style += " QTabBar::tab:selected, QTabBar::tab:hover {background:"+selected_color+"}";
                        if (style.length()>0) {
                            this->setStyleSheet(style);
                            isNotCustomized = false;
                        }
                    } else {
                        // manipulating the palette
                        if (!enabled)
                            tab.palette.setCurrentColorGroup(QPalette::Disabled);

                        // drawing tab background
                        if (enabled || isNotCustomized)
                            painter.drawControl(QStyle::CE_TabBarTabShape, tab);

                        // drawing all the rest
                        QSize iSize = this->iconSize();
                        // icon is aligned to top
                        QRect iconRect = QRect(tab.rect);
                        iconRect.setHeight(iSize.height());
                        // text occupies all the rest
                        QRect labelRect = QRect(tab.rect);
                        labelRect.setTop(labelRect.top()+iSize.height());
                        // drawing
                        painter.drawItemPixmap(iconRect, Qt::AlignCenter, tab.icon.pixmap(iSize));
                        painter.drawItemText(labelRect, Qt::AlignHCenter, tab.palette, enabled, tab.text);
                    }
                }
            }
        }
    }
};