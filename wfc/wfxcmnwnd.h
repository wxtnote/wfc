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
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_COMMAND_HANDLER(WID_BTNID_MAX, BN_CLICKED, OnMax)
		WFX_COMMAND_HANDLER(WID_BTNID_MIN, BN_CLICKED, OnMin)
		WFX_COMMAND_HANDLER(WID_BTNID_CLOSE, BN_CLICKED, OnClose)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMax(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMin(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual BOOL IsCaption(const Point& pt);
	virtual Rect GetClientRect() const;
protected:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	PButton m_pBtnMax;
	PButton m_pBtnMin;
	PButton m_pBtnClose;
	PImage m_pImage;
	Rect m_rcCaption;
	Rect m_rcClient;
	Rect m_rcStatusBar;
};

typedef CommonWid RootWid;
typedef SharedPtr<RootWid> PRootWid;

class WFX_API CommonWnd : public WidgetWnd
{
public:
	CommonWnd();
public:
	virtual BOOL Initialize();
public:
	BOOL Create(const String& strName, const Rect& rc, HWND hParent = NULL);
public:
	WFX_BEGIN_MSG_MAP(CommonWnd)
		WFX_MESSAGE_HANDLER(WM_NCACTIVATE, OnNCActivate)
		WFX_MESSAGE_HANDLER(WM_NCPAINT, OnNCPaint)
		WFX_MESSAGE_HANDLER(WM_NCHITTEST, OnNCHitTest)
		WFX_MESSAGE_HANDLER(WM_NCCALCSIZE, OnNCCalcSize)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	WFX_CHAIN_MSG_MAP(WidgetWnd)
	WFX_END_MSG_MAP()
public:
	virtual LPCWSTR GetWindowClassName() const;
	virtual UINT GetClassStyle() const;
public:
	wfx_msg LRESULT OnNCActivate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnNCPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnNCCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	PRootWid m_pRoot;
};

typedef SharedPtr<CommonWnd> PCommonWnd;

class WFX_API FrameWnd : public CommonWnd
{
public:
	FrameWnd();
public:
	WFX_BEGIN_MSG_MAP(FrameWnd)
		WFX_MESSAGE_HANDLER(WM_NCACTIVATE, OnNCActivate)
		WFX_MESSAGE_HANDLER(WM_CLOSE, OnClose)
		WFX_CHAIN_MSG_MAP(CommonWnd)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnNCActivate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
};

typedef SharedPtr<FrameWnd> PFrameWnd;

class WFX_API Dialog : public CommonWnd
{
public:
	int DoModal();
};

WFX_API int WfxMessageBox(const String& strText);

END_NAMESPACE_WFX
