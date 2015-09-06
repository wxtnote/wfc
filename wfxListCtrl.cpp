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

#include "StdAfx.h"
#include "wfxwidget.h"
#include "wfxcmnctrl.h"
#include "wfxrender.h"

USING_NAMESPACE_WFX;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
TNode::TNode()
: m_bexpanded(TRUE)
{

}

ULONG TNode::addChild(const SPTNode& pNode)
{
	WFX_CONDITION(pNode != NULL);
	pNode->setParent(this);
	m_rgpChildren.push_back(pNode);
	return m_rgpChildren.size();
}

ULONG TNode::InsertChild(ULONG nPos, const SPTNode& pNode)
{
	BOOL bCondition = nPos <= m_rgpChildren.size();
	WFX_CONDITION(bCondition);
	if (bCondition)
	{
		m_rgpChildren.insert(m_rgpChildren.begin() + nPos, pNode);
		return m_rgpChildren.size();
	}
	return 0;
}

ULONG TNode::getChildren(std::vector<SPTNode>& rgpChilren) const
{
	rgpChilren = m_rgpChildren;
	return rgpChilren.size();
}

ULONG TNode::getChildren() const
{
	return m_rgpChildren.size();
}

BOOL TNode::hasChild() const
{
	return m_rgpChildren.size() > 0;
}

ULONG TNode::getMyLayer() const
{
	ULONG nLayer = 0;
	if (m_pParent == NULL)
	{
		return nLayer;
	}
	nLayer++;
	return nLayer + m_pParent->getMyLayer();
}

BOOL TNode::hasLayers() const
{
	for (std::vector<SPTNode>::const_iterator it =
		m_rgpChildren.begin(); it != m_rgpChildren.end(); ++it)
	{
		if ((*it)->hasChild())
		{
			return TRUE;
		}
	}
	return FALSE;
}

TNode* TNode::getAt(ULONG nItem, BOOL bItemexpanded)
{
	return bItemexpanded?
		getAt_expanded(nItem) : getAt_All(nItem);
}

TNode* TNode::operator[](ULONG nItem)
{
	return getAt(nItem, TRUE);
}

TNode* TNode::getParent() const
{
	return m_pParent;
}

void TNode::setParent(TNode* pParent)
{
	m_pParent = pParent;
}

BOOL TNode::isExpanded() const
{
	return m_bexpanded;
}

void TNode::expand(BOOL bexpand /*= TRUE*/)
{
	m_bexpanded = bexpand;
}

BOOL TNode::isExpanded(ULONG nItem, BOOL bItemexpanded /*= TRUE*/)
{
	TNode* pNode = getAt(nItem, bItemexpanded);
	if (pNode != NULL)
	{
		return pNode->isExpanded();
	}
	return FALSE;
}

ULONG TNode::expand(ULONG nItem, BOOL bItemexpanded /*= TRUE*/, BOOL bexpand /*= TRUE*/)
{
	TNode* pNode = getAt(nItem, bItemexpanded);
	ULONG nNodeItems = 0;
	if (pNode != NULL)
	{
		if (!bexpand)
		{
			nNodeItems = pNode->getItems(bItemexpanded);
		}
		pNode->expand(bexpand);
		if (bexpand)
		{
			nNodeItems = pNode->getItems(bItemexpanded);
		}
		return nNodeItems;
	}
	return 0;
}

ULONG TNode::getItems(BOOL bItemexpanded /*= TRUE*/) const
{
	return bItemexpanded? 
		getItems_expanded(this) : getItems_All(this);
}

ULONG TNode::getItems_All(const TNode* pNode) const
{
	ULONG nItems = 1;
	for (std::vector<SPTNode>::const_iterator it = 
		m_rgpChildren.begin();
		it != m_rgpChildren.end(); ++it)
	{
		nItems += getItems_All((*it).get());
	}
	return nItems;
}

ULONG TNode::getItems_expanded(const TNode* pNode) const
{
	ULONG nItems = 0;
	if (pNode->isExpanded())
	{
		std::vector<SPTNode> rgpChilren;
		pNode->getChildren(rgpChilren);
		for (std::vector<SPTNode>::const_iterator it = 
			rgpChilren.begin(); it != rgpChilren.end(); ++it)
		{
			nItems++;
			nItems += getItems_expanded((*it).get());
		}
	}
	return nItems;
}

TNode* TNode::getAt_All(ULONG nItem) const
{
	ULONG nItems = 1;
	for (std::vector<SPTNode>::const_iterator it = 
		m_rgpChildren.begin();
		it != m_rgpChildren.end(); ++it)
	{
		if (nItems - 1 == nItem)
		{
			return (*it).get();
		}
		nItems += getItems_All((*it).get());
	}
	return NULL;
}

TNode* TNode::getAt_expanded(ULONG nItem)
{
	ULONG nItems = 0;
	return getAt_expanded(nItem, nItems, this);
}

TNode* TNode::getAt_expanded(ULONG nItem, ULONG& nItems, TNode* pNode)
{
	TNode* pResultNode = NULL;
	if (pNode->isExpanded())
	{
		std::vector<SPTNode> rgpChilren;
		pNode->getChildren(rgpChilren);
		for (std::vector<SPTNode>::const_iterator it = 
			rgpChilren.begin(); it != rgpChilren.end(); ++it)
		{
			nItems++;
			if (nItems - 1 == nItem)
			{
				pResultNode = (*it).get();
				break;
			}
			pResultNode = getAt_expanded(nItem, nItems, (*it).get());
			if (pResultNode != NULL)
			{
				break;
			}
		}
	}
	else
	{
		if (nItems - 1 == nItem)
		{
			return pNode;
		}
	}
	return pResultNode;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
Cell::Cell()
{

}

LRESULT Cell::onMouseMove( UINT uMsg, 
						  WPARAM wParam, 
						  LPARAM lParam,
						  BOOL& bHandled )
{
	return sendParentMessage(WUM_LC_CELL_DRAW, wParam);
}

LRESULT Cell::onMouseLeave( UINT uMsg, 
						   WPARAM wParam, 
						   LPARAM lParam, 
						   BOOL& bHandled )
{
	return sendParentMessage(WUM_LC_CELL_DRAW, wParam);
}

LRESULT Cell::onLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return sendParentMessage(WUM_LC_CELL_DRAW, wParam);
}

