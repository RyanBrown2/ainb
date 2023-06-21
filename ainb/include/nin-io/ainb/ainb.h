#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nin-io/util/util.h"
#include "StringList.h"

class AINB
{
public:
	AINB();
	~AINB();

	struct FileHeaderData {
		char* type = new char[4];
		int entry_point_command_count = -1;
		int execution_command_count = -1;
		int command_end_pos = -1;
		int string_list_pos = -1;
	};

	void loadFromFile(std::fstream& file);
	void writeToFile(std::fstream& file);

	char* getType() { return m_file_header_data.type; }
	int getEntryPointCommandCount() { return m_file_header_data.entry_point_command_count; }
	int getExecutionCommandCount() { return m_file_header_data.execution_command_count; }


private:
	FileHeaderData m_file_header_data;

	StringList* m_string_list = nullptr;

	static const bool is_big_endian = false;
};