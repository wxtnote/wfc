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
#include "wfxpane.h"
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

LRESULT SplitBar::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (IsFixed())
	{
		return 1;
	}
	if (GetBar() == SB_HORZ)
	{
		::SetCursor(::LoadCursorW(NULL, IDC_SIZENS));
	}
	else
	{
		::SetCursor(::LoadCursorW(NULL, IDC_SIZEWE));
	}

	if (!IsCaptured())
	{
		return 1;
	}

	Point pt = lParam;

	LONG nOffset = 0;
	Rect rcWid = GetRect();
	if (GetBar() == SB_VERT)
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
	
	SendParentMessage(WUM_PANEITEM_RESIZE, (WPARAM)GetID(), nOffset);
	return 1;
}

void SplitBar::SetFixed( BOOL bFixed /*= TRUE*/ )
{
	m_bFixed = bFixed;
}

BOOL SplitBar::IsFixed() const
{
	return m_bFixed;
}

void SplitBar::SetRange( LONG nMin, LONG nMax )
{
	m_nMax = nMax;
	m_nMin = nMin;
}

String SplitBar::GetToolTip() const
{
	return L"Drag to Adjust";
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Pane::Pane(int nBar/* = SB_HORZ*/)
: BarBase(nBar)
{

}

BOOL Pane::AddWidget( Widget* pWidget, float fScale /*= 1.00*/, BOOL bShow /*= TRUE*/ )
{
	PaneItem* pPI = GetLast();
	if (pPI != NULL)
	{
		if (pPI->IsWidget())
		{
			PPaneItem pPISplitBar(new PaneItem(PaneItem::PI_SPLITBAR, 
				GetBar() == SB_HORZ ? SB_VERT : SB_HORZ));
			pPISplitBar->GetSplitInfo()->m_pSplitBar->SetID(m_rgpPaneItem.size());
			m_rgpPaneItem.push_back(pPISplitBar);
		}
	}
	PPaneItem pPIWidget(new PaneItem(PaneItem::PI_WIDGET));
	pPIWidget->GetWidgetInfo()->m_pWidget = pWidget;
	pPIWidget->GetWidgetInfo()->m_fScale = fScale;
	pPIWidget->GetWidgetInfo()->m_bShow = bShow;
	m_rgpPaneItem.push_back(pPIWidget);
	return TRUE;
}

BOOL Pane::AddWidget( ULONG nInitSize, Widget* pWidget, BOOL bShow /*= TRUE*/ )
{
	return TRUE;
}

LRESULT Pane::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, 
					   BOOL& bHandled )
{
	Rect rc;
	PaneItem* pPI = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPI = GetAt(i);
		if (pPI != NULL)
		{
			if (pPI->IsWidget())
			{
				WFX_CONDITION(pPI->GetWidgetInfo()->m_pWidget != NULL);
				pPI->GetWidgetInfo()->m_pWidget->Create(rc, this);
			}
			else
			{
				WFX_CONDITION(pPI->GetSplitInfo() != NULL);
				pPI->GetSplitInfo()->m_pSplitBar->Create(rc, this);
			}
		}
	}
	return 1;
}

LRESULT Pane::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, 
					 BOOL& bHandled )
{
	//TRACE(L"Pane::OnSize");
	GetBar() == SB_HORZ? OnHorzSize(uMsg, wParam, lParam, bHandled) 
		: OnVertSize(uMsg, wParam, lParam, bHandled);
	return 1;
}