void Cell::Draw( HDC hdc, const Rect& rcPaint,  DWORD dwState,
				const String& strText, COLORREF clrText, DWORD dwFormat )
{
	WfxRender::drawHeadCell(hdc, rcPaint, dwState, strText,
		clrText, dwFormat);
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
void LayerCell::Draw(HDC hdc,const Rect& rcPaint, DWORD dwState,
		const String& strText, COLORREF clrText, DWORD dwFormat)
{
	WfxRender::drawLayerCell(hdc, rcPaint, dwState, strText, clrText, dwFormat);
}

LRESULT LayerCell::onLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
		BOOL& bHandled)
{
	return sendParentMessage(WUM_LC_CELL_EXPAND, wParam);;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ULONG HeaderCtrl::getItemCount() const
{
	return m_rgpHdi.size();
}

BOOL HeaderCtrl::getItemRect( ULONG nIndex, 
							 Rect& rc )
{
	return TRUE;
}

ULONG HeaderCtrl::addItem(const HeaderInfo& hdi)
{
	SPHeaderInfo pHdi(new HeaderInfo(hdi));
	m_rgpHdi.push_back(pHdi);
	return m_rgpHdi.size();
}

ULONG HeaderCtrl::insertItem( ULONG nPos, 
							 const HeaderInfo& hdi )
{
	WFX_CONDITION(nPos < m_rgpHdi.size());
	if (nPos < m_rgpHdi.size())
	{
		SPHeaderInfo pHdi(new HeaderInfo(hdi));
		m_rgpHdi.insert(m_rgpHdi.begin() + nPos, pHdi);
		return m_rgpHdi.size();
	}
	return 0;
}

BOOL HeaderCtrl::removeItem( ULONG nPos )
{
	return FALSE;
}

void HeaderCtrl::onDraw( HDC hdc, const Rect& rcPaint )
{
	LONG nStartCol = sendParentMessage(WUM_LC_GET_PROPERTIES, LCP_START_COL);
	LONG nEndCol = sendParentMessage(WUM_LC_GET_PROPERTIES, LCP_END_COL);
	DWORD dwState = WCS_NORMAL;
	LCDrawItemInfo* pDrawItemInfo = getDrawItemInfo();
	pDrawItemInfo->clear();
	pDrawItemInfo->m_hDC = hdc;
	pDrawItemInfo->m_prcItem = &m_rcSeqBarHead;
	pDrawItemInfo->m_dwState = dwState;
	sendParentMessage(WUM_LC_SEQHEAD_DRAW, (WPARAM)pDrawItemInfo);
	LONG nTotalCal = getTotalColumns();
	if (nTotalCal <= 0)
	{
		return;
	}

	for (LONG nCol = nStartCol; nCol <= nEndCol; nCol++)
	{
		if (nCol == m_nSelected)
		{
			dwState |= WCS_SELECTED;
			m_bAscendSort? dwState |= WCS_ASORT : dwState |= WCS_NASORT;
		}
		else
		{
			dwState = WCS_NORMAL;
		}
		pDrawItemInfo->m_nCol = nCol;
		pDrawItemInfo->m_prcItem = &(m_rgpHdi[nCol]->rcPos);
		pDrawItemInfo->m_dwState = dwState;
		pDrawItemInfo->m_dwFormat = m_rgpHdi[nCol]->dwFormat;
		pDrawItemInfo->m_pszText = m_rgpHdi[nCol]->strText.c_str();
		sendParentMessage(WUM_LC_HEAD_DRAW, (WPARAM)pDrawItemInfo);
		
	}
}

LRESULT HeaderCtrl::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Rect rc = getRect();
	Rect rcItem = rc;
	for (ULONG i = 0; i < m_rgpHdi.size(); i++)
	{
		rcItem.right = rcItem.left + m_rgpHdi[i]->cx;
		
		m_rgpHdi[i]->rcPos = rcItem;
		
		rcItem.left += rcItem.right - rcItem.left;
	}
	sendParentMessage(WUM_LC_HEADSIZE);
	return 1;
}

LRESULT HeaderCtrl::onLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (!verify())
	{
		return 1;
	}
	Point pt(lParam);
	LONG nSelected = getSelected(pt);
	m_bAscendSort = !m_bAscendSort;
	m_nSelected = nSelected;
	sendParentMessage(WUM_LC_HITCOL, 
		(WPARAM)m_nSelected, (LPARAM)m_bAscendSort);
	return 1;
}

HeaderCtrl::HeaderCtrl()
: m_nSelected(-1)
, m_bAscendSort(FALSE)
, m_pDrawItemInfo(new LCDrawItemInfo)
{

}

LONG HeaderCtrl::getSelected() const
{
	return m_nSelected;
}

void HeaderCtrl::setSelected( LONG nSelected )
{
	m_nSelected = nSelected;
}

LONG HeaderCtrl::getSelected( POINT pt )
{
	LONG nStartCol = sendParentMessage(WUM_LC_GET_PROPERTIES, LCP_START_COL);
	LONG nEndCol = sendParentMessage(WUM_LC_GET_PROPERTIES, LCP_END_COL);
	Rect rcItem;
	for (LONG nCol = nStartCol; nCol <= nEndCol; nCol++)
	{
		rcItem = m_rgpHdi[nCol]->rcPos;
		if (rcItem.PtInRect(pt))
		{
			return nCol;
		}
	}
	return -1;
}

