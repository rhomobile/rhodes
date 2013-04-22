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

#pragma once

namespace rhoruntime
{
	public interface class IMainPage
	{
	public:
		int getLogicalDpiX();
		int getLogicalDpiY();
        int getScreenWidth();
        int getScreenHeight();
		bool isBrowserInitialized();
		::Platform::String^ getScreenOrientation();

		void bringToFront();

		// webview
		void navigate(::Platform::String^ url, int index);
		::Platform::String^ executeScript(::Platform::String^ script, int index);
		void GoBack(void);
		void GoForward(void);
		void Refresh(int index);
		bool isStarted(void);
		::Platform::String^ getCurrentURL(int index);
		// toolbar
		void toolbarRemoveAllButtons(void);
		void toolbarShow(void);
		void toolbarHide(void);
		int toolbarIsStarted(void);
		int toolbarGetHeight(void);
		void toolbarAddAction(::Platform::String^ text);
		void toolbarAddAction(::Platform::String^ icon, ::Platform::String^ text, ::Platform::String^ action);
		void toolbarAddSeparator(void);
		void setToolbarStyle(bool border, ::Platform::String^ background);
		// menu
		void menuClear(void);
		void menuAddAction(::Platform::String^ text, int item);
		void menuAddSeparator(void);
		// tabbar
		void tabbarInitialize(void);
		void tabbarRemoveAllTabs(bool restore);
		void tabbarShow(void);
		void tabbarHide(void);
		int tabbarGetHeight(void);
		int tabbarIsStarted(void);
		void tabbarSwitch(int index);
		int tabbarGetCurrent();
		void tabbarAddTab(::Platform::String^ label, ::Platform::String^ icon, ::Platform::String^ action, bool disabled, ::Platform::String^ web_bkg_color, ::Platform::String^ selected_color, bool reload, bool use_current_view_for_tab);
		void tabbarSetBadge(int index, ::Platform::String^ badge);
		void exitCommand(void);
		void navigateBackCommand(void);
		void navigateForwardCommand(void);
		void logCommand(void);
		void refreshCommand(int tab_index);
		//void navigateCommand(TNavigateData* nd);
		void takePicture(::Platform::String^ callbackUrl);
		void selectPicture(::Platform::String^ callbackUrl);
		//void alertShowPopup(CAlertParams *);
		void alertHidePopup(void);
		//void dateTimePicker(CDateTimeMessage *);
		//void executeCommand(RhoNativeViewRunnable*);
		//void executeRunnable(rho::common::IRhoRunnable* pTask);
		//void takeSignature(void*); //TODO: Signature::Params*
		void fullscreenCommand(int fullScreen);
		void setCookie(::Platform::String^ url, ::Platform::String^ cookie);

		// misc
		void DoWait(int timeout);
		void DisplayMessage(::Platform::String^ msg);
	};

    public ref class CRhoRuntime sealed
    {
	private:
		CRhoRuntime(IMainPage^ mainPage);
    public:
		static CRhoRuntime^ getInstance(IMainPage^ mainPage);
		static CRhoRuntime^ getInstance();
		IMainPage^ getMainPage();
		// rhodes executed in a separate thread
		void Execute();

		// callbacks from MainPage object
		void updateSizeProperties(int width, int height);
		void onActivate(int active);
		void logEvent(::Platform::String^ message);
		void createCustomMenu(void);
		void onCustomMenuItemCommand(int nItemPos);
		void onToolbarAction(::Platform::String^ action);
		void onTabbarCurrentChanged(int index, ::Platform::String^ action);
		void onWindowClose(void);
		void onWebViewUrlChanged(::Platform::String^ url);
		bool onBackKeyPress(void);
		::Platform::String^ onJSInvoke(::Platform::String^ inJSON);

		// public methods:
		bool Initialize(::Platform::String^ title);
		void DestroyUi(void);
		//CNativeToolbar& getToolbar(){ return m_toolbar; }
		//CNativeTabbar& getTabbar(){ return m_tabbar; }

	private:
		IMainPage^ m_MainPage;
		static CRhoRuntime^ m_instance;
    };
}