// This is a part of the Widget Foundation Classes.
// Copyright (C) Hirota Studio (www.hirotastudio.com)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Widget Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Widget Foundation Classes product.
//

#include "stdafx.h"
#include "wfxwidget.h"
#include "wfxcmnwnd.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;

CommonWid::CommonWid()
: m_pBtnMax(new Button)
, m_pBtnMin(new Button)
, m_pBtnClose(new Button)
, m_pImage(WFX_GET_IMAGE(L"E:\\prj\\wfxdev\\bind\\pop_logo.ico"))
{
	m_pBtnMax->SetText(L"¿Ú"); 
	m_pBtnMin->SetText(L"¡ª");
	m_pBtnClose->SetText(L"X");
	m_pBtnMax->SetID(WID_BTNID_MAX);
	m_pBtnMin->SetID(WID_BTNID_MIN);
	m_pBtnClose->SetID(WID_BTNID_CLOSE);
}

LRESULT CommonWid::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	m_pBtnMax->Create(rc, m_pDispatch, this);
	m_pBtnMin->Create(rc, m_pDispatch, this);
	m_pBtnClose->Create(rc, m_pDispatch, this);
	return 1;
}

LRESULT CommonWid::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid = GetRect();
	m_rcCaption = rcWid;
	m_rcClient = rcWid;
	rcWid.bottom = rcWid.top + 20;
	m_rcCaption.bottom = rcWid.bottom;
	rcWid.left = rcWid.right - 20;
	m_pBtnClose->SetRect(rcWid);
	rcWid.left -= 20;
	rcWid.right = rcWid.left + 20;
	m_pBtnMax->SetRect(rcWid);
	rcWid.left -= 20;
	rcWid.right = rcWid.left + 20;
	m_pBtnMin->SetRect(rcWid);

	m_rcCaption.right = rcWid.left - 2;
	m_rcClient.top = m_rcCaption.bottom;
	return 1;
}

LRESULT CommonWid::OnMax( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (::IsMaximized(m_pDispatch->GetHwnd()))
	{
		m_pBtnMax->SetText(L"¿Ú");
		return ::SendMessageW(m_pDispatch->GetHwnd(), WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	m_pBtnMax->SetText(L"ÂÀ");
	return ::SendMessageW(m_pDispatch->GetHwnd(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

LRESULT CommonWid::OnMin( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return ::SendMessage(m_pDispatch->GetHwnd(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

LRESULT CommonWid::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return ::SendMessage(m_pDispatch->GetHwnd(), WM_CLOSE, 0, 0);
}

BOOL CommonWid::IsCaption( const Point& pt )
{
	if (m_rcCaption.PtInRect(pt))
	{
		return TRUE;
	}
	return FALSE;
}


Rect CommonWid::GetClientRect() const
{
	return m_rcClient;
}

void CommonWid::OnDraw( HDC hdc, const Rect& rcPaint )
{
	__super::OnDraw(hdc, rcPaint);
	Rect rcImage = m_rcCaption;
	rcImage.left += 2;
	rcImage.top += 1;
	WfxRender::DrawImage(hdc, m_pImage, rcImage, DT_LEFT);
}

CommonWnd::CommonWnd()
{
}

BOOL CommonWnd::Initialize()
{
	m_pRoot.reset(new CommonWid);
	return TRUE;
}

LRESULT CommonWnd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	WFX_CONDITION(m_pRoot != NULL);
	LONG styleValue = ::GetWindowLongW(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLongW(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	Rect rc;
	m_pRoot->Create(rc, m_pDispatch.get());
	SendWidMessage(WUM_WIDROOT_CREATE);
	return 1;
}

LRESULT CommonWnd::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	GetClientRect(rc);
	m_pRoot->SetRect(rc);
	 bHandled = FALSE;
	return 0;
}

LRESULT CommonWnd::OnGetMinMaxInfo( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	MONITORINFO oMonitor = {0};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	Rect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;
	if (IsMaximized(*this))
	{
		m_pRoot->SendWidMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}
	else
	{
		m_pRoot->SendWidMessage(WM_SYSCOMMAND, SC_RESTORE);
	}
	return 1;
}

BOOL CommonWnd::Create( const String& strName, const Rect& rc, HWND hParent /*= NULL*/ )
{
	Window::Create(hParent, strName.c_str(), WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0, rc);
	return TRUE;
}

LPCWSTR CommonWnd::GetWindowClassName() const
{
	return L"WidgetFoundationClass";
}

UINT CommonWnd::GetClassStyle() const
{
	return CS_HREDRAW | CS_VREDRAW;
}

LRESULT CommonWnd::OnNCActivate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CommonWnd::OnNCPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT CommonWnd::OnNCHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	::ScreenToClient(m_hWnd, &pt);
	Rect rcClient;
	GetClientRect(rcClient);
	if( !::IsZoomed(*this) ) {
		Rect rcSizeBox;
		rcSizeBox.left = 4;
		rcSizeBox.right = 6;
		rcSizeBox.top = 4;
		rcSizeBox.bottom = 6;
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}
	return m_pRoot->IsCaption(pt)? HTCAPTION : HTCLIENT;
}

LRESULT CommonWnd::OnNCCalcSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

FrameWnd::FrameWnd()
{

}

LRESULT FrameWnd::OnNCActivate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT FrameWnd::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	PostQuitMessage(0);
	return 1;
}

int Dialog::DoModal()
{
	Rect rc(0, 0, 200, 200);
	Initialize();
	Create(L"Dialog", rc, WfxDispatcher::GetMainWnd());
	ShowModal();
	return 1;
}

int WfxMessageBox( const String& strText )
{
	return 1;
}