void HeaderCtrl::calcCellRect()
{
	LONG nStartCol = sendParentMessage(WUM_LC_GET_PROPERTIES, LCP_START_COL);
	LONG nEndCol = sendParentMessage(WUM_LC_GET_PROPERTIES, LCP_END_COL);
	if (nStartCol < 0 || nEndCol < 0)
	{
		return;
	}
	ULONG nRowNumBarWidth = sendParentMessage(WUM_LC_GET_PROPERTIES, LCP_ROWNUM_WIDTH);
	Rect rcWid = getRect();
	m_rcSeqBarHead = rcWid;
	m_rcSeqBarHead.right = m_rcSeqBarHead.left + nRowNumBarWidth;
	Rect rcItem = rcWid;
	rcItem.left += nRowNumBarWidth;
	for (LONG nCol = nStartCol; nCol <= nEndCol && nCol < m_rgpHdi.size(); nCol++)
	{
		rcItem.right = rcItem.left + m_rgpHdi[nCol]->cx;
		m_rgpHdi[nCol]->rcPos = rcItem;
		rcItem.left += rcItem.right - rcItem.left;
	}
}

LCDrawItemInfo* HeaderCtrl::getDrawItemInfo() const
{
	return m_pDrawItemInfo.get();
}

Cell* HeaderCtrl::getCell( ULONG nCol )
{
	static Cell cell;
	return &cell;
}

ULONG HeaderCtrl::getCellType(ULONG nCol) const
{
	WFX_CONDITION(nCol < m_rgpHdi.size());
	if (nCol < m_rgpHdi.size())
	{
		return m_rgpHdi[nCol]->m_nType;
	}
	return 0;
}

BOOL HeaderCtrl::IsAscendSort() const
{
	return m_bAscendSort;
}

ULONG HeaderCtrl::getTotalColumns() const
{
	return m_rgpHdi.size();
}

ULONG HeaderCtrl::getColumnWidth( ULONG nCol ) const
{
	if (nCol >= m_rgpHdi.size())
		return 0;
	return m_rgpHdi[nCol]->cx;
}

BOOL HeaderCtrl::verify() const
{
	ULONG nTotalCol = getTotalColumns();
	return nTotalCol > 0;
}
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
CellID::CellID(LONG nRow /*= -1*/, LONG nCol /*= -1*/)
: m_nRow(nRow)
, m_nCol(nCol)
{
}

BOOL CellID::isValidID() const
{
	return (m_nRow >= 0 && m_nCol >= 0);
}

bool CellID::operator()(const CellID& id1, const CellID& id2) const
{
	if (id1.m_nRow < id2.m_nRow)
		return true;
	else if (id1.m_nRow > id2.m_nRow)
		return false;
	else if (id1.m_nCol < id2.m_nCol )
		return true;
	else
		return false;
}

bool CellID::operator<(const CellID& other) const
{
	return operator()(*this, other);
}

bool CellID::operator==( const CellID& other ) const
{
	return (other.m_nRow == m_nRow && other.m_nCol == m_nCol);
}

bool CellID::operator!=(const CellID& other) const
{
	return !operator==(other);
}

