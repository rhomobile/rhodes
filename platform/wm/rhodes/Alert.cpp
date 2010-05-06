#include "stdafx.h"

#if defined(_WIN32_WCE)
#include <soundfile.h>
#endif

#include <common/RhodesApp.h>
#include <common/rhoparams.h>

#include "Alert.h"
#include "MainWindow.h"
#include "Vibrate.h"

extern "C" HWND getMainWnd();

/**
 ********************************************************************************
 * CAlertDialog members.
 ********************************************************************************
 */

//TODO: 
//      - smart alignment and win32
//      - review for memory leaks.

typedef CWinTraits <WS_CAPTION  | WS_VISIBLE | WS_POPUP | DS_CENTER> CAlertDialogTraits;

CAlertDialog::CAlertDialog(Params *params)
{
	m_title    = params->m_title;
	m_message  = params->m_message;
	m_callback = params->m_callback;
	m_icon     = params->m_icon;

	int id = ID_ALERT_DLG_BUTTON_FIRST;
	for (Hashtable<String, String>::iterator itr = params->m_buttons.begin(); itr != params->m_buttons.end(); ++itr) {
		if(id > ID_ALERT_DLG_BUTTON_LAST) {
			LOG(ERROR) + "too many buttons";
			break;
		}
		m_buttons.addElement(CustomButton(itr->first, itr->second, id++));
	}
}

CAlertDialog::~CAlertDialog()
{
}

void CAlertDialog::DoInitTemplate() 
{
#ifdef OS_WINCE
	int initialWidth  = GetSystemMetrics(SM_CXSCREEN)/3;
#else 
	int initialWidth  = CMainWindow::getScreenWidth()/3;
#endif
	int initialHeight = initialWidth/3;

	m_Template.Create(false, convertToStringW(m_title).c_str(), 
						0, 
						0,
						initialWidth, 
						initialHeight,
						CAlertDialogTraits::GetWndStyle(0), 
						CAlertDialogTraits::GetWndExStyle(0));
}

void CAlertDialog::DoInitControls() 
{
}

