// MainWindow.cpp: Defines main window for this application.

#include "stdafx.h"
#include "MainWindowQt.h"
#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "camera/Camera.h"
#include "common/RhoFilePath.h"
#include "AppManager.h"

IMPLEMENT_LOGCLASS(CMainWindow,"MainWindow");

#include "DateTimePicker.h"

using namespace rho;
using namespace rho::common;

extern "C" void rho_geoimpl_turngpsoff();

int CMainWindow::m_screenWidth;
int CMainWindow::m_screenHeight;

bool CMainWindow::mainWindowClosed = false;

CMainWindow::CMainWindow() { }

CMainWindow::~CMainWindow() { }

void CMainWindow::updateSizeProperties(int width, int height)
{
    m_screenWidth = width;
    m_screenHeight = height;
    LOGCONF().setLogView(&m_logView);
}

void CMainWindow::logEvent(const ::std::string& message)
{
    LOG(INFO) + message;
}

void CMainWindow::Navigate2(BSTR URL) {
    String cleared_url = convertToStringA(OLE2CT(URL));
    if (!cleared_url.empty()) {
        StringW cw = convertToStringW(cleared_url);
        m_mainWindowProxy.navigate(cw.c_str());
    }
}

HWND CMainWindow::Initialize(const wchar_t* title)
{
    HWND hWnd = (HWND)m_mainWindowProxy.init(this, title);
    SubclassWindow(hWnd);

    //rho_rhodesapp_callAppActiveCallback(1);
    rho_rhodesapp_callUiCreatedCallback();
    return hWnd;
}

HWND CMainWindow::getWebViewHWND()
{
    // TODO
    return 0;
}

void CMainWindow::MessageLoop(void)
{
    m_mainWindowProxy.messageLoop();
}

void CMainWindow::createCustomMenu(void)
{
    RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);
    //m_mainWindowProxy.createCustomMenu();
#ifdef ENABLE_DYNAMIC_RHOBUNDLE
    String strIndexPage = CFilePath::join(RHODESAPP().getStartUrl(),"index"RHO_ERB_EXT);
    if ( RHODESAPP().getCurrentUrl().compare(RHODESAPP().getStartUrl()) == 0 ||
         RHODESAPP().getCurrentUrl().compare(strIndexPage) == 0 )
        m_arAppMenuItems.addElement(CAppMenuItem("Reload RhoBundle","reload_rhobundle"));
#endif //ENABLE_DYNAMIC_RHOBUNDLE

    //update UI with custom menu items
    m_mainWindowProxy.menuClear();
    for ( unsigned int i = 0; i < m_arAppMenuItems.size(); i++)
    {
        CAppMenuItem& oItem = m_arAppMenuItems.elementAt(i);
        if (oItem.m_eType == CAppMenuItem::emtSeparator) 
            m_mainWindowProxy.menuAddSeparator();
        else
        {
            m_mainWindowProxy.menuAddAction((oItem.m_eType == CAppMenuItem::emtClose ? "Exit" : oItem.m_strLabel.c_str()), i);
        }
    }
}

void CMainWindow::onCustomMenuItemCommand(int nItemPos)
{    
    if ( nItemPos < 0 || nItemPos >= (int)m_arAppMenuItems.size() )
        return;

    CAppMenuItem& oMenuItem = m_arAppMenuItems.elementAt(nItemPos);
    if ( oMenuItem.m_eType == CAppMenuItem::emtUrl )
    {
        if ( oMenuItem.m_strLink == "reload_rhobundle" )
        {
        #ifdef ENABLE_DYNAMIC_RHOBUNDLE
            if ( RHODESAPP().getRhobundleReloadUrl().length()>0 ) {
                CAppManager::ReloadRhoBundle(m_hWnd,RHODESAPP().getRhobundleReloadUrl().c_str(), NULL);
            } else {
                MessageBox(_T("Path to the bundle is not defined."),_T("Information"), MB_OK | MB_ICONINFORMATION );
            }
        #endif
            return;
        }
    }

    oMenuItem.processCommand();
}

void CMainWindow::DestroyUi(void)
{
    rho_rhodesapp_callUiDestroyedCallback();
}

void CMainWindow::onWindowClose(void)
{
     mainWindowClosed = true;
}

// **************************************************************************
//
// WM_xxx handlers
//
// **************************************************************************

void CMainWindow::performOnUiThread(rho::common::IRhoRunnable* pTask)
{
    PostMessage(WM_EXECUTE_RUNNABLE, 0, (LPARAM)pTask);
}
LRESULT CMainWindow::OnExecuteRunnable(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) 
{
    rho::common::IRhoRunnable* pTask = (rho::common::IRhoRunnable*)lParam;
    if (pTask)
    {
        pTask->runObject();
        delete pTask;
    }
    return 0;
}

LRESULT CMainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    if(m_logView.IsWindow()) {
        m_logView.DestroyWindow();
    }
    LOGCONF().setLogView(NULL);

    if (m_alertDialog && m_alertDialog->IsWindow() )
        m_alertDialog->DestroyWindow();
    if (m_SyncStatusDlg && m_SyncStatusDlg->IsWindow() )
        m_SyncStatusDlg->DestroyWindow();

    PostQuitMessage(0);

    bHandled = FALSE; // Allow ATL's default processing (e.g. NULLing m_hWnd)
    return 0;
}

void CMainWindow::onActivate(int active)
{
    rho_rhodesapp_callAppActiveCallback(active);
    if (!active)
        rho_geoimpl_turngpsoff();
}


// **************************************************************************
//
// WM_COMMAND handlers
//
// **************************************************************************

LRESULT CMainWindow::OnExitCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    SendMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainWindow::OnNavigateBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_mainWindowProxy.GoBack();
    return 0;
}

