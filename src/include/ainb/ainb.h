#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <ocidl.h>
#include "util.h"
#include "StringList.h"
#include "SequenceNode.h"
#include "SequenceHandler.h"

namespace ainb {

class AINB {
public:
	AINB(LPSTREAM stream);
	~AINB();

	int getEntryCommandCount();
	int getExecutionCommandCount();

	SequenceNode* getSequenceNode(int index);

	void writeToStream(LPSTREAM stream);

private:
	LPSTREAM m_stream;

	struct HeaderData {
		char* type = new char[4];
		int entry_command_count = -1;
		int execution_command_count = -1;

		int string_list_start_pos = -1;
	};

	HeaderData m_header_data;

	StringList* m_string_list;
	SequenceHandler* m_sequence_handler;

	void parseHeader();
	void parseCommandHeads();
};

}

extern "C" {
	__declspec(dllexport) ainb::AINB* Create(LPSTREAM stream);
	__declspec(dllexport) void Destroy(ainb::AINB* ainb);
	__declspec(dllexport) int GetEntryCommandCount(ainb::AINB* ainb);
	__declspec(dllexport) int GetExecutionCommandCount(ainb::AINB* ainb);
	__declspec(dllexport) void Write(ainb::AINB* ainb, LPSTREAM stream);
	__declspec(dllexport) ainb::SequenceNode::NodeData GetSequenceNodeData(ainb::AINB* ainb, int index);
}