LRESULT CAlertDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&bHandled)
{
#define MAX(i, j)   ((i) > (j) ? (i) : (j))

	const int GAP = 10;   //space around dialog
	const int INDENT = 8; //for dialog items
	const unsigned int iconHeight = 42;
	//space around label on buttons.
	const int btnHIndent = 12; //horizontal
	const int btnVIndent = 8;  //vertical
#ifdef OS_WINCE
	unsigned int maxWidth  = GetSystemMetrics(SM_CXSCREEN) - (GAP * 2);
	unsigned int maxHeight = GetSystemMetrics(SM_CYSCREEN) - (GAP * 2);
#else
	unsigned int maxWidth  = CMainWindow::getScreenWidth() - (GAP * 2);
	unsigned int maxHeight = CMainWindow::getScreenWidth() - (GAP * 2);
#endif
	unsigned int msgWidth  = 0, msgHeight = 0;
	CClientDC dc(m_hWnd);
	TEXTMETRIC tm = { 0 };
	RECT dlgRect, iconRect = {0 }, msgRect = { 0 }, buttonsRect = { 0 };
	RECT rect = {0}; POINT point = { 0 };
#ifdef OS_WINCE
	int iconId = 0;
#else
	LPWSTR iconId = NULL;
#endif

	GetClientRect(&dlgRect);
	LOG(INFO) + "dlgRect: " + dlgRect.left + " " + dlgRect.top + " " + dlgRect.right + " " + dlgRect.bottom; 

	/**
	 * Icon.
	 */
#ifdef OS_WINCE
	if (m_icon == "alert")
		iconId = MB_ICONWARNING;
	else if (m_icon == "question")
		iconId = MB_ICONQUESTION;
	else if (m_icon == "info")
		iconId = MB_ICONINFORMATION;
#else
	if (m_icon == "alert")
		iconId = IDI_WARNING;
	else if (m_icon == "question")
		iconId = IDI_QUESTION;
	else if (m_icon == "info")
		iconId = IDI_INFORMATION;
#endif 
	
	//if icon has predefined type - try to load it from system resources.
	if (iconId != 0) {
#ifdef OS_WINCE
		HMODULE hGWES = LoadLibraryEx( L"gwes.exe", NULL, LOAD_LIBRARY_AS_DATAFILE );
		HICON hIcon = LoadIcon(hGWES, MAKEINTRESOURCE(iconId));
#else
		HICON hIcon = LoadIcon(NULL, iconId);
#endif

		if (hIcon == NULL) {
			LOG(ERROR) + "Failed to load icon";
		} else {
			iconRect.left = INDENT; 
			iconRect.top  = INDENT;
			iconRect.right  = iconRect.left + iconHeight; 
			iconRect.bottom = iconRect.top + iconHeight;

			m_iconCtrl.Create(m_hWnd, iconRect, NULL, WS_CHILD | WS_VISIBLE | SS_ICON);

			if (m_iconCtrl.SetIcon(hIcon) == NULL)
				LOG(INFO) + "Failed to set icon";
		}
	}

	/**
	 * Message.
	 */
	GetClientRect(&dlgRect);
	dc.GetTextMetrics(&tm);
	
	msgWidth = m_message.length() * tm.tmAveCharWidth + (2 * INDENT);
	msgHeight = (tm.tmHeight + tm.tmExternalLeading + tm.tmInternalLeading);
	point.x = iconRect.right; point.y = iconRect.top;

	int desiredDlgWidth = iconRect.right + msgWidth + (INDENT * 2);

	//if ((desiredDlgWidth > (dlgRect.right - dlgRect.left))) { //adjust dialog window
	if ( m_buttons.size() != 0 ) {
		if ((point.x + msgWidth) > maxWidth) {
			msgWidth = maxWidth - iconRect.right;
			msgHeight = (((m_message.length() * tm.tmAveCharWidth) / msgWidth) + 1) 
						* (tm.tmHeight + tm.tmExternalLeading + tm.tmInternalLeading);
			desiredDlgWidth = iconRect.right + msgWidth + (INDENT * 2);
		} else {
			msgHeight = tm.tmHeight + tm.tmExternalLeading + tm.tmInternalLeading;
		}
			
		if (m_buttons.size() == 0) {
			MoveWindow(GAP, GAP, 
						desiredDlgWidth + 13 + INDENT, 
						MAX((unsigned int )iconRect.bottom, msgHeight) + msgHeight + GetSystemMetrics(SM_CYCAPTION) + INDENT);
		} else { 
			MoveWindow(GAP, GAP, 
						desiredDlgWidth + 13 + INDENT, 
						MAX((unsigned int )iconRect.bottom, msgHeight)  + GetSystemMetrics(SM_CYCAPTION) + INDENT + 
						(tm.tmHeight + btnVIndent + INDENT * 2)); //reserved place for buttons
		}
	}

	msgRect.left = iconRect.right + INDENT;
	msgRect.top  = iconRect.top;
	msgRect.right  = iconRect.right + msgWidth + 2 * INDENT + INDENT * 2; 
	msgRect.bottom = msgRect.top + msgHeight;

	//LOG(DEBUG) + " msgWidth: " + msgWidth;
	//LOG(DEBUG) + " msgRect: " + msgRect.left + " " + msgRect.top + " " + msgRect.right + " " + msgRect.bottom;

	m_messageCtrl.Create(m_hWnd, msgRect, NULL, WS_CHILD | WS_VISIBLE);
	m_messageCtrl.SetWindowText(convertToStringW(m_message).c_str());

	//for Wait dailog text should be centered
	if (m_buttons.size() == 0) {
		m_messageCtrl.CenterWindow();
	}
	
	/**
	 * Buttons.
	 */
	int btnsNum = m_buttons.size(); 
	int btnsWidth = 0, btnsHeight =tm.tmHeight + btnVIndent;

	for (Vector<CustomButton>::iterator itr = m_buttons.begin(); itr != m_buttons.end(); ++itr) {
		btnsWidth += (itr->m_title.length() * tm.tmAveCharWidth) + btnHIndent + (INDENT * 2);
	}

	point.x = INDENT, point.y = (iconHeight > msgHeight ? point.y = iconHeight + 6 : msgHeight + 2) + INDENT;

	unsigned int btnWidth = 0, btnHeight = 0;
	for (Vector<CustomButton>::iterator itr = m_buttons.begin(); itr != m_buttons.end(); ++itr) {
		btnWidth = (itr->m_title.length() * tm.tmAveCharWidth) + btnHIndent;
		btnHeight = tm.tmHeight + btnVIndent;

		RECT rc = {point.x, point.y, point.x + btnWidth, point.y + btnHeight};
		itr->Create(m_hWnd, rc, 
					convertToStringW(itr->m_title).c_str(),
					WS_CHILD | WS_VISIBLE 
#if defined(OS_WINDOWS)
					| BS_DEFPUSHBUTTON 
#endif
					, 0, 
					itr->m_numId);

		point.x += btnWidth + INDENT;
	}

	CenterWindow();

	return bHandled = FALSE;

#undef MAX
}

