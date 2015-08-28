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

#pragma once

#include "wfx.h"

BEGIN_NAMESPACE_WFX

class CommonWnd;
class CommonWid;
class Window;

class WFX_API CommonWid : public Widget
{
public:
	CommonWid();
public:
	WFX_BEGIN_MSG_MAP(CommonWid)
		WFX_ON_MESSAGE(WM_CREATE, onCreate)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_ON_COMMOND(WID_BTNID_MAX, BN_CLICKED, onMax)
		WFX_ON_COMMOND(WID_BTNID_MIN, BN_CLICKED, onMin)
		WFX_ON_COMMOND(WID_BTNID_CLOSE, BN_CLICKED, onClose)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMax(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMin(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual BOOL isCaption(const Point& pt);
	virtual Rect getClientRect() const;
protected:
	virtual void onDraw(HDC hdc, const Rect& rcPaint);
protected:
	SPButton m_pBtnMax;
	SPButton m_pBtnMin;
	SPButton m_pBtnClose;
	SPImage m_pImage;
	Rect m_rcCaption;
	Rect m_rcClient;
	Rect m_rcStatusBar;
};

typedef CommonWid RootWid;
typedef std::tr1::shared_ptr<RootWid> SPRootWid;

class WFX_API CommonWnd : public WidgetWnd
{
public:
	CommonWnd();
public:
	virtual BOOL initialize();
public:
	BOOL create(const String& strName, const Rect& rc, HWND hParent = NULL);
public:
	WFX_BEGIN_MSG_MAP(CommonWnd)
		WFX_ON_MESSAGE(WM_NCACTIVATE, onNCActivate)
		WFX_ON_MESSAGE(WM_NCPAINT, onNCPaint)
		WFX_ON_MESSAGE(WM_NCHITTEST, onNCHitTest)
		WFX_ON_MESSAGE(WM_NCCALCSIZE, onNCcalcSize)
		WFX_ON_MESSAGE(WM_CREATE, onCreate)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_ON_MESSAGE(WM_GETMINMAXINFO, ongetMinMaxInfo)
	WFX_CHAIN_MSG_MAP(WidgetWnd)
	WFX_END_MSG_MAP()
public:
	virtual LPCWSTR getWindowClassName() const;
	virtual UINT getClassStyle() const;
public:
	wfx_msg LRESULT onNCActivate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onNCPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onNCcalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT ongetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	SPRootWid m_pRoot;
};

typedef std::tr1::shared_ptr<CommonWnd> SPCommonWnd;

class WFX_API FrameWnd : public CommonWnd
{
public:
	FrameWnd();
public:
	WFX_BEGIN_MSG_MAP(FrameWnd)
		WFX_ON_MESSAGE(WM_NCACTIVATE, onNCActivate)
		WFX_ON_MESSAGE(WM_CLOSE, onClose)
		WFX_CHAIN_MSG_MAP(CommonWnd)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onNCActivate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
};

typedef std::tr1::shared_ptr<FrameWnd> SPFrameWnd;

class WFX_API Dialog : public CommonWnd
{
public:
	int doModal();
};

WFX_API int WfxMessageBox(const String& strText);

END_NAMESPACE_WFX
