#pragma once


enum eViewrWndMode
{
	eConfigurable,
	eFullScreen
};

struct IViewFinderCallBack
{
	virtual void cancel()=0;
	virtual void captureImage()=0;
	virtual void ResetViewerWndPos(RECT& pos)=0;
};
class CViewFinder
{
private:
	
	IViewFinderCallBack* m_pCallBack;	
	static HWND m_hwndPreview;
	static HWND m_PreviewWndParent;
	static HWND m_CancelButton;
	static HWND m_CaptureButton;
	HINSTANCE m_appInstance;
	HWND m_appMainWnd;

public: 
	CViewFinder();
	void RegisterCallBack(IViewFinderCallBack* pCallBack);	
	HWND CreateViewerWindow(RECT& pos, eViewrWndMode eMode);
	void DestroyViewerWindow();
	void RepositionWindow(const RECT& pos);
	static LRESULT CALLBACK FullScreenWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND getViewerWndHandle();
protected:
	HWND createFullScreenWindow(HWND hwndParent, RECT& pos);
	HWND createPreviewWindow(HWND hwndParent, const RECT& pos);
	static void ReposFullScreen(RECT& pos);
};