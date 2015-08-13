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
#include "wfxrender.h"

USING_NAMESPACE_WFX;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
LRESULT MsgMap::SendWidMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;
	ProcessMessage(uMsg, wParam, lParam, lResult, 0);
	return lResult;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
AttrBase::AttrBase(Widget* pParent /*= NULL*/)
: m_dwFormat(DT_SINGLELINE | DT_VCENTER | DT_LEFT)
, m_clrText(WID_TEXT_STATIC)
, m_clrBkgnd(WID_BKGND_STATIC)
, m_clrFrame(WID_FRAME_STATIC)
, m_wState(WID_STATE_STATIC)
, m_pParent(pParent)
{

}

void AttrBase::SetText( const String& strText )
{
	m_strText = strText;
}

String AttrBase::GetText() const
{
	return m_strText;
}

void AttrBase::SetFormat( DWORD dwFormat )
{
	m_dwFormat = dwFormat;
}

DWORD AttrBase::GetFormat() const
{
	return m_dwFormat;
}

void AttrBase::SetTextColor( COLORREF clrText )
{
	m_clrText = clrText;
}

COLORREF AttrBase::GetTextColor() const
{
	return m_clrText;
}

void AttrBase::SetBkgnd( COLORREF clrBkgnd )
{
	m_clrBkgnd = clrBkgnd;
}

COLORREF AttrBase::GetBkgnd() const
{
	return m_clrBkgnd;
}

void AttrBase::SetFrame( COLORREF clrFrame )
{
	m_clrFrame = clrFrame;
}

COLORREF AttrBase::GetFrame() const
{
	return m_clrFrame;
}

void AttrBase::SetImage( const String& strImage )
{
	m_pImg.reset(WFX_GET_IMAGE(strImage.c_str()));
}

PImage AttrBase::GetImage() const
{
	return m_pImg;
}

void AttrBase::SetState( WORD wState )
{
	m_wState = wState;
}

WORD AttrBase::GetState() const
{
	return m_wState;
}

void AttrBase::SetRect( const Rect& rect )
{
	m_rect = rect;
}

Rect AttrBase::GetRect() const
{
	return m_rect;
}

Widget* AttrBase::GetParent() const
{
	return m_pParent;
}

void AttrBase::SetParent( Widget* pParent )
{
	m_pParent = pParent;
}
String AttrBase::GetToolTip() const
{
	return m_strText;
}

