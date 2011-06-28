#include "AlertDialog.h"
#include "ui_AlertDialog.h"
#include "impl/MainWindowImpl.h"

using namespace rho;

//#define ID_ALERT_DLG_BUTTON_MAX   (8)
//#define ID_ALERT_DLG_BUTTON_FIRST (1)
//#define ID_ALERT_DLG_BUTTON_LAST  (ID_ALERT_DLG_BUTTON_FIRST + (ID_ALERT_DLG_BUTTON_MAX) - 1)

AlertDialog::AlertDialog(Params* params, QWidget *parent) :
    QDialog(parent),
    m_title(params->m_title),
    m_message(params->m_message),
    m_callback(params->m_callback),
    m_icon(params->m_icon),
    ui(new Ui::AlertDialog)
{
    //int id = ID_ALERT_DLG_BUTTON_FIRST;
    for (int i = 0; i < (int)params->m_buttons.size(); i++)
    {
        //if(id > ID_ALERT_DLG_BUTTON_LAST)
        //{
        //    LOG(ERROR) + "too many buttons";
        //    break;
        //}

        //m_buttons.addElement(CustomButton( params->m_buttons.elementAt(i).m_strCaption, params->m_buttons.elementAt(i).m_strID, id++));
    }

    ui->setupUi(this);
    //int width  = CMainWindow::getScreenWidth()/3;
    //this->setGeometry(0, 0, width, width/3);
    this->init();
}

// onAlbertButtonClick:
//{
//    rho_rhodesapp_callPopupCallback(m_callback.c_str(), cbtn.m_strId.c_str(), cbtn.m_title.c_str());
//    EndDialog(wID);
//}

