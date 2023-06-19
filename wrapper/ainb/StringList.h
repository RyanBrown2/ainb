#pragma once
#include <comdef.h>
#include <atlbase.h>
#include "nin-io/ainb/StringList.h"

class AINB_StringList
{
public:
	static AINB_StringList* CreateStringList();

	AINB_StringList(StringList* string_list);
	~AINB_StringList();

	void addString(char* str);
	void clear();
	char* getString(int index);
	void removeString(char* str);
private:
	StringList* m_stringList = nullptr;
};


