#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include "util.h"
#include "StringList.h"
#include "SequenceNode.h"
#include "SequenceHandler.h"
#include "ParameterHandler.h"

namespace ainb {

class AINB {
public:
	AINB(std::fstream& stream);
	~AINB();

	std::string getName() { return m_name; }

	int getEntryCommandCount();
	int getExecutionCommandCount();

	void writeToStream(std::fstream& stream); // todo

	// get an array of the number of parameters for each type
	int* getInternalParameterCounts();
	int* getCommandParameterCounts();

	InternalParameterBase* getInternalParameter(int section_num, int index);
	CommandParameterBase* getCommandParameter(int section_num, int index);

	std::vector<SequenceHandler::EntryCommand*> getEntryCommands() { return m_sequence_handler->getEntryCommands(); }

	std::vector<SequenceNode*> getSequenceNodes() { return m_sequence_handler->getSequenceNodes(); }

	void finalize();

private:
	std::fstream& m_stream;

	// this is the internal name of the file
	// it is the first string in the string list
	std::string m_name;

	struct HeaderData {
		char* type = new char[5];
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

