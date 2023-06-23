#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <ocidl.h>
#include "util.h"

namespace ainb {

class AINB {
public:
	AINB(LPSTREAM stream);
	~AINB();

	int getEntryCommandCount();
	int getExecutionCommandCount();

private:
	LPSTREAM m_stream;

	struct HeaderData {
		char* type = new char[4];
		int entry_command_count = -1;
		int execution_command_count = -1;
	};

	HeaderData m_header_data;

	void parseHeader();

};

}

extern "C" {
	__declspec(dllexport) ainb::AINB* Create(LPSTREAM stream);
	__declspec(dllexport) void Destroy(ainb::AINB* ainb);
	__declspec(dllexport) int GetEntryCommandCount(ainb::AINB* ainb);
	__declspec(dllexport) int GetExecutionCommandCount(ainb::AINB* ainb);
}

