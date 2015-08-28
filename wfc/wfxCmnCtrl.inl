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

template <class _Ty>
ArrayAdapter<_Ty>::ArrayAdapter( Widget* pWidget, const std::vector<_Ty>& rgData )
{

}

template <class _Ty>
ULONG ArrayAdapter<_Ty>::getCount() const
{
	return m_rgData.size();
}

template <class _Ty>
Cell* ArrayAdapter<_Ty>::getCell( ULONG nPos )
{
	if (nPos >= m_rgpCell.size())
	{
		m_rgpCell.push_back(SPCell(new Cell));
	}
	return m_rgpCell[nPos];
}