void Pane::OnVertSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc = GetRect();
	LONG nLeftHeight = rc.GetHeight();
	LONG nTop = rc.top;
	Rect rcItem = rc;
	PaneItem* pPI = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPI = GetAt(i);
		if (pPI != NULL)
		{
			rcItem.top = nTop;
			if (pPI->IsWidget())
			{
				WFX_CONDITION(pPI->GetWidgetInfo()->m_pWidget != NULL);
				if (pPI->GetWidgetInfo()->m_bShow)
				{
					rcItem.bottom = rcItem.top + 
						pPI->GetWidgetInfo()->m_fScale * rc.GetHeight();
					if (rcItem.bottom > rc.bottom)
					{
						rcItem.bottom = rc.bottom;
					}
					rcItem.bottom -= 1;
					pPI->GetWidgetInfo()->m_pWidget->SetRect(rcItem);
					nTop += rcItem.GetHeight();
				}
			}
			else
			{
				rcItem.bottom = rcItem.top + SIZE_SPLITBAR;
				WFX_CONDITION(pPI->GetSplitInfo()->GetSplitBar() != NULL);
				pPI->GetSplitInfo()->GetSplitBar()->SetRect(rcItem);
				nLeftHeight -= rcItem.GetHeight();
				nTop += rcItem.GetHeight();
			}
		}
	}
	PaneItem* pPILast = GetLast();
	if (pPILast != NULL)
	{
		WFX_CONDITION(pPILast->IsWidget());
		WFX_CONDITION(pPILast->GetWidgetInfo()->m_pWidget != NULL);
		Rect rcLast = pPILast->GetWidgetInfo()->m_pWidget->GetRect();
		rcLast.bottom = rc.bottom;
		pPILast->GetWidgetInfo()->m_pWidget->SetRect(rcLast);
	}
	AdjustSplitRangeVert();
}

void Pane::OnHorzSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc = GetRect();
	LONG nLeftWidth = rc.GetWidth();
	LONG nLeft = rc.left;
	Rect rcItem = rc;
	PaneItem* pPI = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPI = GetAt(i);
		if (pPI != NULL)
		{
			rcItem.left = nLeft;
			if (pPI->IsWidget())
			{
				WFX_CONDITION(pPI->GetWidgetInfo()->m_pWidget != NULL);
				if (pPI->GetWidgetInfo()->m_bShow)
				{
					rcItem.right = rcItem.left + 
						pPI->GetWidgetInfo()->m_fScale * rc.GetWidth();
					if (rcItem.right > rc.right)
					{
						rcItem.right = rc.right;
					}
					rcItem.right -= 1;
					pPI->GetWidgetInfo()->m_pWidget->SetRect(rcItem);
					nLeft += rcItem.GetWidth();
				}
			}
			else
			{
				rcItem.right = rcItem.left + SIZE_SPLITBAR;
				WFX_CONDITION(pPI->GetSplitInfo()->GetSplitBar() != NULL);
				pPI->GetSplitInfo()->GetSplitBar()->SetRect(rcItem);
				nLeftWidth -= rcItem.GetWidth();
				nLeft += rcItem.GetWidth();
			}
		}
	}
	PaneItem* pPILast = GetLast();
	if (pPILast != NULL)
	{
		WFX_CONDITION(pPILast->IsWidget());
		WFX_CONDITION(pPILast->GetWidgetInfo()->m_pWidget != NULL);
		Rect rcLast = pPILast->GetWidgetInfo()->m_pWidget->GetRect();
		rcLast.right = rc.right;
		pPILast->GetWidgetInfo()->m_pWidget->SetRect(rcLast);
	}
	AdjustSplitRangeHorz();
}

PaneItem* Pane::GetAt( ULONG nIndex ) const
{
	if (nIndex >= m_rgpPaneItem.size())
	{
		return NULL;
	}
	return m_rgpPaneItem[nIndex].get();
}

PaneItem* Pane::GetLast() const
{
	ULONG nIndex = m_rgpPaneItem.size() - 1;
	return GetAt(nIndex);
}

