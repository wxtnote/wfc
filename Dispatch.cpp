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
#include "stdafx.h"
#include "wfxwid.h"
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
	TimerKey tmk(pWid->GetHwid(), pWid);
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
			nWndTimer = GenerateWndTimer();
			itWidtoWnd->second.insert(std::make_pair(nWidTimer, nWndTimer));
		}
	}
	else
	{
		Timer2Timer t2t;
		nWndTimer = GenerateWndTimer();
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

BOOL TimerTranslator::RemoveWidTimer( Widget* pWid, UINT_PTR nWidTimer )
{
	UINT_PTR nWndTimer = 0;
	WFX_CONDITION(pWid != NULL);
	TimerKey tmk(pWid->GetHwid(), pWid);
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
	RecycleWndTimer(nWndTimer);
	return TRUE;
}

BOOL TimerTranslator::RemoveAllWidTimer( Widget* pWid, std::vector<UINT_PTR>& rgWndTimer )
{
	rgWndTimer.clear();
	UINT_PTR nWndTimer = 0;
	WFX_CONDITION(pWid != NULL);
	TimerKey tmk(pWid->GetHwid(), pWid);
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

UINT_PTR TimerTranslator::GenerateWndTimer()
{
	return ++m_nTimerBase;
}

BOOL TimerTranslator::RecycleWndTimer(UINT_PTR nWndTimer)
{
	return TRUE;
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Timer::Timer( Dispatcher* pDispatch )
: m_pDispatch(pDispatch)
, m_pTimerTranslator(new TimerTranslator)
{

}


UINT_PTR Timer::SetWidTimer( Widget* pWid,
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
	return ::SetTimer(m_pDispatch->GetHwnd(), nWndTimer, uElapse, lpTimerFunc);
}

BOOL Timer::KillWidTimer( Widget* pWid, UINT_PTR nWidTimer )
{
	WFX_CONDITION(m_pDispatch != NULL);
	WFX_CONDITION(pWid!=NULL);
	UINT_PTR nWndTimer = 0;
	if (!m_pTimerTranslator->WidTimerToWndTimer(pWid, nWidTimer, nWndTimer))
	{
		return FALSE;
	}
	BOOL bRet = ::KillTimer(m_pDispatch->GetHwnd(), nWndTimer);
	m_pTimerTranslator->RemoveWidTimer(pWid, nWidTimer);
	return bRet;
}

BOOL Timer::GetWidgetFromTimer( UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer )
{
	if (m_pTimerTranslator->WndTimerToWidTimer(nWndTimer, pWid, nWidTimer))
	{
		return TRUE;
	}
	return FALSE;
}

void Timer::Destroy( Widget* pWid )
{
	std::vector<UINT_PTR> rgWndTimer;
	m_pTimerTranslator->RemoveAllWidTimer(pWid, rgWndTimer);
	for (ULONG i = 0; i < rgWndTimer.size(); i++)
	{
		::KillTimer(m_pDispatch->GetHwnd(), rgWndTimer[i]);
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
	ClearH2O(m_h2oLastMouseMove);
	ClearH2O(m_h2oCaptured);
	ClearH2O(m_h2oFocused);
	ClearH2O(m_h2oLButtonDown);
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

void Dispatcher::SetHwnd( HWND hWnd )
{
	m_hWnd = hWnd;
}

HWND Dispatcher::GetHwnd() const
{
	return m_hWnd;
}

BOOL Dispatcher::Create( Widget* pThis )
{
	WFX_CONDITION(pThis != NULL);
	WFX_CONDITION(pThis->GetHwid() == INVALID_HWID);
	if (pThis->GetHwid() != INVALID_HWID)
	{
		return FALSE;
	}
	HWID hNewWid = GenerateHwid();
	WFX_CONDITION(hNewWid != INVALID_HWID);
	pThis->SetHwid(hNewWid);
	WFX_CONDITION(m_Handle2Object.find(hNewWid) == m_Handle2Object.end());
	m_Handle2Object.insert(std::make_pair(hNewWid, pThis));
	return TRUE;
}

BOOL Dispatcher::Destroy( HWID& hWid )
{
	std::map<HWID, Widget*>::iterator it =
		m_Handle2Object.find(hWid);
	if (it == m_Handle2Object.end())
	{
		return FALSE;
	}
	Widget* pWid = it->second;
	WFX_CONDITION(pWid != NULL);
	Widget* pParent = pWid->GetParent();
	if (pParent != NULL)
	{
		pParent->RemoveChild(pWid);
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
		ClearH2O(m_h2oCaptured);
	}
	m_pTimer->Destroy(pWid);
	RecycleHwid(hWid);
	return TRUE;
}

HWID Dispatcher::GenerateHwid()
{
	return ++s_hWidBase;
}

void Dispatcher::RecycleHwid( HWID& hWid )
{
	hWid = INVALID_HWID;
}

void Dispatcher::DrawWid( Widget* pWid, const Rect& rcPaint )
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	if (!pWid->IsShow())
	{
		return;
	}

	__begin_mem_draw
	WfxRender::MemDC drawdc(m_hWnd, rcPaint);
	DrawBkgnd(pWid, drawdc, rcPaint);
	DrawGen(pWid, drawdc, rcPaint);
	__end_mem_draw;
}

void Dispatcher::DrawGen( Widget* pWid, HDC hdc, const Rect& rcPaint)
{
	WFX_CONDITION(pWid != NULL);
	Rect rcTemp;
	Rect rcItem = pWid->GetRect();

	if (!::IntersectRect(&rcTemp, &rcPaint, &rcItem))
		return;
	if (!pWid->IsShow())
		return;

	WfxRender::RenderClip clip(hdc, rcItem);
	pWid->OnDraw(hdc, rcPaint);

	std::vector<Widget*> rgpChildren;
	pWid->GetChildren(rgpChildren);
	for (std::vector<Widget*>::iterator it = 
		rgpChildren.begin(); it != rgpChildren.end(); ++it)
	{
		WFX_CONDITION((*it) != NULL);
		if (!::IntersectRect(&rcTemp, &rcItem, &(*it)->GetRect()))
			continue;
		DrawGen((*it), hdc, rcPaint);
	}
}

LRESULT Dispatcher::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = 1;
	Point pt(lParam);
	Widget* pWid = NULL;

	if (uMsg != WM_PAINT
		&& uMsg != WM_TIMER
		&& uMsg != WM_LBUTTONUP)

	{
		pWid = GetObject(m_h2oCaptured);
		if (pWid != NULL)
		{
			lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
			return lResult;
		}
	}
	ProcessMessage(uMsg, wParam, lParam, lResult, 0);
	return lResult;
	
}

LRESULT Dispatcher::OnEraseBkgnd( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

LRESULT Dispatcher::OnPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rcPaint;
	if( !::GetUpdateRect(m_hWnd, &rcPaint, FALSE) ) 
		return 1;
	PAINTSTRUCT ps;
	::BeginPaint(m_hWnd, &ps);
	OnPaint(ps.rcPaint);
	::EndPaint(m_hWnd, &ps);
	return 1;
}

LRESULT Dispatcher::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
	pWid = GetWidPt(pt);
	if (pWid == NULL)
	{
		pWid = GetObject(m_h2oLastMouseMove);
		if (pWid != NULL)
		{
			pWid->SendWidMessage(WM_MOUSELEAVE);
			ClearH2O(m_h2oLastMouseMove);
		}
	}
	else
	{
		HWID hWidNowMouse = pWid->GetHwid();
		if (m_h2oLastMouseMove.first != hWidNowMouse)
		{
			Widget* pLastMouse = GetObject(m_h2oLastMouseMove);
			if (pLastMouse != NULL)
			{
				pLastMouse->SendWidMessage(WM_MOUSELEAVE);
			}
			m_pToolTipWnd->EndToolTip();
			SetMouseMoveH2O(std::make_pair(hWidNowMouse, pWid));
		}
		lResult = pWid->SendWidMessage(WM_MOUSEMOVE, wParam, lParam);
	}
	return lResult;
}

LRESULT Dispatcher::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bMouseTracking = FALSE;
	LRESULT lResult = 0;
	Widget* pWid = GetObject(m_h2oLastMouseMove);
	if (pWid != NULL)
	{
		lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
	}
	ClearH2O(m_h2oLastMouseMove);
	m_pToolTipWnd->EndToolTip();
	return lResult;
}

LRESULT Dispatcher::OnMouseHOver( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bMouseTracking = FALSE;
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	pWid = GetWidPt(pt);
	if (pWid != NULL)
	{
		lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
		m_pToolTipWnd->DoToolTip(GetHwnd(), pWid->GetRect(), pWid->GetToolTip());
	}
	return lResult;
}

LRESULT Dispatcher::OnNotify( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LRESULT lResult = 0;
	LPNMHDR lpNMHDR = (LPNMHDR) lParam;
	if( lpNMHDR != NULL ) lResult = ::SendMessageW(lpNMHDR->hwndFrom, OCM__BASE + uMsg, wParam, lParam);
	return lResult;
}

LRESULT Dispatcher::OnKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}


