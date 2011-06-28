#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/RhoAppAdapter.h"
#include "MainWindowImpl.h"
#include "AlertDialog.h"

using namespace rho;

extern "C" {

void alert_show_status(const char* szTitle, const char* szMessage, const char* szHide)
{
    rho::String message = szMessage ? szMessage : "";
    rho::String title = szTitle ? szTitle : "";
    rho::String callback = "";
    rho::String icon = "";
    Vector<AlertDialog::Params::CAlertButton> buttons;
    CMainWindow::getInstance()->alertShowPopup(new AlertDialog::Params(title, message, callback, icon, buttons, AlertDialog::Params::DLG_STATUS ));
}

void alert_show_popup(rho_param *p)
{
    if (p->type == RHO_PARAM_STRING) {
        rho::String msg = rho::String(p->v.string);
        CMainWindow::getInstance()->alertShowPopup(new AlertDialog::Params(msg));
    } else if (p->type == RHO_PARAM_HASH) {
        String title, message, callback, icon;
        String btnId, btnTitle;
        Vector<AlertDialog::Params::CAlertButton> buttons;

        for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
            char *name = p->v.hash->name[i];
            rho_param *value = p->v.hash->value[i];
            
            if (strcasecmp(name, "title") == 0) {
                if (value->type != RHO_PARAM_STRING) {
                    RAWLOG_ERROR("'title' should be string");
                    continue;
                }
                title = value->v.string;
            }
            else if (strcasecmp(name, "message") == 0) {
                if (value->type != RHO_PARAM_STRING) {
                    RAWLOG_ERROR("'message' should be string");
                    continue;
                }
                message = value->v.string;
            }
            else if (strcasecmp(name, "callback") == 0) {
                if (value->type != RHO_PARAM_STRING) {
                    RAWLOG_ERROR("'callback' should be string");
                    continue;
                }
                callback = value->v.string;
            } else if (strcasecmp(name, "icon") == 0) {
                if (value->type != RHO_PARAM_STRING) {
                    RAWLOG_ERROR("'title' should be string");
                    continue;
                }
                icon = value->v.string;
            }

            else if (strcasecmp(name, "buttons") == 0) {
                if (value->type != RHO_PARAM_ARRAY) {
                    RAWLOG_ERROR("'buttons' should be array");
                    continue;
                }
                for (int j = 0, limj = value->v.array->size; j < limj; ++j) {
                    rho_param *arrValue = value->v.array->value[j];
                    switch (arrValue->type) {
                        case RHO_PARAM_STRING:
                            btnId    = arrValue->v.string;
                            btnTitle = arrValue->v.string;
                            break;
                        case RHO_PARAM_HASH:
                            for (int k = 0, limk = arrValue->v.hash->size; k < limk; ++k) {
                                char *sName = arrValue->v.hash->name[k];
                                rho_param *sValue = arrValue->v.hash->value[k];
                                if (sValue->type != RHO_PARAM_STRING) {
                                    RAWLOG_ERROR("Illegal type of button item's value");
                                    continue;
                                }
                                if (strcasecmp(sName, "id") == 0)
                                    btnId = sValue->v.string;
                                else if (strcasecmp(sName, "title") == 0)
                                    btnTitle = sValue->v.string;
                            } 
                            break;
                        default:
                            RAWLOG_ERROR("Illegal type of button item");
                            continue;
                    }
                    if (btnId == "" || btnTitle == "") {
                        RAWLOG_ERROR("Incomplete button item");
                        continue;
                    }

                    buttons.addElement( AlertDialog::Params::CAlertButton(btnTitle, btnId) );
                }
            }//buttons
        }
        CMainWindow::getInstance()->alertShowPopup(new AlertDialog::Params(title, message, icon, callback, buttons, AlertDialog::Params::DLG_CUSTOM));
    }
}

void alert_vibrate(int duration_ms) {
    //TODO: alert_vibrate
}

void alert_play_file(char* file_name, char *media_type)
{
    //TODO: alert_play_file

    //String path = RHODESAPP().getRhoRootPath() + "apps" + fileName;

    //HSOUND hSound;

    //String::size_type pos = 0;
    //while ( (pos = path.find('/', pos)) != String::npos ) {
    //    path.replace( pos, 1, "\\");
    //    pos++;
    //}

    //StringW strPathW = convertToStringW(path);
    //HRESULT hr = SndOpen( strPathW.c_str(), &hSound);
    //hr = SndPlayAsync (hSound, 0);

    //if (hr != S_OK) {
    //    LOG(WARNING) + "OnAlertPlayFile: failed to play file";
    //}

    //WaitForSingleObject(hSound, INFINITE);

    //hr = SndClose(hSound);
    //SndStop(SND_SCOPE_PROCESS, NULL);

}

void alert_hide_popup()
{
    CMainWindow::getInstance()->alertHidePopup();
}

} //extern "C"