void AttrBase::SetToolTip( const String& strToolTip )
{
	m_strToolTip = strToolTip;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Widget::Widget(void)
: m_bNC(FALSE)
, m_pDispatch(NULL)
, m_hWid(INVALID_HWID)
, m_pVScrollbar(NULL)
, m_pHScrollbar(NULL)
, m_uBarFlag(WESB_NONE)
, m_wShow(SW_SHOW)
, m_nHorzPosOffset(0)
, m_nVertPosOffset(0)
, m_bVirtualSizeValid(FALSE)
, m_nID(0)
{
	
}

Widget::~Widget(void)
{
	if (m_pDispatch != NULL)
	{
		m_pDispatch->Destroy(m_hWid);
	}
	TDEL(m_pVScrollbar);
	TDEL(m_pHScrollbar);
}

void Widget::SetRect( const Rect& rc )
{
	AttrBase::SetRect(rc);
	m_pDispatch->SetWidRect(this, rc);
}

Rect Widget::GetParentRect() const
{
	Rect rcParent;
	if (m_pParent != NULL)
	{
		rcParent = m_pParent->GetRect();
	}
	else
	{
		WFX_CONDITION(m_pDispatch != NULL);
		WFX_CONDITION(::IsWindow(m_pDispatch->GetHwnd()));
		::GetClientRect(m_pDispatch->GetHwnd(), &rcParent);
	}
	return rcParent;
}

BOOL Widget::Create( const Rect& rc, Dispatcher* pDispatch, 
					Widget* pParent /*= NULL*/, BOOL bNC /*= FALSE*/ )
{
	WFX_CONDITION(pDispatch != NULL);
	m_pDispatch = pDispatch;
	m_pDispatch->Create(this);
	m_bNC = bNC;
	SetParent(pParent);
	SendWidMessage(WM_CREATE, 0, 0);
	SetRect(rc);
	return TRUE;
}

void Widget::SetParent( Widget* pParent )
{
	WFX_CONDITION(m_pDispatch != NULL);
	m_pDispatch->SetParent(this, pParent);
}

ULONG Widget::GetChildren( std::vector<Widget*>& rgpChildren ) const
{
	rgpChildren = m_rgpChildren;
	return rgpChildren.size();
}

ULONG Widget::GetChildren() const
{
	return m_rgpChildren.size();
}

BOOL Widget::HasChild() const
{
	return m_rgpChildren.size() != 0;
}

void Widget::InvalidWid()
{
	WFX_CONDITION(m_pDispatch != NULL);
	if (!IsShow())
	{
		return;
	}
	Rect rcInvalid = GetRect();
	Rect rcTemp;
	Rect rcParent;
	Widget* pParent = this;
	while(pParent = pParent->GetParent())
	{
		rcTemp = rcInvalid;
		rcParent = pParent->GetRect();
		if (!::IntersectRect(&rcInvalid, &rcTemp, &rcParent))
		{
			return;
		}
	}
	m_pDispatch->Invalidate(GetRect());
}

void Widget::OnDraw( HDC hdc, const Rect& rcPaint )
{
	Rect rcWid = GetRect();
	Rect rc = rcWid;
	rc.left = rc.left - m_nHorzPosOffset;
	rc.top = rc.top - m_nVertPosOffset;
	WfxRender::DrawSolidRect(hdc, rc, WID_BKGND_STATIC, m_pDispatch);
	WfxRender::DrawText(hdc, rc, GetText(), RGB(255, 0, 0), DT_VCENTER | DT_SINGLELINE | DT_LEFT, NULL, m_pDispatch);
	WfxRender::DrawFrame(hdc, rcWid, WBTN_BKGND_MOUSE, m_pDispatch);
}

void Widget::SetHwid( HWID hWid )
{
	m_hWid = hWid;
}

HWID Widget::GetHwid() const
{
	return m_hWid;
}

ULONG Widget::AddChild( Widget* pWid )
{
	std::vector<Widget*>::iterator it = 
		std::find(m_rgpChildren.begin(), m_rgpChildren.end(), pWid);
	if (it == m_rgpChildren.end())
	{
		m_rgpChildren.push_back(pWid);
	}
	return m_rgpChildren.size();
}

ULONG Widget::RemoveChild( Widget* pWid )
{
	std::vector<Widget*>::iterator it = 
		std::find(m_rgpChildren.begin(), m_rgpChildren.end(), pWid);
	if (it != m_rgpChildren.end())
	{
		m_rgpChildren.erase(it);
	}
	return m_rgpChildren.size();
}

void Widget::SetMyParent( Widget* pParent )
{
	m_pParent = pParent;
}

void Widget::EnableScrollBar( UINT uBarFlag, BOOL bEnable /*= TRUE*/ )
{
	WFX_CONDITION(m_pDispatch != NULL);
	m_pDispatch->EnableScrollBar(this, uBarFlag, bEnable);
}

void Widget::SetScrollInfo( int nBar, LPCSCROLLINFO lpsi, BOOL redraw )
{

}

void Widget::GetScrollInfo( int nBar, LPSCROLLINFO lpsi )
{

}

void Widget::SetScrollBar( int nBar, ScrollBar* pScrollBar )
{
	if (SB_VERT == nBar)
	{
		m_pVScrollbar = pScrollBar;
	}
	else
	{
		m_pHScrollbar = pScrollBar;
	}
}


void Widget::SetState( WORD wState )
{
	if (wState == GetState())
	{
		return;
	}

	AttrBase::SetState(wState);

	SendWidMessage(WM_UPDATEUISTATE, wState);
}

void Widget::ShowWid( WORD wShow )
{
	WFX_CONDITION(m_pDispatch != NULL);
	m_pDispatch->ShowWid(this, wShow);
	if (IsShow())
	{
		InvalidWid();
	}
}

BOOL Widget::PostWidMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	LRESULT lResult = 0;
	return ProcessMessage(uMsg, wParam, lParam, lResult, 0);
}

UINT_PTR Widget::SetWidTimer( UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc /*= NULL*/)
{
	WFX_CONDITION(m_pDispatch != NULL);
	return m_pDispatch->SetWidTimer(this, nIDEvent, uElapse, lpTimerFunc);
}

