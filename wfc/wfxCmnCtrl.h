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
	SPImage						pImage;
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
	void setImage(const String& strFile)
	{
		pImage.reset(WFX_GET_IMAGE(strFile.c_str()));
	}
};
typedef  std::tr1::shared_ptr<HeaderInfo> SPHeaderInfo;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef std::tr1::shared_ptr<TNode> SPTNode;
typedef _my_ptr<TNode> PTNode;
class WFX_API TNode /*: public MsgMap*/
{
public:
	TNode();
public:
	ULONG addChild(const SPTNode& pNode);
	ULONG InsertChild(ULONG nPos, const SPTNode& pNode);
	ULONG getChildren(std::vector<SPTNode>& rgChilren) const;
	ULONG getChildren() const;
	BOOL hasChild() const;
	ULONG getMyLayer() const;
	BOOL hasLayers() const;
	TNode* getAt(ULONG nItem, BOOL bItemexpanded = TRUE);
	TNode* operator[](ULONG nItem);
	TNode* getParent() const;
	void setParent(TNode* pParent);
	BOOL isExpanded() const;
	void expand(BOOL bexpand = TRUE);
	BOOL isExpanded(ULONG nItem, BOOL bItemexpanded = TRUE);
	ULONG expand(ULONG nItem, BOOL bItemexpanded = TRUE, BOOL bexpand = TRUE);
	ULONG getItems(BOOL bItemexpanded = TRUE) const;
protected:
	ULONG getItems_All(const TNode* pNode) const;
	ULONG getItems_expanded(const TNode* pNode) const;
	TNode* getAt_All(ULONG nItem) const;
	TNode* getAt_expanded(ULONG nItem);
	TNode* getAt_expanded(ULONG nItem, ULONG& nItems, TNode* pNode);
protected:
	PTNode m_pParent;
	BOOL m_bexpanded;
	std::vector<SPTNode> m_rgpChildren;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Cell : public WidUnit
{
public:
	Cell();
public:
	WFX_BEGIN_MSG_MAP(Cell)
		WFX_ON_MESSAGE(WM_MOUSEMOVE, onMouseMove)
		WFX_ON_MESSAGE(WM_MOUSELEAVE, onMouseLeave)
		WFX_ON_MESSAGE(WM_LBUTTONUP, onLButtonUp)
		WFX_CHAIN_MSG_MAP(WidUnit)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
public:
	virtual void Draw(HDC hdc,const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
};
typedef std::tr1::shared_ptr<Cell> SPCell;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API LayerCell : public Cell
{
public:
	virtual void Draw(HDC hdc,const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat);
public:
	WFX_BEGIN_MSG_MAP(LayerCell)
		WFX_ON_MESSAGE(WM_LBUTTONUP, onLButtonUp)
		WFX_CHAIN_MSG_MAP(Cell)
		WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
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
	void clear()
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
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
typedef std::tr1::shared_ptr<LCDrawItemInfo> SPLCDrawItemInfo;
class ArrayAdapterBase;
typedef _my_ptr<ArrayAdapterBase> PArrayAdapterBase;
typedef std::tr1::shared_ptr<ArrayAdapterBase> SPArrayAdapterBase;
class ObjectAdapterBase;
typedef _my_ptr<ObjectAdapterBase> PObjectAdapterBase;
typedef std::tr1::shared_ptr<ObjectAdapterBase> SPObjectAdapterBase;
class AttributeBase;
typedef _my_ptr<AttributeBase> PAttributeBase;
typedef std::tr1::shared_ptr<AttributeBase> SPAttributeBase;
class ObjectAdapterVisitorBase;
typedef _my_ptr<ObjectAdapterVisitorBase> PObjectAdapterVisitorBase;
typedef std::tr1::shared_ptr<ObjectAdapterVisitorBase> SPObjectAdapterVisitorBase;
class ObjectAdapterFactoryBase;
typedef _my_ptr<ObjectAdapterFactoryBase> PObjectAdapterFactoryBase;
typedef std::tr1::shared_ptr<ObjectAdapterFactoryBase> SPObjectAdapterFactoryBase;

enum Attribute_Type
{
	AT_TEXT_READ,
	AT_TEXT_RW,
	AT_COMBOBOX
};
class WFX_API AttributeBase
{
public:
	AttributeBase(ArrayAdapterBase* pArrayAdapter = NULL):m_pArrayAdapter(pArrayAdapter){}
public:
	virtual String getText() const = 0;
	virtual COLORREF getTextColor() const = 0;
	virtual COLORREF getBkgndColor() const = 0;
	virtual COLORREF getFormat() const = 0;
	virtual DWORD getType() const = 0;
	virtual BOOL isValid() const = 0;
public:
	void setArrayAdapter(ArrayAdapterBase* pArrayAdapter){ m_pArrayAdapter = pArrayAdapter; }
	ArrayAdapterBase* getArrayAdapter() const { return m_pArrayAdapter; }
protected:
	PArrayAdapterBase m_pArrayAdapter;
};

template <class _Ty>
class AdapterBase
{
public:
	typedef _Ty _Ty;
	typedef std::tr1::shared_ptr<_Ty> SP_Ty;
public:
	virtual void addItem(_Ty* pItem)
	{
		m_rgpItems.push_back(SP_Ty(pItem));
	}
	virtual void insertItem(_Ty* pItem, ULONG nPos)
	{
		WFX_CONDITION(nPos <= m_rgpItems.size());
		m_rgpItems.insert(m_rgpItems.begin() + nPos, SP_Ty(pItem));
	}
	virtual void removeItem(ULONG nPos)
	{
		WFX_CONDITION(nPos < m_rgpItems.size());
		m_rgpItems.erase(m_rgpItems.begin() + nPos);
	}
	virtual _Ty* getAt(ULONG nPos) const
	{
		WFX_CONDITION(nPos < m_rgpItems.size());
		return m_rgpItems[nPos].get();
	}
	virtual ULONG getCount() const
	{
		return m_rgpItems.size();
	}
	virtual void reset()
	{
		m_rgpItems.clear();
	}
protected:
	std::vector<SP_Ty> m_rgpItems;
};

class WFX_API ObjectAdapterBase : public AdapterBase<AttributeBase> 
{
public:
	ObjectAdapterBase(ArrayAdapterBase* pArrayAdapter = NULL): m_pArrayAdapter(pArrayAdapter){}
public:
	virtual void createAttributes() = 0;
	virtual void accept(ObjectAdapterVisitorBase* pVisitor) = 0;
public:
	void setArrayAdapter(ArrayAdapterBase* pArrayAdapter){ m_pArrayAdapter = pArrayAdapter; }
	ArrayAdapterBase* getArrayAdapter() const { return m_pArrayAdapter; }
protected:
	PArrayAdapterBase m_pArrayAdapter;
};

class WFX_API ObjectAdapterFactoryBase
{
public:
	virtual ObjectAdapterBase* createObject() = 0;
};

template <class _Ty>
class ObjectAdapterFactory : public ObjectAdapterFactoryBase
{
	virtual ObjectAdapterBase* createObject()
	{
		return new _Ty();
	}
};

class WFX_API ArrayAdapterBase : public AdapterBase<ObjectAdapterBase>
{
public:
	ArrayAdapterBase(Widget* pWidget): m_pOwner(pWidget){}
public:
	void setObjectAdapterVisitor(ObjectAdapterVisitorBase* pObjectAdapterVisitor) { m_pObjectAdapterVisitor.reset(pObjectAdapterVisitor); }
	ObjectAdapterVisitorBase* getObjectAdapterVisitor() const { return m_pObjectAdapterVisitor.get(); }
	void setObjectAdapterFactory(ObjectAdapterFactoryBase* pFactory) { m_pObjectAdapterFactory.reset(pFactory); }
	ObjectAdapterFactoryBase* getObjectAdapterFactory() const { return m_pObjectAdapterFactory.get(); }
protected:
	SPObjectAdapterVisitorBase m_pObjectAdapterVisitor;
	SPObjectAdapterFactoryBase m_pObjectAdapterFactory;
	PWidget m_pOwner;
};

class WFX_API ObjectAdapterVisitorBase : public AdapterBase<AttributeBase>
{
public:
	virtual void visit(AttributeBase* pAttribute)
	{
		m_rgpAttribute.push_back(pAttribute);
	}
	virtual void skip()
	{
		visit(NULL);
	}
	virtual AttributeBase* getAt(ULONG nPos) const
	{
		return m_rgpAttribute[nPos];
	}
	virtual void reset()
	{
		m_rgpAttribute.clear();
	}
protected:
	std::vector<AttributeBase*> m_rgpAttribute;
};

template <class _Ty>
String convert(const _Ty& t)
{
	return L"";
}

template <class _Ty, class _TObjectAdapter = ObjectAdapterBase>
class Attribute : public AttributeBase
{
public:
	typedef _Ty _Ty;
	typedef _Ty (_TObjectAdapter::*GetDelegate)();
	typedef void (_TObjectAdapter::*SetDelegate)(const _Ty&);
	typedef GetDelegate GetDelegate;
	typedef SetDelegate SetDelegate;
public:
	Attribute(GetDelegate pGet, _TObjectAdapter* pObject)
		: m_pGet(pGet)
		, m_pSet(NULL)
		, m_pObject(pObject)
	{
	}
	Attribute(GetDelegate pGet, SetDelegate pSet, _TObjectAdapter* pObject)
		: m_pGet(pGet)
		, m_pSet(pSet)
		, m_pObject(pObject)
	{
	}
public:
	virtual _Ty getValue() const
	{
		WFX_CONDITION(m_pGet != NULL);
		WFX_CONDITION(m_pObject != NULL);
		return (m_pObject->*m_pGet)();
	}
	virtual void setValue(const _Ty& tValue)
	{
		WFX_CONDITION(m_pSet != NULL);
		WFX_CONDITION(m_pObject != NULL);
		(m_pObject->*m_pSet)(tValue);
	}
	virtual String getText() const
	{
		return convert(getValue());
	}

	virtual COLORREF getTextColor() const
	{
		return WBTN_BKGND_MOUSE;
	}
	virtual COLORREF getBkgndColor()
	{
		return WBTN_BKGND_STATIC
	}
	virtual COLORREF getFormat() const
	{
		return DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	}
	virtual DWORD getType() const
	{
		return AT_TEXT_READ;
	}
	virtual BOOL isValid() const
	{
		return TRUE;
	}
protected:
	GetDelegate m_pGet;
	SetDelegate m_pSet;
	_TObjectAdapter* m_pObject;
};

template <class _TWidget = Widget>
class ObjectAdapterVisitor : public ObjectAdapterVisitorBase
{
public:
	typedef void (_TWidget::*OnVisitHandler)(ULONG, AttributeBase*);
public:
	ObjectAdapterVisitor(OnVisitHandler pOnVisit = NULL)
		: m_pOnVisit(pOnVisit)
		, m_nStart(0)
		, m_nEnd(0)
	{

	}
public:
	void setOnVisit(OnVisitHandler pOnVisit, _TWidget* pWidget)
	{
		m_pOnVisit = pOnVisit;
		m_pWidget = pWidget;
	}
	virtual void visit(AttributeBase* pAttribute)
	{
		__super::visit(pAttribute);
		if (m_pOnVisit != NULL && m_pWidget != NULL)
		{
			(m_pWidget->*m_pOnVisit)(m_rgpAttribute.size() - 1, pAttribute);
		}
	}

protected:
	OnVisitHandler m_pOnVisit;
	_my_ptr<_TWidget> m_pWidget;
	ULONG m_nStart;
	ULONG m_nEnd;
};

template <class _Ty, class _TObjectAdapter>
AttributeBase* createConstAttribute(typename Attribute<_Ty, _TObjectAdapter>::GetDelegate pGet, 
									_TObjectAdapter* pObject)
{
	AttributeBase* pAttribute = new Attribute<_Ty, _TObjectAdapter>(pGet, pObject);
	pObject->addItem(pAttribute);
	return pAttribute;
}

template <class _Ty, class _TObjectAdapter>
AttributeBase* createAttribute(typename Attribute<_Ty, _TObjectAdapter>::GetDelegate pGet, 
							   typename Attribute<_Ty, _TObjectAdapter>::SetDelegate pSet,
							   _TObjectAdapter* pObject)
{
	AttributeBase* pAttribute = new Attribute<_Ty, _TObjectAdapter>(pGet, pSet, pObject);
	pObject->addItem(pAttribute);
	return pAttribute;
};

template <class _Tx>
class WFX_API ObjectAdapter : public ObjectAdapterBase
{
	typedef _my_ptr<_Tx> P_Tx;
	typedef _my_ptr<Attribute<_Tx>> P_TxAttribute;
public:
	ObjectAdapter(_Tx* pTx):m_pTx(pTx){}
public:
	_Tx getTy() const { return *m_pTx; } 
	void setTy(const _Tx& rTx) { *m_pTx = rTx; } 
public:
	virtual void createAttributes()
	{
		m_pATx = createAttribute<_Tx, ObjectAdapter>(&ObjectAdapter::getTy, &ObjectAdapter::setTy, this);
	}
	virtual void accept(ObjectAdapterVisitorBase* pVisitor)
	{
		pVisitor->visit(m_pATx);
	}
	virtual void setReference(_Tx* pTx)
	{
		m_pTx = pTx;
	}
protected:
	P_TxAttribute m_pATx;
	P_Tx m_pTx;
};

template <class _Tx>
class ArrayAdapter : public ArrayAdapterBase
{
public:
	ArrayAdapter(Widget* pWidget, const std::vector<_Tx>& rgData)
		: ArrayAdapterBase(pWidget)
		, m_rgData(rgData)
	{
		m_pObjectAdapterFactory.reset(new ObjectAdapterFactory<ObjectAdapter<_Tx>>());
		m_pObjectAdapterVisitor.reset(new ObjectAdapterVisitor);
	}
	void doInit()
	{
		ObjectAdapterFactoryBase* pFactory = getObjectAdapterFactory();
		WFX_CONDITION(pFactory != NULL);
		ObjectAdapterBase* pObjectAdapter = NULL;
		for (std::vector<_Tx>::const_iterator it = m_rgData.begin();
			it != m_rgData.end(); ++it)
		{
			pObjectAdapter = pFactory->createObject();
			dynamic_cast<ObjectAdapter<_Tx>*>(pObjectAdapter)->setReference(&(*it));
			WFX_CONDITION(pObjectAdapter != NULL);
			pObjectAdapter->createAttributes();
			addItem(pObjectAdapter);
		}
	}
protected:
	std::vector<_Tx> m_rgData;
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API HeaderCtrl : public Widget
{
	friend class ListCtrl;
public:
	HeaderCtrl();
public:
	ULONG getItemCount() const;
	BOOL getItemRect(ULONG nIndex, Rect& rc);
	ULONG addItem(const HeaderInfo& hdi);
	ULONG insertItem(ULONG nPos, const HeaderInfo& hdi);
	BOOL removeItem(ULONG nPos);
protected:
	virtual void onDraw(HDC hdc, const Rect& rcPaint);
	Cell* getCell(ULONG nCol);
public:
	ULONG getCellType(ULONG nCol) const;
public:
	WFX_BEGIN_MSG_MAP(HeaderCtrl)
		WFX_ON_MESSAGE(WM_LBUTTONUP, onLButtonUp)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
protected:
	LONG getSelected() const;
	void setSelected(LONG nSelected);
	BOOL IsAscendSort() const;
	ULONG getTotalColumns() const;
	ULONG getColumnWidth(ULONG nCol) const;
	LONG getSelected(POINT pt);
	void calcCellRect();
	inline LCDrawItemInfo* getDrawItemInfo() const;
public:
	wfx_msg LRESULT onLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
protected:
	BOOL verify() const;
protected:
	std::vector<SPHeaderInfo> m_rgpHdi;
	Rect m_rcSeqBarHead;
	LONG m_nSelected;
	BOOL m_bAscendSort;
	SPLCDrawItemInfo m_pDrawItemInfo;
};
typedef std::tr1::shared_ptr<HeaderCtrl> SPHeadCtrl;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API CellID
{
public:
	CellID(LONG nRow = -1, LONG nCol = -1);
public:
	BOOL isValidID() const;
	bool operator()(const CellID& id1, const CellID& id2) const;
	bool operator<(const CellID& other) const;
	bool operator==(const CellID& other) const;
	bool operator!=(const CellID& other) const;
	void clear();
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
		CacheInfo(ULONG nLayer = -1, BOOL bexpanded = TRUE)
			: m_nLayer(nLayer)
			, m_bexpanded(bexpanded)
		{

		}
		ULONG m_nLayer;
		BOOL m_bexpanded;
	};
public:
	VorticalLayerCtrl();
protected:
	ULONG addItem();
	ULONG insertItem(ULONG nItem);
	ULONG insertSubItem(ULONG nItem);
	ULONG expand(ULONG nItem, BOOL bexpand = TRUE);
	BOOL isExpanded(ULONG nItem) const;
	ULONG getLayer(ULONG nItem) const;
	ULONG getTotalItems() const;
	void refreshCache();
protected:
	SPTNode m_pRoot;
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
		WFX_ON_MESSAGE(WM_CREATE, onCreate)
		WFX_ON_MESSAGE(WUM_LC_GET_PROPERTIES, ongetProperties)
		WFX_ON_MESSAGE(WM_SIZE, onSize)
		WFX_ON_MESSAGE(WM_LBUTTONUP, onLButtonUp)
		WFX_ON_MESSAGE(WUM_LC_HITCOL, onColChanged)
		WFX_ON_MESSAGE(WUM_LC_HEADSIZE, onHeadSizeChanged)
		WFX_ON_MESSAGE(WM_HSCROLL, onHScroll)
		WFX_ON_MESSAGE(WM_VSCROLL, onVScroll)
		WFX_ON_MESSAGE(WM_MOUSEWHEEL, onMouseWheel)
		WFX_ON_MESSAGE(WM_MOUSEMOVE, onMouseMove)
		WFX_ON_MESSAGE(WM_MOUSELEAVE, onMouseLeave)
		WFX_ON_MESSAGE(WUM_LC_CELL_DRAW, onCellDraw)
		WFX_ON_MESSAGE(WUM_LC_CELL_EXPAND, onCellexpand)
		WFX_ON_MESSAGE(WM_TIMER, onTimer)
		WFX_ON_MESSAGE(WUM_LC_SEQHEAD_DRAW, onMsgSeqHeadDraw)
		WFX_ON_MESSAGE(WUM_LC_HEAD_DRAW, onMsgHeadDraw)
		WFX_ON_MESSAGE(WM_KEYDOWN, onKeyDown)
		WFX_CHAIN_MSG_MAP(Widget)
	WFX_END_MSG_MAP()
public:
	wfx_msg LRESULT onCreate(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT onSize(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT onLButtonUp(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT onColChanged(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT onHeadSizeChanged(UINT uMsg, WPARAM wParam, 
		LPARAM lParam, BOOL& bHandled);
	wfx_msg LRESULT onHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT ongetProperties(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onCellDraw(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onCellexpand(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMsgSeqHeadDraw(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onMsgHeadDraw(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled);
	wfx_msg LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, 
		BOOL& bHandled);
public:
	HeaderCtrl* getHeaderCtrl() const;
	void setHeaderCtrl(HeaderCtrl* pHeader);
	Cell* getCell(ULONG nRow, ULONG nCol);
	Cell* getCell(const CellID& cellID);
	Cell* getCell(ULONG nCellType);
	BOOL getCellIndirect(const CellID& cellID, 
		std::pair<Cell*, Rect>& cell);
	Rect getCellRect(ULONG nRow, ULONG nCol);
public:
	ULONG getColumnWidth(ULONG nCol) const;
	BOOL setColumnWidth(ULONG nCol, ULONG cx);
	ULONG getRowHeight(ULONG nRow) const;
	BOOL setRowHeight(ULONG nRow, ULONG cy);
	String getItemText(ULONG nItem, ULONG nSubItem) const;
	BOOL setItemText(ULONG nItem, ULONG nSubItem, 
		const String& strText);
public:
	ULONG getTotalRows() const;
	ULONG getTotalColumns() const;

	ULONG getRowNumBarWidth() const;
	void setRowNumBarWidth(ULONG nWidth);
	ULONG getHeadHeight() const;
	void setHeadHeight(ULONG nHeight);
	LONG getFixedRow() const;
	void setFixedRow(LONG nRow);
	LONG getFixedCol() const;
	void setFixedCol(LONG nCol);
	LONG getStartRow() const;
	void setStartRow(LONG nRow);
	LONG getEndRow() const;
	void setEndRow(LONG nRow);
	LONG getStartCol() const;
	void setStartCol(LONG nCol);
	LONG getEndCol() const;
	void setEndCol(LONG nCol);
	LONG getCurrentRow() const;
	LONG getCurrentCol() const;
public:
	ULONG addColumn(const String& strName, 
		UINT nWidth, DWORD dwFormat, ULONG nCellType, BOOL bAdjust = FALSE);
	ULONG insertColumn(ULONG nCol, const String& strName, 
		UINT nWidth, DWORD dwFormat, ULONG nCellType, BOOL bAdjust = FALSE);
	ULONG addRow(LONG nHeight = -1, BOOL bAdjust = FALSE);
	ULONG insertRow(ULONG nRow, LONG nHeight = -1, BOOL bAdjust = FALSE);
	ULONG insertSubRow(ULONG nRow, LONG nHeight = -1, BOOL bAdjust = FALSE);
	ULONG expand(ULONG nRow, BOOL bexpand = TRUE, BOOL bAdjust = FALSE);
	BOOL isExpanded(ULONG nRow) const;
protected:
	BOOL getCellID( POINT pt, std::pair<CellID, Rect>& cellID) const;
	BOOL isValidCellID(const CellID& cellID) const;
protected:
	virtual void onDraw(HDC hdc, const Rect& rcPaint);
	virtual void onDrawItem(const LCDrawItemInfo* pDrawItemInfo);
	virtual void onDrawSeqBar(const LCDrawItemInfo* pDrawItemInfo);
	virtual void onDrawSeqHead(const LCDrawItemInfo* pDrawItemInfo);
	virtual void onDrawHeadCtrl(const LCDrawItemInfo* pDrawItemInfo);
	virtual void onVisit(ULONG nPos, AttributeBase* pAttribute);
protected:
	virtual void calcVirtualSize(Size& sz);
	virtual void getToolTip(String& str) const;
	virtual Rect getClientRect() const;
protected:
	BOOL calcCol();
	BOOL calcRow();
	void calcCellRect();
	BOOL calcPos(int nBar);
	BOOL verify() const;
	inline LCDrawItemInfo* getDrawItemInfo() const;
protected:
	SPHeadCtrl m_pHeadCtrl;
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
	SPArrayAdapterBase m_pArrayAdapter;
};
typedef std::tr1::shared_ptr<ListCtrl> SPListCtrl;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API TreeCtrl : public VorticalLayerCtrl
{
};
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API RichCtrl : public Widget
{

};

END_NAMESPACE_WFX
