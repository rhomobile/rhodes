#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#undef null
#include <QDialog>

namespace Ui {
    class AlertDialog;
}

class AlertDialog : public QDialog
{
    Q_OBJECT
    DEFINE_LOGCLASS

public:
     // Params is a container to pass parameters to UI thread.
    class Params {
    public:
        enum CAlertType {DLG_DEFAULT, DLG_CUSTOM, DLG_STATUS};
        struct CAlertButton
        {
            rho::String m_strCaption;
            rho::String m_strID;
            CAlertButton ( const rho::String& strCaption, const rho::String& strID ): m_strCaption(strCaption), m_strID(strID){}
        };

        Params (rho::String &message)
        {
             m_dlgType  = DLG_DEFAULT;
             m_message  = message;
        }

        Params (rho::String &title, rho::String &message, rho::String &icon, rho::String &callback, rho::Vector<CAlertButton>& buttons, CAlertType dlgType)
        {
             m_dlgType  = dlgType;
             m_title    = title;
             m_message  = message;
             m_icon     = icon;
             m_callback = callback;
             m_buttons  = buttons;
        }

        int    m_dlgType;
        rho::String m_title;
        rho::String m_message;
        rho::String m_icon;
        rho::String m_callback;
        rho::Vector<CAlertButton> m_buttons;
    };

public:
    explicit AlertDialog(Params* params, QWidget *parent = 0);
    ~AlertDialog();

private:
    Ui::AlertDialog *ui;
    rho::String m_title;
    rho::String m_message;
    rho::String m_callback;
    rho::String m_icon;
    void init(void);
};

#endif // ALERTDIALOG_H
