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
#include "StdAfx.h"
#include "wfxwidget.h"
#include "wfxcmnctrl.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ComboWnd::ComboWnd()
: m_pListCtrl(new ListCtrl)
{
}

void ComboWnd::onInPlaceWindowKillFocus()
{
	LONG nCurrent = m_pListCtrl->getCurrentRow();
	if (nCurrent != -1)
	{
		m_pOwner->sendMessage(WUM_COMBO_SET_SEL, nCurrent);
		m_pOwner->invalidWid();
	}
}

HWND ComboWnd::createInPlaceWindow()
{
	WFX_CONDITION(m_pOwner != NULL);
	WFX_CONDITION(m_pOwner->getDispatcher() != NULL);
	m_szComboSize = m_pOwner->sendMessage(WUM_COMBO_GET_SIZE);
	Rect rcWid = m_pOwner->getRect();
	Rect rc = rcWid;
	rc.right = rc.left + m_szComboSize.cx;
	rc.top = rc.bottom;
	rc.bottom = rc.top + m_szComboSize.cy;
	MapWindowRect(m_pOwner->getDispatcher()->getHwnd(), HWND_DESKTOP, &rc);
	create(m_pOwner->getDispatcher()->getHwnd(), NULL, WS_POPUP | WS_BORDER, WS_EX_TOOLWINDOW, rc);
	HWND hWndParent = m_hWnd;
	while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
	::ShowWindow(m_hWnd, SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	return m_hWnd;
}

LPCWSTR ComboWnd::getWindowClassName() const
{
	return L"ComboWnd";
}


LRESULT ComboWnd::onCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	LONG nItems = m_pOwner->sendMessage(WUM_COMBO_GET_CNT);
	m_pListCtrl->create(rc, NULL, getDispatcher());
	for (int i = 0; i < 1; i++)
	{
		String strCol;
		strCol.format(L"LIE%d", i);
		m_pListCtrl->addColumn(strCol, m_szComboSize.cx - 10, 0, 0);
	}
	for (int i = 0; i < nItems; i++)
	{
		m_pListCtrl->addRow();
	}
	m_pListCtrl->setRowNumBarWidth(0);
	m_pListCtrl->setHeadHeight(0);
	m_pListCtrl->enableScrollBar(WESB_VERT);
	return 1;
}

LRESULT ComboWnd::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcClient;
	getClientRect(rcClient);
	m_pListCtrl->setRect(rcClient);
	return 1;
}

LRESULT ComboWnd::onLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_pLButtonDown = lParam;
	return getDispatcher()->handleMessage(uMsg, wParam, lParam);
}

LRESULT ComboWnd::onLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LRESULT lResult = getDispatcher()->handleMessage(uMsg, wParam, lParam);
	if (m_pListCtrl->inClientAera(lParam) && m_pListCtrl->inClientAera(m_pLButtonDown))
	{
		sendMessage(WM_KILLFOCUS);
	}
	m_pLButtonDown.empty();
	return lResult;
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ComboBox::ComboBox()
: m_nSelected(-1)
, m_pTextBox(new TextBox)
{

}

LRESULT ComboBox::onCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	m_pTextBox->create(rc, this);
	return 1;
}

LRESULT ComboBox::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid(getRect());
	m_rcDropDown = rcWid;
	m_rcDropDown.left = rcWid.right - 15;
	rcWid.right = rcWid.right - m_rcDropDown.getWidth();
	m_pTextBox->setRect(rcWid);
	return 1;
}

BOOL ComboBox::initial()
{
	m_pWindow = new ComboWnd;
	m_pWindow->initial(this);
	return TRUE;
}

void ComboBox::onDraw( HDC hdc, const Rect& rcPaint )
{
	WfxRender::drawComboDropDown(hdc, m_rcDropDown, getState());
}

LRESULT ComboBox::ongetComboSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return Size(getRect().getWidth(), 400);
}

LRESULT ComboBox::ongetCount( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 300;
}

LRESULT ComboBox::ongetItemText( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	static String strText;
	strText.format(L"Item = %d", (LONG)wParam);
	return (LRESULT)strText.c_str();
}

LRESULT ComboBox::ongetItemHeight( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 20;
}

LRESULT ComboBox::ongetSelectedItem( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return m_nSelected;
}

LRESULT ComboBox::onsetSelectedItem( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_nSelected = (LONG)wParam;
	m_pTextBox->setText((WCHAR*)sendMessage(WUM_COMBO_GET_ITEM_TEXT, m_nSelected));
	return 1;
}
