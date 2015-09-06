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
LRESULT MsgMap::sendMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 0;
	processMessage(uMsg, wParam, lParam, lResult, 0);
	return lResult;
}

MsgMap::~MsgMap()
{

}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
UnitBase::UnitBase(Widget* pParent /*= NULL*/)
: m_pParent(pParent)
{
	doInit();
}

void UnitBase::doInit()
{
	m_dwFormat = DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	m_clrText = WID_TEXT_STATIC;
	m_clrBkgnd = WID_BKGND_STATIC;
	m_clrFrame = WID_FRAME_STATIC;
	m_wState = WID_STATE_STATIC;
	m_bCachedVirtualSize = FALSE;
}

void UnitBase::setText( const String& strText )
{
	m_strText = strText;
}

void UnitBase::getText(String& str) const
{
	str = m_strText;
}

const String& UnitBase::getText() const
{
	return m_strText;
}

void UnitBase::setFormat( DWORD dwFormat )
{
	m_dwFormat = dwFormat;
}

DWORD UnitBase::getFormat() const
{
	return m_dwFormat;
}

void UnitBase::setTextColor( COLORREF clrText )
{
	m_clrText = clrText;
}

COLORREF UnitBase::getTextColor() const
{
	return m_clrText;
}

void UnitBase::setBkgnd( COLORREF clrBkgnd )
{
	m_clrBkgnd = clrBkgnd;
}

COLORREF UnitBase::getBkgnd() const
{
	return m_clrBkgnd;
}

void UnitBase::setFrame( COLORREF clrFrame )
{
	m_clrFrame = clrFrame;
}

COLORREF UnitBase::getFrame() const
{
	return m_clrFrame;
}

void UnitBase::setState( WORD wState )
{
	m_wState = wState;
}

WORD UnitBase::getState() const
{
	return m_wState;
}

void UnitBase::setRect( const Rect& rc )
{
	m_rect = rc;
}

void UnitBase::getRect(Rect& rc) const
{
	rc = m_rect;
}

const Rect& UnitBase::getRect() const
{
	return m_rect;
}

Widget* UnitBase::getParent() const
{
	return m_pParent;
}

void UnitBase::setParent( Widget* pParent )
{
	m_pParent = pParent;
}

void UnitBase::getToolTip(String& str) const
{
	str = m_strText;
}

void UnitBase::setToolTip( const String& strToolTip )
{
	m_strToolTip = strToolTip;
}

void UnitBase::getVirtualSize(Size& sz)
{
	if (m_bCachedVirtualSize)
	{
		sz = m_szVirtualSize;
	}
	calcVirtualSize(m_szVirtualSize);
	sz = m_szVirtualSize;
}

const Size& UnitBase::getVirtualSize()
{
	Size sz;
	getVirtualSize(sz);
	return m_szVirtualSize;
}


void UnitBase::calcVirtualSize(Size& sz)
{
	setVirtualSizeCached();
	WFX_CONDITION(FALSE);
}

BOOL UnitBase::isVirtualSizeCached() const
{
	return m_bCachedVirtualSize;
}

void UnitBase::setVirtualSizeCached( BOOL bCached /*= TRUE*/ )
{
	m_bCachedVirtualSize = bCached;
}

UnitBase::~UnitBase()
{

}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Widget::Widget(void)
{
	doInit();
}

void Widget::doInit()
{
	m_bNC = FALSE;
	m_hWid = INVALID_HWID;
	m_uBarFlag = WESB_NONE;
	m_wShow = SW_SHOW;
	m_nID = 0;
}

Widget::~Widget(void)
{
	if (getDispatcher() != null)
	{
		getDispatcher()->destroy(m_hWid);
	}
}

void Widget::setRect( const Rect& rc )
{
	UnitBase::setRect(rc);
	getDispatcher()->setWidRect(this, rc);
}

