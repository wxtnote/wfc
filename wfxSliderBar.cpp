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
SliderBar::SliderBar( int nBar /*= SB_HORZ*/ )
: ProcessBar(nBar)
, m_nThumbSize(13)
{
}

LRESULT SliderBar::onLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_ptLButtonDown = lParam;
	if (m_rcThumb.PtInRect(m_ptLButtonDown))
	{
		m_bInThumb = TRUE;
		if (m_nBar == SB_VERT)
		{
			m_ptLButtonDown.y -= m_rcThumb.top;
		}
		else
		{
			m_ptLButtonDown.x -= m_rcThumb.left;
		}
	}
	else
	{
		m_bInThumb = FALSE;
		if (m_nBar == SB_VERT)
		{
			m_bDirection = m_ptLButtonDown.y - 
				(m_rcThumb.top + WFX_ROUND((float)m_rcThumb.getHeight() / 2)) > 0;
		}
		else
		{
			m_bDirection = m_ptLButtonDown.x - 
				(m_rcThumb.left + WFX_ROUND((float)m_rcThumb.getWidth() / 2)) > 0;
		}
		setTimer(1, 50, NULL);
	}
	return 1;
}

LRESULT SliderBar::onLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	killTimer(1);
	m_bInThumb = FALSE;
	return 1;
}

LRESULT SliderBar::onMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_ptMouseMove = lParam;
	if (!isCaptured() || !m_bInThumb)
	{
		return __super::onMouseMove(uMsg, wParam, lParam, bHandled);
	}
	Point pt(lParam);
	LONG nThumbSize = 0;
	if (SB_VERT == m_nBar)
	{
		if (pt.y < m_rcThumbHolder.top || pt.y > m_rcThumbHolder.bottom)
			return 1;
		nThumbSize = m_rcThumb.getHeight();
		LONG nTop = pt.y - m_ptLButtonDown.y;
		if (nTop < m_rcThumbHolder.top)
		{
			nTop = m_rcThumbHolder.top;
		}
		if (nTop > m_rcThumbHolder.bottom)
		{
			nTop = m_rcThumbHolder.bottom - nThumbSize;
		}
		if (nTop == m_rcThumb.top)
		{
			return 1;
		}
		setVertThumbRect(nThumbSize, nTop);
		calcVertPos();
	}
	else
	{
		if (pt.x < m_rcThumbHolder.left || pt.x > m_rcThumbHolder.right)
			return 1;
		nThumbSize = m_rcThumb.getWidth();
		LONG nLeft = pt.x - m_ptLButtonDown.x;
		if (nLeft < m_rcThumbHolder.left)
		{
			nLeft = m_rcThumbHolder.left;
		}
		if (nLeft > m_rcThumbHolder.right)
		{
			nLeft = m_rcThumbHolder.right - nThumbSize;
		}
		if (nLeft == m_rcThumb.left)
		{
			return 1;
		}
		setHorzThumbRect(nThumbSize, nLeft);
		calcHorzPos();
	}
	invalidWid();
	return 1;
}

LRESULT SliderBar::onTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_rcThumb.PtInRect(m_ptMouseMove))
	{
		killTimer(1);
		return 1;
	}
	if (m_nBar == SB_VERT)
	{	
		LONG nThumbSize = m_rcThumb.getHeight();
		if (m_bDirection)
		{
			m_rcThumb.top += getStep();
		}
		else
		{
			m_rcThumb.top -= getStep();
		}
		setVertThumbRect(nThumbSize, m_rcThumb.top);
		calcVertPos();
		invalidWid();
	}
	else
	{
		LONG nThumbSize = m_rcThumb.getWidth();
		if (m_bDirection)
		{
			m_rcThumb.left += getStep();
		}
		else
		{
			m_rcThumb.left -= getStep();
		}
		setHorzThumbRect(nThumbSize, m_rcThumb.left);
		calcHorzPos();
		invalidWid();
	}
	return 1;
}

LRESULT SliderBar::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid(getRect());
	m_rcThumbHolder = rcWid;
	m_rcThumb.empty();
	if (SB_VERT == m_nBar)
	{
		m_rcThumbHolder.left = rcWid.left + WFX_ROUND((float)rcWid.getWidth() / 2);
		m_rcThumbHolder.right = m_rcThumbHolder.left + 1;
		LONG nTop = calcVertThumbPos();
		nTop -= WFX_ROUND((float)m_nThumbSize / 2);
		setVertThumbRect(m_nThumbSize, nTop);

	}
	else
	{
		m_rcThumbHolder.top = rcWid.top + WFX_ROUND((float)rcWid.getHeight() / 2);
		m_rcThumbHolder.bottom = m_rcThumbHolder.top + 1;
		LONG nLeft = calcHorzThumbPos();
		nLeft -= WFX_ROUND((float)m_nThumbSize / 2);
		setHorzThumbRect(m_nThumbSize, nLeft);
	}
	return 1;
}

void SliderBar::setThumbSize( LONG nThumbSize )
{
	m_nThumbSize = nThumbSize;
}

LONG SliderBar::getThumbSize() const
{
	return m_nThumbSize;
}

LONG SliderBar::getStep() const
{
	return 1;
}

