#include <QTabBar>

class QtNativeTabBar : public QTabBar
{
public:
    explicit QtNativeTabBar(QWidget* parent=0);
    void clearStyleSheet(void);
protected:
    QSize tabSizeHint(int) const;
    void paintEvent(QPaintEvent *);
};