LRESULT Dispatcher::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	WFX_CONDITION(m_hWnd != NULL);
	::SetFocus(m_hWnd);
	pWid = GetWidPt(pt);
	if (pWid != NULL)
	{
		if (pWid->GetHwid() != m_h2oFocused.first
			&& pWid != m_h2oFocused.second)
		{
			if (m_h2oFocused.second != NULL)
			{
				m_h2oFocused.second->SendWidMessage(WM_KILLFOCUS, (WPARAM)pWid->GetHwid());
			}
		}
		pWid->SendWidMessage(WM_SETFOCUS, (WPARAM)m_h2oFocused.first);
		SetCapture(pWid);
		SetFocus(pWid);
		SetLButtonDown(pWid);
		lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
	}
	else
	{
		if (m_h2oFocused.second != NULL)
		{
			m_h2oFocused.second->SendWidMessage(WM_KILLFOCUS, INVALID_HWID);
		}
	}
	return lResult;
}

LRESULT Dispatcher::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	pWid = GetObject(m_h2oCaptured);
	if (pWid == NULL)
	{
		pWid = GetWidPt(pt);
	}
	if (m_h2oCaptured.first != INVALID_HWID)
	{
		ReleaseCapture();
		ClearH2O(m_h2oCaptured);
	}
	if (pWid != NULL)
	{
		lResult = pWid->SendWidMessage(uMsg,  wParam, lParam);
		Widget* pLastMM = GetObject(m_h2oLastMouseMove);
		if (pLastMM != NULL)
		{
			pLastMM->SendWidMessage(WM_MOUSELEAVE);
		}
		ClearH2O(m_h2oLastMouseMove);
	}
	pWid = GetWidPt(pt);
	if (pWid != NULL
		&& pWid->IsShow()
		&& m_h2oLButtonDown.first == pWid->GetHwid()
		&& m_h2oLButtonDown.second == pWid)
	{
		pWid->SendWidMessage(WUM_LBUTTONCLICK, wParam, lParam);
	}
	ClearH2O(m_h2oLButtonDown);
	return lResult;
}

