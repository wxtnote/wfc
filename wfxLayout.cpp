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
#include "wfxlayout.h"

USING_NAMESPACE_WFX;

Layout::Layout()
{

}

BOOL Layout::processMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	return TRUE;
}

void Layout::doLayout( Widget* pWid, Desc* pDesc, const Rect& rcGrid )
{

}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Grid::Grid( CSPLayoutFactory& pFactory )
: m_pLayoutFactory(pFactory)
{

}

void Grid::addItem( Widget* pWidget, CLayoutDesc& desc )
{
	m_rgWidLayout.push_back(std::make_pair(pWidget, desc));
}

ULONG Grid::getWidLayout( Wid2DescList& rgWidLayout )
{
	rgWidLayout = m_rgWidLayout;
	return rgWidLayout.size();
}

void Grid::doLayout()
{
	if (m_pLayout == NULL)
	{
		m_pLayout = m_pLayoutFactory->createObject();
	}
	Rect rcGrid = getRect();
	ULONG nItems = m_rgWidLayout.size();
	for (ULONG i = 0; i < nItems; i++)
	{
		m_pLayout->doLayout(m_rgWidLayout[i].first, 
			&(m_rgWidLayout[i].second), rcGrid);
	}
}

LRESULT Grid::onCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc;
	ULONG nItems = m_rgWidLayout.size();
	for (ULONG i = 0; i < nItems; i++)
	{
		WFX_CONDITION(m_rgWidLayout[i].first != NULL);
		m_rgWidLayout[i].first->create(rc, this);
	}
	return 1;
}

LRESULT Grid::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	doLayout();
	return 1;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
LayoutDispatch::LayoutDispatch()
{

}

void LayoutDispatch::setGrid( Grid* pGrid )
{
	m_pRoot = pGrid;
}

Grid* LayoutDispatch::getGrid() const
{
	return m_pRoot;
}

LRESULT LayoutDispatch::onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
						 BOOL& bHandled)
{
	WFX_CONDITION(m_pRoot != NULL);
	if (m_pRoot != NULL)
	{
		Rect rc;
		m_pRoot->create(rc, NULL, this);
	}
	return 1;
}

LRESULT LayoutDispatch::onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
					   BOOL& bHandled)
{
	WFX_CONDITION(m_pRoot != NULL);
	if (m_pRoot != NULL)
	{
		WFX_CONDITION(getHwnd() != NULL);
		Rect rcClient;
		::GetClientRect(getHwnd(), &rcClient);
		m_pRoot->setRect(rcClient);
	}
	return 1;
}

