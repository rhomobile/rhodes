#ifndef CUSTOMWEBVIEWTAB_H
#define CUSTOMWEBVIEWTAB_H

#include <QObject>
#include <QDebug>

#define SENDER_TAB ((CustomWebViewTab *) sender())
class CustomWebViewTab : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString url READ getUrl WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool activeTab READ isTabActive WRITE setActiveTab NOTIFY activeTabChanged)
    Q_PROPERTY(int tabBarIndex READ getTabBarIndex WRITE setTabBarIndex NOTIFY tabBarIndexChanged)
    Q_PROPERTY(int scale READ getScale WRITE setScale NOTIFY scaleChanged)
    public:
        CustomWebViewTab(QString name, QObject *parent) : QObject(parent){
            text = name;
            scale = 3;
        }
        explicit CustomWebViewTab(QString name, QString address, QObject *parent) : QObject(parent){
            text = name;
            url = address;
            scale = 3;
        }




public slots:

        void setText(const QString &value){
            text = value;
            emit textChanged();
        }

        QString getText(){
            return text;
        }
        QString getUrl(){
            return url;
        }

        void setUrl(const QString& url){
            this->url = url;
            emit urlHasBeenChanged(url);
            emit urlChanged();
        }


        void loadUrl(QString url){
            this->url = url;
            emit urlChanged();
        }

        bool isTabActive(){
            return activeTab;
        }

        void setActiveTab(bool value){
            activeTab = value;
            emit activeTabChanged();
        }


        int getTabBarIndex() {
            return tabBarIndex;
        }


        void setTabBarIndex(int value){
            tabBarIndex = value;
            emit tabBarIndexChanged();
        }


        int getScale() {
            return scale;
        }
        void setScale(int value){
            scale = value;
            emit scaleChanged();
        }

        void messageReceived(QString message){
            qDebug() << "Message from JS code: " + message;
        }


private:
        QString text;
        QString url;
        int tabBarIndex;
        bool activeTab;
        int scale;

signals:
        void urlChanged();
        void textChanged();
        void tabBarIndexChanged();
        void activeTabChanged(); //from webView
        void scaleChanged();


        void linkClicked(const QString); //from webView v
        void loadStarted();//from webView    v
        void loadFinished(bool);//from webView    v
        void urlHasBeenChanged(QString);//from webView   v



        void setHtml(QString pHtml);//to webView
        void goBack();//to webView
        void goForward(); //to webView
        void refresh();//to webView
        void evaluateJavaScript(QString pScript); //to webView
        void setCurrentTab(); //to webVew (making this tab - current)

        void isCurrentTabOnTop(); //from webView
        void switchToThisTab(); //to webView

        void openQMLDocument(QString documentName);

};

#endif // CUSTOMWEBVIEWTAB_H