LRESULT CMainWindow::OnNavigateForwardCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_mainWindowProxy.GoForward();
    return 0;
}

LRESULT CMainWindow::OnBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    RHODESAPP().navigateBack();
    return 0;
}

LRESULT CMainWindow::OnLogCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if ( !m_logView.IsWindow() ) {
        LoadLibrary(_T("riched20.dll"));
        m_logView.Create(NULL);
    }
    m_logView.ShowWindow(SW_SHOWNORMAL);
    return 0;
}

LRESULT CMainWindow::OnRefreshCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_mainWindowProxy.Refresh();
    return 0;
}

LRESULT CMainWindow::OnNavigateCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    LPTSTR wcurl = (LPTSTR)hWndCtl;
    if (wcurl) {
        Navigate2(wcurl);
        free(wcurl);
    }
    return 0;
}

LRESULT CMainWindow::OnTakePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
{
    TCHAR image_uri[MAX_PATH];
    HRESULT status;
    //TODO: show browse file dialog
    wsprintf( image_uri, L"%s", L"dashboard.PNG");
    status = S_OK;

    RHODESAPP().callCameraCallback( (const char*)lParam, rho::common::convertToStringA(image_uri),
        (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);

    free ((void *)lParam);
    return 0;
}

LRESULT CMainWindow::OnSelectPicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
{
    TCHAR image_uri[MAX_PATH];
    HRESULT status = S_OK;
    Camera camera;
    status = camera.selectPicture(this->m_hWnd,image_uri);

    RHODESAPP().callCameraCallback( (const char*)lParam, rho::common::convertToStringA(image_uri),
        (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);
    
    free ((void *)lParam);
    
    return 0;
}

LRESULT CMainWindow::OnAlertShowPopup (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    /* TODO */
    StringW strAppName = RHODESAPP().getAppNameW();
    CAlertDialog::Params *params = (CAlertDialog::Params *)lParam;

    if (params->m_dlgType == CAlertDialog::Params::DLG_STATUS) 
    {
        if (m_SyncStatusDlg == NULL) 
            m_SyncStatusDlg = new CSyncStatusDlg();
        m_SyncStatusDlg->setStatusText(convertToStringW(params->m_message).c_str());
        m_SyncStatusDlg->setTitle( convertToStringW(params->m_title).c_str() );
        if ( !m_SyncStatusDlg->m_hWnd )
            m_SyncStatusDlg->Create(m_hWnd, 0);
        else
        {
            m_SyncStatusDlg->ShowWindow(SW_SHOW);
            m_SyncStatusDlg->BringWindowToTop();
        }
    } else if (params->m_dlgType == CAlertDialog::Params::DLG_DEFAULT) {
        MessageBox(convertToStringW(params->m_message).c_str(), strAppName.c_str(), MB_ICONWARNING | MB_OK);
    } else if (params->m_dlgType == CAlertDialog::Params::DLG_CUSTOM) 
    {
        if ( params->m_buttons.size() == 1 && strcasecmp(params->m_buttons[0].m_strCaption.c_str(), "ok") == 0)
            MessageBox(convertToStringW(params->m_message).c_str(), convertToStringW(params->m_title).c_str(), MB_ICONWARNING | MB_OK);
        else if (params->m_buttons.size() == 2 && strcasecmp(params->m_buttons[0].m_strCaption.c_str(), "ok") == 0 &&
            strcasecmp(params->m_buttons[1].m_strCaption.c_str(), "cancel") == 0)
        {
            int nRes = MessageBox(convertToStringW(params->m_message).c_str(), convertToStringW(params->m_title).c_str(), 
                    MB_ICONWARNING | MB_OKCANCEL);
            int nBtn = nRes == IDCANCEL ? 1 : 0;
            RHODESAPP().callPopupCallback(params->m_callback, params->m_buttons[nBtn].m_strID, params->m_buttons[nBtn].m_strCaption);
        }
        else
        {
            if (m_alertDialog == NULL) 
            {
                m_alertDialog = new CAlertDialog(params);
                m_alertDialog->DoModal();
                delete m_alertDialog;
                m_alertDialog = NULL;
            } else {
                LOG(WARNING) + "Trying to show alert dialog while it exists.";
            }
        }
    }

    delete params;
    /*  */
    return 0;
}

LRESULT CMainWindow::OnAlertHidePopup (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    /* TODO */
    if (m_alertDialog != NULL) {
        m_alertDialog->EndDialog(0);
        m_alertDialog = NULL;
    }
    /*  */
    return 0;
}

LRESULT CMainWindow::OnExecuteCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
    RhoNativeViewRunnable* command = (RhoNativeViewRunnable*)wParam;
    if (command != NULL) {
        command->run();
    }
    return 0;
}    

LRESULT CMainWindow::OnDateTimePicker (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    CDateTimeMessage *msg = (CDateTimeMessage *)lParam;
    int retCode    = -1;
    time_t ret_time = 0;

    if (msg->m_format == CDateTimeMessage::FORMAT_TIME) {
        CTimePickerDialog timeDialog(msg);
        retCode = timeDialog.DoModal(m_hWnd);
        ret_time = timeDialog.GetTime();
    } else {
        CDateTimePickerDialog dateTimeDialog(msg);
        retCode = dateTimeDialog.DoModal(m_hWnd);
        ret_time = dateTimeDialog.GetTime();
    }

    rho_rhodesapp_callDateTimeCallback( msg->m_callback, 
                                        retCode == IDOK ? (long )ret_time : 0,
                                        msg->m_data,
                                        retCode == IDOK ? 0 : 1);
    delete msg;

    return 0;
}
