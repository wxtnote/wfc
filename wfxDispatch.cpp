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

TimerKey::TimerKey()
: m_hWid(INVALID_HWID)
, m_pWid(NULL)
{

}

TimerKey::TimerKey( HWID hWid, Widget* pWid )
: m_hWid(hWid)
, m_pWid(pWid)
{

}

bool TimerKey::operator()( const TimerKey& key1, const TimerKey& key2 ) const
{
	if (key1.m_hWid < key2.m_hWid)
		return true;
	else if (key1.m_hWid > key2.m_hWid)
		return false;
	else if (key1.m_pWid < key2.m_pWid )
		return true;
	else
		return false;
}

bool TimerKey::operator<( const TimerKey& key ) const
{
	if (m_hWid < key.m_hWid)
	{
		return true;
	}
	if (m_hWid > key.m_hWid)
	{
		return false;
	}
	if (m_pWid < key.m_pWid)
	{
		return true;
	}
	if (m_pWid > key.m_pWid)
	{
		return false;
	}
	return false;
}

bool TimerKey::operator==( const TimerKey& key ) const
{
	return (m_hWid == key.m_hWid) && (m_pWid == key.m_pWid);
}

bool TimerKey::operator!=( const TimerKey& key ) const
{
	return !operator==(key);
}

TimerTranslator::TimerTranslator()
: m_nTimerBase(0)
{

}

BOOL TimerTranslator::WidTimerToWndTimer( Widget* pWid, UINT_PTR nWidTimer, UINT_PTR& nWndTimer )
{
	WFX_CONDITION(pWid != NULL);
	TimerKey tmk(pWid->getHwid(), pWid);
	std::map<TimerKey, Timer2Timer>::iterator itWidtoWnd = m_WidToWnd.find(tmk);
	if (itWidtoWnd != m_WidToWnd.end())
	{
		Timer2Timer::iterator itTimer2Timer = itWidtoWnd->second.find(nWidTimer);
		if (itTimer2Timer != itWidtoWnd->second.end())
		{
			nWndTimer = itTimer2Timer->second;
		}
		else
		{
			nWndTimer = generateWndTimer();
			itWidtoWnd->second.insert(std::make_pair(nWidTimer, nWndTimer));
		}
	}
	else
	{
		Timer2Timer t2t;
		nWndTimer = generateWndTimer();
		t2t.insert(std::make_pair(nWidTimer, nWndTimer));
		m_WidToWnd.insert(std::make_pair(tmk, t2t));
	}
	TimerValue tv(tmk, nWidTimer);
	std::map<UINT_PTR, TimerValue>::iterator itWndToWid = m_WndToWid.find(nWndTimer);
	if (itWndToWid != m_WndToWid.end())
	{
		itWndToWid->second = tv;
	}
	else
	{
		m_WndToWid.insert(std::make_pair(nWndTimer, tv));
	}
	return TRUE;
}

BOOL TimerTranslator::WndTimerToWidTimer( UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer )
{
	std::map<UINT_PTR, TimerValue>::iterator itWndToWid = m_WndToWid.find(nWndTimer);
	if (itWndToWid != m_WndToWid.end())
	{
		pWid = itWndToWid->second.m_tk.m_pWid;
		nWidTimer = itWndToWid->second.m_nWidTimer;
		return TRUE;
	}
	return FALSE;
}

BOOL TimerTranslator::removeWidTimer( Widget* pWid, UINT_PTR nWidTimer )
{
	UINT_PTR nWndTimer = 0;
	WFX_CONDITION(pWid != NULL);
	TimerKey tmk(pWid->getHwid(), pWid);
	std::map<TimerKey, Timer2Timer>::iterator itWidtoWnd = m_WidToWnd.find(tmk);
	if (itWidtoWnd != m_WidToWnd.end())
	{
		Timer2Timer::iterator itTimer2Timer = itWidtoWnd->second.find(nWidTimer);
		if (itTimer2Timer != itWidtoWnd->second.end())
		{
			itWidtoWnd->second.erase(itTimer2Timer);
		}
	}

	std::map<UINT_PTR, TimerValue>::iterator itWndToWid = m_WndToWid.find(nWndTimer);
	if (itWndToWid != m_WndToWid.end())
	{
		m_WndToWid.erase(itWndToWid);
	}
	recycleWndTimer(nWndTimer);
	return TRUE;
}

