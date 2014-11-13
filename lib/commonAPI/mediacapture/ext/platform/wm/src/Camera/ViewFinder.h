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
};
class CViewFinder
{
private:
	
	IViewFinderCallBack* m_pCallBack;	
	HWND m_hwndPreview;
	//int m_iX;
	//int m_iY;
	//int m_iH;
	//bool m_bIsLandscape;
	HINSTANCE m_appInstance;
	HWND m_appMainWnd;

public: 
	CViewFinder();
	void RegisterCallBack(IViewFinderCallBack* pCallBack);	
	HWND CreateViewerWindow(RECT& pos, eViewrWndMode eMode);
	void DestroyViewerWindow();
	void RepositionWindow(const RECT& pos);
	static LRESULT CALLBACK FullScreenWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
protected:
	int scaledpx( int size);
	int scaleForY( int size);
	int scaleForMargain(int size);
	HWND createFullScreenWindow(HWND hwndParent, RECT& pos);
	HWND createPreviewWindow(HWND hwndParent, const RECT& pos);
};