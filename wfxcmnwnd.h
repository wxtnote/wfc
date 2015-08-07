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
	WFX_DECLARE_FACTORY(CommonWid);
protected:
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
protected:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
protected:
	PButton m_pBtnMax;
	PButton m_pBtnMin;
	PButton m_pBtnClose;
	PButton m_pAdd;
	PImage m_pImage;
	Rect m_rcCaption;
};

typedef CommonWid RootWid;
typedef SharedPtr<RootWid> PRootWid;
typedef SharedPtr<Factory<CommonWid> > PRootFactory;

class WFX_API CommonWnd : public Window
{
public:
	CommonWnd(const PRootFactory& pRootFactory);
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
	WFX_CHAIN_MSG_MAP(Window)
	WFX_END_MSG_MAP()
public:
	virtual LPCTSTR GetWindowClassName() const;
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
	PRootFactory m_pRootFactory;
};

typedef SharedPtr<CommonWnd> PCommonWnd;

class WFX_API FrameWnd : public CommonWnd
{
public:
	FrameWnd(const PRootFactory& pWidFactory);
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

END_NAMESPACE_WFX
