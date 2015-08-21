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
#include "wfxcmn.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
CheckBox::CheckBox()
: Button(TRUE)
, m_lOffset(0)
{
	SetText(L"CheckBox");
}

LRESULT CheckBox::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc = GetRect();
	rc.left += 2;
	rc.right = rc.left + WID_CKB_SIZE;
	rc.top = rc.top + (rc.bottom - rc.top - WID_CKB_SIZE) / 2;
	rc.bottom = rc.top + WID_CKB_SIZE;
	m_rcItem = rc;
	m_lOffset = rc.right - rc.left + 2;
	return 0;
}

void CheckBox::OnDraw( HDC hdc, const Rect& rcPaint )
{
	WfxRender::DrawCheckBox(hdc, GetText(), GetRect(), 0, m_lOffset, m_pDispatch);
	WfxRender::DrawCheckBoxItem(hdc, GetItemRect(), GetState(), IsChecked(), m_pDispatch);
}


Gdiplus::Image* CheckBox::GetItemImage() const
{
	return m_bChecked? m_pImageChecked.get() : m_pImageUnCheck.get();
}

Rect CheckBox::GetItemRect() const
{
	return m_rcItem;
}