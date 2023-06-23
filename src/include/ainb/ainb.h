#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <ocidl.h>

namespace ainb {

class AINB {
public:
	AINB(LPSTREAM stream);
	~AINB();
private:

};




}

extern "C" {
	__declspec(dllexport) ainb::AINB* CreateAINB(LPSTREAM stream);
	__declspec(dllexport) void DestroyAINB(ainb::AINB* ainb);
}

