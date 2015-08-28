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
#include "wfxrender.h"
#include "wfxContainer.h"

USING_NAMESPACE_WFX;

void Container::addItem( Widget* pItem )
{
	m_rgpItem.push_back(SPWidget(pItem));
}

void Container::removeItem( Widget* pItem )
{
	for (std::vector<SPWidget>::iterator it = m_rgpItem.begin();
		it != m_rgpItem.end(); ++it)
	{
		if ((*it).get() == pItem)
		{
			(*it).reset();
			m_rgpItem.erase(it);
			break;
		}
	}
}

void Container::removeAll()
{
	m_rgpItem.clear();
}

Container::~Container()
{

}
