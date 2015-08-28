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
#include "wfxcmnctrl.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;

ProcessBar::ProcessBar(int nBar /*= SB_HORZ*/)
: BarBase(nBar)
, m_nMin(0)
, m_nMax(0)
, m_nPos(0)
{

}

void ProcessBar::setRange( LONG nMin, LONG nMax )
{
	m_nMin = nMin;
	m_nMax = nMax;
}

LONG ProcessBar::getRange() const
{
	return m_nMax - m_nMin;
}

LONG ProcessBar::getMax() const
{
	return m_nMax;
}

LONG ProcessBar::getMin() const
{
	return m_nMin;
}

void ProcessBar::setPos( LONG nPos )
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
	if (isWidget())
	{
		invalidWid();
	}
	sendParentMessage(WUM_CMN_SETPOS, (WPARAM)getID(), (LPARAM)nPos);
}

LONG ProcessBar::getPos() const
{
	return m_nPos;
}

BOOL ProcessBar::isCompleted() const
{
	return m_nPos == m_nMax;
}

void ProcessBar::reset()
{
	m_nPos = m_nMin;
}

void ProcessBar::onDraw( HDC hdc, const Rect& rc )
{
	WfxRender::drawProcessBar(hdc, getRect(), getState(), m_nMax, m_nPos, getDispatcher());
}
