/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "QtNativeTabBar.h"
#include <QStylePainter>
#include <QStyleOptionTabV3>
#include "QtMainWindow.h"

// default badge style (badge is implemented as a QLabel)
static const char* defaultStyle = "QTabBar::tab QLabel {color:white; border: 1px solid white; background:red}";
// QTabBar::scroller - scroller panel; QTabBar QToolButton - scroller buttons

QtNativeTabBar::QtNativeTabBar(QWidget* parent):
    QTabBar(parent)
{
    this->setIconSize(QSize(32, 32));
    this->setElideMode(Qt::ElideNone);
    this->setExpanding(true);
    this->setUsesScrollButtons(true);
}

void QtNativeTabBar::clearStyleSheet(void)
{
    this->setStyleSheet(defaultStyle);
}

QSize QtNativeTabBar::tabSizeHint(int) const
{
    int width = (int)(this->width()/this->count());
    return QSize((width > 96 ? width : 96), 56);
}

void QtNativeTabBar::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);
    bool isNotCustomized = true;
    for (int pass=0; pass<3; ++pass) {
        for (int i = 0; i < this->count(); ++i) {
            bool isCurrent = this->currentIndex()==i;
            if (((pass==1) && (this->currentIndex()!=i)) || isCurrent) {
                QStyleOptionTabV3 tab;
                initStyleOption(&tab, i);
                bool enabled = (tab.state & QStyle::State_Enabled);

                QtMainWindow::QTabBarRuntimeParams tbrp = this->tabData(i).toHash();

                if (pass==0) {
                    // manipulating the style
                    QString background_color = tbrp["background_color"].toString();
                    QString selected_color = tbrp["selected_color"].toString();
                    QString style = defaultStyle;
                    if (background_color.length()>0)
                        style += " QTabBar::tab {background:"+background_color+"}";
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
                    iconRect.translate(0, 4);
                    // text occupies all the rest
                    QRect labelRect = QRect(tab.rect);
                    labelRect.setTop(labelRect.top()+iSize.height());
                    labelRect.setHeight(labelRect.height()-6);
                    labelRect.translate(0, 6);
                    // drawing
                    painter.drawItemPixmap(iconRect, Qt::AlignCenter,
                        tab.icon.pixmap(iSize, (enabled ? QIcon::Normal : QIcon::Disabled), (isCurrent ? QIcon::On : QIcon::Off ))
                    );
                    painter.drawItemText(labelRect, Qt::AlignHCenter | Qt::AlignTop, tab.palette, enabled, tab.text);
                }
            }
        }
    }
}