BOOL Widget::KillWidTimer( UINT_PTR uIDEvent )
{
	WFX_CONDITION(m_pDispatch != NULL);
	return m_pDispatch->KillWidTimer(this, uIDEvent);
}

BOOL Widget::IsShow() const
{
	return (m_wShow != SW_HIDE);
}

void Widget::MyShowWid( WORD wShow )
{
	m_wShow = wShow;
	PostWidMessage(WM_SHOWWINDOW, m_wShow);
}

LRESULT Widget::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (wParam == MK_LBUTTON)
		return 0;
	SetState(WID_STATE_MOUSE);
	return 0;
}

LRESULT Widget::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetState(WID_STATE_STATIC);
	return 0;
}

LRESULT Widget::OnStateChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(wParam)
	{
	case WID_STATE_MOUSE:
		{
		
		}
		break;
	case WID_STATE_STATIC:
		{
			
		}
		break;
	case WID_STATE_PUSH:
		{
			
		}
		break;
	default:
		{
			
		}
	}
	InvalidWid();
	return 0;
}

Widget::operator HWID() const
{
	return m_hWid;
}

void Widget::SetCapture()
{
	WFX_CONDITION(m_pDispatch != NULL);
	m_pDispatch->SetCapture(this);
}

void Widget::ReleaseCapture()
{
	WFX_CONDITION(m_pDispatch != NULL);
	m_pDispatch->ReleaseCapture();
}

BOOL Widget::IsCaptured() const
{
	return m_pDispatch->IsCaptured(this);
}

ScrollBar* Widget::GetScrollBar( int nBar ) const
{
	if (SB_VERT == nBar)
		return m_pVScrollbar;
	return m_pHScrollbar;
}

LRESULT Widget::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetState(WID_STATE_PUSH);
	return 1;
}

LRESULT Widget::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetState(WID_STATE_MOUSE);
	return 1;
}

UINT Widget::GetSBFlag() const
{
	return m_uBarFlag;
}

void Widget::SetSBFlag( UINT uSBFlag )
{
	m_uBarFlag = uSBFlag;
}

BOOL Widget::ScrollWid( int XAmount, int YAmount )
{
	return TRUE;
}

LRESULT Widget::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT Widget::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT Widget::OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_wState == WID_STATE_STATIC)
	{
		SetState(WID_STATE_MOUSE);
	}
	if (m_wState == WID_STATE_MOUSE)
	{
		SetState(WID_STATE_PUSH);
	}
	if (m_wState == WID_STATE_PUSH)
	{
		SetState(WID_STATE_STATIC);
	}
	return 0;
}

LRESULT Widget::OnKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	InvalidWid();
	return 0;
}

LRESULT Widget::OnQueryVirtualSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	/*if (m_bVirtualSizeValid)
	{
		return MAKELRESULT(m_szVirtual.cx, m_szVirtual.cy);
	}*/
	m_szVirtual = EstimateVirualSize();
	m_bVirtualSizeValid = TRUE;
	return MAKELRESULT(m_szVirtual.cx, m_szVirtual.cy);
}

LRESULT Widget::OnGetVirtualSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return MAKELRESULT(m_szVirtual.cx, m_szVirtual.cy);
}

void Widget::SetVirtualSizeValid( BOOL bValid /*= TRUE*/ )
{
	m_bVirtualSizeValid = bValid;
}

Size Widget::EstimateVirualSize()
{
	return WfxRender::EstimateWidgetSize(GetRect(), 
		GetText(), GetState(), m_pDispatch);
}

LRESULT Widget::OnQueryVisualSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid = GetRect();
	return MAKELRESULT(rcWid.GetWidth(), rcWid.GetHeight());
}

