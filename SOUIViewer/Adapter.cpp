#include "stdafx.h"
#include "Adapter.h"

int CMcAdapter::getCount()
{
	return 100;
}

void CMcAdapter::getView(int position, SWindow * pItem, pugi::xml_node xmlTemplate)
{
	if (pItem->GetChildrenCount() == 0)
	{
		pItem->InitFromXml(xmlTemplate);
	}
}

SOUI::SStringW CMcAdapter::GetColumnName(int iCol) const
{
	return SStringW().Format(L"col%d", iCol + 1);
}
