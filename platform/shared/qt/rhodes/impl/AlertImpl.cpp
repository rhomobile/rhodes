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

#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/RhoAppAdapter.h"
#include "MainWindowImpl.h"
#include "../DateTimeDialog.h"
#undef null
#include <QSound>

using namespace rho;

extern "C" {

void alert_show_status(const char* szTitle, const char* szMessage, const char* szHide)
{
    rho::String message = szMessage ? szMessage : "";
    rho::String title = szTitle ? szTitle : "";
    rho::String callback = "";
    rho::String icon = "";
    Vector<CAlertParams::CAlertButton> buttons;
    CMainWindow::getInstance()->alertShowPopup(new CAlertParams(title, message, callback, icon, buttons, CAlertParams::DLG_STATUS ));
}

void alert_show_popup(rho_param *p)
{
    if (p->type == RHO_PARAM_STRING) {
        rho::String msg = rho::String(p->v.string);
        CMainWindow::getInstance()->alertShowPopup(new CAlertParams(msg));
    } else if (p->type == RHO_PARAM_HASH) {
        String title, message, callback, icon;
        String btnId, btnTitle;
        Vector<CAlertParams::CAlertButton> buttons;

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

                    buttons.addElement( CAlertParams::CAlertButton(btnTitle, btnId) );
                }
            }//buttons
        }
        CMainWindow::getInstance()->alertShowPopup(new CAlertParams(title, message, icon, callback, buttons, CAlertParams::DLG_CUSTOM));
    }
}

void alert_vibrate(int duration_ms) {
    //TODO: alert_vibrate
}

void alert_play_file(char* file_name, char *media_type)
{
    String path = RHODESAPP().getRhoRootPath() + file_name;
    if (QSound::isAvailable()) {
        QSound::play(QString(path.c_str()));
    } else {
        RAWLOGC_INFO("AlertImpl", "OnAlertPlayFile: failed to play file");
    }
}

void alert_hide_popup()
{
    CMainWindow::getInstance()->alertHidePopup();
}

} //extern "C"