LRESULT Widget::OnScrollBarOffset( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ScrollBar* pScrollBar = GetScrollBar(wParam);
	if (pScrollBar == NULL)
	{
		return FALSE;
	}

	SharedPtr<SCROLLINFO> psinfo(new SCROLLINFO);
	psinfo->cbSize = sizeof(SCROLLINFO);
	pScrollBar->GetScrollInfo(psinfo.get());

	TRACE(L"nPos = %d", psinfo->nPos);
	float fOffset = 0;
	Rect rcDraw = GetDrawRect();
	if (wParam == SB_HORZ)
	{
		fOffset = (rcDraw.right - rcDraw.left) * (float)psinfo->nPos / (float)(psinfo->nMax - psinfo->nMin);
		fOffset *= (float)m_szVirtual.cx / (float)(m_rcDraw.right - m_rcDraw.left);
		SetHOffset(fOffset);
	}
	else
	{
		fOffset = (rcDraw.bottom - rcDraw.top) * (float)psinfo->nPos / (float)(psinfo->nMax - psinfo->nMin);
		fOffset *= (float)m_szVirtual.cy / (float)(m_rcDraw.bottom - m_rcDraw.top);
		SetVOffset(fOffset);
	}
	return 1;
}

LRESULT Widget::OnHScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	InvalidWid();
	return 1;
}

LRESULT Widget::SendParentMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	if (m_pParent != NULL)
	{
		if (!m_pParent->SendWidMessage(uMsg, wParam, lParam))
		{
			Widget* pParent = m_pParent->GetParent();
			if (pParent != NULL)
			{
				return pParent->SendWidMessage(uMsg, wParam, lParam);
			}
		}
	}
	return 0;
}

Rect Widget::GetDrawRect() const
{
	return m_rcDraw;
}

void Widget::SetDrawRect( const Rect& rc )
{
	m_rcDraw = rc;
}

LONG Widget::GetVOffset() const
{
	return m_nVertPosOffset;
}

void Widget::SetVOffset( LONG nOffset )
{
	m_nVertPosOffset = nOffset;
}

LONG Widget::GetHOffset() const
{
	return m_nHorzPosOffset;
}

void Widget::SetHOffset( LONG nOffset )
{
	m_nHorzPosOffset = nOffset;
}

Size Widget::GetVirtualSize() const
{
	return m_szVirtual;
}

UINT Widget::GetID() const
{
	return m_nID;
}

void Widget::SetID( UINT nID )
{
	m_nID = nID;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
UnitBase::UnitBase(Widget* pWid /*= NULL*/)
: AttrBase(pWid)
{

}

BOOL UnitBase::ProcessMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	return TRUE;
}

LRESULT UnitBase::SendParentMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	if (m_pParent != NULL)
		return m_pParent->SendWidMessage(uMsg, wParam, lParam);
	return 0;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ScrollBar::ScrollBar( int nBar )
: m_nBar(nBar)
, m_pScrollInfo(new SCROLLINFO)
, m_nArrorSize(0)
, m_bInThumb(FALSE)
, m_bDirection(FALSE)
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

int ScrollBar::GetBar() const
{
	return m_nBar;
}

void ScrollBar::SetBar( int nBar )
{
	m_nBar = nBar;
}

void ScrollBar::GetScrollInfo( SCROLLINFO* pScrollInfo ) const
{
	WFX_CONDITION(pScrollInfo != NULL);
	if (pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		memcpy(pScrollInfo, m_pScrollInfo.get(), sizeof(SCROLLINFO));
	}
}

void ScrollBar::SetScrollInfo( const SCROLLINFO* pScrollInfo )
{
	WFX_CONDITION(pScrollInfo != NULL);
	if (pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		memcpy(m_pScrollInfo.get(), pScrollInfo, sizeof(SCROLLINFO));
	}
}

void ScrollBar::SetRange( int nMin, int nMax )
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	if (m_pScrollInfo->cbSize == sizeof(SCROLLINFO))
	{
		m_pScrollInfo->nMin = nMin;
		m_pScrollInfo->nMax = nMax;
	}
}