LONG SliderBar::getHorzThumbPosMin() const
{
	return WFX_ROUND(m_rcThumbHolder.left + (float)getThumbSize() / 2);
}

LONG SliderBar::getHorzThumbPosMax() const
{
	return WFX_ROUND(m_rcThumbHolder.right - (float)getThumbSize() / 2);
}

LONG SliderBar::getHorzThumbPos() const
{
	return WFX_ROUND(m_rcThumb.left + (float)getThumbSize() / 2);
}

LONG SliderBar::getVertThumbPosMin() const
{
	return m_rcThumbHolder.top + WFX_ROUND((float)getThumbSize() / 2);
}

LONG SliderBar::getVertThumbPosMax() const
{
	return m_rcThumbHolder.bottom - WFX_ROUND((float)getThumbSize() / 2);
}

LONG SliderBar::getVertThumbPos() const
{
	return m_rcThumb.top + WFX_ROUND((float)getThumbSize() / 2);
}

LONG SliderBar::calcHorzThumbPos()
{
	LONG nThumbPos = 0;
	LONG nThumbPosMin = getHorzThumbPosMin();
	LONG nThumbPosMax = getHorzThumbPosMax();
	LONG nMin = getMin();
	LONG nMax = getMax();
	LONG nPos = getPos();
	if (nMax - nMin > 0)
	{
		nThumbPos = WFX_ROUND((float)(nPos - nMin) / 
			(float)(nMax - nMin) * (nThumbPosMax - nThumbPosMin) + nThumbPosMin);
	}
	return nThumbPos;
}

LONG SliderBar::calcVertThumbPos()
{
	LONG nThumbPos = 0;
	LONG nThumbPosMin = getVertThumbPosMin();
	LONG nThumbPosMax = getVertThumbPosMax();
	LONG nMin = getMin();
	LONG nMax = getMax();
	LONG nPos = getPos();
	if (nMax - nMin > 0)
	{
		nThumbPos = WFX_ROUND((float)(nPos - nMin) / 
			(float)(nMax - nMin) * (nThumbPosMax - nThumbPosMin) + nThumbPosMin);
	}
	return nThumbPos;
}

void SliderBar::calcHorzPos()
{
	LONG nThumbPos = getHorzThumbPos();
	LONG nThumbPosMin = getHorzThumbPosMin();
	LONG nThumbPosMax = getHorzThumbPosMax();
	LONG nMin = getMin();
	LONG nMax = getMax();
	if (nThumbPosMax - nThumbPosMin > 0)
	{
		LONG nPos = WFX_ROUND((float)(nThumbPos - nThumbPosMin) / 
			(float)(nThumbPosMax - nThumbPosMin) * (nMax - nMin) + nMin);
		setPos(nPos);
		//TRACE(L"nPos=%d", nPos);
	}
}

void SliderBar::calcVertPos()
{
	LONG nThumbPos = getVertThumbPos();
	LONG nThumbPosMin = getVertThumbPosMin();
	LONG nThumbPosMax = getVertThumbPosMax();
	LONG nMin = getMin();
	LONG nMax = getMax();
	if (nThumbPosMax - nThumbPosMin > 0)
	{
		LONG nPos = WFX_ROUND((float)(nThumbPos - nThumbPosMin) / 
			(float)(nThumbPosMax - nThumbPosMin) * (nMax - nMin) + nMin);
		setPos(nPos);
		//TRACE(L"nPos=%d", nPos);
	}
}

void SliderBar::setHorzThumbRect( LONG nThumbSize, LONG x )
{
	m_rcThumb = getRect();
	m_rcThumb.left = x;
	m_rcThumb.right = m_rcThumb.left + nThumbSize;
	if (m_rcThumb.left < m_rcThumbHolder.left)
	{
		m_rcThumb.left = m_rcThumbHolder.left;
		m_rcThumb.right = m_rcThumb.left + nThumbSize;
	}
	if (m_rcThumb.right > m_rcThumbHolder.right)
	{
		m_rcThumb.right = m_rcThumbHolder.right;
		m_rcThumb.left = m_rcThumb.right - nThumbSize;
	}
}

void SliderBar::setVertThumbRect( LONG nThumbSize, LONG y )
{
	m_rcThumb = getRect();
	m_rcThumb.top = y;
	m_rcThumb.bottom = m_rcThumb.top + nThumbSize;
	if (m_rcThumb.top < m_rcThumbHolder.top)
	{
		m_rcThumb.top = m_rcThumbHolder.top;
		m_rcThumb.bottom = m_rcThumb.top + nThumbSize;
	}
	if (m_rcThumb.bottom > m_rcThumbHolder.bottom)
	{
		m_rcThumb.bottom = m_rcThumbHolder.bottom;
		m_rcThumb.top = m_rcThumb.bottom - nThumbSize;
	}
}

void SliderBar::onDraw( HDC hdc, const Rect& rcPaint )
{
	WfxRender::drawSolidRect(hdc, getRect(), RGB(255, 255, 255));
	WfxRender::drawSolidRect(hdc, m_rcThumbHolder, WBTN_BKGND_MOUSE);
	WfxRender::drawSolidRect(hdc, m_rcThumb, WBTN_BKGND_MOUSE);
}
