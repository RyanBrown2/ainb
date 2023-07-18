#pragma once
#include <ainb/ainb.h>

#if _WIN32
#define CAINB __declspec(dllexport)
#else
#define CAINB __attribute__((cdecl))
#endif

extern "C" {

CAINB void Create();
}
