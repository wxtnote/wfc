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
class WFX_API SplitBar : public UnitBase, public BarBase
{
public:
	SplitBar(int nBar = SB_VERT);
};
typedef SharedPtr<SplitBar> PSplitBar;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
struct WFX_API SplitInfo
{
	Widget* m_pWidget1;
	PSplitBar m_pSplitBar;
	Widget* m_pWidget2;
	float m_fInerScale;
	float m_fOuterScale;
	SplitInfo(int nBar = SB_VERT)
		: m_pWidget1(NULL)
		, m_pSplitBar(new SplitBar(nBar))
		, m_pWidget2(NULL)
		, m_fInerScale(0.5)
		, m_fOuterScale(1.0)
	{

	}
	SplitBar* GetSplitBar() const{
		return m_pSplitBar.get();
	}
};
typedef SharedPtr<SplitInfo> PSplitInfo;
typedef std::vector<PSplitInfo> SplitList;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Pane : public Widget, public BarBase
{
public:
	Pane(int nBar = SB_HORZ);
public:
	void Split(Widget* pWiget1, Widget* pWiget2, 
		float fInerScale, float fOuterScale);
public:
	WFX_BEGIN_MSG_MAP(Pane)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	void OnVertSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	void OnHorzSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	virtual void AdjustSplitSizeVert(LONG nStart, LONG nTotal, 
		const Rect& rcWid, const SplitInfo* pSplitInfo);
	virtual void AdjustSplitSizeHorz(LONG nStart, LONG nTotal, 
		const Rect& rcWid, const SplitInfo* pSplitInfo);
protected:
	virtual void OnDraw(HDC hdc, const Rect& rc);
protected:
	BOOL InSplitBar(const Point& pt) const;
	SplitInfo* GetSplitInPt(const Point& pt) const; 
	void AddWidget(Widget* pWidget);
protected:
	SplitList m_rgpSplitInfo;
	std::vector<Widget*> m_rgpItem;
};
typedef SharedPtr<Pane> PPane;

END_NAMESPACE_WFX
