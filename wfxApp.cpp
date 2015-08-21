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

Application::Application( HINSTANCE hInstance, PFrame pFrame /*= PFrame()*/ )
: m_hInstance(hInstance)
, m_pDocMgr(new DocManager)
, m_pFrame(pFrame)
{

}

void Application::AddDocument( CPDocFactory& pDocFactory, CPViewFactroy& pViewFactory )
{

}

BOOL Application::InitInstance()
{
	return FALSE;
}

int Application::ExitInstance()
{
	return 1;
}

int Application::Run()
{
	return 1;
}
