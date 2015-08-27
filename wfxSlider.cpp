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
Slider::Slider( int nBar /*= SB_HORZ*/ )
: ProcessBar(nBar)
, m_nThumbSize(13)
{
}

LRESULT Slider::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
				(m_rcThumb.top + WFX_ROUND((float)m_rcThumb.GetHeight() / 2)) > 0;
		}
		else
		{
			m_bDirection = m_ptLButtonDown.x - 
				(m_rcThumb.left + WFX_ROUND((float)m_rcThumb.GetWidth() / 2)) > 0;
		}
		SetWidTimer(1, 50, NULL);
	}
	return 1;
}

LRESULT Slider::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	KillWidTimer(1);
	m_bInThumb = FALSE;
	return 1;
}

LRESULT Slider::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_ptMouseMove = lParam;
	if (!IsCaptured() || !m_bInThumb)
	{
		return __super::OnMouseMove(uMsg, wParam, lParam, bHandled);
	}
	Point pt(lParam);
	LONG nThumbSize = 0;
	if (SB_VERT == m_nBar)
	{
		if (pt.y < m_rcThumbHolder.top || pt.y > m_rcThumbHolder.bottom)
			return 1;
		nThumbSize = m_rcThumb.GetHeight();
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
		SetVertThumbRect(nThumbSize, nTop);
		CalcVertPos();
	}
	else
	{
		if (pt.x < m_rcThumbHolder.left || pt.x > m_rcThumbHolder.right)
			return 1;
		nThumbSize = m_rcThumb.GetWidth();
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
		SetHorzThumbRect(nThumbSize, nLeft);
		CalcHorzPos();
	}
	InvalidWid();
	return 1;
}

LRESULT Slider::OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_rcThumb.PtInRect(m_ptMouseMove))
	{
		KillWidTimer(1);
		return 1;
	}
	if (m_nBar == SB_VERT)
	{	
		LONG nThumbSize = m_rcThumb.GetHeight();
		if (m_bDirection)
		{
			m_rcThumb.top += GetStep();
		}
		else
		{
			m_rcThumb.top -= GetStep();
		}
		SetVertThumbRect(nThumbSize, m_rcThumb.top);
		CalcVertPos();
		InvalidWid();
	}
	else
	{
		LONG nThumbSize = m_rcThumb.GetWidth();
		if (m_bDirection)
		{
			m_rcThumb.left += GetStep();
		}
		else
		{
			m_rcThumb.left -= GetStep();
		}
		SetHorzThumbRect(nThumbSize, m_rcThumb.left);
		CalcHorzPos();
		InvalidWid();
	}
	return 1;
}

LRESULT Slider::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid(GetRect());
	m_rcThumbHolder = rcWid;
	m_rcThumb.Empty();
	if (SB_VERT == m_nBar)
	{
		m_rcThumbHolder.left = rcWid.left + WFX_ROUND((float)rcWid.GetWidth() / 2);
		m_rcThumbHolder.right = m_rcThumbHolder.left + 1;
		LONG nTop = CalcVertThumbPos();
		nTop -= WFX_ROUND((float)m_nThumbSize / 2);
		SetVertThumbRect(m_nThumbSize, nTop);

	}
	else
	{
		m_rcThumbHolder.top = rcWid.top + WFX_ROUND((float)rcWid.GetHeight() / 2);
		m_rcThumbHolder.bottom = m_rcThumbHolder.top + 1;
		LONG nLeft = CalcHorzThumbPos();
		nLeft -= WFX_ROUND((float)m_nThumbSize / 2);
		SetHorzThumbRect(m_nThumbSize, nLeft);
	}
	return 1;
}

void Slider::SetThumbSize( LONG nThumbSize )
{
	m_nThumbSize = nThumbSize;
}

LONG Slider::GetThumbSize() const
{
	return m_nThumbSize;
}

LONG Slider::GetStep() const
{
	return 1;
}

LONG Slider::GetHorzThumbPosMin() const
{
	return WFX_ROUND(m_rcThumbHolder.left + (float)GetThumbSize() / 2);
}

