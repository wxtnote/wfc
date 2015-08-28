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
CheckBox::CheckBox()
: Button(TRUE)
, m_lOffset(0)
{
	setText(L"CheckBox");
}

LRESULT CheckBox::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc = getRect();
	rc.left += 2;
	rc.right = rc.left + WID_CKB_SIZE;
	rc.top = rc.top + (rc.bottom - rc.top - WID_CKB_SIZE) / 2;
	rc.bottom = rc.top + WID_CKB_SIZE;
	m_rcItem = rc;
	m_lOffset = rc.right - rc.left + 2;
	return 0;
}

void CheckBox::onDraw( HDC hdc, const Rect& rcPaint )
{
	WfxRender::drawCheckBox(hdc, getText(), getRect(), 0, m_lOffset, getDispatcher());
	WfxRender::drawCheckBoxItem(hdc, getItemRect(), getState(), isChecked(), getDispatcher());
}


Gdiplus::Image* CheckBox::getItemImage() const
{
	return m_bChecked? m_pImageChecked.get() : m_pImageUnCheck.get();
}

Rect CheckBox::getItemRect() const
{
	return m_rcItem;
}