BOOL TimerTranslator::removeAllWidTimer( Widget* pWid, std::vector<UINT_PTR>& rgWndTimer )
{
	rgWndTimer.clear();
	UINT_PTR nWndTimer = 0;
	WFX_CONDITION(pWid != NULL);
	TimerKey tmk(pWid->getHwid(), pWid);
	std::map<TimerKey, Timer2Timer>::iterator itWidtoWnd = m_WidToWnd.find(tmk);
	if (itWidtoWnd != m_WidToWnd.end())
	{
		for (Timer2Timer::iterator itT2T = itWidtoWnd->second.begin(); 
			itT2T != itWidtoWnd->second.end(); ++itT2T)
		{
			std::map<UINT_PTR, TimerValue>::iterator itWndToWid = m_WndToWid.find(itT2T->second);
			if (itWndToWid != m_WndToWid.end())
			{
				m_WndToWid.erase(itWndToWid);
			}
			rgWndTimer.push_back(itT2T->second);
		}
		m_WidToWnd.erase(itWidtoWnd);
	}
	return TRUE;
}

UINT_PTR TimerTranslator::generateWndTimer()
{
	return ++m_nTimerBase;
}

BOOL TimerTranslator::recycleWndTimer(UINT_PTR nWndTimer)
{
	return TRUE;
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Timer::Timer( Dispatcher* pDispatch )
: m_pDispatch(pDispatch)
, m_pTimerTranslator(new TimerTranslator)
{

}


UINT_PTR Timer::setTimer( Widget* pWid,
							UINT_PTR nWidTimer, UINT uElapse, 
							TIMERPROC lpTimerFunc )
{
	WFX_CONDITION(m_pDispatch != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(uElapse > 0);
	UINT_PTR nWndTimer = 0;
	if (!m_pTimerTranslator->WidTimerToWndTimer(pWid, nWidTimer, nWndTimer))
	{
		return 0;
	}
	return ::SetTimer(m_pDispatch->getHwnd(), nWndTimer, uElapse, lpTimerFunc);
}

BOOL Timer::killTimer( Widget* pWid, UINT_PTR nWidTimer )
{
	WFX_CONDITION(m_pDispatch != NULL);
	WFX_CONDITION(pWid!=NULL);
	UINT_PTR nWndTimer = 0;
	if (!m_pTimerTranslator->WidTimerToWndTimer(pWid, nWidTimer, nWndTimer))
	{
		return FALSE;
	}
	BOOL bRet = ::KillTimer(m_pDispatch->getHwnd(), nWndTimer);
	m_pTimerTranslator->removeWidTimer(pWid, nWidTimer);
	return bRet;
}

BOOL Timer::getWidgetFromTimer( UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer )
{
	if (m_pTimerTranslator->WndTimerToWidTimer(nWndTimer, pWid, nWidTimer))
	{
		return TRUE;
	}
	return FALSE;
}

void Timer::destroy( Widget* pWid )
{
	std::vector<UINT_PTR> rgWndTimer;
	m_pTimerTranslator->removeAllWidTimer(pWid, rgWndTimer);
	for (ULONG i = 0; i < rgWndTimer.size(); i++)
	{
		::KillTimer(m_pDispatch->getHwnd(), rgWndTimer[i]);
	}
}

HWID Dispatcher::s_hWidBase = INVALID_HWID;
std::vector<Dispatcher*> Dispatcher::s_rgpDispatch;

HINSTANCE Dispatcher::s_hInstance = NULL;
HWND Dispatcher::s_hMainWnd = NULL;
int Dispatcher::s_nAlpha = 255;
short Dispatcher::s_nL = 180;
short Dispatcher::s_nS = 100;
short Dispatcher::s_nH = 100;

Dispatcher::Dispatcher( HWND hWnd /*= NULL*/ )
: m_hWnd(hWnd)
, m_pToolTipWnd(new ToolTipWnd)
{
	clearH2O(m_h2oLastMouseMove);
	clearH2O(m_h2oCaptured);
	clearH2O(m_h2oFocused);
	clearH2O(m_h2oLButtonDown);
	m_pTimer.reset(new Timer(this));
	s_rgpDispatch.push_back(this);
	m_bMouseTracking = FALSE;
}

Dispatcher::~Dispatcher()
{
	std::vector<Dispatcher*>::iterator 
		it = std::find(s_rgpDispatch.begin(), s_rgpDispatch.end(), this);

	if (it != s_rgpDispatch.end())
	{
		s_rgpDispatch.erase(it);
	}
}

void Dispatcher::setHwnd( HWND hWnd )
{
	m_hWnd = hWnd;
}

HWND Dispatcher::getHwnd() const
{
	return m_hWnd;
}

BOOL Dispatcher::createWid( Widget* pThis )
{
	WFX_CONDITION(pThis != NULL);
	WFX_CONDITION(pThis->getHwid() == INVALID_HWID);
	if (pThis->getHwid() != INVALID_HWID)
	{
		return FALSE;
	}
	HWID hNewWid = generateHwid();
	WFX_CONDITION(hNewWid != INVALID_HWID);
	pThis->setHwid(hNewWid);
	WFX_CONDITION(m_Handle2Object.find(hNewWid) == m_Handle2Object.end());
	m_Handle2Object.insert(std::make_pair(hNewWid, pThis));
	return TRUE;
}

BOOL Dispatcher::destroy( HWID& hWid )
{
	std::map<HWID, Widget*>::iterator it =
		m_Handle2Object.find(hWid);
	if (it == m_Handle2Object.end())
	{
		return FALSE;
	}
	Widget* pWid = it->second;
	WFX_CONDITION(pWid != NULL);
	Widget* pParent = pWid->getParent();
	if (pParent != NULL)
	{
		pParent->removeChild(pWid);
	}
	m_Handle2Object.erase(it);
	std::map<HWID, Widget*>::iterator itPaint = 
		m_h2oOrphan.find(hWid);
	if (itPaint != m_h2oOrphan.end())
	{
		m_h2oOrphan.erase(itPaint);
	}
	if (m_h2oCaptured.first == hWid)
	{
		clearH2O(m_h2oCaptured);
	}
	m_pTimer->destroy(pWid);
	recycleHwid(hWid);
	return TRUE;
}

HWID Dispatcher::generateHwid()
{
	return ++s_hWidBase;
}

void Dispatcher::recycleHwid( HWID& hWid )
{
	hWid = INVALID_HWID;
}

void Dispatcher::drawWid( Widget* pWid, const Rect& rcPaint )
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	if (!pWid->isShow())
	{
		return;
	}

	__begin_mem_draw
	WfxRender::MemDC drawdc(m_hWnd, rcPaint);
	drawBkgnd(pWid, drawdc, rcPaint);
	drawGen(pWid, drawdc, rcPaint);
	__end_mem_draw;
}

void Dispatcher::drawGen( Widget* pWid, HDC hdc, const Rect& rcPaint)
{
	WFX_CONDITION(pWid != NULL);
	Rect rcTemp;
	Rect rcItem = pWid->getRect();

	if (!::IntersectRect(&rcTemp, &rcPaint, &rcItem))
		return;
	if (!pWid->isShow())
		return;

	WfxRender::RenderClip clip(hdc, rcItem);
	pWid->onDraw(hdc, rcPaint);
	WfxRender::drawFrame(hdc, rcItem, WBTN_BKGND_MOUSE);

	std::vector<Widget*> rgpChildren;
	pWid->getChildren(rgpChildren);
	for (std::vector<Widget*>::iterator it = 
		rgpChildren.begin(); it != rgpChildren.end(); ++it)
	{
		WFX_CONDITION((*it) != NULL);
		if (!::IntersectRect(&rcTemp, &rcItem, &(*it)->getRect()))
			continue;
		drawGen((*it), hdc, rcPaint);
	}
}

LRESULT Dispatcher::handleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 1;
	Point pt(lParam);
	Widget* pWid = NULL;

	if (uMsg != WM_PAINT
		&& uMsg != WM_TIMER
		&& uMsg != WM_LBUTTONUP)

	{
		pWid = getObject(m_h2oCaptured);
		if (pWid != NULL)
		{
			lResult = pWid->sendMessage(uMsg, wParam, lParam);
			return lResult;
		}
	}
	processMessage(uMsg, wParam, lParam, lResult, 0);
	return lResult;
	
}

