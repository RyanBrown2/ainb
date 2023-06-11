#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nin-io/util/util.h"
#include "StringList.h"
#include "ParameterHandler.h"

class AINB
{
public:
	AINB();
	~AINB();
	void load(std::fstream& file);

	struct FileHeaderData {
		char type[4]; // 0x00
		int address_section_pointer = -1; // 0x03 
		int entry_point_command_count = -1; // 0x0c
		int execution_command_count = -1; // 0x10
		int unknown_value = -1; // 0x14
		int command_body_start_address = -1; // 0x20
		int string_list_start_address = -1; // 0x24
		int four_bytes_before_string_list = -1; // 0x28
		int parameter_table_start_address = -1; // 0x2c
		int parameter_subsection_three_start_address = -1; // 0x30
		int parameter_structure_start_address = -1; // 0x34
	};

	FileHeaderData getFileHeaderData();
	
private:


	FileHeaderData m_file_header_data;
	void loadFileHeaderData(std::fstream& file);
	
	// String List
	StringList* m_string_list = nullptr;

	ParameterHandler* m_parameter_handler = nullptr;


	static const bool is_big_endian = false;
};