LRESULT Dispatcher::OnKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	pWid = GetObject(m_h2oFocused);
	if (pWid != NULL)
	{
		lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
	}
	return lResult;
}

LRESULT Dispatcher::OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	pWid = GetObject(m_h2oFocused);
	if (pWid != NULL)
	{
		lResult = pWid->SendWidMessage(uMsg, wParam, lParam);
	}
	return lResult;
}

LRESULT Dispatcher::OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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

LRESULT Dispatcher::OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Point pt(lParam);
	Widget* pWid = NULL;
	LRESULT lResult = 0;
	UINT_PTR nWidTimer = 0;
	if (GetWidgetFromTimer(wParam, pWid, nWidTimer))
	{
		lResult = pWid->SendWidMessage(WM_TIMER, nWidTimer);
	}
	return lResult;
}

Widget* Dispatcher::GetWidPt( POINT pt )
{
	Widget* pWid = NULL;
	std::vector<Widget*> rgpWidInPt;
	for (std::map<HWID, Widget*>::iterator it = m_Handle2Object.begin();
		it != m_Handle2Object.end(); ++it)
	{
		WFX_CONDITION(it->second != NULL);
		if (!it->second->IsShow())
		{
			continue;
		}
		Rect rcWid = it->second->GetRect();
		if (rcWid.PtInRect(pt))
		{
			rgpWidInPt.push_back(it->second);
		}
	}
	if (rgpWidInPt.size() > 0)
	{
		if (rgpWidInPt.size() != 1)
		{
			pWid = GetWidPt(rgpWidInPt);
		}
		else
		{
			pWid = rgpWidInPt[0];
		}
	}

	return pWid;
}

