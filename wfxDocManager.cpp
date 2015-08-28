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

DocManager::DocManager()
: m_nCurrent(-1)
{

}

BOOL DocManager::processMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	return TRUE;
}

ULONG DocManager::addDoc( CSPDocFactory& pDocFactory, CSPViewFactroy& pViewFactory )
{
	m_rgpDocToView.push_back(std::make_pair(pDocFactory, pViewFactory));
	return m_rgpDocToView.size();
}

Document* DocManager::next()
{
	return NULL;
}

Document* DocManager::pre()
{
	return NULL;
}

Document* DocManager::getAt( int i )
{
	return NULL;
}

BOOL DocManager::open( int i )
{
	return FALSE;
}

BOOL DocManager::close( int i )
{
	return FALSE;
}
