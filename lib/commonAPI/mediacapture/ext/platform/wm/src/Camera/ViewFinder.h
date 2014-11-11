#pragma once

enum ViewrWndMode
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
	HWND m_hWndViewer;
	HWND m_hWndViewerParent;
	int m_iX;
	int m_iY;
	int m_iH;
	bool m_bIsLandscape;
	ViewrWndMode eMode;

public: 
	CViewFinder();
	void RegisterCallBack(IViewFinderCallBack* pCallBack);	
	HWND CreateViewerWindow(HWND hwndParent,RECT& pos, ViewrWndMode eMode);
	void DestroyViewerWindow();
	void RepositionWindow(const RECT& pos);
	static LRESULT CALLBACK FullScreenWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
protected:
	int scaledpx( int size);
	int scaleForY( int size);
	int scaleForMargain(int size);
};