#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <ainb/ainb.h>
#include <comdef.h>
#include "data_types.h"
#include "c_util.h"

#if _WIN32
#define CAINB __declspec(dllexport)
#else
#define CAINB __attribute__((cdecl))
#endif

extern "C" {

CAINB ainb::AINB* Create(char* dir);

CAINB int GetEntryCommandCount(ainb::AINB* ainb);
CAINB int GetExecutionCommandCount(ainb::AINB* ainb);

CAINB int* GetInternalParameterCounts(ainb::AINB* ainb);
CAINB int* GetCommandParameterCounts(ainb::AINB* ainb);

CAINB InternalParameterData GetInternalParameter(ainb::AINB* ainb, int type_index, int param_index);
CAINB CommandParameterData GetCommandParameter(ainb::AINB* ainb, int type_index, int param_index);

}