LRESULT Dispatcher::onEraseBkgnd( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

LRESULT Dispatcher::onPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcPaint;
	if( !::GetUpdateRect(m_hWnd, &rcPaint, FALSE) ) 
		return 1;
	PAINTSTRUCT ps;
	::BeginPaint(m_hWnd, &ps);
	onPaint(ps.rcPaint);
	::EndPaint(m_hWnd, &ps);
	return 1;
}

LRESULT Dispatcher::onMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 500;
		_TrackMouseEvent(&tme);
		m_bMouseTracking = TRUE;
	}
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	pWid = getWidPt(pt);
	if (pWid == NULL)
	{
		pWid = getObject(m_h2oLastMouseMove);
		if (pWid != NULL)
		{
			pWid->sendMessage(WM_MOUSELEAVE);
			clearH2O(m_h2oLastMouseMove);
		}
	}
	else
	{
		HWID hWidNowMouse = pWid->getHwid();
		if (m_h2oLastMouseMove.first != hWidNowMouse)
		{
			Widget* pLastMouse = getObject(m_h2oLastMouseMove);
			if (pLastMouse != NULL)
			{
				pLastMouse->sendMessage(WM_MOUSELEAVE);
			}
			m_pToolTipWnd->endToolTip();
			setMouseMoveH2O(std::make_pair(hWidNowMouse, pWid));
		}
		lResult = pWid->sendMessage(WM_MOUSEMOVE, wParam, lParam);
	}
	return lResult;
}