Widget* Dispatcher::GetWidPt(const std::vector<Widget*>& rgpWid)
{
	Widget* pWid = NULL;
	std::vector<Widget*>::const_iterator it = rgpWid.begin();
	if (it != rgpWid.end())
	{
		pWid = *it;
		WFX_CONDITION(pWid != NULL);
		Rect rcWid = pWid->GetRect();
		float fArea = (rcWid.right - rcWid.left) * (rcWid.bottom - rcWid.top);
		float fMinArea = fArea;
		for (; it != rgpWid.end(); ++it)
		{
			rcWid = pWid->GetRect();
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

HINSTANCE Dispatcher::GetInstance()
{
	return s_hInstance;
}

void Dispatcher::SetInstance( HINSTANCE hInstance )
{
	s_hInstance = hInstance;
}

HWND Dispatcher::GetMainWnd()
{
	return s_hMainWnd;
}

void Dispatcher::SetMainWnd( HWND hMainWnd )
{
	s_hMainWnd = hMainWnd;
}

BOOL Dispatcher::SetParent( Widget* pThis, Widget* pParent )
{
	WFX_CONDITION(pThis != NULL);
	if (pParent == NULL)
	{
		HWID hWid = pThis->GetHwid();
		std::map<HWID, Widget*>::iterator it = 
			m_h2oOrphan.find(hWid);
		if (it == m_h2oOrphan.end())
		{
			m_h2oOrphan.insert(std::make_pair(hWid, pThis));
		}
		return FALSE;
	}

	ULONG nChilren = pParent->GetChildren();
	if (pParent->AddChild(pThis) != nChilren + 1)
	{
		return FALSE;
	}
	pThis->SetMyParent(pParent);
	return TRUE;
}

Widget* Dispatcher::FromHwid( HWID hWid ) const
{
	std::map<HWID, Widget*>::const_iterator it = 
		m_Handle2Object.find(hWid);
	if (it != m_Handle2Object.end())
	{
		return it->second;
	}
	return NULL;
}

int Dispatcher::MessageLoop()
{
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

void Dispatcher::GetHSL( short& nH, short& nS, short& nL )
{
	nH = s_nH;
	nS = s_nS;
	nL = s_nL;
}

void Dispatcher::SetHSL( short nH, short nS, short nL )
{
	s_nH = nH;
	s_nS = nS;
	s_nL = nL;
}

int Dispatcher::GetAlpha()
{
	return s_nAlpha;
}

void Dispatcher::SetAlpha( int nAlpha )
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
		hWnd = (*it)->GetHwnd();
		SetAlpha(s_nAlpha, hWnd);
	}
}

void Dispatcher::SetAlpha( int nAlpha, HWND hWnd )
{
	if (!::IsWindow(hWnd))
	{
		return;
	}
	typedef BOOL (__stdcall *PFUNCSETLAYEREDWINDOWATTR)(HWND, COLORREF, BYTE, DWORD);
	PFUNCSETLAYEREDWINDOWATTR fSetLayeredWindowAttributes;

	HMODULE hUser32 = ::GetModuleHandleW(L"User32.dll");
	if (hUser32)
	{
		fSetLayeredWindowAttributes = 
			(PFUNCSETLAYEREDWINDOWATTR)::GetProcAddress(hUser32, "SetLayeredWindowAttributes");
		if( fSetLayeredWindowAttributes == NULL ) return;
	}

	DWORD dwStyle = ::GetWindowLongW(hWnd, GWL_EXSTYLE);
	DWORD dwNewStyle = dwStyle;
	if( nAlpha >= 0 && nAlpha < 256 ) dwNewStyle |= WS_EX_LAYERED;
	else dwNewStyle &= ~WS_EX_LAYERED;
	if(dwStyle != dwNewStyle) ::SetWindowLongW(hWnd, GWL_EXSTYLE, dwNewStyle);
	fSetLayeredWindowAttributes(hWnd, 0, nAlpha, LWA_ALPHA);
}

void Dispatcher::OnPaint(const Rect& rcPaint)
{
	// Note: Only orphans need to handle WM_PAINT message,
	// parents will handle it for their children.
	for (std::map<HWID, Widget*>::iterator it = m_h2oOrphan.begin();
		it != m_h2oOrphan.end(); ++it)
	{
		WFX_CONDITION(it->second != NULL);
		DrawWid(it->second, rcPaint);
	}
}

void Dispatcher::ShowWid( Widget* pWid, WORD wShow )
{
	WFX_CONDITION(pWid != NULL);
	pWid->MyShowWid(wShow);
	std::vector<Widget*> rgpChilren;
	pWid->GetChildren(rgpChilren);
	for (std::vector<Widget*>::iterator it = 
		rgpChilren.begin(); it != rgpChilren.end(); ++it)
	{
		ShowWid((*it), wShow);
	}
}

void Dispatcher::SetCapture( Widget* pWid )
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(*pWid != INVALID_HWID);
	m_h2oCaptured = std::make_pair(pWid->GetHwid(), pWid);
	::SetCapture(m_hWnd);
}

BOOL Dispatcher::IsCaptured( const Widget* pWid ) const
{
	WFX_CONDITION(pWid != NULL);
	return (pWid == m_h2oCaptured.second) && (pWid->GetHwid() == m_h2oCaptured.first);
}

void Dispatcher::SetFocus(Widget* pWid)
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(*pWid != INVALID_HWID);
	m_h2oFocused = std::make_pair(pWid->GetHwid(), pWid);
}

