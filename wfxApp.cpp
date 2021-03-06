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

Application::Application( HINSTANCE hInstance, SPFrame pFrame /*= SPFrame()*/ )
: m_hInstance(hInstance)
, m_pDocMgr(new DocManager)
, m_pFrame(pFrame)
{

}

void Application::addDocument( CSPDocFactory& pDocFactory, CSPViewFactroy& pViewFactory )
{

}

BOOL Application::initInstance()
{
	return FALSE;
}

int Application::exitInstance()
{
	return 1;
}

int Application::run()
{
	return 1;
}
