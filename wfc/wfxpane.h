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

class Widget;
class UnitBase;
class BarBase;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API SplitBar : public Widget, public BarBase
{
public:
	SplitBar(int nBar = SB_VERT, BOOL bFixed = FALSE);
public:
	WFX_BEGIN_MSG_MAP(SplitBar)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual String GetToolTip() const;
public:
	void SetFixed(BOOL bFixed = TRUE);
	BOOL IsFixed() const;
	void SetRange(LONG nMax, LONG nMin);
protected:
	BOOL m_bFixed;
	LONG m_nMax;
	LONG m_nMin;
};
typedef SharedPtr<SplitBar> PSplitBar;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
struct WFX_API SplitInfo
{
	PSplitBar m_pSplitBar;
	LONG m_nMax;
	LONG m_nMin;
	SplitInfo(int nBar = SB_VERT)
		: m_pSplitBar(new SplitBar(nBar))
		, m_nMax(0)
		, m_nMin(0)
	{

	}
	SplitBar* GetSplitBar() const{
		return m_pSplitBar.get();
	}
};
typedef SharedPtr<SplitInfo> PSplitInfo;
typedef std::vector<PSplitInfo> SplitList;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
struct WFX_API WidgetInfo
{
	Widget* m_pWidget;
	float m_fScale;
	BOOL m_bShow;
	WidgetInfo()
	{
		memset(this, 0, sizeof(WidgetInfo));
		m_bShow = TRUE;
	}
};
typedef SharedPtr<WidgetInfo> PWidgetInfo;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
struct WFX_API PaneItem
{
	enum
	{
		PI_SPLITBAR,
		PI_WIDGET
	};
	int m_nItemType;
	union
	{
		SplitInfo* m_pSplitInfo;
		WidgetInfo* m_pWidgetInfo;
	};
	PaneItem(int nItemType, int nBar = SB_VERT)
		: m_nItemType(nItemType)
	{
		if (m_nItemType == PI_SPLITBAR)
		{
			m_pSplitInfo = new SplitInfo(nBar);
		}
		else
		{
			m_pWidgetInfo = new WidgetInfo;
		}
	}
	~PaneItem()
	{
		if (m_nItemType == PI_SPLITBAR)
		{
			TDEL(m_pSplitInfo);
		}
		else
		{
			TDEL(m_pWidgetInfo);
		}
	}
	SplitInfo* GetSplitInfo() const
	{
		return m_pSplitInfo;
	}
	WidgetInfo* GetWidgetInfo() const
	{
		return m_pWidgetInfo;
	}
	BOOL IsWidget() const
	{
		return m_nItemType == PI_WIDGET;
	}
	BOOL IsSplitBar() const
	{
		return m_nItemType == PI_SPLITBAR;
	}
};
typedef SharedPtr<PaneItem> PPaneItem;
typedef std::vector<PPaneItem> PaneItemList;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Pane : public Widget, public BarBase
{
public:
	Pane(int nBar = SB_HORZ);
public:
	BOOL AddWidget(Widget* pWidget, float fScale = 1.00, BOOL bShow = TRUE);
	BOOL AddWidget(ULONG nInitSize, Widget* pWidget, BOOL bShow = TRUE);
public:
	WFX_BEGIN_MSG_MAP(Pane)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WUM_PANEITEM_RESIZE, OnResizePaneItem)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnResizePaneItem(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	void OnVertSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	void OnHorzSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	PaneItem* GetAt(ULONG nIndex) const;
	PaneItem* GetLast() const;
	void AdjustSplitRangeVert();
	void AdjustSplitRangeHorz();
protected:
	PaneItemList m_rgpPaneItem;
};
typedef SharedPtr<Pane> PPane;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API GridPane : public Pane
{

};

END_NAMESPACE_WFX