void CellID::clear()
{
	m_nRow = -1;
	m_nCol = -1;
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
VorticalLayerCtrl::VorticalLayerCtrl()
: m_pRoot(new TNode)
, m_bCached(TRUE)
{

}

ULONG VorticalLayerCtrl::addItem()
{
	setVirtualSizeCached(FALSE);
	m_pRoot->addChild(SPTNode(new TNode));
	CacheInfo chinfo(1, TRUE);
	m_rgCacheInfo.push_back(chinfo);
	return m_rgCacheInfo.size();
}

ULONG VorticalLayerCtrl::insertItem(ULONG nItem)
{
	if (m_pRoot->InsertChild(nItem, SPTNode(new TNode)) != 0)
	{
		setVirtualSizeCached(FALSE);
		CacheInfo chinfo(1, TRUE);
		WFX_CONDITION(nItem + 1 <= m_rgCacheInfo.size());
		m_rgCacheInfo.insert(m_rgCacheInfo.begin() + nItem + 1, chinfo);
	}
	return m_rgCacheInfo.size();
}

ULONG VorticalLayerCtrl::insertSubItem(ULONG nItem)
{
	TNode* pNode = m_pRoot->getAt(nItem);
	if (pNode != NULL)
	{
		setVirtualSizeCached(FALSE);
		pNode->addChild(SPTNode(new TNode));
		if (pNode->isExpanded())
		{
			CacheInfo chinfo(pNode->getMyLayer() + 1, TRUE);
			WFX_CONDITION(nItem + 1 <= m_rgCacheInfo.size());
			m_rgCacheInfo.insert(m_rgCacheInfo.begin() + nItem + 1, chinfo);
		}
	}
	return m_rgCacheInfo.size();
}
	
ULONG VorticalLayerCtrl::expand(ULONG nItem, BOOL bexpand /*= TRUE*/)
{
	m_bCached = FALSE;
	ULONG nLine = m_pRoot->expand(nItem, TRUE, bexpand);
	if (bexpand)
	{
		WFX_CONDITION(nItem < m_rgCacheInfo.size());
		m_rgCacheInfo[nItem].m_bexpanded = TRUE;
		for (int i = 0; i < nLine; i++)
		{
			WFX_CONDITION(nItem + i + 1 <= m_rgCacheInfo.size());
			CacheInfo chinfo(getLayer(nItem + i + 1), TRUE);
			m_rgCacheInfo.insert(m_rgCacheInfo.begin() + nItem + 1, chinfo);
		}
	}
	else
	{
		m_rgCacheInfo[nItem].m_bexpanded = FALSE;
		for (int i = 0; i < nLine; i++)
		{
			WFX_CONDITION(nItem + i + 1 < m_rgCacheInfo.size());
			m_rgCacheInfo.erase(m_rgCacheInfo.begin() + nItem + 1);
		}
	}
	refreshCache();
	return m_rgCacheInfo.size();
}

BOOL VorticalLayerCtrl::isExpanded(ULONG nItem) const
{
	if (m_bCached)
	{
		WFX_CONDITION(nItem < m_rgCacheInfo.size());
		return m_rgCacheInfo[nItem].m_bexpanded;
	}
	return m_pRoot->isExpanded(nItem);
}

ULONG VorticalLayerCtrl::getLayer(ULONG nItem) const
{
	if (m_bCached)
	{
		WFX_CONDITION(nItem < m_rgCacheInfo.size());
		return m_rgCacheInfo[nItem].m_nLayer;
	}
	TNode* pNode = m_pRoot->getAt(nItem);
	if (pNode != NULL)
	{
		return pNode->getMyLayer();
	}
	return 0;
}

ULONG VorticalLayerCtrl::getTotalItems() const
{
	if (m_bCached)
	{
		return m_rgCacheInfo.size();
	}
	return m_pRoot->getItems();
}

void VorticalLayerCtrl::refreshCache()
{
	m_bCached = TRUE;
}

///////////////////////////*** a gorgeous partition line ***/////////////////////////////
ListCtrl::ListCtrl()
: m_pHeadCtrl(new HeaderCtrl)
, m_nHeadHeight(WID_LC_HEAD_HEIGHT)
, m_nStartRow(0)
, m_nEndRow(0)
, m_nStartCol(0)
, m_nEndCol(0)
, m_nFixedRow(-1)
, m_nFixedCol(-1)
, m_nRowNumBarWidth(20)
, m_bHasSubItem(FALSE)
{

}

ULONG ListCtrl::getColumnWidth( ULONG nCol ) const
{
	return m_pHeadCtrl->getColumnWidth(nCol);
}

BOOL ListCtrl::setColumnWidth( ULONG nCol, ULONG cx )
{
	return TRUE;
}

HeaderCtrl* ListCtrl::getHeaderCtrl() const
{
	return m_pHeadCtrl.get();
}

void ListCtrl::setHeaderCtrl( HeaderCtrl* pHeader )
{

}

String ListCtrl::getItemText( ULONG nItem, 
								   ULONG nSubItem ) const
{
	return L"";
}

BOOL ListCtrl::setItemText( ULONG nItem, 
						   ULONG nSubItem, 
						   const String& strText )
{
	return FALSE;
}

LRESULT ListCtrl::onCreate( UINT uMsg, 
						   WPARAM wParam, 
						   LPARAM lParam, 
						   BOOL& bHandled )
{
	Rect rc;
	m_pHeadCtrl->create(rc, this);
	return 0;
}

LRESULT ListCtrl::onSize( UINT uMsg, 
						 WPARAM wParam, 
						 LPARAM lParam, 
						 BOOL& bHandled )
{
	Rect rc = getClientRect();
	rc.bottom = rc.top + m_nHeadHeight;
	rc.left += 1;
	rc.right -= 1;
	rc.top += 1;
	if (!rc.isValid())
	{
		rc.empty();
	}
	m_pHeadCtrl->setRect(rc);
	calcRow();
	calcCol();
	calcCellRect();
	m_pHeadCtrl->calcCellRect();
	return 1;
}

ULONG ListCtrl::addColumn(const String& strName, 
				UINT nWidth, DWORD dwFormat, ULONG nCellType, BOOL bAdjust /*= FALSE*/)
{
	setVirtualSizeCached(FALSE);
	HeaderInfo hi;
	hi.cx = nWidth;
	hi.strText = strName;
	hi.m_nType = nCellType;

	ULONG nTotalCols = m_pHeadCtrl->addItem(hi);
	if (bAdjust)
	{

	}
	return nTotalCols;
}

ULONG ListCtrl::insertColumn(ULONG nCol, const String& strName, 
						   UINT nWidth, 
						   DWORD dwFormat, 
						   ULONG nCellType,
						   BOOL bAdjust /*= FALSE*/)
{
	setVirtualSizeCached(FALSE);
	HeaderInfo hi;
	hi.cx = nWidth;
	hi.strText = strName;
	hi.m_nType = nCellType;

	ULONG nTotalCols = m_pHeadCtrl->insertItem(nCol, hi);
	if (bAdjust)
	{

	}
	return nTotalCols;
}

Cell* ListCtrl::getCell(ULONG nCellType)
{
	switch(nCellType)
	{
	case WCT_TEXT:
		{
			static Cell cell;
			cell.setParent(this);
			return &cell;
		}
	case WCT_Layer:
		{
			static LayerCell cell;
			cell.setParent(this);
			return &cell;
		}
	default:
		{
			static Cell cell;
			cell.setParent(this);
			return &cell;
		}
	}
}

Cell* ListCtrl::getCell( ULONG nRow, ULONG nCol )
{
	return getCell(m_pHeadCtrl->getCellType(nCol));
}

Cell* ListCtrl::getCell(const CellID& cellID)
{
	return getCell(cellID.m_nRow, cellID.m_nCol);
}

BOOL ListCtrl::getCellIndirect(const CellID& cellID, 
					 std::pair<Cell*, Rect>& cell)
{
	LONG nOffsetRow = cellID.m_nRow - m_nStartRow;
	LONG nOffsetCol = cellID.m_nCol - m_nStartCol;
	if (nOffsetRow >= m_rgRectFast.size())
	{
		return FALSE;
	}
	if (nOffsetCol >= m_rgRectFast[nOffsetRow].size())
	{
		return FALSE;
	}
	cell.first = getCell(cellID);
	cell.second = m_rgRectFast[nOffsetRow][nOffsetCol];
	return TRUE;
}

BOOL ListCtrl::getCellID( POINT pt, std::pair<CellID, Rect>& cellID) const
{
	for(std::map<CellID, Rect>::const_iterator it = m_rgRect.begin();
		it != m_rgRect.end(); ++it)
	{
		if (it->second.PtInRect(pt))
		{
			cellID = (*it);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL ListCtrl::isValidCellID( const CellID& cellID ) const
{
	return m_rgRect.find(cellID) != m_rgRect.end();
}

LRESULT ListCtrl::onColChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	invalidWid();
	return 1;
}

ULONG ListCtrl::addRow(LONG nHeight /*= -1*/, BOOL bAdjust /*= FALSE*/)
{
	ULONG nTotalItems = addItem();
	if (bAdjust)
	{

	}
	return nTotalItems;
}

ULONG ListCtrl::insertRow( ULONG nRow, LONG nHeight /*= -1*/, BOOL bAdjust /*= FALSE*/ )
{
	ULONG nTotalItems = insertItem(nRow);
	if (bAdjust)
	{

	}
	return nTotalItems;
}

ULONG ListCtrl::insertSubRow(ULONG nRow, LONG nHeight /*= -1*/, BOOL bAdjust /*= FALSE*/)
{
	if (!m_bHasSubItem)
	{
		insertColumn(0, L"Layer", 400, DT_VCENTER | DT_SINGLELINE | DT_CENTER, WCT_Layer);
		m_bHasSubItem = TRUE;
	}
	ULONG nTotalItems = insertSubItem(nRow);
	if (bAdjust)
	{

	}
	return nTotalItems;
}

ULONG ListCtrl::expand(ULONG nRow, BOOL bexpand /*= TRUE*/, BOOL bAdjust /*= FALSE*/)
{
	ULONG nTotalItems = __super::expand(nRow, bexpand);
	if (1/*bAdjust*/)
	{
		calcRow();
		calcCellRect();
	}
	return nTotalItems;
}

BOOL ListCtrl::isExpanded(ULONG nRow) const
{
	return __super::isExpanded(nRow);
}

COLORREF getColor(ULONG nLayer)
{
	ULONG nRed = 255;
	ULONG nGreen = 0;
	nRed -= (nLayer - 1) * 100;
	nGreen += (nLayer - 1) * 100;
	return RGB(nRed, nGreen, 0);
}
void ListCtrl::onDraw( HDC hdc, const Rect& rcPaint )
{
	Rect rc = getRect();
	if (!verify())
	{
		WfxRender::drawFrame(hdc, rc, WBTN_BKGND_MOUSE, getDispatcher());
		return;
	}
	LCDrawItemInfo* pDrawItemInfo = getDrawItemInfo();
	pDrawItemInfo->clear();
	pDrawItemInfo->m_hDC = hdc;
	Rect rcCell;
	DWORD dwState = WCS_NORMAL;
	ULONG nLayer = 0;
	BOOL bRowexpanded = FALSE;
	COLORREF clrText = 0;
	LONG nLayerOffset = 0;
	LONG nTotalRow = getTotalRows();
	LONG nTotalCol = getTotalColumns();
	ULONG nSeqBarWidth = getRowNumBarWidth();
	ObjectAdapterBase* pOjbect = NULL;
	for (LONG nRow = m_nStartRow, nRowNum = 0; nRow <= m_nEndRow; nRow++, nRowNum++)
	{
		pDrawItemInfo->m_nRow = nRow;
		WFX_CONDITION(m_rgRowNumRect.size() > nRowNum);
		pDrawItemInfo->m_prcItem = &m_rgRowNumRect[nRowNum];
		pDrawItemInfo->m_dwState = 0;
		onDrawSeqBar(pDrawItemInfo);
		nLayer = getLayer(nRow);
		nLayerOffset = (nLayer - 1) * 30;
		clrText = getColor(nLayer);
		if (m_bHasSubItem)
		{
			bRowexpanded = isExpanded(nRow);
		}
		pOjbect = m_pArrayAdapter->getAt(nRow);
		WFX_CONDITION(pOjbect != NULL);

		for (LONG nCol = m_nStartCol; nCol <= m_nEndCol; nCol++)
		{
			WFX_CONDITION(nCol < pOjbect->getCount());
			AttributeBase* pAttribute = pOjbect->getAt(nCol);
			WFX_CONDITION(pAttribute != NULL);

			CellID cellID(nRow, nCol);
			dwState = WCS_NORMAL;
			if (m_bHasSubItem && nCol == 0)
			{
				if (bRowexpanded)
				{
					dwState |= WCS_EXPAND;
				}
				else
				{
					dwState |= WCS_CLOSE;
				}
			}
			if (m_cellSelected == cellID)
			{
				dwState |= WCS_SELECTED;
			}
			if (m_cellMouseMove == cellID)
			{
				dwState |= WCS_MOUSEMOVE;
			}
			rcCell = getCellRect(nRow, nCol);
			rcCell.left += nLayerOffset;
			pDrawItemInfo->m_nCol = nCol;
			pDrawItemInfo->m_prcItem = &rcCell;
			pDrawItemInfo->m_dwState = dwState;
			pDrawItemInfo->m_pszText = pAttribute->getText().c_str();
			pDrawItemInfo->m_dwFormat = pAttribute->getFormat();
			onDrawItem(pDrawItemInfo);
		}
	}
	WfxRender::drawFrame(hdc, rc, WBTN_BKGND_MOUSE, getDispatcher());
}

void ListCtrl::onDrawItem( const LCDrawItemInfo* pDrawItemInfo )
{
	Cell* pCell = getCell(pDrawItemInfo->m_nRow, pDrawItemInfo->m_nCol);
	if (pCell == NULL)
	{
		return;
	}
	String strCellText;
	strCellText.format(L"ROW:%d COL:%d", 
		pDrawItemInfo->m_nRow, pDrawItemInfo->m_nCol);
	pCell->Draw(pDrawItemInfo->m_hDC, 
		*(pDrawItemInfo->m_prcItem), 
		pDrawItemInfo->m_dwState, strCellText, 
		WBTN_BKGND_MOUSE, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
}

void ListCtrl::onDrawSeqBar( const LCDrawItemInfo* pDrawItemInfo )
{
	Cell* pCell = getCell(0, 0);
	if (pCell == NULL)
	{
		return;
	}
	String strCellText;
	strCellText.format(L"%d", pDrawItemInfo->m_nRow);
	pCell->Draw(pDrawItemInfo->m_hDC, 
		*(pDrawItemInfo->m_prcItem), pDrawItemInfo->m_dwState, 
		strCellText, WBTN_BKGND_MOUSE,
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
}

LRESULT ListCtrl::onMsgSeqHeadDraw( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	onDrawSeqHead((const LCDrawItemInfo*)wParam);
	return 1;
}

LRESULT ListCtrl::onMsgHeadDraw( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	onDrawHeadCtrl((const LCDrawItemInfo*)wParam);
	return 1;
}

LRESULT ListCtrl::onKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (getScrollBar(SB_VERT) != NULL)
	{
		LONG nHeight = 0;
		LONG nTotal = getEndRow() + 1;
		LONG nPos = 0;
		for (LONG n = 0; n < nTotal; n++)
		{
			nPos += getRowHeight(n);
		}
		getScrollBar(SB_VERT)->setPos(nPos);
		BOOL bh;
		getScrollBar(SB_VERT)->onSize(0, 0, 0, bh);
	}
	return 1;
}

void ListCtrl::onDrawSeqHead( const LCDrawItemInfo* pDrawItemInfo )
{
	WFX_CONDITION(pDrawItemInfo != NULL);
	Cell* pCell = getCell(0, 0);
	if (pCell == NULL)
	{
		return;
	}
	WFX_CONDITION(pDrawItemInfo->m_prcItem != NULL);
	pCell->Draw(pDrawItemInfo->m_hDC, *(pDrawItemInfo->m_prcItem), 
		pDrawItemInfo->m_dwState, L"SEQ", WBTN_BKGND_MOUSE, 0);
}

void ListCtrl::onDrawHeadCtrl( const LCDrawItemInfo* pDrawItemInfo )
{
	WFX_CONDITION(pDrawItemInfo != NULL);
	Cell* pCell = getCell(0, 0);
	if (pCell == NULL)
	{
		return;
	}
	WFX_CONDITION(pDrawItemInfo->m_prcItem != NULL);
	WFX_CONDITION(pDrawItemInfo->m_pszText != NULL);
	pCell->Draw(pDrawItemInfo->m_hDC, *(pDrawItemInfo->m_prcItem), pDrawItemInfo->m_dwState, 
		pDrawItemInfo->m_pszText, WBTN_BKGND_MOUSE, pDrawItemInfo->m_dwFormat);
}

void ListCtrl::onVisit( ULONG nPos, AttributeBase* pAttribute )
{

}

ULONG ListCtrl::getTotalRows() const
{
	/*return getTotalItems();*/
	if (m_pArrayAdapter == NULL)
	{
		return 0;
	}
	return m_pArrayAdapter->getCount();
}

ULONG ListCtrl::getTotalColumns() const
{
	return m_pHeadCtrl->getTotalColumns();
}

LRESULT ListCtrl::onHeadSizeChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

LRESULT ListCtrl::onHScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (calcPos(SB_HORZ))
	{
		calcCellRect();
		m_pHeadCtrl->calcCellRect();
	}
	invalidWid();
	return 1;
}

LRESULT ListCtrl::onVScroll( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (calcPos(SB_VERT))
	{
		calcCellRect();
	}
	invalidWid();
	return 1;
}

LRESULT ListCtrl::onMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 1;
}

LRESULT ListCtrl::onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!verify())
	{
		return 0;
	}
	m_ptMouseMove = lParam;
	setTimer(LIST_TIMER_MOUSEMOVE, 10);
	return 1;
	Point pt(lParam);
	std::pair<CellID, Rect> cellpt;
	std::pair<Cell*, Rect> cell;
	CellID cellMouseLeave;
	CellID cellMouseMove;
	Cell* pCell = NULL;
	if (getCellID(pt, cellpt))
	{
		cellMouseMove = cellpt.first;
		if (cellMouseMove != m_cellMouseMove)
		{
			cellMouseLeave = m_cellMouseMove;
			m_cellMouseMove = cellMouseMove;
			//TRACE(L"cellMouseLeave.m_nRow = %d, cellMouseLeave.m_nCol = %d", cellMouseLeave.m_nRow, cellMouseLeave.m_nCol);
			//TRACE(L"m_cellMouseMove.m_nRow = %d, m_cellMouseMove.m_nCol = %d", m_cellMouseMove.m_nRow, m_cellMouseMove.m_nCol);
		}
	}
	if (getCellIndirect(cellMouseLeave, cell))
	{
		cell.first->sendMessage(WM_MOUSELEAVE, (WPARAM)&cell.second);
	}
	if (getCellIndirect(m_cellMouseMove, cell))
	{
		cell.first->sendMessage(WM_MOUSEMOVE, (WPARAM)&cell.second);
	}
	return 1;
}

