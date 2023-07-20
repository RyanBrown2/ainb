#pragma once
#include "data_types.h"
#include <ainb/ainb.h>

static InternalParameterData getInternalParameterData(ainb::InternalParameterBase* param)
{
	InternalParameterData data{};
	_bstr_t name = param->name.c_str();
	_bstr_t value = param->value.c_str();
	data.name = SysAllocString(name);
	data.value = SysAllocString(value);
	return data;
}
