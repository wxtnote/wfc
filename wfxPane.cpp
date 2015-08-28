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
SplitBar::SplitBar( int nBar /*= SB_VERT*/, BOOL bFixed /*= FALSE*/ )
: BarBase(nBar)
, m_bFixed(bFixed)
, m_nMax(0)
, m_nMin(0)
{

}

LRESULT SplitBar::onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (isFixed())
	{
		return 1;
	}
	if (getBar() == SB_HORZ)
	{
		::SetCursor(::LoadCursorW(NULL, IDC_SIZENS));
	}
	else
	{
		::SetCursor(::LoadCursorW(NULL, IDC_SIZEWE));
	}

	if (!isCaptured())
	{
		return 1;
	}

	Point pt = lParam;

	LONG nOffset = 0;
	Rect rcWid = getRect();
	if (getBar() == SB_VERT)
	{
		if (pt.x > m_nMax || pt.x < m_nMin)
		{
			return 1;
		}
		nOffset = pt.x - rcWid.left;
	}
	else
	{
		if (pt.y > m_nMax || pt.y < m_nMin)
		{
			return 1;
		}
		nOffset = pt.y - rcWid.top;
	}
	
	sendParentMessage(WUM_PANEITEM_RESIZE, (WPARAM)getID(), nOffset);
	return 1;
}

void SplitBar::setFixed( BOOL bFixed /*= TRUE*/ )
{
	m_bFixed = bFixed;
}

BOOL SplitBar::isFixed() const
{
	return m_bFixed;
}

void SplitBar::setRange( LONG nMin, LONG nMax )
{
	m_nMax = nMax;
	m_nMin = nMin;
}

String SplitBar::getToolTip() const
{
	return L"Drag to Adjust";
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Pane::Pane(int nBar/* = SB_HORZ*/)
: BarBase(nBar)
{

}

BOOL Pane::addItem( Widget* pWidget, float fScale /*= 1.00*/, BOOL bShow /*= TRUE*/ )
{
	PaneItem* pPI = getLast();
	if (pPI != NULL)
	{
		if (pPI->isWidget())
		{
			SPPaneItem pPISplitBar(new PaneItem(PaneItem::PI_SPLITBAR, 
				getBar() == SB_HORZ ? SB_VERT : SB_HORZ));
			pPISplitBar->getSplitInfo()->m_pSplitBar->setID(m_rgpPaneItem.size());
			m_rgpPaneItem.push_back(pPISplitBar);
		}
	}
	SPPaneItem pPIWidget(new PaneItem(PaneItem::PI_WIDGET));
	pPIWidget->getWidgetInfo()->m_pWidget = pWidget;
	pPIWidget->getWidgetInfo()->m_fScale = fScale;
	pPIWidget->getWidgetInfo()->m_bShow = bShow;
	m_rgpPaneItem.push_back(pPIWidget);
	addItem(pWidget);
	return TRUE;
}

BOOL Pane::addItem( ULONG nInitSize, Widget* pWidget, BOOL bShow /*= TRUE*/ )
{
	return TRUE;
}

LRESULT Pane::onCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, 
					   BOOL& bHandled )
{
	Rect rc;
	PaneItem* pPI = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPI = getAt(i);
		if (pPI != NULL)
		{
			if (pPI->isWidget())
			{
				WFX_CONDITION(pPI->getWidgetInfo()->m_pWidget != NULL);
				pPI->getWidgetInfo()->m_pWidget->create(rc, this);
			}
			else
			{
				WFX_CONDITION(pPI->getSplitInfo() != NULL);
				pPI->getSplitInfo()->m_pSplitBar->create(rc, this);
			}
		}
	}
	return 1;
}

LRESULT Pane::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, 
					 BOOL& bHandled )
{
	//TRACE(L"Pane::onSize");
	getBar() == SB_HORZ? onHorzSize(uMsg, wParam, lParam, bHandled) 
		: onVertSize(uMsg, wParam, lParam, bHandled);
	return 1;
}