LRESULT ListCtrl::onMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!verify())
	{
		return 0;
	}
	std::pair<Cell*, Rect> cell;
	if (getCellIndirect(m_cellMouseMove, cell))
	{
		cell.first->sendMessage(WM_MOUSELEAVE, (WPARAM)&cell.second);
	}
	m_cellMouseMove.clear();
	return 1;
}

LRESULT ListCtrl::onCellDraw(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	invalidWid();
	return 1;
}

LRESULT ListCtrl::onCellexpand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bexpanded = isExpanded(m_cellSelected.m_nRow);
	expand(m_cellSelected.m_nRow, !bexpanded);
	invalidWid();
	return 1;
}

LRESULT ListCtrl::onTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (wParam == LIST_TIMER_MOUSEMOVE)
	{
		killTimer(LIST_TIMER_MOUSEMOVE);
		Point pt = m_ptMouseMove;
		std::pair<CellID, Rect> cellpt;
		std::pair<Cell*, Rect> cell;
		CellID cellMouseLeave;
		CellID cellMouseMove;
		Cell* pCell = NULL;
		if (getCellID(pt, cellpt))
		{
			cellMouseMove = cellpt.first;
			if (cellMouseMove != m_cellMouseMove)
			{
				cellMouseLeave = m_cellMouseMove;
				m_cellMouseMove = cellMouseMove;
				//TRACE(L"cellMouseLeave.m_nRow = %d, cellMouseLeave.m_nCol = %d", cellMouseLeave.m_nRow, cellMouseLeave.m_nCol);
				//TRACE(L"m_cellMouseMove.m_nRow = %d, m_cellMouseMove.m_nCol = %d", m_cellMouseMove.m_nRow, m_cellMouseMove.m_nCol);
			}
		}
		if (getCellIndirect(cellMouseLeave, cell))
		{
			cell.first->sendMessage(WM_MOUSELEAVE, (WPARAM)&cell.second);
		}
		if (getCellIndirect(m_cellMouseMove, cell))
		{
			cell.first->sendMessage(WM_MOUSEMOVE, (WPARAM)&cell.second);
		}
	}
	return 1;
}

