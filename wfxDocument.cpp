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
#include "wfxfrmwk.h"

USING_NAMESPACE_WFX;

Document::Document()
: m_pView(NULL)
{

}

BOOL Document::processMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	return TRUE;
}

BOOL Document::open()
{
	return TRUE;
}

BOOL Document::close()
{
	return TRUE;
}

View* Document::getView() const
{
	return m_pView;
}

void Document::setView( View* pView )
{
	m_pView = pView;
}
