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

class ListCtrl;
class TreeCtrl;
class TNode;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
struct WFX_API HeaderInfo
{
	LONG						cx;
	String						strText;
	PImage						pImage;
	DWORD						dwFormat;
	LONG						nOrder;
	DWORD						dwState;
	Rect						rcPos;
	ULONG						m_nType;
	HeaderInfo()
		: cx(0)
		, strText(L"HeaderInfo")
		, dwFormat(DT_CENTER | DT_SINGLELINE | DT_VCENTER)
		, nOrder(0)
		, dwState(WCS_NORMAL)
		, m_nType(WCT_TEXT)
	{
	}
	void SetImage(const String& strFile)
	{
		pImage.reset(WFX_GET_IMAGE(strFile.c_str()));
	}
};
typedef SharedPtr<HeaderInfo> PHeaderInfo;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef SharedPtr<TNode> PTNode;
class WFX_API TNode /*: public MsgMap*/
{
public:
	TNode();
public:
	ULONG AddChild(const PTNode& pNode);
	ULONG InsertChild(ULONG nPos, const PTNode& pNode);
	ULONG GetChildren(std::vector<PTNode>& rgChilren) const;
	ULONG GetChildren() const;
	BOOL HasChild() const;
	ULONG GetMyLayer() const;
	BOOL HasLayers() const;
	TNode* GetAt(ULONG nItem, BOOL bItemExpanded = TRUE);
	TNode* operator[](ULONG nItem);
	TNode* GetParent() const;
	void SetParent(TNode* pParent);
	BOOL IsExpanded() const;
	void Expand(BOOL bExpand = TRUE);
	BOOL IsExpanded(ULONG nItem, BOOL bItemExpanded = TRUE);
	ULONG Expand(ULONG nItem, BOOL bItemExpanded = TRUE, BOOL bExpand = TRUE);
	ULONG GetItems(BOOL bItemExpanded = TRUE) const;
protected:
	ULONG GetItems_All(const TNode* pNode) const;
	ULONG GetItems_Expanded(const TNode* pNode) const;
	TNode* GetAt_All(ULONG nItem) const;
	TNode* GetAt_Expanded(ULONG nItem);
	TNode* GetAt_Expanded(ULONG nItem, ULONG& nItems, TNode* pNode);
protected:
	TNode* m_pParent;
	BOOL m_bExpanded;
	std::vector<PTNode> m_rgpChildren;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Cell : public UnitBase
{
public:
	Cell();
public:
	WFX_BEGIN_MSG_MAP(Cell)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_CHAIN_MSG_MAP(UnitBase)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual void Draw(HDC hdc,const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API LayerCell : public Cell
{
public:
	virtual void Draw(HDC hdc,const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
public:
	WFX_BEGIN_MSG_MAP(LayerCell)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_CHAIN_MSG_MAP(Cell)
		WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API HeadCell : public Cell
{
	
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API InPlaceCell : public Cell
{

};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API TextCell : public InPlaceCell
{

};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API ComboBoxCell : public InPlaceCell
{

};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API CalendarCell : public InPlaceCell
{

};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API LinkCell : public Cell
{

};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
struct WFX_API LCDrawItemInfo
{
	HDC m_hDC;
	LONG m_nRow;
	LONG m_nCol;
	const Rect* m_prcItem;
	DWORD m_dwState;
	DWORD m_dwFormat;
	const wchar_t* m_pszText;
	LCDrawItemInfo()
		: m_hDC(NULL)
		, m_nRow(-1)
		, m_nCol(-1)
		, m_prcItem(NULL)
		, m_dwState(0)
		, m_dwFormat(0)
		, m_pszText(NULL)
	{

	}
	LCDrawItemInfo(HDC hDC, LONG nRow, LONG nCol, 
		const Rect* prcItem, DWORD dwState, 
		DWORD dwFormat, const wchar_t* pszText)
		: m_hDC(hDC)
		, m_nRow(nRow)
		, m_nCol(nCol)
		, m_prcItem(prcItem)
		, m_dwState(dwState)
		, m_dwFormat(dwFormat)
		, m_pszText(pszText)
	{

	}
	void Clear()
	{
		m_hDC = NULL;
		m_nRow = -1;
		m_nCol = -1;
		m_prcItem = NULL;
		m_dwState = 0;
		m_dwFormat = 0;
		m_pszText = NULL;
	}
};
typedef SharedPtr<LCDrawItemInfo> PLCDrawItemInfo;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API HeaderCtrl : public Widget
{
	friend class ListCtrl;
public:
	HeaderCtrl();
public:
	ULONG GetItemCount() const;
	BOOL GetItemRect(ULONG nIndex, Rect& rc);
	ULONG AddItem(const HeaderInfo& hdi);
	ULONG InsertItem(ULONG nPos, const HeaderInfo& hdi);
	BOOL RemoveItem(ULONG nPos);
protected:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
	Cell* GetCell(ULONG nCol);
public:
	ULONG GetCellType(ULONG nCol) const;
public:
	WFX_BEGIN_MSG_MAP(HeaderCtrl)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
protected:
	LONG GetSelected() const;
	void SetSelected(LONG nSelected);
	BOOL IsAscendSort() const;
	ULONG GetTotalColumns() const;
	ULONG GetColumnWidth(ULONG nCol) const;
	LONG GetSelected(POINT pt);
	void CalcCellRect();
	inline LCDrawItemInfo* GetDrawItemInfo() const;
public:
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	BOOL Verify() const;
protected:
	std::vector<PHeaderInfo> m_rgpHdi;
	Rect m_rcSeqBarHead;
	LONG m_nSelected;
	BOOL m_bAscendSort;
	PLCDrawItemInfo m_pDrawItemInfo;
};
typedef SharedPtr<HeaderCtrl> PHeadCtrl;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API CellID
{
public:
	CellID(LONG nRow = -1, LONG nCol = -1);
public:
	BOOL IsValidID() const;
	bool operator()(const CellID& id1, const CellID& id2) const;
	bool operator<(const CellID& other) const;
	bool operator==(const CellID& other) const;
	bool operator!=(const CellID& other) const;
	void Clear();
public:
	LONG m_nRow;
	LONG m_nCol;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API VorticalLayerCtrl : public Widget
{
public:
	struct CacheInfo
	{
		CacheInfo(ULONG nLayer = -1, BOOL bExpanded = TRUE)
			: m_nLayer(nLayer)
			, m_bExpanded(bExpanded)
		{

		}
		ULONG m_nLayer;
		BOOL m_bExpanded;
	};
public:
	VorticalLayerCtrl();
protected:
	ULONG AddItem();
	ULONG InsertItem(ULONG nItem);
	ULONG InsertSubItem(ULONG nItem);
	ULONG Expand(ULONG nItem, BOOL bExpand = TRUE);
	BOOL IsExpanded(ULONG nItem) const;
	ULONG GetLayer(ULONG nItem) const;
	ULONG GetTotalItems() const;
	void RefreshCache();
protected:
	PTNode m_pRoot;
	std::vector<CacheInfo> m_rgCacheInfo;
	BOOL m_bCached;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
#define LIST_TIMER_MOUSEMOVE 1121
class WFX_API ListCtrl : public VorticalLayerCtrl
{
public:
	ListCtrl();
public:
	WFX_BEGIN_MSG_MAP(ListCtrl)
		WFX_MESSAGE_HANDLER(WM_CREATE, OnCreate)
		WFX_MESSAGE_HANDLER(WUM_LC_GET_PROPERTIES, OnGetProperties)
		WFX_MESSAGE_HANDLER(WM_SIZE, OnSize)
		WFX_MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		WFX_MESSAGE_HANDLER(WUM_LC_HITCOL, OnColChanged)
		WFX_MESSAGE_HANDLER(WUM_LC_HEADSIZE, OnHeadSizeChanged)
		WFX_MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		WFX_MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		WFX_MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		WFX_MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		WFX_MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		WFX_MESSAGE_HANDLER(WUM_LC_CELL_DRAW, OnCellDraw)
		WFX_MESSAGE_HANDLER(WUM_LC_CELL_EXPAND, OnCellExpand)
		WFX_MESSAGE_HANDLER(WM_TIMER, OnTimer)
		WFX_MESSAGE_HANDLER(WUM_LC_SEQHEAD_DRAW, OnMsgSeqHeadDraw)
		WFX_MESSAGE_HANDLER(WUM_LC_HEAD_DRAW, OnMsgHeadDraw)
		WFX_MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT OnCreate(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnSize(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnColChanged(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnHeadSizeChanged(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnGetProperties(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnCellDraw(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnCellExpand(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMsgSeqHeadDraw(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnMsgHeadDraw(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
public:
	HeaderCtrl* GetHeaderCtrl() const;
	void SetHeaderCtrl(HeaderCtrl* pHeader);
	Cell* GetCell(ULONG nRow, ULONG nCol);
	Cell* GetCell(const CellID& cellID);
	Cell* GetCell(ULONG nCellType);
	BOOL GetCellIndirect(const CellID& cellID, 
		std::pair<Cell*, Rect>& cell);
	Rect GetCellRect(ULONG nRow, ULONG nCol);
public:
	ULONG GetColumnWidth(ULONG nCol) const;
	BOOL SetColumnWidth(ULONG nCol, ULONG cx);
	ULONG GetRowHeight(ULONG nRow) const;
	BOOL SetRowHeight(ULONG nRow, ULONG cy);
	String GetItemText(ULONG nItem, ULONG nSubItem) const;
	BOOL SetItemText(ULONG nItem, ULONG nSubItem, 
		const String& strText);
public:
	ULONG GetTotalRows() const;
	ULONG GetTotalColumns() const;

	ULONG GetRowNumBarWidth() const;
	void SetRowNumBarWidth(ULONG nWidth);
	ULONG GetHeadHeight() const;
	void SetHeadHeight(ULONG nHeight);
	LONG GetFixedRow() const;
	void SetFixedRow(LONG nRow);
	LONG GetFixedCol() const;
	void SetFixedCol(LONG nCol);
	LONG GetStartRow() const;
	void SetStartRow(LONG nRow);
	LONG GetEndRow() const;
	void SetEndRow(LONG nRow);
	LONG GetStartCol() const;
	void SetStartCol(LONG nCol);
	LONG GetEndCol() const;
	void SetEndCol(LONG nCol);
	LONG GetCurrentRow() const;
	LONG GetCurrentCol() const;
public:
	ULONG AddColumn(const String& strName, 
		UINT nWidth, DWORD dwFormat, ULONG nCellType, BOOL bAdjust = FALSE);
	ULONG InsertColumn(ULONG nCol, const String& strName, 
		UINT nWidth, DWORD dwFormat, ULONG nCellType, BOOL bAdjust = FALSE);
	ULONG AddRow(LONG nHeight = -1, BOOL bAdjust = FALSE);
	ULONG InsertRow(ULONG nRow, LONG nHeight = -1, BOOL bAdjust = FALSE);
	ULONG InsertSubRow(ULONG nRow, LONG nHeight = -1, BOOL bAdjust = FALSE);
	ULONG Expand(ULONG nRow, BOOL bExpand = TRUE, BOOL bAdjust = FALSE);
	BOOL IsExpanded(ULONG nRow) const;
protected:
	BOOL GetCellID( POINT pt, std::pair<CellID, Rect>& cellID) const;
	BOOL IsValidCellID(const CellID& cellID) const;
protected:
	virtual void OnDraw(HDC hdc, const Rect& rcPaint);
	virtual void OnDrawItem(const LCDrawItemInfo* pDrawItemInfo);
	virtual void OnDrawSeqBar(const LCDrawItemInfo* pDrawItemInfo);
	virtual void OnDrawSeqHead(const LCDrawItemInfo* pDrawItemInfo);
	virtual void OnDrawHeadCtrl(const LCDrawItemInfo* pDrawItemInfo);
protected:
	virtual Size CalcVirtualSize();
	virtual String GetToolTip() const;
protected:
	BOOL CalcCol();
	BOOL CalcRow();
	void CalcCellRect();
	BOOL CalcPos(int nBar);
	BOOL Verify() const;
	inline LCDrawItemInfo* GetDrawItemInfo() const;
protected:
	PHeadCtrl m_pHeadCtrl;
	std::map<CellID, Rect> m_rgRect;
	std::vector<Rect> m_rgRowNumRect;
	ULONG m_nHeadHeight;
	ULONG m_nRowNumBarWidth;
	LONG m_nStartRow;
	LONG m_nEndRow;
	LONG m_nStartCol;
	LONG m_nEndCol;
	LONG m_nFixedRow;
	LONG m_nFixedCol;
	CellID m_cellSelected;
	CellID m_cellMouseMove;
	BOOL m_bHasSubItem;
	Point m_ptMouseMove;
	std::vector<std::vector<Rect> > m_rgRectFast;
};

typedef SharedPtr<ListCtrl> PListCtrl;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API TreeCtrl : public VorticalLayerCtrl
{
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API RichCtrl : public Widget
{

};

END_NAMESPACE_WFX