LRESULT ListCtrl::ongetProperties( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(wParam)
	{
	case LCP_START_ROW:
		return getStartRow();
	case LCP_END_ROW:
		return getEndRow();
	case LCP_START_COL:
		return getStartCol();
	case LCP_END_COL:
		return getEndCol();
	case LCP_ROWNUM_WIDTH:
		return getRowNumBarWidth();
	default:
		return 0;
	}
}

ULONG ListCtrl::getRowHeight( ULONG nRow ) const
{
	return 20;
}

BOOL ListCtrl::setRowHeight( ULONG nRow, ULONG cy )
{
	return TRUE;
}

ULONG ListCtrl::getRowNumBarWidth() const
{
	return m_nRowNumBarWidth;
}

void ListCtrl::setRowNumBarWidth( ULONG nWidth )
{
	m_nRowNumBarWidth = nWidth;
}

ULONG ListCtrl::getHeadHeight() const
{
	return m_nHeadHeight;
}

void ListCtrl::setHeadHeight( ULONG nHeight )
{
	m_nHeadHeight = nHeight;
}

LONG ListCtrl::getFixedRow() const
{
	return m_nFixedRow;
}

void ListCtrl::setFixedRow( LONG nRow )
{
	m_nFixedRow = nRow;
}

LONG ListCtrl::getFixedCol() const
{
	return m_nFixedCol;
}

