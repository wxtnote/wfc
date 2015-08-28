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

class MsgMap;
class Widget;
class Dispatcher;
class Grid;
class Layout;

class Layout : public MsgMap
{
	WFX_DECLARE_FACTORY(Layout);

protected:
	Layout();
public:
	virtual BOOL processMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
public:
	struct Desc
	{
		INT		m_nPos;
		Rect	m_rcMargin;
	};
public:
	virtual void doLayout(Widget* pWid, Desc* pDesc, const Rect& rcGrid);

};

typedef std::tr1::shared_ptr<Layout>				SPLayout;

typedef Layout::Desc					LayoutDesc;
typedef const Layout::Desc				CLayoutDesc;
typedef std::pair<Widget*, LayoutDesc>	Wid2Desc;
typedef std::vector<Wid2Desc>			Wid2DescList;

typedef Factory<Layout>					LayoutFactory;
typedef std::tr1::shared_ptr<LayoutFactory>		SPLayoutFactory;
typedef const SPLayoutFactory			CSPLayoutFactory;
typedef _my_ptr<Grid> PGrid;
class WFX_API Grid : public Widget
{
public:
	Grid(CSPLayoutFactory& pFactory);
public:
	void addItem(Widget* pWidget, CLayoutDesc& desc);
	ULONG getWidLayout(Wid2DescList& rgWidLayout);
	void doLayout();
	WFX_BEGIN_MSG_MAP(Grid)
		WFX_ON_MESSAGE(WM_CREATE, onCreate)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
	BOOL& bHandled);
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	Wid2DescList m_rgWidLayout;
	SPLayout m_pLayout;
	SPLayoutFactory m_pLayoutFactory;
};

typedef std::tr1::shared_ptr<Grid> SPGrid;

class WFX_API LayoutDispatch : public Dispatcher
{
public:
	LayoutDispatch();
public:
	void setGrid(Grid* pGrid);
	Grid* getGrid() const;
public:
	WFX_BEGIN_MSG_MAP(LayoutDispatch)
		WFX_ON_MESSAGE(WM_CREATE, onCreate)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_CHAIN_MSG_MAP(Dispatcher)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	PGrid m_pRoot;
};

typedef std::tr1::shared_ptr<LayoutDispatch> SPLayoutDispatch;

END_NAMESPACE_WFX