bool CAlertDialog::findButton(int id, CustomButton &btn)
{
	for (Vector<CustomButton>::iterator itr = m_buttons.begin(); itr != m_buttons.end(); ++itr) {
		if (itr->m_numId == id) {
			btn = *itr;
			return true;
		}
	}
	
	return false;
}

LRESULT CAlertDialog::OnAlertDialogButton (WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CustomButton cbtn;
	if (findButton((int) wID, cbtn))
		rho_rhodesapp_callPopupCallback(m_callback.c_str(), cbtn.m_strId.c_str(), cbtn.m_title.c_str());
	else
		LOG(ERROR) + "internal error";

	EndDialog(wID);
	return 0;
}

/**
 ********************************************************************************
 * CAlert members.
 ********************************************************************************
 */

IMPLEMENT_LOGCLASS(CAlert, "Alert");

void CAlert::showPopup(CAlertDialog::Params *params)
{
    HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd, WM_ALERT_SHOW_POPUP, 0, (LPARAM ) params);
}

#if defined(_WIN32_WCE)
void CAlert::vibrate()
{
    CVibrate::getCVibrate().toggle();
}

void CAlert::playFile(String fileName)
{
    rho::String path = RHODESAPP().getRhoRootPath() + "apps" + fileName;
    HSOUND hSound;
    
    rho::String::size_type pos = 0;
    while ( (pos = path.find('/', pos)) != rho::String::npos ) {
        path.replace( pos, 1, "\\");
        pos++;
    }

    USES_CONVERSION;
    //SndPlaySync(A2T(path.c_str()),  SND_PLAY_IGNOREUSERSETTINGS);
    HRESULT hr = SndOpen(A2T(path.c_str()), &hSound);
    hr = SndPlayAsync (hSound, 0);
      
    if (hr != S_OK) {
        LOG(WARNING) + "OnAlertPlayFile: failed to play file"; 
    }
    
    WaitForSingleObject(hSound, INFINITE);
                        
    hr = SndClose(hSound);
    SndStop(SND_SCOPE_PROCESS, NULL);
}

#endif //_WIN32_WCE

extern "C" void alert_show_popup(rho_param *p)
{
	if (p->type == RHO_PARAM_STRING) {
		CAlert::showPopup(new CAlertDialog::Params(String(p->v.string)));
	} else if (p->type == RHO_PARAM_HASH) {
		String title, message, callback, icon;
		String btnId, btnTitle;
		Hashtable<String, String> buttons;

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

					buttons.put(btnTitle, btnId);
				}
			}//buttons
		}
		
		CAlert::showPopup(new CAlertDialog::Params(title, message, icon, callback, buttons));
	}
}

extern "C" void alert_vibrate(void*) {
#if defined(_WIN32_WCE)
    CAlert::vibrate();
#endif
}

extern "C" void alert_play_file(char* file_name, ...) {
#if defined(_WIN32_WCE)
    CAlert::playFile(file_name);
#endif
}

extern "C" void alert_hide_popup()
{
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd, WM_ALERT_HIDE_POPUP, 0, 0);
}