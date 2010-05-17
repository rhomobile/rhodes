#pragma once

#if defined(OS_WINDOWS)

template <class T> class COkCancelModalDialog : public CDialogImpl<T> 
{
public:
	CButton		m_btnOk;
	CButton		m_btnCancel;

	COkCancelModalDialog() 
	{
		m_width = m_height = 0;
		m_xborder = m_yborder  =0;
		m_btnHeight = 0;
	};

	void CreateButtons () 
	{
		Maximize();

		RECT rect;
		rect.left    = 1;
		rect.top     = m_height - m_btnHeight;
		rect.right   = rect.left + (m_width >> 1) - 2;
		rect.bottom  = rect.top  + m_btnHeight;
		m_btnOk.Create(m_hWnd, rect, TEXT("Ok"), WS_CHILD | WS_VISIBLE, 0, IDOK);
	
		rect.left    = (m_width >> 1);
		rect.top     = m_height - m_btnHeight;
		rect.right   = rect.left + (m_width >> 1) + 2;
		rect.bottom  = rect.top  + m_btnHeight;
		m_btnCancel.Create(m_hWnd, rect,TEXT("Cancel"),WS_CHILD|WS_VISIBLE,0,IDCANCEL);
	}

private:

	void Maximize () 
	{
		RECT rect;
		NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) };

		GetParent().GetWindowRect(&rect);
		SystemParametersInfo ( SPI_GETNONCLIENTMETRICS, 0, &ncm, false );

		m_width = rect.right - rect.left - GetSystemMetrics(SM_CXEDGE)*2;
		m_height = rect.bottom - rect.top - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYEDGE)*2;
		m_xborder = GetSystemMetrics(SM_CXEDGE) > GetSystemMetrics(SM_CXBORDER) ? 
									GetSystemMetrics(SM_CXEDGE) : GetSystemMetrics(SM_CXBORDER);
		m_yborder = GetSystemMetrics(SM_CYEDGE) > GetSystemMetrics(SM_CYBORDER) ? 
									GetSystemMetrics(SM_CYEDGE) : GetSystemMetrics(SM_CYBORDER);
		m_btnHeight = ncm.iMenuHeight+ncm.iBorderWidth * 4 + 2;

		MoveWindow(rect.left + m_xborder, rect.top  + GetSystemMetrics(SM_CYCAPTION), m_width, m_height);
	}

private:
	int m_width, m_height;
	int m_xborder, m_yborder;
	int m_btnHeight;
};

#endif