void Dispatcher::SetLButtonDown(Widget* pWid)
{
	WFX_CONDITION(m_hWnd != NULL);
	WFX_CONDITION(pWid != NULL);
	WFX_CONDITION(*pWid != INVALID_HWID);
	m_h2oLButtonDown = std::make_pair(pWid->GetHwid(), pWid);
}
void Dispatcher::ReleaseCapture()
{
	ClearH2O(m_h2oCaptured);
	::ReleaseCapture();
}

BOOL Dispatcher::IsWidget( const Widget* pWidget ) const
{
	if (this == NULL)
	{
		return FALSE;
	}
	if (pWidget == NULL)
	{
		return FALSE;
	}
	if (pWidget->GetHwid() == INVALID_HWID)
	{
		return FALSE;
	}
	return TRUE;
}

void Dispatcher::ClearH2O( std::pair<HWID, Widget*>& h2o )
{
	h2o.first = INVALID_HWID;
	h2o.second = NULL;
}

Widget* Dispatcher::GetObject( const std::pair<HWID, Widget*>& h2o )
{
	if (h2o.first != INVALID_HWID)
	{
		WFX_CONDITION(h2o.second != NULL);
		return h2o.second;
	}
	return NULL;
}

void Dispatcher::SetMouseMoveH2O( const std::pair<HWID, Widget*>& h2o )
{
	m_h2oLastMouseMove.first = h2o.first;
	m_h2oLastMouseMove.second = h2o.second;
}