void ListCtrl::setFixedCol( LONG nCol )
{
	m_nFixedCol = nCol;
}

LONG ListCtrl::getStartRow() const
{
	return m_nStartRow;
}

void ListCtrl::setStartRow( LONG nRow )
{
	m_nStartRow = nRow;
}

LONG ListCtrl::getEndRow() const
{
	return m_nEndRow;
}

void ListCtrl::setEndRow( LONG nRow )
{
	m_nEndRow = nRow;
}

LONG ListCtrl::getStartCol() const
{
	return m_nStartCol;
}

void ListCtrl::setStartCol( LONG nCol )
{
	m_nStartCol = nCol;
}

LONG ListCtrl::getEndCol() const
{
	return m_nEndCol;
}

void ListCtrl::setEndCol( LONG nCol )
{
	m_nEndCol = nCol;
}

LONG ListCtrl::getCurrentRow() const
{
	return m_cellSelected.m_nRow;
}

LONG ListCtrl::getCurrentCol() const
{
	return m_cellSelected.m_nCol;
}

Rect ListCtrl::getCellRect( ULONG nRow, ULONG nCol )
{
	Rect rcCell;
	LONG nOffsetRow = nRow - m_nStartRow;
	LONG nOffsetCol = nCol - m_nStartCol;
	if (nOffsetRow >= m_rgRectFast.size())
		return rcCell;
	if (nOffsetCol >= m_rgRectFast[nOffsetRow].size())
		return rcCell;
	rcCell = m_rgRectFast[nOffsetRow][nOffsetCol];
	return rcCell; 
}

LRESULT ListCtrl::onLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (!verify())
	{
		return 0;
	}
	Point pt(lParam);
	std::pair<CellID, Rect> cellID;
	if (!getCellID(pt, cellID))
	{
		return 0;
	}
	m_cellSelected = cellID.first;
	if (m_cellSelected.m_nCol == 0)
	{
	
	}
	//TRACE(L"m_cellSelected.m_nRow=%d, m_cellSelected.m_nCol=%d", m_cellSelected.m_nRow, m_cellSelected.m_nCol);
	Cell* pCell = getCell(cellID.first.m_nRow, cellID.first.m_nCol);
	if (pCell != NULL)
	{
		pCell->setRect(cellID.second);
		return pCell->sendMessage(WM_LBUTTONUP, wParam, lParam);
	}
	return 0;
}

void ListCtrl::calcVirtualSize( Size& sz )
{
	setVirtualSizeCached();
	sz.empty();
	int nTotalRow = getTotalRows();
	for (int i = 0; i < nTotalRow; i++)
	{
		sz.cy += getRowHeight(i);
	}
	int nTotalCol = getTotalColumns();
	for (int i = 0; i < nTotalCol; i++)
	{
		sz.cx += getColumnWidth(i);
	}
}

void ListCtrl::getToolTip(String& str) const
{
	std::pair<CellID, Rect> cellpt;
	getCellID(m_ptMouseMove, cellpt);
	if (!isValidCellID(cellpt.first))
	{
		str = L"";
	}
	str.format(L"ROW:%d COL:%d", 
		cellpt.first.m_nRow, cellpt.first.m_nCol);
}

Rect ListCtrl::getClientRect() const
{
	Rect rcWidget = getRect();
	if (getSBFlag() & WESB_HORZ)
	{
		rcWidget.right -= SIZE_SCROLLBAR;
	}
	if (getSBFlag() & WESB_VERT)
	{
		rcWidget.bottom -= SIZE_SCROLLBAR;
	}
	return rcWidget;
}