void Widget::getParentRect(Rect& rc) const
{
	if (m_pParent != null)
	{
		m_pParent->getRect(rc);
	}
	else
	{
		WFX_CONDITION(getDispatcher() != NULL);
		WFX_CONDITION(Window::isWindow(getDispatcher()->getHwnd()));
		Window::getClientRect(getDispatcher()->getHwnd(), rc);
	}
}

BOOL Widget::create( const Rect& rc, Widget* pParent /*= NULL*/, 
					Dispatcher* pDispatch /*= NULL*/, BOOL bNC /*= FALSE*/ )
{
	WFX_CONDITION(pDispatch != NULL || pParent != NULL);
	if (pDispatch == NULL)
	{
		pDispatch = pParent->searchDispather();
	}
	WFX_CONDITION(pDispatch != NULL);
	m_pDispatch = pDispatch;
	m_pDispatch->createWid(this);
	m_bNC = bNC;
	setParent(pParent);
	sendMessage(WM_CREATE, 0, 0);
	if (!rc.isEmpty())
	{
		setRect(rc);
	}
	return TRUE;
}

void Widget::setParent( Widget* pParent )
{
	WFX_CONDITION(m_pDispatch != NULL);
	m_pDispatch->setParent(this, pParent);
}

ULONG Widget::getChildren( std::vector<Widget*>& rgpChildren ) const
{
	rgpChildren = m_rgpChildren;
	return rgpChildren.size();
}

ULONG Widget::getChildren() const
{
	return m_rgpChildren.size();
}

BOOL Widget::hasChild() const
{
	return m_rgpChildren.size() != 0;
}

void Widget::invalidWid()
{
	WFX_CONDITION(getDispatcher() != NULL);
	if (!isShow())
	{
		return;
	}
	Rect rcInvalid;
	getRect(rcInvalid);
	Rect rcTemp;
	Rect rcParent;
	Widget* pParent = this;
	while(pParent = pParent->getParent())
	{
		rcTemp = rcInvalid;
		pParent->getRect(rcParent);
		if (!::IntersectRect(&rcInvalid, &rcTemp, &rcParent))
		{
			return;
		}
	}
	getDispatcher()->invalidate(rcInvalid);
}

void Widget::onDraw( HDC hdc, const Rect& rcPaint )
{
	WfxRender::drawSolidRect(hdc, getRect(), WID_BKGND_STATIC, getDispatcher());
	WfxRender::drawText(hdc, getRect(), getText(), RGB(255, 0, 0), DT_VCENTER | DT_SINGLELINE | DT_LEFT, NULL, m_pDispatch);
	WfxRender::drawFrame(hdc, getRect(), WBTN_BKGND_MOUSE, getDispatcher());
}

void Widget::setHwid( HWID hWid )
{
	m_hWid = hWid;
}

HWID Widget::getHwid() const
{
	return m_hWid;
}

ULONG Widget::addChild( Widget* pWid )
{
	std::vector<Widget*>::iterator it = 
		std::find(m_rgpChildren.begin(), m_rgpChildren.end(), pWid);
	if (it == m_rgpChildren.end())
	{
		m_rgpChildren.push_back(pWid);
	}
	return m_rgpChildren.size();
}

ULONG Widget::removeChild( Widget* pWid )
{
	std::vector<Widget*>::iterator it = 
		std::find(m_rgpChildren.begin(), m_rgpChildren.end(), pWid);
	if (it != m_rgpChildren.end())
	{
		m_rgpChildren.erase(it);
	}
	return m_rgpChildren.size();
}

void Widget::setMyParent( Widget* pParent )
{
	m_pParent = pParent;
}

void Widget::enableScrollBar( UINT uBarFlag, BOOL bEnable /*= TRUE*/ )
{
	WFX_CONDITION(getDispatcher() != NULL);
	getDispatcher()->enableScrollBar(this, uBarFlag, bEnable);
}