LRESULT Dispatcher::onMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bMouseTracking = FALSE;
	LRESULT lResult = 0;
	Widget* pWid = getObject(m_h2oLastMouseMove);
	if (pWid != NULL)
	{
		lResult = pWid->sendMessage(uMsg, wParam, lParam);
	}
	clearH2O(m_h2oLastMouseMove);
	m_pToolTipWnd->endToolTip();
	return lResult;
}

LRESULT Dispatcher::onMouseHOver( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bMouseTracking = FALSE;
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	pWid = getWidPt(pt);
	if (pWid != NULL)
	{
		String strTootip;
		pWid->getToolTip(strTootip);
		lResult = pWid->sendMessage(uMsg, wParam, lParam);
		m_pToolTipWnd->doToolTip(getHwnd(), pWid->getRect(), strTootip);
	}
	return lResult;
}

LRESULT Dispatcher::onNotify( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LRESULT lResult = 0;
	LPNMHDR lpNMHDR = (LPNMHDR) lParam;
	if( lpNMHDR != NULL ) lResult = ::SendMessageW(lpNMHDR->hwndFrom, OCM__BASE + uMsg, wParam, lParam);
	return lResult;
}

LRESULT Dispatcher::onKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}


LRESULT Dispatcher::onLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	WFX_CONDITION(m_hWnd != NULL);
	::SetFocus(m_hWnd);
	pWid = getWidPt(pt);
	if (pWid != NULL)
	{
		if (pWid->getHwid() != m_h2oFocused.first
			&& pWid != m_h2oFocused.second)
		{
			if (m_h2oFocused.second != NULL)
			{
				m_h2oFocused.second->sendMessage(WM_KILLFOCUS, (WPARAM)pWid->getHwid());
			}
		}
		pWid->sendMessage(WM_SETFOCUS, (WPARAM)m_h2oFocused.first);
		setCapture(pWid);
		setFocus(pWid);
		setLButtonDown(pWid);
		lResult = pWid->sendMessage(uMsg, wParam, lParam);
	}
	else
	{
		if (m_h2oFocused.second != NULL)
		{
			m_h2oFocused.second->sendMessage(WM_KILLFOCUS, INVALID_HWID);
		}
	}
	return lResult;
}

LRESULT Dispatcher::onLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	pWid = getObject(m_h2oCaptured);
	if (pWid == NULL)
	{
		pWid = getWidPt(pt);
	}
	if (m_h2oCaptured.first != INVALID_HWID)
	{
		releaseCapture();
		clearH2O(m_h2oCaptured);
	}
	if (pWid != NULL)
	{
		lResult = pWid->sendMessage(uMsg,  wParam, lParam);
		Widget* pLastMM = getObject(m_h2oLastMouseMove);
		if (pLastMM != NULL)
		{
			pLastMM->sendMessage(WM_MOUSELEAVE);
		}
		clearH2O(m_h2oLastMouseMove);
	}
	pWid = getWidPt(pt);
	if (pWid != NULL
		&& pWid->isShow()
		&& m_h2oLButtonDown.first == pWid->getHwid()
		&& m_h2oLButtonDown.second == pWid)
	{
		pWid->sendMessage(WUM_LBUTTONCLICK, wParam, lParam);
	}
	clearH2O(m_h2oLButtonDown);
	return lResult;
}

LRESULT Dispatcher::onKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	pWid = getObject(m_h2oFocused);
	if (pWid != NULL)
	{
		lResult = pWid->sendMessage(uMsg, wParam, lParam);
	}
	return lResult;
}

