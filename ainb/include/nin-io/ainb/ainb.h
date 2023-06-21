#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nin-io/util/util.h"

class AINB
{
public:
	AINB();
	~AINB();

	struct FileHeaderData {
		char* type = new char[4];
		int entry_point_command_count = -1;
		int execution_command_count = -1;
	};

	void loadFromFile(std::fstream& file);
	void writeToFile(std::fstream& file);

	char* getType() { return m_file_header_data.type; }
	int getEntryPointCommandCount() { return m_file_header_data.entry_point_command_count; }
	int getExecutionCommandCount() { return m_file_header_data.execution_command_count; }


private:
	FileHeaderData m_file_header_data;

	static const bool is_big_endian = false;
};