BOOL ListCtrl::calcCol()
{
	ULONG nOldStartCol = m_nStartCol;
	ULONG nOldEndCol = m_nEndCol;
	m_nStartCol = m_nEndCol = -1;
	Rect rcDraw = getClientRect();
	LONG nOffset = getHOffset();
	//TRACE(L"nOffset = %d", nOffset);
	ULONG nTotalCol = getTotalColumns();
	LONG nSumWidth = 0;
	LONG nColWidth = 0;
	for (ULONG nCol = 0; nCol < nTotalCol; nCol++)
	{
		nColWidth = getColumnWidth(nCol);
		nSumWidth += nColWidth;
		if (nSumWidth >= nOffset)
		{
			m_nStartCol = nCol;
			break;
		}
	}
	nSumWidth = 0;
	LONG nViewWidth = rcDraw.right - rcDraw.left;
	for (ULONG nCol = m_nStartCol; nCol < nTotalCol; nCol++)
	{
		nSumWidth += getColumnWidth(nCol);
		if (nSumWidth > nViewWidth)
		{
			m_nEndCol = nCol;
			break;
		}
	}
	if (nSumWidth < nViewWidth)
	{
		m_nEndCol = nTotalCol - 1;
	}
	//TRACE(L"m_nStartCol = %d", m_nStartCol);
	//TRACE(L"m_nEndCol = %d", m_nEndCol);
	return (m_nStartCol != nOldStartCol || m_nEndCol != nOldEndCol);
}

BOOL ListCtrl::calcRow()
{
	ULONG nOldStartRow = m_nStartRow;
	ULONG nOldEndRow = m_nEndRow;
	m_nStartRow = m_nEndRow = -1;
	Rect rcDraw = getClientRect();
	LONG nOffset = getVOffset();
	//TRACE(L"nOffset = %d", nOffset);
	ULONG nTotalRow = getTotalRows();
	LONG nSumHeight = 0;
	for (ULONG nRow = 0; nRow < nTotalRow; nRow++)
	{
		nSumHeight += getRowHeight(nRow);
		if (nSumHeight >= nOffset)
		{
			m_nStartRow = nRow;
			break;
		}
	}
	nSumHeight = 0;
	LONG nViewHeight = rcDraw.bottom - rcDraw.top;
	for (ULONG nRow = m_nStartRow; nRow < nTotalRow; nRow++)
	{
		nSumHeight += getRowHeight(nRow);
		if (nSumHeight > nViewHeight)
		{
			m_nEndRow = nRow;
			break;
		}
	}

	if (nSumHeight <= nViewHeight)
	{
		m_nEndRow = nTotalRow - 1;
	}
	//TRACE(L"m_nStartRow = %d", m_nStartRow);
	//TRACE(L"m_nEndRow = %d", m_nEndRow);
	return (m_nStartRow != nOldStartRow || m_nEndRow != nOldEndRow);
}

void ListCtrl::calcCellRect()
{
	WFX_CONDITION(m_nStartRow != -1);
	WFX_CONDITION(m_nEndRow != -1);
	WFX_CONDITION(m_nStartCol != -1);
	WFX_CONDITION(m_nEndCol != -1);
	WFX_CONDITION(m_nEndRow >= m_nStartRow);
	WFX_CONDITION(m_nEndCol >= m_nEndCol);
	m_rgRectFast.clear();
	m_rgRect.clear();
	m_rgRowNumRect.clear();
	ULONG nTotalRow = getTotalRows();
	ULONG nTotalCol = getTotalColumns();
	ULONG nSeqBarWidth = getRowNumBarWidth();
	ULONG nHeadHeight = getHeadHeight();
	Rect rcWid = getClientRect();
	Rect rcCell = rcWid;
	Rect rcRowNum = rcWid;
	ULONG nWidth = 0;
	ULONG nHeight = 0;
	rcRowNum.right = rcRowNum.left + nSeqBarWidth;
	rcCell.left += nSeqBarWidth + 1;
	rcCell.top += nHeadHeight + 1;
	m_rgRectFast.resize(m_nEndRow - m_nStartRow + 1);
	for (LONG nRow = m_nStartRow; nRow <= m_nEndRow; nRow++)
	{
		nHeight = getRowHeight(nRow);
		rcCell.bottom = rcCell.top + nHeight;
		rcRowNum.top = rcCell.top;
		rcRowNum.bottom = rcCell.bottom;
		m_rgRowNumRect.push_back(rcRowNum);
		if (rcCell.bottom > rcWid.bottom)
			rcCell.bottom = rcWid.bottom;
		m_rgRectFast[nRow - m_nStartRow].resize(m_nEndCol - m_nStartCol + 1);
		for (LONG nCol = m_nStartCol; nCol <= m_nEndCol; nCol++)
		{
			nWidth = getColumnWidth(nCol);
			rcCell.right = rcCell.left + nWidth;
			m_rgRect.insert(std::make_pair(CellID(nRow, nCol), rcCell));
			m_rgRectFast[nRow - m_nStartRow][nCol - m_nStartCol] = rcCell;
			rcCell.left += rcCell.right - rcCell.left;
		}
		rcCell.left = rcWid.left + nSeqBarWidth + 1;
		rcCell.top += rcCell.bottom - rcCell.top;
	}
}

BOOL ListCtrl::calcPos(int nBar)
{
	if (nBar == SB_HORZ)
	{
		return calcCol();
	}
	else
	{
		return calcRow();
	}
}

BOOL ListCtrl::verify() const
{
	ULONG nTotalRow = getTotalRows();
	ULONG nTotalCol = getTotalColumns();
	return nTotalRow > 0 && nTotalCol > 0;
}

LCDrawItemInfo* ListCtrl::getDrawItemInfo() const
{
	return m_pHeadCtrl->getDrawItemInfo();
}