LRESULT Dispatcher::onMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	pWid = getObject(m_h2oFocused);
	if (pWid != NULL)
	{
		lResult = pWid->sendMessage(uMsg, wParam, lParam);
	}
	return lResult;
}

LRESULT Dispatcher::onCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	if (lParam == 0)
	{
		return lResult;
	}
	HWND hWndChild = (HWND)lParam;
	lResult = ::SendMessageW(hWndChild, OCM__BASE + uMsg, wParam, lParam);
	return lResult;
}

LRESULT Dispatcher::onTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	UINT_PTR nWidTimer = 0;
	if (getWidgetFromTimer(wParam, pWid, nWidTimer))
	{
		lResult = pWid->sendMessage(WM_TIMER, nWidTimer);
	}
	return lResult;
}

Widget* Dispatcher::getWidPt( const Point& pt )
{
	//Widget* pWid = NULL;
	//std::vector<Widget*> rgpWidInPt;
	//for (std::map<HWID, Widget*>::iterator it = m_Handle2Object.begin();
	//	it != m_Handle2Object.end(); ++it)
	//{
	//	WFX_CONDITION(it->second != NULL);
	//	if (!it->second->isShow())
	//	{
	//		continue;
	//	}
	//	Rect rcWid = it->second->getRect();
	//	if (rcWid.PtInRect(pt))
	//	{
	//		rgpWidInPt.push_back(it->second);
	//	}
	//}
	//if (rgpWidInPt.size() > 0)
	//{
	//	///TRACE(L"%d", rgpWidInPt.size());
	//	//WFX_CONDITION(rgpWidInPt.size() != 4);
	//	if (rgpWidInPt.size() != 1)
	//	{
	//		pWid = getWidPt(rgpWidInPt);
	//	}
	//	else
	//	{
	//		pWid = rgpWidInPt[0];
	//	}
	//}

	//return pWid;
	Widget* pWidget = NULL;
	for (std::map<HWID, Widget*>::iterator it = m_h2oOrphan.begin();
		it != m_h2oOrphan.end(); ++it)
	{
		WFX_CONDITION(it->second != NULL);
		pWidget = getWidPt(it->second, pt);
		if (pWidget != NULL)
		{
			break;
		}
	}
	return pWidget;
}

Widget* Dispatcher::getWidPt(Widget* pWidget, const Point& pt)
{
	WFX_CONDITION(pWidget != NULL);
	Widget* pWigetFound = NULL;
	if (pWidget->getRect().PtInRect(pt))
	{
		if (!pWidget->hasChild())
		{
			return pWidget;
		}
		std::vector<Widget*> rgpChildren;
		pWidget->getChildren(rgpChildren);
		for (std::vector<Widget*>::iterator it = 
			rgpChildren.begin(); it != rgpChildren.end(); ++it)
		{
			WFX_CONDITION((*it) != NULL);
			pWigetFound = getWidPt((*it), pt);
			if (pWigetFound != NULL)
			{
				return pWigetFound;
			}
		}
		if (pWigetFound == NULL)
		{
			return pWidget;
		}
	}
	
	return NULL;
}

Widget* Dispatcher::getWidPt(const std::vector<Widget*>& rgpWid)
{
	Widget* pWid = NULL;
	std::vector<Widget*>::const_iterator it = rgpWid.begin();
	if (it != rgpWid.end())
	{
		pWid = *it;
		WFX_CONDITION(pWid != NULL);
		Rect rcWid = pWid->getRect();
		float fArea = (rcWid.right - rcWid.left) * (rcWid.bottom - rcWid.top);
		float fMinArea = fArea;
		for (; it != rgpWid.end(); ++it)
		{
			rcWid = pWid->getRect();
			fArea = (rcWid.right - rcWid.left) * (rcWid.bottom - rcWid.top);
			if (fArea < fMinArea)
			{
				fMinArea = fArea;
			}
			pWid = (*it);
		}
	}
	return pWid;
}

HINSTANCE Dispatcher::getInstance()
{
	return s_hInstance;
}

void Dispatcher::setInstance( HINSTANCE hInstance )
{
	s_hInstance = hInstance;
}

HWND Dispatcher::getMainWnd()
{
	return s_hMainWnd;
}

void Dispatcher::setMainWnd( HWND hMainWnd )
{
	s_hMainWnd = hMainWnd;
}