void Pane::onVertSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc = getRect();
	LONG nLeftHeight = rc.getHeight();
	LONG nTop = rc.top;
	Rect rcItem = rc;
	PaneItem* pPI = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPI = getAt(i);
		if (pPI != NULL)
		{
			rcItem.top = nTop;
			if (pPI->isWidget())
			{
				WFX_CONDITION(pPI->getWidgetInfo()->m_pWidget != NULL);
				if (pPI->getWidgetInfo()->m_bShow)
				{
					rcItem.bottom = rcItem.top + 
						pPI->getWidgetInfo()->m_fScale * rc.getHeight();
					if (rcItem.bottom > rc.bottom)
					{
						rcItem.bottom = rc.bottom;
					}
					rcItem.bottom -= 1;
					pPI->getWidgetInfo()->m_pWidget->setRect(rcItem);
					nTop += rcItem.getHeight();
				}
			}
			else
			{
				rcItem.bottom = rcItem.top + SIZE_SPLITBAR;
				WFX_CONDITION(pPI->getSplitInfo()->getSplitBar() != NULL);
				pPI->getSplitInfo()->getSplitBar()->setRect(rcItem);
				nLeftHeight -= rcItem.getHeight();
				nTop += rcItem.getHeight();
			}
		}
	}
	PaneItem* pPILast = getLast();
	if (pPILast != NULL)
	{
		WFX_CONDITION(pPILast->isWidget());
		WFX_CONDITION(pPILast->getWidgetInfo()->m_pWidget != NULL);
		Rect rcLast = pPILast->getWidgetInfo()->m_pWidget->getRect();
		rcLast.bottom = rc.bottom;
		pPILast->getWidgetInfo()->m_pWidget->setRect(rcLast);
	}
	adjustSplitRangeVert();
}

void Pane::onHorzSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc = getRect();
	LONG nLeftWidth = rc.getWidth();
	LONG nLeft = rc.left;
	Rect rcItem = rc;
	PaneItem* pPI = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPI = getAt(i);
		if (pPI != NULL)
		{
			rcItem.left = nLeft;
			if (pPI->isWidget())
			{
				WFX_CONDITION(pPI->getWidgetInfo()->m_pWidget != NULL);
				if (pPI->getWidgetInfo()->m_bShow)
				{
					rcItem.right = rcItem.left + 
						pPI->getWidgetInfo()->m_fScale * rc.getWidth();
					if (rcItem.right > rc.right)
					{
						rcItem.right = rc.right;
					}
					rcItem.right -= 1;
					pPI->getWidgetInfo()->m_pWidget->setRect(rcItem);
					nLeft += rcItem.getWidth();
				}
			}
			else
			{
				rcItem.right = rcItem.left + SIZE_SPLITBAR;
				WFX_CONDITION(pPI->getSplitInfo()->getSplitBar() != NULL);
				pPI->getSplitInfo()->getSplitBar()->setRect(rcItem);
				nLeftWidth -= rcItem.getWidth();
				nLeft += rcItem.getWidth();
			}
		}
	}
	PaneItem* pPILast = getLast();
	if (pPILast != NULL)
	{
		WFX_CONDITION(pPILast->isWidget());
		WFX_CONDITION(pPILast->getWidgetInfo()->m_pWidget != NULL);
		Rect rcLast = pPILast->getWidgetInfo()->m_pWidget->getRect();
		rcLast.right = rc.right;
		pPILast->getWidgetInfo()->m_pWidget->setRect(rcLast);
	}
	adjustSplitRangeHorz();
}

PaneItem* Pane::getAt( ULONG nIndex ) const
{
	if (nIndex >= m_rgpPaneItem.size())
	{
		return NULL;
	}
	return m_rgpPaneItem[nIndex].get();
}

PaneItem* Pane::getLast() const
{
	ULONG nIndex = m_rgpPaneItem.size() - 1;
	return getAt(nIndex);
}

