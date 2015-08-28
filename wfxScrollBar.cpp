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

#include "stdafx.h"
#include "wfxwidget.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ScrollBar::ScrollBar( int nBar /*= SB_HORZ*/ )
: SliderBar(nBar)
, m_pScrollInfo(new SCROLLINFO)
, m_nArrorSize(0)
{
	memset(m_pScrollInfo.get(), 0, sizeof(SCROLLINFO));
	m_pScrollInfo->cbSize = sizeof(SCROLLINFO);
	m_pScrollInfo->nMax = 10000;
	m_pScrollInfo->nMin = 0;
	m_ptLButtonDown.x = -1;
	m_ptLButtonDown.y = -1;
}

ScrollBar::~ScrollBar()
{
}

void ScrollBar::getScrollInfo( SCROLLINFO* pScrollInfo ) const
{
	WFX_CONDITION(pScrollInfo != NULL);
	if (pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		memcpy(pScrollInfo, m_pScrollInfo.get(), sizeof(SCROLLINFO));
	}
}

void ScrollBar::setScrollInfo( const SCROLLINFO* pScrollInfo )
{
	WFX_CONDITION(pScrollInfo != NULL);
	if (pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		memcpy(m_pScrollInfo.get(), pScrollInfo, sizeof(SCROLLINFO));
	}
}

LONG ScrollBar::calcHorzThumbSize(const Rect& rcWid)
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	LONG nThumbSize = 0;
	if (m_pScrollInfo->nMax - m_pScrollInfo->nMin > 0)
	{
		nThumbSize = m_rcThumbHolder.getWidth() * (float)rcWid.getWidth() / (float)(m_pScrollInfo->nMax - m_pScrollInfo->nMin);
	}
	if (nThumbSize < MIN_SCROLLBAR_THUMB)
	{
		nThumbSize = MIN_SCROLLBAR_THUMB;
	}
	return nThumbSize;
}

LONG ScrollBar::calcVertThumbSize(const Rect& rcWid)
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	LONG nThumbSize = 0;
	if (m_pScrollInfo->nMax - m_pScrollInfo->nMin > 0)
	{
		nThumbSize = m_rcThumbHolder.getHeight() * (float)rcWid.getHeight() / (float)(m_pScrollInfo->nMax - m_pScrollInfo->nMin);
	}
	if (nThumbSize < MIN_SCROLLBAR_THUMB)
	{
		nThumbSize = MIN_SCROLLBAR_THUMB;
	}
	return nThumbSize;
}

void ScrollBar::setRange( LONG nMin, LONG nMax )
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	if (m_pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		m_pScrollInfo->nMin = nMin;
		m_pScrollInfo->nMax = nMax;
	}
}

LONG ScrollBar::getRange() const
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	if (m_pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		return m_pScrollInfo->nMax - m_pScrollInfo->nMin;
	}
	return 0;
}

LONG ScrollBar::getMax() const
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	if (m_pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		return m_pScrollInfo->nMax;
	}
	return 0;
}

LONG ScrollBar::getMin() const
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	if (m_pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		return m_pScrollInfo->nMin;
	}
	return 0;
}

LRESULT ScrollBar::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid(getRect());
	m_rcArrow1 = rcWid;
	m_rcArrow2 = rcWid;
	m_rcThumbHolder = rcWid;
	m_rcThumb.empty();
	LONG nThumbSize = 0;
	if (SB_VERT == m_nBar)
	{
		m_rcArrow1.bottom = rcWid.top + m_nArrorSize;
		m_rcArrow2.top = rcWid.bottom - m_nArrorSize;
		m_rcThumbHolder.top = m_rcArrow1.bottom;
		m_rcThumbHolder.bottom = m_rcArrow2.top;
		nThumbSize = calcVertThumbSize(rcWid);
		setThumbSize(nThumbSize);
		LONG y = calcVertThumbPos();
		y -= WFX_ROUND((float)nThumbSize / 2);
		setVertThumbRect(nThumbSize, y);
	}
	else
	{
		m_rcArrow1.right = rcWid.left + m_nArrorSize;
		m_rcArrow2.left = rcWid.right - m_nArrorSize;
		m_rcThumbHolder.left = m_rcArrow1.right;
		m_rcThumbHolder.right = m_rcArrow2.left;
		nThumbSize = calcHorzThumbSize(rcWid);
		setThumbSize(nThumbSize);
		LONG x = calcHorzThumbPos();
		x -= WFX_ROUND((float)nThumbSize / 2);
		setHorzThumbRect(nThumbSize, x);
	}

	return 1;
}

void ScrollBar::setPos( LONG nPos )
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	if (m_pScrollInfo->nPos != nPos)
	{
		m_pScrollInfo->nPos = nPos;
		if (m_pScrollInfo->nPos < m_pScrollInfo->nMin)
			m_pScrollInfo->nPos = m_pScrollInfo->nMin;
		if (m_pScrollInfo->nPos > m_pScrollInfo->nMax)
			m_pScrollInfo->nPos = m_pScrollInfo->nMax;
		sendParentMessage(WUM_SB_OFFSET, m_nBar);
		if (m_nBar == SB_VERT) 
		{
			sendParentMessage(WM_VSCROLL);
		}
		else
		{
			sendParentMessage(WM_HSCROLL);
		}
	}
}

LONG ScrollBar::getPos() const
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	return m_pScrollInfo->nPos;
}


void ScrollBar::reset()
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	m_pScrollInfo->nPos = m_pScrollInfo->nMin;
}

void ScrollBar::onDraw( HDC hdc, const Rect& rcPaint )
{
	WfxRender::drawSolidRect(hdc, getRect(), RGB(0, 0, 0));
	WfxRender::drawArror(hdc, m_rcArrow1, 0, 0);
	WfxRender::drawArror(hdc, m_rcArrow2, 0, 0);
	WfxRender::drawThumb(hdc, m_rcThumb, 0);
}
