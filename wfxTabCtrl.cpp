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
#include "wfxContainer.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
TabItem::TabItem( const ULONG nTabID, const String& strLabel, 
				 Widget* pWid, Widget* pParent /*= NULL*/ )
: UnitBase(pParent)
, m_nTabID(nTabID)
, m_bVisible(TRUE)
, m_pWid(pWid)
{
	setText(strLabel);
}

void TabItem::Draw( HDC hdc, const Rect& rcPaint, BOOL bSelected /*= FALSE*/ )
{
	COLORREF clr = bSelected? WBTN_BKGND_MOUSE : WBTN_BKGND_STATIC;
	WfxRender::drawSolidRect(hdc, getRect(), clr);
	WfxRender::drawText(hdc, getRect(), getText(), 
		WID_TEXT_STATIC, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	WfxRender::drawFrame(hdc ,getRect(), WBTN_BKGND_MOUSE);
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
TabCtrl::TabCtrl()
: m_nLocation(LOCACTION_TOP)
, m_nTabHeight(20)
, m_nNextTabID(0)
, m_nCurrentTab(0)
, m_nTabWidth(80)
{

}

void TabCtrl::setTabHeight( ULONG nTabHeight )
{
	m_nTabHeight = nTabHeight;
}

ULONG TabCtrl::getTabHeight() const
{
	return m_nTabHeight;
}

Rect TabCtrl::getTabRect() const
{
	Rect rcWid = getRect();
	return rcWid;
}

ULONG TabCtrl::addTab( Widget* pTabWid, const String& strTabLabel )
{
	return insertTab(pTabWid, strTabLabel, m_rgpTabs.size());
}

ULONG TabCtrl::insertTab( Widget* pTabWid, 
						 const String& strTabLabel, ULONG nInsertAt )
{
	WFX_CONDITION(pTabWid != NULL);
	m_rgpTabs.insert(m_rgpTabs.begin() + nInsertAt, 
		PTabItem(new TabItem(m_nNextTabID, strTabLabel, pTabWid, this)));
	m_nCurrentTab = m_nNextTabID;
	m_nNextTabID++;
	addItem(pTabWid);
	return m_rgpTabs.size();
}

ULONG TabCtrl::removeTab( ULONG nTab, BOOL brecalcLayout /*= TRUE*/ )
{
	WFX_CONDITION(nTab < m_rgpTabs.size());
	if (nTab < m_rgpTabs.size())
	{
		removeItem(m_rgpTabs[nTab]->m_pWid);
		m_rgpTabs.erase(m_rgpTabs.begin() + nTab);
	}
	return m_rgpTabs.size();
}

void TabCtrl::removeAllTabs()
{
	m_rgpTabs.clear();
	removeAll();
	m_nNextTabID = 0;
}

ULONG TabCtrl::getVisibleTabCount() const
{
	Rect rcWid = getRect();
	return rcWid.getWidth();
}

BOOL TabCtrl::showTab( ULONG nTab, BOOL bShow /*= TRUE*/, 
					  BOOL brecalcLayout /*= TRUE*/, BOOL bActivate /*= TRUE*/ )
{
	WFX_CONDITION(nTab < m_rgpTabs.size());
	if (isValidTab(m_nCurrentTab))
	{
		Rect rcOldTab;
		m_rgpTabs[m_nCurrentTab]->m_pWid->setRect(rcOldTab);
		m_rgpTabs[m_nCurrentTab]->m_pWid->showWid(SW_HIDE);
	}
	m_nCurrentTab = nTab;
	m_rgpTabs[m_nCurrentTab]->m_pWid->showWid(SW_SHOW);
	m_rgpTabs[m_nCurrentTab]->m_pWid->setRect(m_rcActivate);
	recalcLayout();
	invalidWid();
	m_rgpTabs[m_nCurrentTab]->m_pWid->invalidWid();
	return FALSE;
}

ULONG TabCtrl::getTabCount() const
{
	return m_rgpTabs.size();
}

String TabCtrl::getTabLabel( ULONG nTab ) const
{
	WFX_CONDITION(nTab < m_rgpTabs.size());
	if (nTab < m_rgpTabs.size())
	{
		return m_rgpTabs[nTab]->getText();
	}
	return L"";
}

void TabCtrl::setTabLabel( ULONG nTab, const String& strTabLabel )
{
	WFX_CONDITION(nTab < m_rgpTabs.size());
	if (nTab < m_rgpTabs.size())
	{
		m_rgpTabs[nTab]->setText(strTabLabel);
	}
}

void TabCtrl::recalcLayout()
{
	sendMessage(WM_SIZE);
}

LRESULT TabCtrl::onCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ULONG nItems = m_rgpTabs.size();
	Rect rcItem;
	for (ULONG nTab = 0; nTab < nItems; nTab++)
	{
		m_rgpTabs[nTab]->m_pWid->create(rcItem, this);
	}
	return 1;
}

LRESULT TabCtrl::onLButtonClik( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	ULONG nTabCliked = 0;
	if (m_rcTabArea.PtInRect(pt))
	{
		nTabCliked = getTabPt(pt);
	}
	if (isValidTab(nTabCliked))
	{
		showTab(nTabCliked);
	}
	return 1;
}

LRESULT TabCtrl::onMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

LRESULT TabCtrl::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid = getRect();
	Rect rcTabItem = rcWid;
	rcTabItem.bottom = rcWid.top + m_nTabHeight;
	m_rcTabArea = rcTabItem;
	ULONG nItems = m_rgpTabs.size();
	for (ULONG nTab = 0; nTab < nItems; nTab++)
	{
		rcTabItem.right = rcTabItem.left + m_nTabWidth;
		m_rgpTabs[nTab]->setRect(rcTabItem);
		rcTabItem.left += m_nTabWidth;
	}
	m_rcActivate = rcWid;
	m_rcActivate.top = rcWid.top + m_nTabHeight;
	WFX_CONDITION(m_nCurrentTab < m_rgpTabs.size());
	if (m_nCurrentTab < m_rgpTabs.size())
	{
		WFX_CONDITION(m_rgpTabs[m_nCurrentTab]->m_pWid != NULL);
		m_rgpTabs[m_nCurrentTab]->m_pWid->setRect(m_rcActivate);
		m_rgpTabs[m_nCurrentTab]->m_pWid->showWid(SW_SHOW);
	}
	return 1;
}

void TabCtrl::onDraw( HDC hdc, const Rect& rcPaint )
{
	ULONG nItems = m_rgpTabs.size();
	for (ULONG nTab = 0; nTab < nItems; nTab++)
	{
		m_rgpTabs[nTab]->Draw(hdc, rcPaint, nTab == m_nCurrentTab);
	}
}

ULONG TabCtrl::getTabPt( const Point& pt ) const
{
	ULONG nItems = m_rgpTabs.size();
	for (ULONG nTab = 0; nTab < nItems; nTab++)
	{
		if (m_rgpTabs[nTab]->getRect().PtInRect(pt))
		{
			return nTab;
		}
	}
	return 0;
}

BOOL TabCtrl::isValidTab( ULONG nTab ) const
{
	return nTab < m_rgpTabs.size();
}
