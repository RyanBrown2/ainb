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


}

void AINB::writeToFile(fstream& file)
{

}

