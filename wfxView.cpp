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

View::View()
: m_pDoc(NULL)
{

}

Document* View::getDocument() const
{
	return m_pDoc;
}

void View::setDocument( Document* pDoc )
{
	m_pDoc = pDoc;
}