void Dispatcher::SetCapturedH2O( const std::pair<HWID, Widget*>& h2o )
{
	m_h2oCaptured.first = h2o.first;
	m_h2oCaptured.second = h2o.second;
}

void Dispatcher::EnableScrollBar( Widget* pWid, UINT uBarFlag, BOOL bEnable /*= TRUE*/ )
{
	WFX_CONDITION(pWid != NULL);
	Rect rcWid = pWid->GetRect();
	Rect rcSB;
	Size szVirtual = pWid->GetVirtualSize();
	if (uBarFlag == WESB_BOTH)
	{
		if (bEnable)
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_VERT);
			if (pBar == NULL)
			{
				pBar = new ScrollBar(SB_VERT);
				pBar->SetRange(0, szVirtual.cy);
				rcSB = pWid->GetScrollBarRect(SB_VERT);
				pBar->Create(rcSB, this, pWid);
				pWid->SetScrollBar(SB_VERT, pBar);
			}
			pBar = pWid->GetScrollBar(SB_HORZ);

			if (pBar == NULL)
			{
				pBar = new ScrollBar(SB_HORZ);
				pBar->SetRange(0, szVirtual.cx);
				rcSB = pWid->GetScrollBarRect(SB_HORZ);
				pBar->Create(rcSB, this, pWid);
				pWid->SetScrollBar(SB_HORZ, pBar);
			}
			pWid->SetSBFlag(WESB_BOTH);
			rcWid = pWid->GetRect();
		}
		else
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_VERT);
			TDEL(pBar);
			pBar = pWid->GetScrollBar(SB_HORZ);
			TDEL(pBar);
			pWid->SetSBFlag(WESB_NONE);
			rcWid = pWid->GetRect();
		}
	}
	else if (uBarFlag == WESB_HORZ)
	{
		if (bEnable)
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_HORZ);
			if (pBar == NULL)
			{
				pBar = new ScrollBar(SB_HORZ);
				pBar->SetRange(0, szVirtual.cx);
				rcSB = pWid->GetScrollBarRect(SB_HORZ);
				pBar->Create(rcSB, this, pWid);
				pWid->SetScrollBar(SB_HORZ, pBar);
			}
			pWid->SetSBFlag(pWid->GetSBFlag() | WESB_HORZ);
			rcWid = pWid->GetRect();
		}
		else
		{
			ScrollBar* pBar = pWid->GetScrollBar(SB_HORZ);
			TDEL(pBar);
			pWid->SetSBFlag(pWid->GetSBFlag() & ~WESB_HORZ);
			rcWid = pWid->GetRect();
		}
	}
	else if (uBarFlag == WESB_VERT)
	{
		if (bEnable)
		{
			ScrollBar* pBar = pWid->GetScrollBar(WESB_VERT);
			if (pBar == NULL)
			{
				pBar = new ScrollBar(WESB_VERT);
				pBar->SetRange(0, szVirtual.cy);
				rcSB = pWid->GetScrollBarRect(WESB_VERT);
				pBar->Create(rcSB, this, pWid);
				pWid->SetScrollBar(WESB_VERT, pBar);
			}
			pWid->SetSBFlag(pWid->GetSBFlag() | WESB_VERT);
			rcWid = pWid->GetRect();
		}
		else
		{
			ScrollBar* pBar = pWid->GetScrollBar(WESB_VERT);
			TDEL(pBar);
			pWid->SetSBFlag(pWid->GetSBFlag() & ~WESB_VERT);
			rcWid = pWid->GetRect();
		}
	}
	else
	{
		WFX_CONDITION(FALSE);
	}
}

