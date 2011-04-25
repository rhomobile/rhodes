#pragma warning(disable:4996)
#include "MainWindowProxy.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "NativeToolbarQt.h"
#include "rho/rubyext/NativeToolbarExt.h"
#undef null
#include <QString>
#include <QApplication>
#include <QtGui/QAction>
#include "QtMainWindow.h"

IMPLEMENT_LOGCLASS(CMainWindowProxy,"MainWindowProxy");

extern "C" int rho_wmsys_has_touchscreen();

using namespace rho;
using namespace rho::common;

CMainWindowProxy::CMainWindowProxy(void):
    qtApplication(NULL),
    qtMainWindow(NULL)
{
}

CMainWindowProxy::~CMainWindowProxy(void)
{
    if (qtMainWindow) delete (QtMainWindow*)qtMainWindow;
    if (qtApplication) delete (QApplication*)qtApplication;
}

void CMainWindowProxy::navigate(const wchar_t* url)
{
    LOG(INFO) + "navigate: '"+url+"'";
    ((QtMainWindow*)qtMainWindow)->navigate(QUrl(QString::fromWCharArray(url)));
}

void CMainWindowProxy::setCallback(IMainWindowCallback* callback)
{
    ((QtMainWindow*)qtMainWindow)->setCallback(callback);
}

void* CMainWindowProxy::init(IMainWindowCallback* callback, const wchar_t* title)
{
    int argc = 0;
    qtApplication = (void*)new QApplication(argc, 0);
    qtMainWindow = (void*)new QtMainWindow();
    ((QtMainWindow*)qtMainWindow)->setWindowTitle(QString::fromWCharArray(title));
    ((QtMainWindow*)qtMainWindow)->setCallback(callback);
    ((QtMainWindow*)qtMainWindow)->show();

    return (void*)((QtMainWindow*)qtMainWindow)->winId();
}

void CMainWindowProxy::messageLoop(void)
{
    qApp->exec();
}

void CMainWindowProxy::GoBack(void)
{
    LOG(INFO) + "back";
    ((QtMainWindow*)qtMainWindow)->GoBack();
}

void CMainWindowProxy::GoForward(void)
{
    LOG(INFO) + "forward";
    ((QtMainWindow*)qtMainWindow)->GoForward();
}

void CMainWindowProxy::Refresh(void)
{
    LOG(INFO) + "refresh";
    ((QtMainWindow*)qtMainWindow)->Refresh();
}

bool CMainWindowProxy::isStarted()
{
    return true;
}

int CMainWindowProxy::getHeight()
{
    return ((QtMainWindow*)qtMainWindow)->toolbarGetHeight();
}


void CMainWindowProxy::removeToolbar()
{
    ((QtMainWindow*)qtMainWindow)->toolbarHide();
}

void CMainWindowProxy::removeAllButtons()
{
    ((QtMainWindow*)qtMainWindow)->toolbarRemoveAllButtons();
}

static QColor getColorFromString(const char* szColor)
{
    if ( !szColor || !*szColor )
        return QColor(0, 0, 0);

    int c = atoi(szColor);

    int cR = (c & 0xFF0000) >> 16;
    int cG = (c & 0xFF00) >> 8;
    int cB = (c & 0xFF);

    return QColor(cR, cG, cB);
}

