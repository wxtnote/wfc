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
RadioButton::RadioButton()
{
	SetText(L"Radio");
}

void RadioButton::OnDraw( HDC hdc, const Rect& rc )
{
	WfxRender::DrawCheckBox(hdc, GetText(), GetRect(), 0, m_lOffset, m_pDispatch);
	WfxRender::DrawRadioBoxItem(hdc, GetItemRect(), GetState(), IsChecked(), m_pDispatch);
}