void Dispatcher::SetScrollInfo( Widget* pWid, int nBar, LPCSCROLLINFO lpsi, BOOL redraw )
{

}

void Dispatcher::GetScrollInfo( Widget* pWid, int nBar, LPSCROLLINFO lpsi )
{

}

void Dispatcher::PreProcessMsg( Widget* pWid, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

}

void Dispatcher::SetWidRect( Widget* pWid, const Rect& rc )
{
	WFX_CONDITION(pWid != NULL);

	Rect rcWid;
	rcWid = pWid->GetRect();
	pWid->SetVirtualSizeCached(FALSE);
	Rect rcSB = rcWid;
	Rect rcDraw = rcWid;
	Size szVirtual = pWid->GetVirtualSize();
	Size szView(rcWid.GetWidth(), rcWid.GetHeight());
	BOOL bOutRangeHorz = szView.cx < szVirtual.cx;
	BOOL bOutRangeVert = szView.cy < szVirtual.cy;
	if (pWid->GetSBFlag() & WESB_VERT)
	{
		WFX_CONDITION(pWid->GetScrollBar(SB_VERT) != NULL);
		if (!bOutRangeVert)
		{
			rcSB.Empty();
			pWid->GetScrollBar(SB_VERT)->Reset();
		}
		else
		{
			rcSB = pWid->GetScrollBarRect(SB_VERT);
		}
		pWid->GetScrollBar(SB_VERT)->SetRect(rcSB);
		rcDraw.right -= rcSB.right - rcSB.left;
	}
	rcSB = rcWid;
	if (pWid->GetSBFlag() & WESB_HORZ)
	{
		WFX_CONDITION(pWid->GetScrollBar(SB_HORZ) != NULL);
		if (!bOutRangeHorz)
		{
			rcSB.Empty();
			pWid->GetScrollBar(SB_HORZ)->Reset();
		}
		else
		{
			rcSB = pWid->GetScrollBarRect(SB_HORZ);
		}
		pWid->GetScrollBar(SB_HORZ)->SetRect(rcSB);
		rcDraw.bottom -= rcSB.bottom - rcSB.top;
	}
	pWid->SetDrawRect(rcDraw);
	pWid->SendWidMessage(WM_SIZE, 0, 0);
}

UINT_PTR Dispatcher::SetWidTimer( Widget* pWid, UINT_PTR nWidTimer, UINT uElapse, TIMERPROC lpTimerFunc )
{
	return m_pTimer->SetWidTimer(pWid, nWidTimer, uElapse, lpTimerFunc);
}

BOOL Dispatcher::KillWidTimer( Widget* pWid, UINT_PTR nWidTimer )
{
	return m_pTimer->KillWidTimer(pWid, nWidTimer);
}

BOOL Dispatcher::GetWidgetFromTimer( UINT_PTR nWndTimer, Widget*& pWid, UINT_PTR& nWidTimer )
{
	return m_pTimer->GetWidgetFromTimer(nWndTimer, pWid, nWidTimer);
}

void Dispatcher::Invalidate( const Rect& rc )
{
	::InvalidateRect(m_hWnd, &rc, FALSE);
}

void Dispatcher::DrawBkgnd( Widget* pWid, HDC hdc, const Rect& rc )
{
	//WFX_CONDITION(pWid != NULL);
	//Widget* pParent = pWid->GetParent();

	//Gdiplus::Color clrBkgnd;
	//if (pParent != NULL)
	//{
	//	clrBkgnd = pParent->GetBkgnd();
	//	Gdiplus::SolidBrush bkgnd(clrBkgnd);
	//	grph.FillRectangle(&bkgnd, rc);
	//	Gdiplus::Pen pnbkgnd(clrBkgnd);
	//	grph.DrawRectangle(&pnbkgnd, rc);
	//}
	//else
	//{
	//	//clrBkgnd.SetFromCOLORREF(::GetBkColor(grph.GetHDC()));
	//}
}
