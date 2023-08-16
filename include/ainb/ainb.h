#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include "util.h"
#include "StringList.h"
#include "SequenceNode.h"
#include "SequenceHandler.h"
#include "ParameterHandler.h"
#include "AddressManager.h"
#include "ExternalHandler.h"
#include "parameters/ParameterBase.h"
#include "parameters/InternalParameter.h"

namespace ainb {

	static const std::map<int, std::string> FileTypeNumToName = {
		{0, "AI"},
		{1, "Logic"},
		{2, "Sequence"}
	};

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
		char* head = new char[5]; // 0x00
		int version = -1; // 0x04 - normally 0x407
		int entry_command_count = -1; // 0x0c
		int execution_command_count = -1; // 0x10

		// todo: figure out what these are
		// assuming they are counts of some sort
		int unknown_1 = -1; // 0x14
		int unknown_2 = -1; // 0x18
		int unknown_3 = -1; // 0x1c

		int command_heads_end = -1; // 0x20
		int string_list_start_pos = -1; // 0x24
		int string_list_4byte_padding = -1; // 0x28 in header - will probably be renamed
		int internal_parameters_start = -1; // 0x2c
		int command_parameters_start = -1; // 0x34
		int command_parameters_end = -1; // 0x38

		int parameter_section_start = -1; // 0x40

		int func_start_address = -1; // 0x44 in header - not sure if should be renamed

		int func_end_address = -1; // 0x5c in header - not sure if should be renamed

		// string offset for type string in string list
		int type_name_offset = -1; // 0x60

		// AI = 0, Logic = 1, Sequence = 2
		int file_type = -1; // 0x64

		int end_data_4byte_padding = -1; // 0x68 in header - will be renamed
		int string_list_8byte_padding = -1; // 0x6c in header - will be renamed

		int end_data_address = -1; // 0x70 in header - will be renamed

		void writeToStream(std::fstream& stream);

	};

	HeaderData m_header_data;

	StringList* m_string_list;
	ParameterHandler* m_parameter_handler;
	SequenceHandler* m_sequence_handler;
	ExternalHandler* m_external_handler;
	AddressManager* m_address_manager;

	void parseHeader();
	void parseParameters();

};

}