LRESULT Pane::onResizePaneItem( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	TRACE(L"(LONG)lParam = %d", (LONG)lParam);
	PaneItem* pPISplit = getAt((ULONG)wParam);
	WFX_CONDITION(pPISplit != NULL);
	WFX_CONDITION(pPISplit->getSplitInfo()->getSplitBar() != NULL);
	PaneItem* pPIPre = getAt((ULONG)wParam - 1);
	WFX_CONDITION(pPIPre != NULL);
	PaneItem* pPINext = getAt((ULONG)wParam + 1);
	WFX_CONDITION(pPINext != NULL);
	WFX_CONDITION(pPIPre->getWidgetInfo()->m_pWidget != NULL);
	WFX_CONDITION(pPINext->getWidgetInfo()->m_pWidget != NULL);
	Rect rcSplit = pPISplit->getSplitInfo()->getSplitBar()->getRect();
	Rect rcPre = pPIPre->getWidgetInfo()->m_pWidget->getRect();
	Rect rcNext = pPINext->getWidgetInfo()->m_pWidget->getRect();
	if (getBar() == SB_HORZ)
	{
		rcPre.right += (LONG)lParam;
		rcSplit.left += (LONG)lParam;
		rcSplit.right += (LONG)lParam;
		rcNext.left += (LONG)lParam;
		pPIPre->getWidgetInfo()->m_fScale = (float)rcPre.getWidth() / (float)getRect().getWidth();
		pPINext->getWidgetInfo()->m_fScale = (float)rcNext.getWidth() / (float)getRect().getWidth();
		pPIPre->getWidgetInfo()->m_pWidget->setRect(rcPre);
		pPISplit->getSplitInfo()->getSplitBar()->setRect(rcSplit);
		pPINext->getWidgetInfo()->m_pWidget->setRect(rcNext);
		adjustSplitRangeHorz();
	}
	else
	{
		rcPre.bottom += (LONG)lParam;
		rcSplit.top += (LONG)lParam;
		rcSplit.bottom += (LONG)lParam;
		rcNext.top += (LONG)lParam;
		pPIPre->getWidgetInfo()->m_fScale = (float)rcPre.getHeight() / (float)getRect().getHeight();
		pPINext->getWidgetInfo()->m_fScale = (float)rcNext.getHeight() / (float)getRect().getHeight();
		pPIPre->getWidgetInfo()->m_pWidget->setRect(rcPre);
		pPISplit->getSplitInfo()->getSplitBar()->setRect(rcSplit);
		pPINext->getWidgetInfo()->m_pWidget->setRect(rcNext);
		adjustSplitRangeVert();
	}
	invalidWid();
	return 1;
}

void Pane::adjustSplitRangeVert()
{
	PaneItem* pPISplit = NULL;
	PaneItem* pPIPre = NULL;
	PaneItem* pPINext = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPISplit = getAt(i);
		WFX_CONDITION(pPISplit != NULL);
		if (pPISplit->isSplitBar())
		{
			pPIPre = getAt(i - 1);
			WFX_CONDITION(pPIPre != NULL);
			WFX_CONDITION(pPIPre->isWidget());
			pPIPre->getWidgetInfo()->m_pWidget->getRect();
			pPINext = getAt(i + 1);
			WFX_CONDITION(pPINext != NULL);
			WFX_CONDITION(pPINext->isWidget());
			pPISplit->getSplitInfo()->getSplitBar()->setRange(
				pPIPre->getWidgetInfo()->m_pWidget->getRect().top,
				pPINext->getWidgetInfo()->m_pWidget->getRect().bottom);
		}	
	}
}

void Pane::adjustSplitRangeHorz()
{
	PaneItem* pPISplit = NULL;
	PaneItem* pPIPre = NULL;
	PaneItem* pPINext = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPISplit = getAt(i);
		WFX_CONDITION(pPISplit != NULL);
		if (pPISplit->isSplitBar())
		{
			pPIPre = getAt(i - 1);
			WFX_CONDITION(pPIPre != NULL);
			WFX_CONDITION(pPIPre->isWidget());
			pPIPre->getWidgetInfo()->m_pWidget->getRect();
			pPINext = getAt(i + 1);
			WFX_CONDITION(pPINext != NULL);
			WFX_CONDITION(pPINext->isWidget());
			pPISplit->getSplitInfo()->getSplitBar()->setRange(
				pPIPre->getWidgetInfo()->m_pWidget->getRect().left,
				pPINext->getWidgetInfo()->m_pWidget->getRect().right);
		}	
	}
}