BOOL Dispatcher::setParent( Widget* pThis, Widget* pParent )
{
	WFX_CONDITION(pThis != NULL);
	if (pParent == NULL)
	{
		HWID hWid = pThis->getHwid();
		std::map<HWID, Widget*>::iterator it = 
			m_h2oOrphan.find(hWid);
		if (it == m_h2oOrphan.end())
		{
			m_h2oOrphan.insert(std::make_pair(hWid, pThis));
		}
		return FALSE;
	}

	ULONG nChilren = pParent->getChildren();
	if (pParent->addChild(pThis) != nChilren + 1)
	{
		return FALSE;
	}
	pThis->setMyParent(pParent);
	return TRUE;
}

Widget* Dispatcher::fromHwid( HWID hWid ) const
{
	std::map<HWID, Widget*>::const_iterator it = 
		m_Handle2Object.find(hWid);
	if (it != m_Handle2Object.end())
	{
		return it->second;
	}
	return NULL;
}

int Dispatcher::messageLoop()
{
	MSG msg = {0};
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

void Dispatcher::getHSL( short& nH, short& nS, short& nL )
{
	nH = s_nH;
	nS = s_nS;
	nL = s_nL;
}

void Dispatcher::setHSL( short nH, short nS, short nL )
{
	s_nH = nH;
	s_nS = nS;
	s_nL = nL;
}

int Dispatcher::getAlpha()
{
	return s_nAlpha;
}

void Dispatcher::setAlpha( int nAlpha )
{
	if (nAlpha < 0)
	{
		nAlpha = 0;
	}
	if (nAlpha > 255)
	{
		nAlpha = 255;
	}
	s_nAlpha = nAlpha;
	HWND hWnd = NULL;
	for (std::vector<Dispatcher*>::iterator it = s_rgpDispatch.begin();
		it != s_rgpDispatch.end(); ++it)
	{
		WFX_CONDITION((*it) != NULL);
		hWnd = (*it)->getHwnd();
		setAlpha(s_nAlpha, hWnd);
	}
}

void Dispatcher::setAlpha( int nAlpha, HWND hWnd )
{
	if (!::IsWindow(hWnd))
	{
		return;
	}
	typedef BOOL (__stdcall *PFUNCSETLAYEREDWINDOWATTR)(HWND, COLORREF, BYTE, DWORD);
	PFUNCSETLAYEREDWINDOWATTR fsetLayeredWindowAttributes;

	HMODULE hUser32 = ::GetModuleHandleW(L"User32.dll");
	if (hUser32)
	{
		fsetLayeredWindowAttributes = 
			(PFUNCSETLAYEREDWINDOWATTR)::GetProcAddress(hUser32, "SetLayeredWindowAttributes");
		if( fsetLayeredWindowAttributes == NULL ) return;
	}

	DWORD dwStyle = ::GetWindowLongW(hWnd, GWL_EXSTYLE);
	DWORD dwNewStyle = dwStyle;
	if( nAlpha >= 0 && nAlpha < 256 ) dwNewStyle |= WS_EX_LAYERED;
	else dwNewStyle &= ~WS_EX_LAYERED;
	if(dwStyle != dwNewStyle) ::SetWindowLongW(hWnd, GWL_EXSTYLE, dwNewStyle);
	fsetLayeredWindowAttributes(hWnd, 0, nAlpha, LWA_ALPHA);
}

void Dispatcher::onPaint(const Rect& rcPaint)
{
	// Note: only orphans need to handle WM_PAINT message,
	// parents will handle it for their children.
	for (std::map<HWID, Widget*>::iterator it = m_h2oOrphan.begin();
		it != m_h2oOrphan.end(); ++it)
	{
		WFX_CONDITION(it->second != NULL);
		drawWid(it->second, rcPaint);
	}
}

void Dispatcher::showWid( Widget* pWid, WORD wShow )
{
	WFX_CONDITION(pWid != NULL);
	pWid->doshowWid(wShow);
	std::vector<Widget*> rgpChilren;
	pWid->getChildren(rgpChilren);
	for (std::vector<Widget*>::iterator it = 
		rgpChilren.begin(); it != rgpChilren.end(); ++it)
	{
		showWid((*it), wShow);
	}
}

void Dispatcher::setCapture( Widget* pWid )
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(*pWid != INVALID_HWID);
	m_h2oCaptured = std::make_pair(pWid->getHwid(), pWid);
	::SetCapture(m_hWnd);
}

BOOL Dispatcher::isCaptured( const Widget* pWid ) const
{
	WFX_CONDITION(pWid != NULL);
	return (pWid == m_h2oCaptured.second) && (pWid->getHwid() == m_h2oCaptured.first);
}

