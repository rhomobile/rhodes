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