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
, m_bCachedVirtualSize(FALSE)
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

Size AttrBase::GetVirtualSize()
{
	if (m_bCachedVirtualSize)
	{
		return m_szVirtualSize;
	}
	m_szVirtualSize = CalcVirtualSize();
	return m_szVirtualSize;
}

Size AttrBase::CalcVirtualSize()
{
	SetVirtualSizeCached();
	WFX_CONDITION(FALSE);
	return Size();
}

BOOL AttrBase::IsVirtualSizeCached() const
{
	return m_bCachedVirtualSize;
}

void AttrBase::SetVirtualSizeCached( BOOL bCached /*= TRUE*/ )
{
	m_bCachedVirtualSize = bCached;
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
	if (!rc.IsEmpty())
	{
		SetRect(rc);
	}
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

BOOL Widget::IsWidget() const
{
	if (this == NULL)
	{
		return FALSE;
	}
	return m_pDispatch->IsWidget(this);
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

BOOL Widget::InClientAera( const Point& pt )
{
	return m_rcClient.PtInRect(pt);
}

Rect Widget::GetScrollBarRect( int nBar )
{
	Rect rcWid = GetRect();
	Rect rcScrollBar = rcWid;
	if (SB_HORZ == nBar)
	{
		if (m_uBarFlag & WESB_VERT)
		{
			rcScrollBar.top = rcWid.bottom - SIZE_SCROLLBAR;
		}
		else
		{
			rcScrollBar.Empty();
		}
	}
	else
	{
		if (m_uBarFlag & WESB_VERT)
		{
			rcScrollBar.left = rcWid.right - SIZE_SCROLLBAR;
			if (m_uBarFlag & WESB_HORZ)
			{
				rcScrollBar.bottom = rcWid.bottom - SIZE_SCROLLBAR;
			}
		}
		else
		{
			rcScrollBar.Empty();
		}
	}
	return rcScrollBar;
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

LRESULT Widget::OnHScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	InvalidWid();
	return 1;
}

LRESULT Widget::SendParentMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	LRESULT lResult = 0;
	if (m_pParent != NULL)
	{
		lResult = m_pParent->SendWidMessage(uMsg, wParam, lParam);
		if (!lResult)
		{
			Widget* pParent = m_pParent->GetParent();
			if (pParent != NULL)
			{
				return pParent->SendWidMessage(uMsg, wParam, lParam);
			}
		}
		else
		{
			return lResult;
		}
	}
	return 0;
}

Rect Widget::GetClientRect() const
{
	return m_rcClient;
}

void Widget::SetClientRect( const Rect& rc )
{
	m_rcClient = rc;
}

LONG Widget::GetVOffset() const
{
	if (GetScrollBar(SB_VERT) != NULL)
	{
		return GetScrollBar(SB_VERT)->GetPos();
	}
	return 0;
}

LONG Widget::GetHOffset() const
{
	if (GetScrollBar(SB_HORZ) != NULL)
	{
		return GetScrollBar(SB_HORZ)->GetPos();
	}
	return 0;
}

UINT Widget::GetID() const
{
	return m_nID;
}

void Widget::SetID( UINT nID )
{
	m_nID = nID;
}

Size Widget::CalcVirtualSize()
{
	return Size();
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

void UnitBase::OnDraw( HDC hdc, const Rect& rc )
{
	WfxRender::DrawSolidRect(hdc, GetRect(), RGB(255, 0, 0));
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
LRESULT WidCtrlBase::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (wParam == MK_LBUTTON)
		return 0;
	SetState(WID_STATE_MOUSE);
	return 1;
}

LRESULT WidCtrlBase::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetState(WID_STATE_STATIC);
	return 1;
}

LRESULT WidCtrlBase::OnStateChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	InvalidWid();
	return 1;
}

LRESULT WidCtrlBase::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetState(WID_STATE_PUSH);
	return 1;
}

LRESULT WidCtrlBase::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetState(WID_STATE_MOUSE);
	return 1;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
BarBase::BarBase( int nBar /*= SB_HORZ*/ )
: m_nBar(nBar)
{

}

int BarBase::GetBar() const
{
	return m_nBar;
}

void BarBase::SetBar( int nBar )
{
	m_nBar = nBar;
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

Gdiplus::Image* ImageWid::GetImageFromState()
{
	Gdiplus::Image* pImage;
	switch(GetState())
	{
	case WID_STATE_STATIC:
		pImage = m_pImgStatic.get();
		break;
	case WID_STATE_MOUSE:
		pImage = m_pImgMouse.get();
		break;
	case WID_STATE_PUSH:
		pImage = m_pImgPush.get();
		break;
	case WID_STATE_CHECKED:
		pImage = m_pImgChecked.get();
		break;
	default:
		WFX_CONDITION(FALSE);
		pImage = m_pImgStatic.get();
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

void InPlaceWid::Reset()
{
	m_pWindow = NULL;
}