void AlertDialog::init()
{
//    const int GAP    = 12; //space around dialog
//    const int INDENT = 14; //for dialog items

//    int maxWidth  = CMainWindow::getScreenWidth() - (GAP * 2);
//    int maxHeight = CMainWindow::getScreenHeight() - (GAP * 2);

//    int xBorderWidth = GetSystemMetrics(SM_CXEDGE);
//    int yBorderWidth = GetSystemMetrics(SM_CYEDGE);
//    CClientDC dc(m_hWnd);
//    TEXTMETRIC tm = { 0 };
//    RECT rect = {0}, dlgRect, iconRect = {0 }, msgRect = { 0 }, buttonsRect = { 0 };

//    GetClientRect(&dlgRect);

//    /***************************************************************************
//     Icon.
//     ***************************************************************************/
//    HICON hIcon = loadIcon();
// //    { "alert",    IDI_WARNING },
// //    { "question", IDI_QUESTION    },
// //    { "info",     IDI_INFORMATION }
//    if (hIcon == NULL) {
//            LOG(ERROR) + "Failed to load icon";
//    } else {
//            iconRect.left = INDENT;
//            iconRect.top  = INDENT;
//            iconRect.right  = iconRect.left + GetSystemMetrics(SM_CXICON);
//            iconRect.bottom = iconRect.top + GetSystemMetrics(SM_CYICON);

//            m_iconCtrl.Create(m_hWnd, iconRect, NULL, WS_CHILD | WS_VISIBLE | SS_ICON);
//            m_iconCtrl.SetIcon(hIcon);
//    }
//    int iconHeight = iconRect.bottom - iconRect.top;
//    int iconWidth  = iconRect.left - iconRect.right;

//    /***************************************************************************
//      Message.
//     ***************************************************************************/
//    GetClientRect(&dlgRect);

//    //create control
//    m_messageCtrl.Create(m_hWnd, CWindow::rcDefault, NULL, WS_CHILD | WS_VISIBLE);

//    //change control  font to normal
//    HFONT hFont = m_messageCtrl.GetFont();
//    LOGFONT fontAttributes = { 0 };
//    ::GetObject(hFont, sizeof(fontAttributes), &fontAttributes);
//    fontAttributes.lfWeight = FW_NORMAL;
//    hFont = CreateFontIndirect(&fontAttributes);
//    m_messageCtrl.SetFont(hFont);


//    //get text metrics
//    dc.GetTextMetrics(&tm);

//    int msgWidth  =  (int)(m_message.length() * (tm.tmAveCharWidth * 1.3) + (2 * INDENT));
//    int msgHeight =  tm.tmHeight + tm.tmExternalLeading + tm.tmInternalLeading;

//    int desiredDlgWidth = iconRect.right + msgWidth + (INDENT * 2);

//    //space around label on buttons.
//    const int btnHIndent = 12; //horizontal
//    const int btnVIndent = 8;  //vertical

//    int btnsNum = m_buttons.size();
//    int btnsHeight =tm.tmHeight + btnVIndent;

//    //if desired widht is bigger than current - make dialog window bigger
//    if (desiredDlgWidth > (dlgRect.right - dlgRect.left)) {
//            // if desired width is bigger than maximum width
//            // - recalculate dialog window with wrapped text
//            if (desiredDlgWidth >= (int)maxWidth) {
//                    msgWidth = maxWidth - (iconRect.right + INDENT*2 + xBorderWidth*2);
//                    msgHeight = (((m_message.length() * tm.tmAveCharWidth) / msgWidth) + 1)
//                                            * (tm.tmHeight + tm.tmExternalLeading + tm.tmInternalLeading);
//                    desiredDlgWidth = maxWidth;
//            }

//            MoveWindow(0, 0, desiredDlgWidth,
//                                    MAX(iconRect.bottom, msgHeight) + GetSystemMetrics(SM_CYCAPTION) + INDENT + yBorderWidth * 2 +
//                btnsHeight);
//    } else {
//            //TODO: centering message
//    }

//    m_messageCtrl.SetWindowText(convertToStringW(m_message).c_str());

//    //LOG(INFO) + "iconHeight  msgHeight " + iconHeight + " "+ msgHeight;

//    if (iconHeight > msgHeight) {
//            msgRect.left = iconRect.right + INDENT;
//            msgRect.top  = iconRect.top + (iconHeight - msgHeight)/2;
//            msgRect.right  = msgRect.left + msgWidth;
//            msgRect.bottom = msgRect.top + msgHeight;
//            //center message on icon
//            m_messageCtrl.MoveWindow(iconRect.right + INDENT, iconRect.top + (iconHeight - msgHeight)/2, msgWidth, msgHeight);
//    } else {
//            msgRect.left = iconRect.right + INDENT;
//            msgRect.top  = iconRect.top;
//            msgRect.right  = msgRect.left + msgWidth;
//            msgRect.bottom = msgRect.top + msgHeight;
//            m_messageCtrl.MoveWindow(iconRect.right + INDENT, iconRect.top, msgWidth, msgHeight);
//            //center icon on message
//            if (hIcon) {
//                    m_iconCtrl.MoveWindow(INDENT, iconRect.top + (msgHeight - iconHeight)/2, iconWidth, iconHeight);
//                    m_iconCtrl.SetIcon(hIcon);
//            }
//    }

//    //for Wait dailog text should be centered
//    if (m_buttons.size() == 0) {
//            m_messageCtrl.CenterWindow();
//    }

//    /***************************************************************************
//      Buttons.
//     ***************************************************************************/

//    GetClientRect(&dlgRect);

//    //for (Vector<CustomButton>::iterator itr = m_buttons.begin(); itr != m_buttons.end(); ++itr) {
//    //	btnsWidth += (itr->m_title.length() * tm.tmAveCharWidth) + btnHIndent + (INDENT * 2);
//    //}

//    POINT point;
//    point.x = INDENT, point.y = (iconHeight > msgHeight ? point.y = iconHeight + 6 : msgHeight + 2) + INDENT;

//    unsigned int btnWidth = 87, btnHeight = 0;
//    btnHeight = tm.tmHeight + btnVIndent;

//    //if (iconHeight + msgHeight + INDENT + btnHeight + INDENT > dlgRect.) {
//    //}

//    for (Vector<CustomButton>::iterator itr = m_buttons.begin(); itr != m_buttons.end(); ++itr)
//    {
//            //btnWidth = (itr->m_title.length() * tm.tmAveCharWidth) + btnHIndent;

//            RECT rc = {point.x, point.y, point.x + btnWidth, point.y + btnHeight};
//            itr->Create(m_hWnd, rc,
//                                    convertToStringW(itr->m_title).c_str(),
//                                    WS_CHILD | WS_VISIBLE, 0,
//                                    itr->m_numId);

//            point.x += btnWidth + INDENT;
//    }

//    CenterWindow();
}

AlertDialog::~AlertDialog()
{
    delete ui;
}