LONG Slider::GetHorzThumbPosMax() const
{
	return WFX_ROUND(m_rcThumbHolder.right - (float)GetThumbSize() / 2);
}

LONG Slider::GetHorzThumbPos() const
{
	return WFX_ROUND(m_rcThumb.left + (float)GetThumbSize() / 2);
}

LONG Slider::GetVertThumbPosMin() const
{
	return m_rcThumbHolder.top + WFX_ROUND((float)GetThumbSize() / 2);
}

LONG Slider::GetVertThumbPosMax() const
{
	return m_rcThumbHolder.bottom - WFX_ROUND((float)GetThumbSize() / 2);
}

LONG Slider::GetVertThumbPos() const
{
	return m_rcThumb.top + WFX_ROUND((float)GetThumbSize() / 2);
}

LONG Slider::CalcHorzThumbPos()
{
	LONG nThumbPos = 0;
	LONG nThumbPosMin = GetHorzThumbPosMin();
	LONG nThumbPosMax = GetHorzThumbPosMax();
	LONG nMin = GetMin();
	LONG nMax = GetMax();
	LONG nPos = GetPos();
	if (nMax - nMin > 0)
	{
		nThumbPos = WFX_ROUND((float)(nPos - nMin) / 
			(float)(nMax - nMin) * (nThumbPosMax - nThumbPosMin) + nThumbPosMin);
	}
	return nThumbPos;
}

LONG Slider::CalcVertThumbPos()
{
	LONG nThumbPos = 0;
	LONG nThumbPosMin = GetVertThumbPosMin();
	LONG nThumbPosMax = GetVertThumbPosMax();
	LONG nMin = GetMin();
	LONG nMax = GetMax();
	LONG nPos = GetPos();
	if (nMax - nMin > 0)
	{
		nThumbPos = WFX_ROUND((float)(nPos - nMin) / 
			(float)(nMax - nMin) * (nThumbPosMax - nThumbPosMin) + nThumbPosMin);
	}
	return nThumbPos;
}

void Slider::CalcHorzPos()
{
	LONG nThumbPos = GetHorzThumbPos();
	LONG nThumbPosMin = GetHorzThumbPosMin();
	LONG nThumbPosMax = GetHorzThumbPosMax();
	LONG nMin = GetMin();
	LONG nMax = GetMax();
	if (nThumbPosMax - nThumbPosMin > 0)
	{
		LONG nPos = WFX_ROUND((float)(nThumbPos - nThumbPosMin) / 
			(float)(nThumbPosMax - nThumbPosMin) * (nMax - nMin) + nMin);
		SetPos(nPos);
		//TRACE(L"nPos=%d", nPos);
	}
}

void Slider::CalcVertPos()
{
	LONG nThumbPos = GetVertThumbPos();
	LONG nThumbPosMin = GetVertThumbPosMin();
	LONG nThumbPosMax = GetVertThumbPosMax();
	LONG nMin = GetMin();
	LONG nMax = GetMax();
	if (nThumbPosMax - nThumbPosMin > 0)
	{
		LONG nPos = WFX_ROUND((float)(nThumbPos - nThumbPosMin) / 
			(float)(nThumbPosMax - nThumbPosMin) * (nMax - nMin) + nMin);
		SetPos(nPos);
		//TRACE(L"nPos=%d", nPos);
	}
}

void Slider::SetHorzThumbRect( LONG nThumbSize, LONG x )
{
	m_rcThumb = GetRect();
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

void Slider::SetVertThumbRect( LONG nThumbSize, LONG y )
{
	m_rcThumb = GetRect();
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

void Slider::OnDraw( HDC hdc, const Rect& rcPaint )
{
	WfxRender::DrawSolidRect(hdc, GetRect(), RGB(255, 255, 255));
	WfxRender::DrawSolidRect(hdc, m_rcThumbHolder, WBTN_BKGND_MOUSE);
	WfxRender::DrawSolidRect(hdc, m_rcThumb, WBTN_BKGND_MOUSE);
}
