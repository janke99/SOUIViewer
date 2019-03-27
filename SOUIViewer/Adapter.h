#pragma once
#include <helper/SAdapterBase.h>

class CMcAdapter : public SMcAdapterBase
{
public:

public:
	/* LvAdatperImpl */
	virtual int getCount();
	virtual void getView(int position, SWindow * pItem, pugi::xml_node xmlTemplate);
	/* IMcAdapter */
	//获取一个列在模板中对应的窗口名称
	//int iCol: 列序号
	virtual SStringW GetColumnName(int iCol) const;
public:
};

