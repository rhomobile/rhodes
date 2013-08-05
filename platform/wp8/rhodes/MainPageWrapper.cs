using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rhodes
{
    public class MainPageWrapper : rhoruntime.IMainPage
    {
        private MainPage _mainPage;
        public MainPageWrapper(MainPage mainPage) { _mainPage = mainPage; }

        public int getLogicalDpiX() { return _mainPage.getLogicalDpiX(); }
        public int getLogicalDpiY() { return _mainPage.getLogicalDpiY(); }
        public int getScreenWidth() { return _mainPage.getScreenWidth();  }
        public int getScreenHeight() { return _mainPage.getScreenHeight();  }
        public string getUserAgent() { return _mainPage.getUserAgent(); }
        public string getWebviewFramework() { return _mainPage.getWebviewFramework(); }
        public string getScreenOrientation() { return _mainPage.getScreenOrientation(); }
        public bool isBrowserInitialized(int index) { return _mainPage.isBrowserInitialized(index); }
        public void bringToFront() { _mainPage.bringToFront(); }
        public void performOnUiThread(Int64 native) { _mainPage.performOnUiThread(native); }

		// webview
        public void navigate(string url, int index) { _mainPage.navigate(url, index); }
        public string executeScript(string script, int index) { return _mainPage.executeScript(script, index); }
        public string executeScriptAsync(string script, int index) { return _mainPage.executeScriptAsync(script, index); }
        public void GoBack() { _mainPage.GoBack(); }
        public void GoBack(int index) { _mainPage.GoBack(index); }
        public void GoForward() { _mainPage.GoForward(); }
        public void Refresh(int index) { _mainPage.Refresh(index); }
        public bool isStarted() { return _mainPage.isStarted(); }
        public bool isEmulator() { return _mainPage.isEmulator(); }
        public string getCurrentURL(int index) { return _mainPage.getCurrentURL(index); }
		// toolbar
        public void toolbarRemoveAllButtons() { _mainPage.toolbarRemoveAllButtons(); }
        public void toolbarShow() { _mainPage.toolbarShow(); }
        public void toolbarHide() { _mainPage.toolbarHide(); }
        public int toolbarIsStarted() { return _mainPage.toolbarIsStarted(); }
        public int toolbarGetHeight() { return _mainPage.toolbarGetHeight(); }
        public void toolbarAddAction(string text) { _mainPage.toolbarAddAction(text); }
        public void toolbarAddAction(string icon, string text, string action) { _mainPage.toolbarAddAction(icon, text, action); }
        public void toolbarAddSeparator() { _mainPage.toolbarAddSeparator(); }
        public void setToolbarStyle(bool border, string background, string mask) { _mainPage.setToolbarStyle(border, background, mask); }
		// menu
        public void menuClear() { _mainPage.menuClear(); }
        public void menuAddAction(string text, int item) { _mainPage.menuAddAction(text, item); }
        public void menuAddSeparator() { _mainPage.menuAddSeparator(); }
		// tabbar
        public void tabbarInitialize() { _mainPage.tabbarInitialize(); }
        public void tabbarRemoveAllTabs() { _mainPage.tabbarRemoveAllTabs(); }
        public void tabbarShow() { _mainPage.tabbarShow(); }
        public void tabbarHide() { _mainPage.tabbarHide(); }
        public int tabbarIsStarted() { return _mainPage.tabbarIsStarted(); }
        public int tabbarGetHeight() { return _mainPage.tabbarGetHeight(); }
        public void tabbarRemove(int index) { _mainPage.tabbarRemove(index); }
        public void tabbarSwitch(int index) { _mainPage.tabbarSwitch(index); }
        public int tabbarGetCurrent() { return _mainPage.tabbarGetCurrent(); }
        public void tabbarAddTab(string label, string icon, string action, bool disabled, string web_bkg_color, string selected_color, string background_color, bool reload, bool use_current_view_for_tab, bool hasCallback, rhoruntime.IMethodResult oResult)
        { _mainPage.tabbarAddTab(label, icon, action, disabled, web_bkg_color, selected_color, background_color, reload, use_current_view_for_tab, hasCallback, oResult); }
        public void tabbarSetBadge(int index, string badge) { _mainPage.tabbarSetBadge(index, badge); }
        public void exitCommand() { _mainPage.exitCommand(); }
        public void navigateBackCommand() { _mainPage.navigateBackCommand(); }
        public void navigateForwardCommand() { _mainPage.navigateForwardCommand(); }
        public void logCommand() { _mainPage.logCommand(); }
        public void refreshCommand(int tab_index) { _mainPage.refreshCommand(tab_index); }
        public void takePicture(string callbackUrl) { _mainPage.takePicture(callbackUrl); }
        public void selectPicture(string callbackUrl) { _mainPage.selectPicture(callbackUrl); }
        public void alertHidePopup() { _mainPage.alertHidePopup(); }
        //public void dateTimePicker(CDateTimeMessage *);
        //public void takeSignature(void*); //TODO: Signature::Params*
        public void fullscreenCommand(int fullScreen) { _mainPage.fullscreenCommand(fullScreen); }
        public void setCookie(string url, string cookie) { _mainPage.setCookie(url, cookie); }

		// misc
        public void DoWait(int timeout) { _mainPage.DoWait(timeout); }
        public void DisplayMessage(string msg) { _mainPage.DisplayMessage(msg); }
    }
}
