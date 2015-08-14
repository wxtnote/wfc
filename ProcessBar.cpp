// This is a part of the Widget Foundation Classes.
// Copyright (C) Grant Ward (grant.ward@gmail.com)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Widget Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Widget Foundation Classes product.
//
#include "StdAfx.h"
#include "wfxwid.h"
#include "wfxcmn.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;

ProcessBar::ProcessBar(int nBar /*= SB_HORZ*/)
: m_nBar(nBar)
, m_nMin(0)
, m_nMax(0)
, m_nPos(0)
{

}

int ProcessBar::GetBar() const
{
	return m_nBar;
}

void ProcessBar::SetBar( int nBar )
{
	m_nBar = nBar;
}

void ProcessBar::SetRange( LONG nMin, LONG nMax )
{
	m_nMin = nMin;
	m_nMax = nMax;
}

LONG ProcessBar::GetRange() const
{
	return m_nMax - m_nMin;
}

LONG ProcessBar::GetMax() const
{
	return m_nMax;
}

LONG ProcessBar::GetMin() const
{
	return m_nMin;
}

void ProcessBar::SetPos( LONG nPos )
{
	if (nPos > m_nMax)
	{
		nPos = m_nMax;
	}
	if (nPos < m_nMin)
	{
		nPos = m_nMin;
	}
	if (m_nPos == nPos)
	{
		return;
	}
	m_nPos = nPos;
	if (IsWidget())
	{
		InvalidWid();
	}
}

LONG ProcessBar::GetPos() const
{
	return m_nPos;
}

BOOL ProcessBar::IsCompleted() const
{
	return m_nPos == m_nMax;
}

void ProcessBar::Reset()
{
	m_nPos = m_nMin;
}

void ProcessBar::OnDraw( HDC hdc, const Rect& rc )
{
	WfxRender::DrawProcessBar(hdc, GetRect(), GetState(), m_nMax, m_nPos, m_pDispatch);
}