LRESULT ScrollBar::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcWid(GetRect());
	m_rcArrow1 = rcWid;
	m_rcArrow2 = rcWid;
	m_rcThumbHolder = rcWid;
	m_rcThumb.Empty();
	LONG nThumbSize = 0;
	LONG nThumbHolderSize = 0;
	LONG nOffset = m_pScrollInfo->nPos - m_pScrollInfo->nMin;
	float fRange = m_pScrollInfo->nMax - m_pScrollInfo->nMin;
	if (SB_VERT == m_nBar)
	{
		m_rcArrow1.bottom = rcWid.top + m_nArrorSize;
		m_rcArrow2.top = rcWid.bottom - m_nArrorSize;
		m_rcThumbHolder.top = m_rcArrow1.bottom;
		m_rcThumbHolder.bottom = m_rcArrow2.top;
		int nThumbHolderSize = m_rcThumbHolder.GetHeight();
		if (fRange > 0)
		{
			nThumbSize = nThumbHolderSize * (float)rcWid.GetHeight() / fRange;
			if (nThumbSize < 30)
			{
				nThumbSize = 30;
			}
			LONG y = nOffset / fRange * nThumbHolderSize - nThumbSize / 2;
			SetVertThumbPos(nThumbSize, y);
		}
	}
	else
	{
		m_rcArrow1.right = rcWid.left + m_nArrorSize;
		m_rcArrow2.left = rcWid.right - m_nArrorSize;
		m_rcThumbHolder.left = m_rcArrow1.right;
		m_rcThumbHolder.right = m_rcArrow2.left;
		int nThumbHolderSize = m_rcThumbHolder.GetWidth();
		if (fRange > 0)
		{
			nThumbSize = nThumbHolderSize * (float)rcWid.GetWidth() / fRange;
			LONG x = nOffset / fRange * nThumbHolderSize - nThumbSize / 2;
			SetHorzThumbPos(nThumbSize, x);
		}
	}
	
	return 1;
}

LRESULT ScrollBar::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
		SetVertThumbPos(nThumbSize, nTop);
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
		SetHorzThumbPos(nThumbSize, nLeft);
	}
	InvalidWid();
	return 1;
}

LRESULT ScrollBar::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
			m_bDirection = m_ptLButtonDown.y - (m_rcThumb.top + m_rcThumb.GetHeight() / 2) > 0;
		}
		else
		{
			m_bDirection = m_ptLButtonDown.x - (m_rcThumb.left + m_rcThumb.GetWidth() / 2) > 0;
		}
		SetWidTimer(1, 50, NULL);
	}
	return 1;
}

LRESULT ScrollBar::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	KillWidTimer(1);
	m_bInThumb = FALSE;
	return 1;
}

LRESULT ScrollBar::OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
			m_rcThumb.top++;
		}
		else
		{
			m_rcThumb.top--;
		}
		SetVertThumbPos(nThumbSize, m_rcThumb.top);
		InvalidWid();
	}
	else
	{
		LONG nThumbSize = m_rcThumb.GetWidth();
		if (m_bDirection)
		{
			m_rcThumb.left++;
		}
		else
		{
			m_rcThumb.left--;
		}
		SetHorzThumbPos(nThumbSize, m_rcThumb.left);
		InvalidWid();
	}
	return 1;
}

void ScrollBar::SetHorzThumbPos( LONG nThumbSize, LONG x )
{
	m_rcThumb = m_rcThumbHolder;
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

void ScrollBar::SetVertThumbPos( LONG nThumbSize, LONG y )
{
	m_rcThumb = m_rcThumbHolder;
	m_rcThumb.top = y;
	m_rcThumb.bottom = m_rcThumb.top + nThumbSize;
	TRACE(L"%d, %d", m_rcThumb.top, m_rcThumb.bottom);
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

void ScrollBar::SetPos( int nPos )
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	BOOL bFurther = nPos > m_pScrollInfo->nPos;
	if (m_pScrollInfo->nPos != nPos)
	{
		m_pScrollInfo->nPos = nPos;
		if (m_pScrollInfo->nPos < m_pScrollInfo->nMin)
			m_pScrollInfo->nPos = m_pScrollInfo->nMin;
		if (m_pScrollInfo->nPos > m_pScrollInfo->nMax)
			m_pScrollInfo->nPos = m_pScrollInfo->nMax;
		SendParentMessage(WUM_SB_OFFSET, m_nBar);
		if (m_nBar == SB_VERT) 
		{
			SendParentMessage(WM_VSCROLL, (WPARAM)bFurther);
		}
		else
		{
			SendParentMessage(WM_HSCROLL,(WPARAM)bFurther);
		}
	}
}

int ScrollBar::GetPos() const
{
	WFX_CONDITION(m_pScrollInfo != NULL);
	return m_pScrollInfo->nPos;
}

void ScrollBar::OnDraw( HDC hdc, const Rect& rcPaint )
{
	WfxRender::DrawSolidRect(hdc, GetRect(), RGB(0, 0, 0));
	WfxRender::DrawArror(hdc, m_rcArrow1, 0, 0);
	WfxRender::DrawArror(hdc, m_rcArrow2, 0, 0);
	WfxRender::DrawThumb(hdc, m_rcThumb, 0);
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Slider::Slider( int nBar )
: m_nBar(nBar)
, m_bInSlider(FALSE)
, m_bLButtonDown(FALSE)
{
}

Slider::~Slider()
{

}

LRESULT Slider::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bLButtonDown = TRUE;
	m_ptLButtonDown.x = GET_X_LPARAM(lParam);
	m_ptLButtonDown.y = GET_Y_LPARAM(lParam);
	return 0;
}

LRESULT Slider::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bLButtonDown = FALSE;
	m_ptLButtonDown.x = -1;
	m_ptLButtonDown.y = -1;
	return 0;
}