void Dispatcher::setFocus(Widget* pWid)
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(*pWid != INVALID_HWID);
	m_h2oFocused = std::make_pair(pWid->getHwid(), pWid);
}

void Dispatcher::setLButtonDown(Widget* pWid)
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(*pWid != INVALID_HWID);
	m_h2oLButtonDown = std::make_pair(pWid->getHwid(), pWid);
}
void Dispatcher::releaseCapture()
{
	clearH2O(m_h2oCaptured);
	::ReleaseCapture();
}

BOOL Dispatcher::isWidget( const Widget* pWidget ) const
{
	if (this == NULL)
	{
		return FALSE;
	}
	if (pWidget == NULL)
	{
		return FALSE;
	}
	if (pWidget->getHwid() == INVALID_HWID)
	{
		return FALSE;
	}
	return TRUE;
}

void Dispatcher::clearH2O( std::pair<HWID, Widget*>& h2o )
{
	h2o.first = INVALID_HWID;
	h2o.second = NULL;
}

Widget* Dispatcher::getObject( const std::pair<HWID, Widget*>& h2o )
{
	if (h2o.first != INVALID_HWID)
	{
		WFX_CONDITION(h2o.second != NULL);
		return h2o.second;
	}
	return NULL;
}

void Dispatcher::setMouseMoveH2O( const std::pair<HWID, Widget*>& h2o )
{
	m_h2oLastMouseMove.first = h2o.first;
	m_h2oLastMouseMove.second = h2o.second;
}

void Dispatcher::setCapturedH2O( const std::pair<HWID, Widget*>& h2o )
{
	m_h2oCaptured.first = h2o.first;
	m_h2oCaptured.second = h2o.second;
}

void Dispatcher::enableScrollBar( Widget* pWid, UINT uBarFlag, BOOL bEnable /*= TRUE*/ )
{
	WFX_CONDITION(pWid != NULL);
	Rect rcWid = pWid->getRect();
	Rect rcSB;
	Size szVirtual = pWid->getVirtualSize();
	if (uBarFlag == WESB_BOTH)
	{
		if (bEnable)
		{
			ScrollBar* pBar = pWid->getScrollBar(SB_VERT);
			if (pBar == NULL)
			{
				pBar = new ScrollBar(SB_VERT);
				pBar->setRange(0, szVirtual.cy);
				rcSB = pWid->getScrollBarRect(SB_VERT);
				pBar->create(rcSB, pWid);
				pWid->setScrollBar(SB_VERT, pBar);
			}
			pBar = pWid->getScrollBar(SB_HORZ);

			if (pBar == NULL)
			{
				pBar = new ScrollBar(SB_HORZ);
				pBar->setRange(0, szVirtual.cx);
				rcSB = pWid->getScrollBarRect(SB_HORZ);
				pBar->create(rcSB, pWid);
				pWid->setScrollBar(SB_HORZ, pBar);
			}
			pWid->setSBFlag(WESB_BOTH);
			rcWid = pWid->getRect();
		}
		else
		{
			ScrollBar* pBar = pWid->getScrollBar(SB_VERT);
			TDEL(pBar);
			pBar = pWid->getScrollBar(SB_HORZ);
			TDEL(pBar);
			pWid->setSBFlag(WESB_NONE);
			rcWid = pWid->getRect();
		}
	}
	else if (uBarFlag == WESB_HORZ)
	{
		if (bEnable)
		{
			ScrollBar* pBar = pWid->getScrollBar(SB_HORZ);
			if (pBar == NULL)
			{
				pBar = new ScrollBar(SB_HORZ);
				pBar->setRange(0, szVirtual.cx);
				rcSB = pWid->getScrollBarRect(SB_HORZ);
				pBar->create(rcSB, pWid);
				pWid->setScrollBar(SB_HORZ, pBar);
			}
			pWid->setSBFlag(pWid->getSBFlag() | WESB_HORZ);
			rcWid = pWid->getRect();
		}
		else
		{
			ScrollBar* pBar = pWid->getScrollBar(SB_HORZ);
			TDEL(pBar);
			pWid->setSBFlag(pWid->getSBFlag() & ~WESB_HORZ);
			rcWid = pWid->getRect();
		}
	}
	else if (uBarFlag == WESB_VERT)
	{
		if (bEnable)
		{
			ScrollBar* pBar = pWid->getScrollBar(WESB_VERT);
			if (pBar == NULL)
			{
				pBar = new ScrollBar(WESB_VERT);
				pBar->setRange(0, szVirtual.cy);
				rcSB = pWid->getScrollBarRect(WESB_VERT);
				pBar->create(rcSB, pWid);
				pWid->setScrollBar(WESB_VERT, pBar);
			}
			pWid->setSBFlag(pWid->getSBFlag() | WESB_VERT);
			rcWid = pWid->getRect();
		}
		else
		{
			ScrollBar* pBar = pWid->getScrollBar(WESB_VERT);
			TDEL(pBar);
			pWid->setSBFlag(pWid->getSBFlag() & ~WESB_VERT);
			rcWid = pWid->getRect();
		}
	}
	else
	{
		WFX_CONDITION(FALSE);
	}
}

