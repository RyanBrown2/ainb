#pragma once
#include <comdef.h>

struct InternalParameterData
{
	BSTR name;
	BSTR value;
	int type_num; // number corresponding to the type (int = 0, bool = 1, float = 2, etc)
};

struct CommandParameterData
{
	BSTR name;
	int type_num; // number corresponding to the type (int = 0, bool = 1, float = 2, etc)
	BSTR udt_name; // only used by udt params
};
