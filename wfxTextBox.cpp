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
HWND TextBoxWnd::createInPlaceWindow()
{
	TextBox* pTextBox = dynamic_cast<TextBox*>(m_pOwner.get());
	WFX_CONDITION(pTextBox != NULL);
	WFX_CONDITION(pTextBox->getDispatcher() != NULL);
	Rect rcParent;
	pTextBox->getParentRect(rcParent);
	Rect rcWid = pTextBox->getRect();
	rcWid.bottom = min(rcParent.bottom, rcWid.bottom);
	rcWid.right = min(rcParent.right , rcWid.right);
	Rect rc = rcWid;
	rc.deflate(1, 1);
	create(m_pOwner->getDispatcher()->getHwnd(),
		NULL, WS_CHILD, ES_AUTOHSCROLL, rc);
	setFont(WfxRender::getFontObject());
	setText(m_pOwner->getText());
	Edit_SetModify(m_hWnd, FALSE);
	SendMessageW(m_hWnd, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(2, 2));
	Edit_SetSel(m_hWnd, 0, -1);
	Edit_Enable(m_hWnd, TRUE);
	Edit_SetReadOnly(m_hWnd, FALSE);
	if (pTextBox->getMode() & WID_TBM_PW)
	{
		Edit_SetPasswordChar(m_hWnd,L'*');
	}
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	::SetFocus(m_hWnd);
	return m_hWnd;
}

LPCWSTR TextBoxWnd::getWindowClassName() const
{
	return L"TextBoxWnd";
}

LPCWSTR TextBoxWnd::getSuperClassName() const
{
	return WC_EDITW;
}

LRESULT TextBoxWnd::onEditChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE && m_pOwner != NULL)
	{
		m_pOwner->setText(getText());
	}
	else
	{
		bHandled = FALSE;
	}
	return 0;
}

LRESULT TextBoxWnd::onMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	if (m_pOwner != NULL)
	{
		m_pOwner->sendMessage(uMsg, wParam, lParam);
	}
	return 1;
}


void TextBoxWnd::onInPlaceWindowKillFocus()
{
	if (m_pOwner == NULL)
	{
		return;
	}
	m_pOwner->setText(getText());
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
TextBox::TextBox( WORD wMode /*= WID_TBM_READWRITE*/ )
: m_wMode(wMode)
, m_bEditting(FALSE)
{
	setText(L"TextBox");
}

BOOL TextBox::initial()
{
	if (isReadonly())
	{
		return FALSE;
	}
	m_bEditting = TRUE;
	m_pWindow = new TextBoxWnd;
	m_pWindow->initial(this);
	return FALSE;
}

String TextBox::getShowText() const
{
	String str;
	if (getMode() & WID_TBM_PW)
	{
		str.assign(getText().length(), L'*');
	}
	else
	{
		return getText();
	}
	return str;
}

void TextBox::onDraw(HDC hdc, const Rect& rcPaint)
{
	String str = getShowText();
	WfxRender::drawTextBox(hdc, str, getRect(), getState(), getMode(), getDispatcher());
}

String TextBox::getToolTip() const
{
	return getShowText();
}

LRESULT TextBox::onMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (!isReadonly())
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
	}
	return __super::onMouseMove(uMsg, wParam, lParam, bHandled);
}

LRESULT TextBox::onMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

void TextBox::setMode( WORD wMode, BOOL bAdd /*= FALSE*/ )
{
	if (bAdd)
	{
		m_wMode |= wMode;
	}
	else
	{
		m_wMode = wMode;
	}
}

WORD TextBox::getMode() const
{
	return m_wMode;
}

BOOL TextBox::isReadonly() const
{
	return !(WID_TBM_WRITE & m_wMode);
}

BOOL TextBox::isPassword() const
{
	return WID_TBM_PW & m_wMode;
}