void Dispatcher::setScrollInfo( Widget* pWid, int nBar, LPCSCROLLINFO lpsi, BOOL redraw )
{

}

void Dispatcher::getScrollInfo( Widget* pWid, int nBar, LPSCROLLINFO lpsi )
{

}

void Dispatcher::preProcessMsg( Widget* pWid, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

}

void Dispatcher::setWidRect( Widget* pWid, const Rect& rc )
{
	WFX_CONDITION(pWid != NULL);

	Rect rcWid;
	rcWid = pWid->getRect();
	pWid->setVirtualSizeCached(FALSE);
	Rect rcSB = rcWid;
	Rect rcDraw = rcWid;
	Size szVirtual = pWid->getVirtualSize();
	Size szView(rcWid.getWidth(), rcWid.getHeight());
	BOOL bOutRangeHorz = szView.cx < szVirtual.cx;
	BOOL bOutRangeVert = szView.cy < szVirtual.cy;
	if (pWid->getSBFlag() & WESB_VERT)
	{
		WFX_CONDITION(pWid->getScrollBar(SB_VERT) != NULL);
		if (!bOutRangeVert)
		{
			rcSB.empty();
			pWid->getScrollBar(SB_VERT)->reset();
		}
		else
		{
			rcSB = pWid->getScrollBarRect(SB_VERT);
		}
		pWid->getScrollBar(SB_VERT)->setRect(rcSB);
		rcDraw.right -= rcSB.right - rcSB.left;
	}
	rcSB = rcWid;
	if (pWid->getSBFlag() & WESB_HORZ)
	{
		WFX_CONDITION(pWid->getScrollBar(SB_HORZ) != NULL);
		if (!bOutRangeHorz)
		{
			rcSB.empty();
			pWid->getScrollBar(SB_HORZ)->reset();
		}
		else
		{
			rcSB = pWid->getScrollBarRect(SB_HORZ);
		}
		pWid->getScrollBar(SB_HORZ)->setRect(rcSB);
		rcDraw.bottom -= rcSB.bottom - rcSB.top;
	}
	pWid->setClientRect(rcDraw);
	pWid->sendMessage(WM_SIZE, 0, 0);
}

UINT_PTR Dispatcher::setTimer( Widget* pWid, UINT_PTR nWidTimer, UINT uElapse, TIMERPROC lpTimerFunc )
{
	return m_pTimer->setTimer(pWid, nWidTimer, uElapse, lpTimerFunc);
}

BOOL Dispatcher::killTimer( Widget* pWid, UINT_PTR nWidTimer )
{
	return m_pTimer->killTimer(pWid, nWidTimer);
}

BOOL Dispatcher::getWidgetFromTimer( UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer )
{
	return m_pTimer->getWidgetFromTimer(nWndTimer, pWid, nWidTimer);
}

void Dispatcher::invalidate( const Rect& rc )
{
	::InvalidateRect(m_hWnd, &rc, FALSE);
}

void Dispatcher::drawBkgnd( Widget* pWid, HDC hdc, const Rect& rc )
{
	//WFX_CONDITION(pWid != NULL);
	//Widget* pParent = pWid->getParent();

	//Gdiplus::Color clrBkgnd;
	//if (pParent != NULL)
	//{
	//	clrBkgnd = pParent->getBkgnd();
	//	Gdiplus::SolidBrush bkgnd(clrBkgnd);
	//	grph.FillRectangle(&bkgnd, rc);
	//	Gdiplus::Pen pnbkgnd(clrBkgnd);
	//	grph.DrawRectangle(&pnbkgnd, rc);
	//}
	//else
	//{
	//	//clrBkgnd.setFromCOLORREF(::getBkColor(grph.getHDC()));
	//}
}
