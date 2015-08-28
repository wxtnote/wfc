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

Button::Button(BOOL bCheckable /*= FALSE*/)
: m_bChecked(FALSE)
, m_bCheckable(bCheckable)
{
	setText(L"Button");
}

void Button::onDraw( HDC hdc, const Rect& rcPaint )
{
	Gdiplus::Image* pImage = getImageFromState();
	if (pImage == NULL)
	{
		WfxRender::drawButtton(hdc, getText(), getRect(), getState(), getDispatcher());
	}
}

void Button::check( BOOL bCheck /*= TRUE*/ )
{
	m_bChecked = bCheck;
}

BOOL Button::isChecked() const
{
	return m_bChecked;
}

LRESULT Button::onLButtonClik( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bChecked = !m_bChecked;
	sendParentMessage(WM_COMMAND, MAKELONG(getID(), BN_CLICKED), (LPARAM)getHwid());
	invalidWid();
	return 1;
}

LRESULT Button::onStateChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_bCheckable && m_bChecked)
	{
		m_clrText = (WID_TEXT_CHECKED);
		m_clrBkgnd = (WID_BKGND_CHECKED);
		m_clrFrame = (WID_FRAME_CHECKED);
		invalidWid();
		return 0;
	}
	invalidWid();
	return 1;
}