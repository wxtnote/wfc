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
RadioButton::RadioButton()
{
	setText(L"Radio");
}

void RadioButton::onDraw( HDC hdc, const Rect& rc )
{
	WfxRender::drawCheckBox(hdc, getText(), getRect(), 0, m_lOffset, getDispatcher());
	WfxRender::drawRadioBoxItem(hdc, getItemRect(), getState(), isChecked(), getDispatcher());
}
