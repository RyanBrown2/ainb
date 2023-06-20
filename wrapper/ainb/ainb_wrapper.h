#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <comdef.h>
#include <atlbase.h>
#include <unordered_map>
#include "nin-io/ainb/ainb.h"


class AINB_FILE
{
public:
	AINB_FILE();
	~AINB_FILE();

	void loadFile(std::string file_dir);

	BSTR getName();

	int getEntryPointCount();
	int getExecutionCommandCount();

	struct ExportedCallTableEntry {
		int index; // index of the command to call
		//BSTR parameter; // parameter to pass to the command
	};

	struct ExportedSequenceNode {
		//ExportedSequenceNode* caller = nullptr;
		BSTR name;
		int index;
		int command_id;
		BSTR guid;
		//ExportedCallTableEntry* call_table_entries;
		int* test;
	};

	ExportedSequenceNode getTestNode() {
		return toExportedSequenceNode(m_test_node);
	}

private:
	AINB* m_ainb;
	BSTR m_name;

	AINB::FileHeaderData m_file_header_data;

	SequenceHandler::SequenceNode* m_test_node;

	std::unordered_map<ExportedSequenceNode*, SequenceHandler::SequenceNode*> m_exported_sequence_node_map;

	ExportedSequenceNode toExportedSequenceNode(SequenceHandler::SequenceNode* sequence_node);
	
};


extern "C" __declspec(dllexport) void* CreateAINB()
{
	return new AINB_FILE();
}

extern "C" __declspec(dllexport) void* LoadFileAINB(AINB_FILE* ainb_file, char* file_dir)
{
	ainb_file->loadFile(file_dir);
	return ainb_file;
}

extern "C" __declspec(dllexport) BSTR GetName(AINB_FILE* ainb_file)
{
	return ainb_file->getName();
}

extern "C" __declspec(dllexport) int GetEntryPointCount(AINB_FILE * ainb_file)
{
	return ainb_file->getEntryPointCount();
}

extern "C" __declspec(dllexport) int GetExecutionCommandCount(AINB_FILE * ainb_file)
{
	return ainb_file->getExecutionCommandCount();
}


extern "C" __declspec(dllexport) AINB_FILE::ExportedSequenceNode GetTestNode(AINB_FILE * ainb_file)
{
	return ainb_file->getTestNode();
}


//extern "C" __declspec(dllexport) int Adding(AINB_FILE * ainb_file, int y) {
//	return ainb_file->adding(y);
//}
