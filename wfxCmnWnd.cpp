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
	m_pBtnMax->setText(L"¿Ú"); 
	m_pBtnMin->setText(L"¡ª");
	m_pBtnClose->setText(L"X");
	m_pBtnMax->setID(WID_BTNID_MAX);
	m_pBtnMin->setID(WID_BTNID_MIN);
	m_pBtnClose->setID(WID_BTNID_CLOSE);
}

LRESULT CommonWid::onCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	m_pBtnMax->create(rc, this);
	m_pBtnMin->create(rc, this);
	m_pBtnClose->create(rc, this);
	return 1;
}

LRESULT CommonWid::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid = getRect();
	m_rcCaption = rcWid;
	m_rcClient = rcWid;
	rcWid.bottom = rcWid.top + 20;
	m_rcCaption.bottom = rcWid.bottom;
	rcWid.left = rcWid.right - 20;
	m_pBtnClose->setRect(rcWid);
	rcWid.left -= 20;
	rcWid.right = rcWid.left + 20;
	m_pBtnMax->setRect(rcWid);
	rcWid.left -= 20;
	rcWid.right = rcWid.left + 20;
	m_pBtnMin->setRect(rcWid);

	m_rcCaption.right = rcWid.left - 2;
	m_rcClient.top = m_rcCaption.bottom;
	return 1;
}

LRESULT CommonWid::onMax( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (::IsMaximized(getDispatcher()->getHwnd()))
	{
		m_pBtnMax->setText(L"¿Ú");
		return ::SendMessageW(getDispatcher()->getHwnd(), WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	m_pBtnMax->setText(L"ÂÀ");
	return ::SendMessageW(getDispatcher()->getHwnd(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

LRESULT CommonWid::onMin( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return ::SendMessage(getDispatcher()->getHwnd(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

LRESULT CommonWid::onClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return ::SendMessage(getDispatcher()->getHwnd(), WM_CLOSE, 0, 0);
}

BOOL CommonWid::isCaption( const Point& pt )
{
	if (m_rcCaption.PtInRect(pt))
	{
		return TRUE;
	}
	return FALSE;
}


Rect CommonWid::getClientRect() const
{
	return m_rcClient;
}

void CommonWid::onDraw( HDC hdc, const Rect& rcPaint )
{
	__super::onDraw(hdc, rcPaint);
	Rect rcImage = m_rcCaption;
	rcImage.left += 2;
	rcImage.top += 1;
	WfxRender::drawImage(hdc, m_pImage, rcImage, DT_LEFT);
}

CommonWnd::CommonWnd()
{
}

BOOL CommonWnd::initialize()
{
	m_pRoot.reset(new CommonWid);
	return TRUE;
}

LRESULT CommonWnd::onCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	WFX_CONDITION(m_pRoot != NULL);
	LONG styleValue = ::GetWindowLongW(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLongW(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	Rect rc;
	m_pRoot->create(rc, NULL, getDispatcher());
	sendMessage(WUM_WIDROOT_CREATE);
	return 1;
}

LRESULT CommonWnd::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	getClientRect(rc);
	m_pRoot->setRect(rc);
	 bHandled = FALSE;
	return 0;
}

LRESULT CommonWnd::ongetMinMaxInfo( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	MONITORINFO oMonitor = {0};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	Rect rcWork = oMonitor.rcWork;
	rcWork.offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;
	if (IsMaximized(*this))
	{
		m_pRoot->sendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}
	else
	{
		m_pRoot->sendMessage(WM_SYSCOMMAND, SC_RESTORE);
	}
	return 1;
}

BOOL CommonWnd::create( const String& strName, const Rect& rc, HWND hParent /*= NULL*/ )
{
	Window::create(hParent, strName.c_str(), WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0, rc);
	return TRUE;
}

LPCWSTR CommonWnd::getWindowClassName() const
{
	return L"WidgetFoundationClasses(WFC)";
}

UINT CommonWnd::getClassStyle() const
{
	return CS_HREDRAW | CS_VREDRAW;
}

LRESULT CommonWnd::onNCActivate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CommonWnd::onNCPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT CommonWnd::onNCHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	::ScreenToClient(m_hWnd, &pt);
	Rect rcClient;
	getClientRect(rcClient);
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
	return m_pRoot->isCaption(pt)? HTCAPTION : HTCLIENT;
}

LRESULT CommonWnd::onNCcalcSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

FrameWnd::FrameWnd()
{

}

LRESULT FrameWnd::onNCActivate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT FrameWnd::onClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	PostQuitMessage(0);
	return 1;
}

int Dialog::doModal()
{
	Rect rc(0, 0, 200, 200);
	initialize();
	create(L"Dialog", rc, WfxDispatcher::getMainWnd());
	showModal();
	return 1;
}

int WfxMessageBox( const String& strText )
{
	return 1;
}