void CMainWindowProxy::createToolbar(rho_param *p)
{
    if (!rho_rhodesapp_check_mode() || !rho_wmsys_has_touchscreen() )
        return;

    int bar_type = TOOLBAR_TYPE;
	std::auto_ptr<QColor> m_rgbBackColor (NULL);
    std::auto_ptr<QColor> m_rgbMaskColor (NULL);
    int m_nHeight = CNativeToolbar::MIN_TOOLBAR_HEIGHT;

    rho_param *params = NULL;
    switch (p->type) 
    {
        case RHO_PARAM_ARRAY:
            params = p;
            break;
        case RHO_PARAM_HASH: 
            {
                for (int i = 0, lim = p->v.hash->size; i < lim; ++i) 
                {
                    const char *name = p->v.hash->name[i];
                    rho_param *value = p->v.hash->value[i];
                    
                    if (strcasecmp(name, "background_color") == 0) 
                        m_rgbBackColor.reset(new QColor(getColorFromString(value->v.string)));
                    else if (strcasecmp(name, "mask_color") == 0) 
                        m_rgbMaskColor.reset(new QColor(getColorFromString(value->v.string)));
                    else if (strcasecmp(name, "view_height") == 0) 
                        m_nHeight = atoi(value->v.string);
                    else if (strcasecmp(name, "buttons") == 0 || strcasecmp(name, "tabs") == 0) 
                        params = value;
                }
            }
            break;
        default: {
            LOG(ERROR) + "Unexpected parameter type for create_nativebar, should be Array or Hash";
            return;
        }
    }
    
    if (!params) {
        LOG(ERROR) + "Wrong parameters for create_nativebar";
        return;
    }

    int size = params->v.array->size;
    if ( size == 0 )
    {
        removeToolbar();
        return;
    }

    removeAllButtons();

    int nSeparators = 0;
    bool wasSeparator = false;
    for (int ipass=0; ipass < 2; ++ipass) {
        for (int i = 0; i < size; ++i) 
        {
            rho_param *hash = params->v.array->value[i];
            if (hash->type != RHO_PARAM_HASH) {
                LOG(ERROR) + "Unexpected type of array item for create_nativebar, should be Hash";
                return;
            }
            
            const char *label = NULL;
            const char *action = NULL;
            const char *icon = NULL;
            const char *colored_icon = NULL;
            int  nItemWidth = 0;

            for (int j = 0, lim = hash->v.hash->size; j < lim; ++j) 
            {
                const char *name = hash->v.hash->name[j];
                rho_param *value = hash->v.hash->value[j];
                if (value->type != RHO_PARAM_STRING) {
                    LOG(ERROR) + "Unexpected '" + name + "' type, should be String";
                    return;
                }
                
                if (strcasecmp(name, "label") == 0)
                    label = value->v.string;
                else if (strcasecmp(name, "action") == 0)
                    action = value->v.string;
                else if (strcasecmp(name, "icon") == 0)
                    icon = value->v.string;
                else if (strcasecmp(name, "colored_icon") == 0)
                    colored_icon = value->v.string;
                else if (strcasecmp(name, "width") == 0)
                    nItemWidth = atoi(value->v.string);
            }
            
            if (label == NULL && bar_type == TOOLBAR_TYPE)
                label = "";
            
            if ( label == NULL || action == NULL) {
                LOG(ERROR) + "Illegal argument for create_nativebar";
                return;
            }
            if ( strcasecmp(action, "forward") == 0 && rho_conf_getBool("jqtouch_mode") )
                continue;

            if (!action) action = "";

            if (ipass==0) {
                if (strcasecmp(action, "separator")==0)
                    ++nSeparators;
            } else {
                LOG(INFO) + "addToolbarButton: Label: '"+label+"';Action: '"+action+"'";
                if (strcasecmp(action, "separator")==0) {
                    if (nSeparators!=1)
                        ((QtMainWindow*)qtMainWindow)->toolbarAddSeparator();
                    else
                        wasSeparator = true;
                } else {
                    String strImagePath;
                    if ( icon && *icon )
                        strImagePath = rho::common::CFilePath::join( RHODESAPP().getAppRootPath(), icon );
                    else {
                        if ( strcasecmp(action, "options")==0 )
                            strImagePath = "lib/res/options_btn.png";
                        else if ( strcasecmp(action, "home")==0 )
                            strImagePath = "lib/res/home_btn.png";
                        else if ( strcasecmp(action, "refresh")==0 )
                            strImagePath = "lib/res/refresh_btn.png";
                        else if ( strcasecmp(action, "back")==0 )
                            strImagePath = "lib/res/back_btn.png";
                        else if ( strcasecmp(action, "forward")==0 )
                            strImagePath = "lib/res/forward_btn.png";
                        strImagePath = strImagePath.length() > 0 ? CFilePath::join( RHODESAPP().getRhoRootPath(), strImagePath) : String();
                    }

                    ((QtMainWindow*)qtMainWindow)->toolbarAddAction(QIcon(QString(strImagePath.c_str())), QString(label), action, wasSeparator);
                }
            }
        }
    }
	((QtMainWindow*)qtMainWindow)->setToolbarStyle(false, (m_rgbBackColor.get()!=NULL ? m_rgbBackColor->name() : ""));
    ((QtMainWindow*)qtMainWindow)->toolbarShow();
}

void CMainWindowProxy::menuClear()
{
    ((QtMainWindow*)qtMainWindow)->menuClear();
}

void CMainWindowProxy::menuAddSeparator()
{
    ((QtMainWindow*)qtMainWindow)->menuAddSeparator();
}

void CMainWindowProxy::menuAddAction(const char* label, int item)
{
    ((QtMainWindow*)qtMainWindow)->menuAddAction(QString(label), item);
}
