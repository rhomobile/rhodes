#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/RhoAppAdapter.h"

extern "C" {

void alert_show_status(const char* szTitle, const char* szMessage, const char* szHide)
{
    rho::String message = szMessage ? szMessage : "";
    rho::String title = szTitle ? szTitle : "";
    //TODO: alert_show_status
    //Vector<CAlertDialog::Params::CAlertButton> buttons;
    //CAlert::showPopup(new CAlertDialog::Params(title, message, String(), String(), buttons, CAlertDialog::Params::DLG_STATUS ));
}

void alert_show_popup(rho_param *p)
{
    if (p->type == RHO_PARAM_STRING) {
        //TODO: CAlert::showPopup(new CAlertDialog::Params(String(p->v.string)));
    } else if (p->type == RHO_PARAM_HASH) {
        rho::String title, message, callback, icon;
        rho::String btnId, btnTitle;
        //Hashtable<String, String> buttons;
        //TODO: Vector<CAlertDialog::Params::CAlertButton> buttons;

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

                    //TODO: buttons.addElement( CAlertDialog::Params::CAlertButton(btnTitle, btnId) );
                }
            }//buttons
        }
        
        //TODO: CAlert::showPopup(new CAlertDialog::Params(title, message, icon, callback, buttons, CAlertDialog::Params::DLG_CUSTOM));
    }
}

void alert_vibrate(void*) {
    //TODO: alert_vibrate
}

void alert_play_file(char* file_name, ...)
{
    //TODO: alert_play_file
}

void alert_hide_popup()
{
    //TODO: ::PostMessage(main_wnd, WM_ALERT_HIDE_POPUP, 0, 0);
}

} //extern "C"
