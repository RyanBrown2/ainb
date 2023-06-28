#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <ocidl.h>
#include "util.h"
#include "StringList.h"
#include "SequenceNode.h"
#include "SequenceHandler.h"
#include "ParameterHandler.h"

namespace ainb {

class AINB {
public:
	AINB(LPSTREAM stream);
	~AINB();

	int getEntryCommandCount();
	int getExecutionCommandCount();

	SequenceNode* getSequenceNode(int index);

	void writeToStream(LPSTREAM stream); // todo

	struct ParameterStruct {
		int address;
		BSTR name;
		int value;
	};

	// todo: decide on data return types
	ParameterStruct getTableParameter(int section_num, int index);
	ParameterStruct getStructureParameter(int section_num, int index);

	int getTableParameterCount(int section_num);
	int getStructureParameterCount(int section_num);


private:
	LPSTREAM m_stream;

	struct HeaderData {
		char* type = new char[4];
		int entry_command_count = -1;
		int execution_command_count = -1;

		int string_list_start_pos = -1;
		int parameter_table_start = -1;
		int parameter_structure_start = -1;
		int parameter_structure_end = -1;
	};

	HeaderData m_header_data;

	StringList* m_string_list;
	ParameterHandler* m_parameter_handler;
	SequenceHandler* m_sequence_handler;

	void parseHeader();
	void parseParameters();
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
	__declspec(dllexport) ainb::AINB::ParameterStruct GetTableParameter(ainb::AINB*, int section_num, int param_num);
	__declspec(dllexport) ainb::AINB::ParameterStruct GetStructureParameter(ainb::AINB*, int section_num, int param_num);
}

