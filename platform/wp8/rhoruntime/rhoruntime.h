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
		void bringToFront();

		// webview
		void navigate(::Platform::String^ url, int index);
		void GoBack(void);
		void GoForward(void);
		void Refresh(int index);
		bool isStarted(void);
		// toolbar
		void toolbarRemoveAllButtons(void);
		void toolbarShow(void);
		void toolbarHide(void);
		int toolbarGetHeight(void);
		void toolbarAddAction(::Platform::String^ text);
		//void toolbarAddAction(Icon^ icon, String^ text, char* action, bool rightAlign /= false/);
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
		void tabbarSwitch(int index);
		int tabbarGetCurrent();
		//int tabbarAddTab(String^ label, char* icon, bool disabled, Color^ web_bkg_color, QTabBarRuntimeParams& tbri);
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
		void fullscreenCommand(int);
		void setCookie(::Platform::String^ url, ::Platform::String^ cookie);

		// misc
		void DoWait(int timeout);
		void DisplayMessage(::Platform::String^ msg);
	};

    public ref class CRhoRuntime sealed
    {
    public:
		CRhoRuntime(IMainPage^ mainPage);
		// rhodes executed in a separate thread
		void Execute();

		// callbacks from MainPage object
		virtual void updateSizeProperties(int width, int height);
		virtual void onActivate(int active);
		virtual void logEvent(::Platform::String^ message);
		virtual void createCustomMenu(void);
		virtual void onCustomMenuItemCommand(int nItemPos);
		virtual void onWindowClose(void);
		virtual void onWebViewUrlChanged(::Platform::String^ url);

		// public methods:
		bool Initialize(::Platform::String^ title);
		void DestroyUi(void);
		//CNativeToolbar& getToolbar(){ return m_toolbar; }
		//CNativeTabbar& getTabbar(){ return m_tabbar; }

	private:
		IMainPage^ m_MainPage;
    };
}