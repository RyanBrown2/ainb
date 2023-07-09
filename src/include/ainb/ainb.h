#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ocidl.h>
#include "util.h"
#include "StringList.h"
#include "SequenceNode.h"
#include "SequenceHandler.h"
#include "ParameterHandler.h"
#include "comutil.h"
#include "atlbase.h"
#include <unknwn.h>

namespace ainb {

class AINB {
public:
	AINB(LPSTREAM stream);
	~AINB();

	struct InternalParameter {
		BSTR name;
		BSTR value;
	};

	struct CommandParameter {
		BSTR name;
	};

	int getEntryCommandCount();
	int getExecutionCommandCount();


	void writeToStream(std::fstream& stream); // todo

	// get an array of the number of internal parameters for each type
	int* getInternalParameterCounts();
	int* getCommandParameterCounts();

	//template <ParameterHandler::ParameterType T>
	//ParameterHandler::InternalParameter<T>* getInternalParameterBase(int index);
	ParameterHandler::InternalParameterBase* getInternalParameterBase(int section_num, int index);
	ParameterHandler::CommandParameterBase* getCommandParameterBase(int section_num, int index);

	std::vector<SequenceNode*> getSequenceNodes() { return m_sequence_handler->getSequenceNodes(); }

	static InternalParameter exportInternalParameter(ParameterHandler::InternalParameterBase* parameter);
	static CommandParameter exportCommandParameter(ParameterHandler::CommandParameterBase* parameter);

	static AINB* loadFromStream(std::fstream& file);

private:
	LPSTREAM m_stream;

	struct HeaderData {
		char* type = new char[4];
		int entry_command_count = -1;
		int execution_command_count = -1;

		int string_list_start_pos = -1;
		int internal_parameters_start = -1;
		int command_parameters_start = -1;
		int command_parameters_end = -1;
	};

	HeaderData m_header_data;

	StringList* m_string_list;
	ParameterHandler* m_parameter_handler;
	SequenceHandler* m_sequence_handler;

	void parseHeader();
	void parseParameters();
};

}

extern "C" {
	__declspec(dllexport) ainb::AINB* Create(LPSTREAM stream);
	__declspec(dllexport) void Destroy(ainb::AINB* ainb);
	__declspec(dllexport) int GetEntryCommandCount(ainb::AINB* ainb);
	__declspec(dllexport) int GetExecutionCommandCount(ainb::AINB* ainb);
	//__declspec(dllexport) void Write(ainb::AINB* ainb, LPSTREAM stream);
	__declspec(dllexport) int* GetInternalParameterCounts(ainb::AINB* ainb);
	__declspec(dllexport) int* GetCommandParameterCounts(ainb::AINB* ainb);
	__declspec(dllexport) ainb::AINB::InternalParameter GetInternalParameter(ainb::AINB* ainb, int section_num, int index);
	__declspec(dllexport) ainb::AINB::CommandParameter GetCommandParameter(ainb::AINB* ainb, int section_num, int index);
}