LRESULT Pane::OnResizePaneItem( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	TRACE(L"(LONG)lParam = %d", (LONG)lParam);
	PaneItem* pPISplit = GetAt((ULONG)wParam);
	WFX_CONDITION(pPISplit != NULL);
	WFX_CONDITION(pPISplit->GetSplitInfo()->GetSplitBar() != NULL);
	PaneItem* pPIPre = GetAt((ULONG)wParam - 1);
	WFX_CONDITION(pPIPre != NULL);
	PaneItem* pPINext = GetAt((ULONG)wParam + 1);
	WFX_CONDITION(pPINext != NULL);
	WFX_CONDITION(pPIPre->GetWidgetInfo()->m_pWidget != NULL);
	WFX_CONDITION(pPINext->GetWidgetInfo()->m_pWidget != NULL);
	Rect rcSplit = pPISplit->GetSplitInfo()->GetSplitBar()->GetRect();
	Rect rcPre = pPIPre->GetWidgetInfo()->m_pWidget->GetRect();
	Rect rcNext = pPINext->GetWidgetInfo()->m_pWidget->GetRect();
	if (GetBar() == SB_HORZ)
	{
		rcPre.right += (LONG)lParam;
		rcSplit.left += (LONG)lParam;
		rcSplit.right += (LONG)lParam;
		rcNext.left += (LONG)lParam;
		pPIPre->GetWidgetInfo()->m_fScale = (float)rcPre.GetWidth() / (float)GetRect().GetWidth();
		pPINext->GetWidgetInfo()->m_fScale = (float)rcNext.GetWidth() / (float)GetRect().GetWidth();
		pPIPre->GetWidgetInfo()->m_pWidget->SetRect(rcPre);
		pPISplit->GetSplitInfo()->GetSplitBar()->SetRect(rcSplit);
		pPINext->GetWidgetInfo()->m_pWidget->SetRect(rcNext);
		AdjustSplitRangeHorz();
	}
	else
	{
		rcPre.bottom += (LONG)lParam;
		rcSplit.top += (LONG)lParam;
		rcSplit.bottom += (LONG)lParam;
		rcNext.top += (LONG)lParam;
		pPIPre->GetWidgetInfo()->m_fScale = (float)rcPre.GetHeight() / (float)GetRect().GetHeight();
		pPINext->GetWidgetInfo()->m_fScale = (float)rcNext.GetHeight() / (float)GetRect().GetHeight();
		pPIPre->GetWidgetInfo()->m_pWidget->SetRect(rcPre);
		pPISplit->GetSplitInfo()->GetSplitBar()->SetRect(rcSplit);
		pPINext->GetWidgetInfo()->m_pWidget->SetRect(rcNext);
		AdjustSplitRangeVert();
	}
	InvalidWid();
	return 1;
}

void Pane::AdjustSplitRangeVert()
{
	PaneItem* pPISplit = NULL;
	PaneItem* pPIPre = NULL;
	PaneItem* pPINext = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPISplit = GetAt(i);
		WFX_CONDITION(pPISplit != NULL);
		if (pPISplit->IsSplitBar())
		{
			pPIPre = GetAt(i - 1);
			WFX_CONDITION(pPIPre != NULL);
			WFX_CONDITION(pPIPre->IsWidget());
			pPIPre->GetWidgetInfo()->m_pWidget->GetRect();
			pPINext = GetAt(i + 1);
			WFX_CONDITION(pPINext != NULL);
			WFX_CONDITION(pPINext->IsWidget());
			pPISplit->GetSplitInfo()->GetSplitBar()->SetRange(
				pPIPre->GetWidgetInfo()->m_pWidget->GetRect().top,
				pPINext->GetWidgetInfo()->m_pWidget->GetRect().bottom);
		}	
	}
}

void Pane::AdjustSplitRangeHorz()
{
	PaneItem* pPISplit = NULL;
	PaneItem* pPIPre = NULL;
	PaneItem* pPINext = NULL;
	for (ULONG i = 0; i < m_rgpPaneItem.size(); i++)
	{
		pPISplit = GetAt(i);
		WFX_CONDITION(pPISplit != NULL);
		if (pPISplit->IsSplitBar())
		{
			pPIPre = GetAt(i - 1);
			WFX_CONDITION(pPIPre != NULL);
			WFX_CONDITION(pPIPre->IsWidget());
			pPIPre->GetWidgetInfo()->m_pWidget->GetRect();
			pPINext = GetAt(i + 1);
			WFX_CONDITION(pPINext != NULL);
			WFX_CONDITION(pPINext->IsWidget());
			pPISplit->GetSplitInfo()->GetSplitBar()->SetRange(
				pPIPre->GetWidgetInfo()->m_pWidget->GetRect().left,
				pPINext->GetWidgetInfo()->m_pWidget->GetRect().right);
		}	
	}
}
