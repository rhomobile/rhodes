#include <QTabBar>

class QtNativeTabBar : public QTabBar
{
public:
    explicit QtNativeTabBar(QWidget* parent=0) : QTabBar(parent)
    {
        setIconSize(QSize(80, 80));
        setExpanding(true);
    }
protected:
    QSize tabSizeHint(int) const
    {
        return QSize(80, 80);
    }
};