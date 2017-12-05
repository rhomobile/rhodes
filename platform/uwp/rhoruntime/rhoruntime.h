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

#include "json/JSONIterator.h"
#include "api_generator/uwp/IMethodResult.h"

namespace rhoruntime
{
	public interface class IMainPage
	{
	public:
		int getLogicalDpiX();
		int getLogicalDpiY();
        int getScreenWidth();
        int getScreenHeight();
		bool isBrowserInitialized(int index);
		::Platform::String^ getUserAgent();
		::Platform::String^ getWebviewFramework();
		::Platform::String^ getScreenOrientation();
		bool  isEmulator();
		void bringToFront();
		void performOnUiThread(int64 native);

		// webview
		void navigate(::Platform::String^ url, int index);
		::Platform::String^ executeScript(::Platform::String^ script, int index);
		::Platform::String^ executeScriptAsync(::Platform::String^ script, int index);
		void GoBack(void);
		void GoBack(int index);
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
		void setToolbarStyle(bool border, ::Platform::String^ background, ::Platform::String^ mask);
		// menu
		void menuClear(void);
		void menuAddAction(::Platform::String^ text, int item);
		void menuAddSeparator(void);
		// tabbar
		void tabbarInitialize(void);
		void tabbarRemoveAllTabs(void);
		void tabbarShow(void);
		void tabbarHide(void);
		int tabbarGetHeight(void);
		int tabbarIsStarted(void);
		void tabbarRemove(int index);
		void tabbarSwitch(int index);
		int tabbarGetCurrent();
		void tabbarAddTab(::Platform::String^ label, ::Platform::String^ icon, ::Platform::String^ action, bool disabled, ::Platform::String^ web_bkg_color, ::Platform::String^ selected_color, ::Platform::String^ background_color, bool reload, bool use_current_view_for_tab, bool hasCallback, IMethodResult^ oResult);
		void tabbarSetBadge(int index, ::Platform::String^ badge);
		void exitCommand(void);
		void navigateBackCommand(void);
		void navigateForwardCommand(void);
		void logCommand(void);
		void refreshCommand(int tab_index);
		void takePicture(::Platform::String^ callbackUrl);
		void selectPicture(::Platform::String^ callbackUrl);
		void alertHidePopup(void);
		//void dateTimePicker(CDateTimeMessage *);
		//void takeSignature(void*); //TODO: Signature::Params*
		void fullscreenCommand(int fullScreen);
		void setCookie(::Platform::String^ url, ::Platform::String^ cookie);

		// misc
		void DoWait(int timeout);
		void DisplayMessage(::Platform::String^ msg);
	};

	public interface class ICryptoEngine
	{
	public:
		int dbDecrypt(::Platform::String^ partition, int size, ::Platform::String^ data, ::Platform::String^* decryptedData);
		int dbEncrypt(::Platform::String^ partition, int size, ::Platform::String^ data, ::Platform::String^* dataOut);
		int setDbCryptKey(::Platform::String^ partition, ::Platform::String^ key, bool bPersistent);
	};

	public ref class CJSONEntryProxy sealed
	{
	private:
		rho::json::CJSONEntry* m_Entry;
	public:
		CJSONEntryProxy(::Platform::String^ data);
		bool isEmpty();
		bool hasName(::Platform::String^ name);
		bool isString();
		bool isArray();
		bool isObject();
		bool isNull();
		bool isInteger();
		bool isFloat();
		bool isBoolean();

		int getInt(::Platform::String^ name);
		uint64 getUInt64(::Platform::String^ name);
		double getDouble(::Platform::String^ name);
		int getInt();
		uint64 getUInt64();
		double getDouble();
		bool getBoolean();
		::Platform::String^ getString(::Platform::String^ name);
		::Platform::String^ getString(::Platform::String^ name, ::Platform::String^ szDefValue);
		::Platform::String^ getString();
	};

    public ref class CRhoRuntime sealed
    {
	private:
		CRhoRuntime(IMainPage^ mainPage, ::Platform::String^ storagePath);
    public:
		static CRhoRuntime^ getInstance(IMainPage^ mainPage, ::Platform::String^ storagePath);
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
		void executeRhoRunnable(int64 native);
		void setLocalStoragePath(::Platform::String^ storage);
		void setOsVersion(int major, int minor, int build);
		// public methods:
		bool Initialize(::Platform::String^ title);
		void DestroyUi(void);
		void setCryptoEngine(ICryptoEngine^ cryptoEngine);
		ICryptoEngine^ getCryptoEngine() {return m_CryptoEngine;}
		// proxy methods needed for C# implementation of CoreAPI
		::Platform::String^ canonicalizeRhoUrl(::Platform::String^ url);
		::Platform::String^ getRootPath(::Platform::String^ path);
		::Platform::String^ getAppRootPath(::Platform::String^ path);
		::Platform::String^ getRERuntimePath(::Platform::String^ path);

	private:
		IMainPage^ m_MainPage;
		ICryptoEngine^ m_CryptoEngine;
		static CRhoRuntime^ m_instance;
		rho::StringW localStorage;
		rho::StringW localNullFile;
		rho::String localRhoPath;
    };
}