void Widget::setScrollBar( int nBar, ScrollBar* pScrollBar )
{
	if (SB_VERT == nBar)
	{
		m_pVScrollbar.reset(pScrollBar);
	}
	else
	{
		m_pHScrollbar.reset(pScrollBar);
	}
}

void Widget::setState( WORD wState )
{
	if (wState == getState())
	{
		return;
	}

	UnitBase::setState(wState);

	sendMessage(WM_UPDATEUISTATE, wState);
}

void Widget::showWid( WORD wShow )
{
	WFX_CONDITION(getDispatcher() != NULL);
	getDispatcher()->showWid(this, wShow);
	if (isShow())
	{
		invalidWid();
	}
}

BOOL Widget::postMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	LRESULT lResult = 0;
	return processMessage(uMsg, wParam, lParam, lResult, 0);
}

UINT_PTR Widget::setTimer( UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc /*= NULL*/)
{
	WFX_CONDITION(getDispatcher() != NULL);
	return getDispatcher()->setTimer(this, nIDEvent, uElapse, lpTimerFunc);
}

BOOL Widget::killTimer( UINT_PTR uIDEvent )
{
	WFX_CONDITION(getDispatcher() != NULL);
	return getDispatcher()->killTimer(this, uIDEvent);
}

BOOL Widget::isShow() const
{
	return (m_wShow != SW_HIDE);
}

void Widget::doshowWid( WORD wShow )
{
	m_wShow = wShow;
	sendMessage(WM_SHOWWINDOW, m_wShow);
}

Widget::operator HWID() const
{
	return m_hWid;
}

void Widget::setCapture()
{
	WFX_CONDITION(getDispatcher() != NULL);
	getDispatcher()->setCapture(this);
}

void Widget::releaseCapture()
{
	WFX_CONDITION(getDispatcher() != NULL);
	getDispatcher()->releaseCapture();
}

BOOL Widget::isWidget() const
{
	if (this == NULL)
	{
		return FALSE;
	}
	return getDispatcher()->isWidget(this);
}

BOOL Widget::isCaptured() const
{
	return getDispatcher()->isCaptured(this);
}

ScrollBar* Widget::getScrollBar( int nBar ) const
{
	if (SB_VERT == nBar)
		return m_pVScrollbar.get();
	return m_pHScrollbar.get();
}

UINT Widget::getSBFlag() const
{
	return m_uBarFlag;
}

void Widget::setSBFlag( UINT uSBFlag )
{
	m_uBarFlag = uSBFlag;
}

BOOL Widget::scrollWid( int XAmount, int YAmount )
{
	return TRUE;
}

BOOL Widget::inClientAera( const Point& pt )
{
	return m_rcClient.PtInRect(pt);
}

Rect Widget::getScrollBarRect( int nBar )
{
	Rect rcWid = getRect();
	Rect rcScrollBar = rcWid;
	if (SB_HORZ == nBar)
	{
		if (m_uBarFlag & WESB_VERT)
		{
			rcScrollBar.top = rcWid.bottom - SIZE_SCROLLBAR;
		}
		else
		{
			rcScrollBar.empty();
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
			rcScrollBar.empty();
		}
	}
	return rcScrollBar;
}

LRESULT Widget::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT Widget::onCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT Widget::onTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_wState == WID_STATE_STATIC)
	{
		setState(WID_STATE_MOUSE);
	}
	if (m_wState == WID_STATE_MOUSE)
	{
		setState(WID_STATE_PUSH);
	}
	if (m_wState == WID_STATE_PUSH)
	{
		setState(WID_STATE_STATIC);
	}
	return 0;
}

LRESULT Widget::onKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	invalidWid();
	return 0;
}

LRESULT Widget::onHScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	invalidWid();
	return 1;
}

LRESULT Widget::sendParentMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	LRESULT lResult = 0;
	if (m_pParent != NULL)
	{
		lResult = m_pParent->sendMessage(uMsg, wParam, lParam);
		if (!lResult)
		{
			Widget* pParent = m_pParent->getParent();
			if (pParent != NULL)
			{
				return pParent->sendMessage(uMsg, wParam, lParam);
			}
		}
		else
		{
			return lResult;
		}
	}
	return 0;
}

