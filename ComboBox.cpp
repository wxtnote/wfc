// This is a part of the Widget Foundation Classes.
// Copyright (C) Grant Ward (grant.ward@gmail.com)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Widget Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Widget Foundation Classes product.
//
#include "StdAfx.h"
#include "wfxwid.h"
#include "wfxcmn.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ComboWnd::ComboWnd()
: m_pListCtrl(new ListCtrl)
{
}

void ComboWnd::OnInPlaceWindowKillFocus()
{
	LONG nCurrent = m_pListCtrl->GetCurrentRow();
	if (nCurrent != -1)
	{
		m_pOwner->SendWidMessage(WUM_COMBO_SET_SEL, nCurrent);
		m_pOwner->InvalidWid();
	}
}

HWND ComboWnd::CreateInPlaceWindow()
{
	WFX_CONDITION(m_pOwner != NULL);
	WFX_CONDITION(m_pOwner->m_pDispatch != NULL);
	Size szCombo = m_pOwner->SendWidMessage(WUM_COMBO_GET_SIZE);
	Rect rcWid = m_pOwner->GetRect();
	Rect rc = rcWid;
	rc.right = rc.left + szCombo.cx;
	rc.top = rc.bottom;
	rc.bottom = rc.top + szCombo.cy;
	MapWindowRect(m_pOwner->m_pDispatch->GetHwnd(), HWND_DESKTOP, &rc);
	Create(m_pOwner->m_pDispatch->GetHwnd(), NULL, WS_POPUP | WS_BORDER, WS_EX_TOOLWINDOW, rc);
	HWND hWndParent = m_hWnd;
	while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
	::ShowWindow(m_hWnd, SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	return m_hWnd;
}

LPCWSTR ComboWnd::GetWindowClassName() const
{
	return L"ComboWnd";
}


LRESULT ComboWnd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	LONG nItems = m_pOwner->SendWidMessage(WUM_COMBO_GET_CNT);
	m_pListCtrl->Create(rc, m_pDispatch.get());
	for (int i = 0; i < 1; i++)
	{
		String strCol;
		strCol.Format(L"LIE%d", i);
		m_pListCtrl->AddColumn(strCol, 100, 0, 0);
	}
	for (int i = 0; i < nItems; i++)
	{
		m_pListCtrl->AddRow();
	}
	m_pListCtrl->SetRowNumBarWidth(0);
	m_pListCtrl->SetHeadHeight(0);
	m_pListCtrl->EnableScrollBar(WESB_VERT);
	return 1;
}

LRESULT ComboWnd::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcClient;
	GetClientRect(rcClient);
	m_pListCtrl->SetRect(rcClient);
	return 1;
}

LRESULT ComboWnd::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_pLButtonDown = lParam;
	return m_pDispatch->HandleMessage(uMsg, wParam, lParam);
}

LRESULT ComboWnd::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LRESULT lResult = m_pDispatch->HandleMessage(uMsg, wParam, lParam);
	if (m_pListCtrl->InFunctionAera(lParam) && m_pListCtrl->InFunctionAera(m_pLButtonDown))
	{
		SendWidMessage(WM_KILLFOCUS);
	}
	m_pLButtonDown.Empty();
	return lResult;
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ComboBox::ComboBox()
: m_nSelected(-1)
, m_pTextBox(new TextBox)
{

}

LRESULT ComboBox::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	m_pTextBox->Create(rc, m_pDispatch, this);
	return 1;
}

LRESULT ComboBox::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid(GetRect());
	m_rcDropDown = rcWid;
	m_rcDropDown.left = rcWid.right - 15;
	rcWid.right = rcWid.right - m_rcDropDown.GetWidth();
	m_pTextBox->SetRect(rcWid);
	return 1;
}

BOOL ComboBox::Initial()
{
	m_pWindow = new ComboWnd;
	m_pWindow->Initial(this);
	return TRUE;
}

void ComboBox::OnDraw( HDC hdc, const Rect& rcPaint )
{
	WfxRender::DrawComboDropDown(hdc, m_rcDropDown, GetState());
}

LRESULT ComboBox::OnGetComboSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return Size(100, 400);
}

LRESULT ComboBox::OnGetCount( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 300;
}

LRESULT ComboBox::OnGetItemText( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	static String strText;
	strText.Format(L"Item = %d", (LONG)wParam);
	return (LRESULT)strText.c_str();
}

LRESULT ComboBox::OnGetItemHeight( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 20;
}

LRESULT ComboBox::OnGetSelectedItem( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return m_nSelected;
}

LRESULT ComboBox::OnSetSelectedItem( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_nSelected = (LONG)wParam;
	m_pTextBox->SetText((WCHAR*)SendWidMessage(WUM_COMBO_GET_ITEM_TEXT, m_nSelected));
	return 1;
}
