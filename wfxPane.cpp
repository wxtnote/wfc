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
SplitBar::SplitBar( int nBar /*= SB_VERT*/ )
: BarBase(nBar)
{

}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Pane::Pane(int nBar/* = SB_HORZ*/)
: BarBase(nBar)
{

}

void Pane::Split(Widget* pWiget1, Widget* pWiget2, 
				 float fInerScale, float fOuterScale )
{
	int nBar = GetBar() == SB_HORZ ? SB_VERT : SB_HORZ;
	PSplitInfo pSplitInfo(new SplitInfo(nBar));
	pSplitInfo->m_pWidget1 = pWiget1;
	pSplitInfo->m_pWidget2 = pWiget2;
	pSplitInfo->m_fInerScale = fInerScale;
	pSplitInfo->m_fOuterScale = fOuterScale;
	m_rgpSplitInfo.push_back(pSplitInfo);
	m_rgpItem.push_back(pWiget1);
	m_rgpItem.push_back(pWiget2);
}

BOOL Pane::InSplitBar( const Point& pt ) const
{
	for (SplitList::const_iterator it = m_rgpSplitInfo.begin();
		it != m_rgpSplitInfo.end(); ++it)
	{
		WFX_CONDITION((*it)->m_pSplitBar != NULL);
		if ((*it)->m_pSplitBar->GetRect().PtInRect(pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

SplitInfo* Pane::GetSplitInPt( const Point& pt ) const
{
	for (SplitList::const_iterator it = m_rgpSplitInfo.begin();
		it != m_rgpSplitInfo.end(); ++it)
	{
		WFX_CONDITION((*it) != NULL);
		WFX_CONDITION((*it)->m_pSplitBar != NULL);
		if ((*it)->m_pSplitBar->GetRect().PtInRect(pt))
		{
			return (*it).get();
		}
	}
	return NULL;
}

void Pane::AddWidget( Widget* pWidget )
{
	std::vector<Widget*>::iterator it = std::find(m_rgpItem.begin(), m_rgpItem.end(), pWidget);
	if (it == m_rgpItem.end())
	{
		m_rgpItem.push_back(pWidget);
	}
}

LRESULT Pane::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, 
					   BOOL& bHandled )
{
	Rect rc;
	for (std::vector<Widget*>::iterator it = m_rgpItem.begin();
		it != m_rgpItem.end(); ++it)
	{
		(*it)->Create(rc, m_pDispatch, this);
	}
	return 1;
}

LRESULT Pane::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, 
						  BOOL& bHandled )
{
	SplitInfo* pSplitInfo = GetSplitInPt(lParam);
	if (pSplitInfo == NULL)
	{
		return 1;
	}
	return 1;
}

LRESULT Pane::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, 
					 BOOL& bHandled )
{
	GetBar() == SB_HORZ? OnHorzSize(uMsg, wParam, lParam, bHandled) 
		: OnVertSize(uMsg, wParam, lParam, bHandled);
	return 1;
}

void Pane::OnVertSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid = GetRect();
	LONG nStart = rcWid.top;
	LONG nTotalSize = 0;
	for (SplitList::const_iterator it = m_rgpSplitInfo.begin();
		it != m_rgpSplitInfo.end(); ++it)
	{
		WFX_CONDITION((*it) != NULL);
		WFX_CONDITION((*it)->m_pSplitBar != NULL);
		nTotalSize = (*it)->m_fOuterScale * rcWid.GetHeight();
		AdjustSplitSizeVert(nStart, nTotalSize, rcWid, (*it).get());
		nStart += nTotalSize;
	}
}

void Pane::OnHorzSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid = GetRect();
	LONG nStart = rcWid.left;
	LONG nTotalSize = 0;
	for (SplitList::const_iterator it = m_rgpSplitInfo.begin();
		it != m_rgpSplitInfo.end(); ++it)
	{
		WFX_CONDITION((*it) != NULL);
		WFX_CONDITION((*it)->m_pSplitBar != NULL);
		nTotalSize = (*it)->m_fOuterScale * rcWid.GetHeight();
		AdjustSplitSizeHorz(nStart, nTotalSize, rcWid, (*it).get());
		nStart += nTotalSize;
	}
}


void Pane::AdjustSplitSizeVert( LONG nStart, LONG nTotal, 
							   const Rect& rcWid, const SplitInfo* pSplitInfo)
{
	LONG nTopSize = pSplitInfo->m_fInerScale * nTotal;
	LONG nBottomSize = (1 - pSplitInfo->m_fInerScale) * nTotal;
	LONG nMidSize = 10;
	nTopSize > nBottomSize? nTopSize -= nMidSize : nBottomSize -= nMidSize;
	Rect rc = rcWid;
	rc.top = nStart;
	rc.bottom = rc.top + nTopSize;
	pSplitInfo->m_pWidget1->SetRect(rc);
	rc.top = rc.bottom;
	rc.bottom = rc.top + nMidSize;
	pSplitInfo->GetSplitBar()->SetRect(rc);
	rc.top = rc.bottom;
	rc.bottom = rc.top + nBottomSize;
	rc.bottom = rcWid.bottom;
	pSplitInfo->m_pWidget2->SetRect(rc);
}

void Pane::AdjustSplitSizeHorz( LONG nStart, LONG nTotal, 
							   const Rect& rcWid, const SplitInfo* pSplitInfo )
{
	LONG nLeftSize = pSplitInfo->m_fInerScale * nTotal;
	LONG nRightSize = (1 - pSplitInfo->m_fInerScale) * nTotal;
	LONG nMidSize = 10;
	nLeftSize > nRightSize? nLeftSize -= nMidSize : nRightSize -= nMidSize;
	Rect rc = rcWid;
	rc.left = nStart;
	rc.right = rc.left + nLeftSize;
	pSplitInfo->m_pWidget1->SetRect(rc);
	rc.left = rc.right;
	rc.right = rc.left + nMidSize;
	pSplitInfo->GetSplitBar()->SetRect(rc);
	rc.left = rc.right;
	rc.right = rc.left + nRightSize;
	rc.right = rcWid.right;
	pSplitInfo->m_pWidget2->SetRect(rc);
}

void Pane::OnDraw( HDC hdc, const Rect& rc )
{
	for (SplitList::const_iterator it = m_rgpSplitInfo.begin();
		it != m_rgpSplitInfo.end(); ++it)
	{
		WFX_CONDITION((*it) != NULL);
		WFX_CONDITION((*it)->m_pSplitBar != NULL);
		(*it)->m_pSplitBar->OnDraw(hdc, rc);
	}
}
