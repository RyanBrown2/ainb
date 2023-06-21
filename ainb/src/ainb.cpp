#include "nin-io/ainb/ainb.h"

using namespace std;

AINB::AINB()
{

}

AINB::~AINB()
{

}

void AINB::loadFromFile(fstream& file)
{
	file.seekg(0, ios::beg);
	file.clear();

	// HEADER DATA
	file.read(m_file_header_data.type, 3);
	m_file_header_data.type[3] = '\0';

	file.seekg(0xc, ios::beg); 
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.entry_point_command_count);
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.execution_command_count);

	file.seekg(0x20, ios::beg);
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.command_end_pos);
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.string_list_pos);

	// STRING LIST
	m_string_list = new StringList(&m_file_header_data.string_list_pos);

	m_string_list->loadFromFile(file);

	m_string_list->getOffsetFromString("test");

}

void AINB::writeToFile(fstream& file)
{
	file.seekg(0, ios::beg);
	file.clear();

	file.write("AIB", 3);

	file.seekg(0xc, ios::beg);
	file.write((char*)&m_file_header_data.entry_point_command_count, 4);
	file.write((char*)&m_file_header_data.execution_command_count, 4);

	file.seekg(0x20, ios::beg);
	file.write((char*)&m_file_header_data.command_end_pos, 4);
	file.write((char*)&m_file_header_data.string_list_pos, 4);

	m_string_list->writeToFile(file);

}