Rect Widget::getClientRect() const
{
	return m_rcClient;
}

void Widget::setClientRect( const Rect& rc )
{
	m_rcClient = rc;
}

LONG Widget::getVOffset() const
{
	if (getScrollBar(SB_VERT) != NULL)
	{
		return getScrollBar(SB_VERT)->getPos();
	}
	return 0;
}

LONG Widget::getHOffset() const
{
	if (getScrollBar(SB_HORZ) != NULL)
	{
		return getScrollBar(SB_HORZ)->getPos();
	}
	return 0;
}

UINT Widget::getID() const
{
	return m_nID;
}

void Widget::setID( UINT nID )
{
	m_nID = nID;
}

void Widget::calcVirtualSize(Size& sz)
{
}

Dispatcher* Widget::getDispatcher() const
{
	return m_pDispatch.get();
}

Dispatcher* Widget::searchDispather() const
{
	if (m_pDispatch == NULL)

	{

	}
	if (m_pDispatch != NULL)
	{
		return m_pDispatch.get();
	}
	Widget* pParent = getParent();
	Dispatcher* pDispatch = NULL;
	while(pParent != NULL)
	{
		pDispatch = pParent->getDispatcher();
		if (pDispatch != NULL)
		{
			return pDispatch;
		}
		pParent = pParent->getParent();
	}
	return pDispatch;
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
WidUnit::WidUnit(Widget* pWid /*= NULL*/)
: UnitBase(pWid)
{

}

BOOL WidUnit::processMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID )
{
	return TRUE;
}

LRESULT WidUnit::sendParentMessage( UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	if (m_pParent != NULL)
		return m_pParent->sendMessage(uMsg, wParam, lParam);
	return 0;
}

void WidUnit::onDraw( HDC hdc, const Rect& rc )
{
	WfxRender::drawSolidRect(hdc, getRect(), WBTN_BKGND_MOUSE);
	WfxRender::drawFrame(hdc, getRect(), RGB(30, 30, 30));
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
LRESULT WidCtrl::onMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (wParam == MK_LBUTTON)
		return 0;
	setState(WID_STATE_MOUSE);
	return 1;
}

LRESULT WidCtrl::onMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	setState(WID_STATE_STATIC);
	return 1;
}

LRESULT WidCtrl::onStateChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	invalidWid();
	return 1;
}

LRESULT WidCtrl::onLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	setState(WID_STATE_PUSH);
	return 1;
}

LRESULT WidCtrl::onLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	setState(WID_STATE_MOUSE);
	return 1;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
BarBase::BarBase( int nBar /*= SB_HORZ*/ )
: m_nBar(nBar)
{

}

int BarBase::getBar() const
{
	return m_nBar;
}

void BarBase::setBar( int nBar )
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

void ImageWid::setImage( WORD wState, const String& strImage )
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

void ImageWid::setImage( const String& strStatic, 
						const String& strMouse, 
						const String& strPush,
						const String& strChecked)
{
	m_pImgStatic.reset(WFX_GET_IMAGE(strStatic.c_str()));
	m_pImgMouse.reset(WFX_GET_IMAGE(strMouse.c_str()));
	m_pImgPush.reset(WFX_GET_IMAGE(strPush.c_str()));
	m_pImgChecked.reset(WFX_GET_IMAGE(strChecked.c_str()));
}

Gdiplus::Image* ImageWid::getImageFromState()
{
	Gdiplus::Image* pImage;
	switch(getState())
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

LRESULT InPlaceWid::onSetFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (initial())
	{
		return 0;
	}
	return 1;
}

void InPlaceWid::reset()
{
	m_pWindow = NULL;
}


void TextBox::setReadonly()
{
	m_wMode &= ~WID_TBM_WRITE;
}
