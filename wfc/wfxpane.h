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

//#pragma once
//
//#include "wfx.h"
//
//BEGIN_NAMESPACE_WFX
//
//class Widget;
//class UnitBase;
//class BarBase;
/////////////////////////////*** a gorgeous partition line ***/////////////////////////////
//class WFX_API SplitBar : public Widget, public BarBase
//{
//public:
//	SplitBar(int nBar = SB_VERT, BOOL bFixed = FALSE);
//public:
//	WFX_BEGIN_MSG_MAP(SplitBar)
//		WFX_ON_MESSAGE(WM_MOUSEMOVE, onMouseMove)
//		WFX_CHAIN_MSG_MAP(Widget)
//	WFX_END_MSG_MAP()
//public:
//	wfx_msg LRESULT onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
//		BOOL& bHandled);
//public:
//	virtual String getToolTip() const;
//public:
//	void setFixed(BOOL bFixed = TRUE);
//	BOOL isFixed() const;
//	void setRange(LONG nMax, LONG nMin);
//protected:
//	BOOL m_bFixed;
//	LONG m_nMax;
//	LONG m_nMin;
//};
//typedef std::tr1::shared_ptr<SplitBar> SPSplitBar;
/////////////////////////////*** a gorgeous partition line ***/////////////////////////////
//struct WFX_API SplitInfo
//{
//	SPSplitBar m_pSplitBar;
//	LONG m_nMax;
//	LONG m_nMin;
//	SplitInfo(int nBar = SB_VERT)
//		: m_pSplitBar(new SplitBar(nBar))
//		, m_nMax(0)
//		, m_nMin(0)
//	{
//
//	}
//	SplitBar* getSplitBar() const{
//		return m_pSplitBar.get();
//	}
//};
//typedef std::tr1::shared_ptr<SplitInfo> SPSplitInfo;
//typedef std::vector<SPSplitInfo> SplitList;
/////////////////////////////*** a gorgeous partition line ***/////////////////////////////
//struct WFX_API WidgetInfo
//{
//	Widget* m_pWidget;
//	float m_fScale;
//	BOOL m_bShow;
//	WidgetInfo()
//	{
//		memset(this, 0, sizeof(WidgetInfo));
//		m_bShow = TRUE;
//	}
//};
//typedef std::tr1::shared_ptr<WidgetInfo> SPWidgetInfo;
/////////////////////////////*** a gorgeous partition line ***/////////////////////////////
//struct WFX_API PaneItem
//{
//	enum
//	{
//		PI_SPLITBAR,
//		PI_WIDGET
//	};
//	int m_nItemType;
//	union
//	{
//		SplitInfo* m_pSplitInfo;
//		WidgetInfo* m_pWidgetInfo;
//	};
//	PaneItem(int nItemType, int nBar = SB_VERT)
//		: m_nItemType(nItemType)
//	{
//		if (m_nItemType == PI_SPLITBAR)
//		{
//			m_pSplitInfo = new SplitInfo(nBar);
//		}
//		else
//		{
//			m_pWidgetInfo = new WidgetInfo;
//		}
//	}
//	~PaneItem()
//	{
//		if (m_nItemType == PI_SPLITBAR)
//		{
//			TDEL(m_pSplitInfo);
//		}
//		else
//		{
//			TDEL(m_pWidgetInfo);
//		}
//	}
//	SplitInfo* getSplitInfo() const
//	{
//		return m_pSplitInfo;
//	}
//	WidgetInfo* getWidgetInfo() const
//	{
//		return m_pWidgetInfo;
//	}
//	BOOL isWidget() const
//	{
//		return m_nItemType == PI_WIDGET;
//	}
//	BOOL isSplitBar() const
//	{
//		return m_nItemType == PI_SPLITBAR;
//	}
//};
//typedef std::tr1::shared_ptr<PaneItem> SPPaneItem;
//typedef std::vector<SPPaneItem> PaneItemList;
/////////////////////////////*** a gorgeous partition line ***/////////////////////////////
//class WFX_API Pane : public Widget, public BarBase
//{
//public:
//	Pane(int nBar = SB_HORZ);
//public:
//	BOOL addWidget(Widget* pWidget, float fScale = 1.00, BOOL bShow = TRUE);
//	BOOL addWidget(ULONG nInitSize, Widget* pWidget, BOOL bShow = TRUE);
//public:
//	WFX_BEGIN_MSG_MAP(Pane)
//		WFX_ON_MESSAGE(WM_CREATE, onCreate)
//		WFX_ON_MESSAGE(WM_SIZE, onSize)
//		WFX_ON_MESSAGE(WUM_PANEITEM_RESIZE, onResizePaneItem)
//		WFX_CHAIN_MSG_MAP(Widget)
//	WFX_END_MSG_MAP()
//public:
//	wfx_msg LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
//		BOOL& bHandled);
//	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
//		BOOL& bHandled);
//	wfx_msg LRESULT onResizePaneItem(UINT uMsg, WPARAM wParam, LPARAM lParam,
//		BOOL& bHandled);
//protected:
//	void onVertSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
//		BOOL& bHandled);
//	void onHorzSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
//		BOOL& bHandled);
//protected:
//	PaneItem* getAt(ULONG nIndex) const;
//	PaneItem* getLast() const;
//	void adjustSplitRangeVert();
//	void adjustSplitRangeHorz();
//protected:
//	PaneItemList m_rgpPaneItem;
//};
//typedef std::tr1::shared_ptr<Pane> SPPane;
/////////////////////////////*** a gorgeous partition line ***/////////////////////////////
//class WFX_API GridPane : public Pane
//{
//
//};
//
//END_NAMESPACE_WFX