LRESULT Slider::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int nOffset = 0.00;
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	if (m_bLButtonDown)
	{
		Rect rc = GetParentRect();
		if (SB_VERT == m_nBar)
		{
			if (pt.y < rc.top || pt.y > rc.bottom)
				return 0;
			nOffset = pt.y - m_ptLButtonDown.y;
		}
		else
		{
			if (pt.x < rc.left || pt.x > rc.right)
				return 0;
			nOffset = pt.x - m_ptLButtonDown.x;
		}
		SendParentMessage(WUM_SB_OFFSET, (WPARAM)nOffset);
		m_ptLButtonDown = pt;
	}
	else
	{
		__super::OnMouseMove(uMsg, wParam, lParam, bHandled);
	}
	return 0;
}

void Slider::OnDraw( HDC hdc, const Rect& rcPaint )
{
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ImageWid::ImageWid()
{

}

ImageWid::ImageWid(const String& strStatic,
				   const String& strMouse, 
				   const String& strPush,
				   const String& strChecked)
				   : m_pImgStatic(WFX_GET_IMAGE(strStatic.c_str()))
				   , m_pImgMouse(WFX_GET_IMAGE(strMouse.c_str()))
				   , m_pImgPush(WFX_GET_IMAGE(strPush.c_str()))
				   , m_pImgChecked(WFX_GET_IMAGE(strChecked.c_str()))
{
}

void ImageWid::SetImage( WORD wState, const String& strImage )
{
	switch(wState)
	{
	case WID_STATE_STATIC:
		m_pImgStatic.reset(WFX_GET_IMAGE(strImage.c_str()));
		break;
	case WID_STATE_MOUSE:
		m_pImgMouse.reset(WFX_GET_IMAGE(strImage.c_str()));
		break;
	case WID_STATE_PUSH:
		m_pImgPush.reset(WFX_GET_IMAGE(strImage.c_str()));
		break;
	case WID_STATE_CHECKED:
		m_pImgChecked.reset(WFX_GET_IMAGE(strImage.c_str()));
		break;
	default:
		WFX_CONDITION(FALSE);
	}
}

void ImageWid::SetImage( const String& strStatic, 
						const String& strMouse, 
						const String& strPush,
						const String& strChecked)
{
	m_pImgStatic.reset(WFX_GET_IMAGE(strStatic.c_str()));
	m_pImgMouse.reset(WFX_GET_IMAGE(strMouse.c_str()));
	m_pImgPush.reset(WFX_GET_IMAGE(strPush.c_str()));
	m_pImgChecked.reset(WFX_GET_IMAGE(strChecked.c_str()));
}

PImage ImageWid::GetImageFromState()
{
	PImage pImage;
	switch(GetState())
	{
	case WID_STATE_STATIC:
		pImage = m_pImgStatic;
		break;
	case WID_STATE_MOUSE:
		pImage = m_pImgMouse;
		break;
	case WID_STATE_PUSH:
		pImage = m_pImgPush;
		break;
	case WID_STATE_CHECKED:
		pImage = m_pImgChecked;
		break;
	default:
		WFX_CONDITION(FALSE);
		pImage = m_pImgStatic;
	}
	return pImage;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
InPlaceWid::InPlaceWid()
: m_pWindow(NULL)
{

}

LRESULT InPlaceWid::OnSetFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (Initial())
	{
		return 0;
	}
	return 1;
}
