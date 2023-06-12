#include "nin-io/ainb/ExecutionCommand.h"

using namespace std;

ostream& operator<<(ostream& os, ExecutionCommand command) {
	os << command.m_name << endl;
	os << "Address: " << hex << command.m_address << endl;
	os << "Index: " << hex << command.m_index << endl;
	os << "Data Pointer: " << hex << command.m_data_pointer << endl;
	os << "Unknown 1: " << hex << command.m_unknown1 << endl;
	os << "Unknown 2: " << hex << command.m_unknown2 << endl;
	os << "Command ID: " << hex << command.m_command_id << endl;
	os << "GUID: ";
	displayCharArrayAsHex(os, command.m_guid, 16);

	return os;
}

ExecutionCommand::ExecutionCommand()
{
	m_index = -1;
	m_data_pointer = -1;
	m_unknown1 = -1;
	m_unknown2 = -1;

	m_command_id = -1;
}

ExecutionCommand::~ExecutionCommand()
{

}

void ExecutionCommand::load(fstream& file, StringList* string_list)
{

	m_address = file.tellg();

	// unknown 1
	readIntFromStream(file, is_big_endian, 2, m_unknown1);

	// get index
	readIntFromStream(file, is_big_endian, 4, m_index);

	// unknown 2
	//file.seekg(m_address, ios::beg);
	//file.seekg(0x6, ios::cur);
	readIntFromStream(file, is_big_endian, 2, m_unknown2);

	// string pointer
	int name_tag;
	readIntFromStream(file, is_big_endian, 4, name_tag);
	m_name = string_list->getString(name_tag);

	// 4 byte data chunk
	file.seekg(m_address, ios::beg);
	file.seekg(0xc, ios::cur);
	readIntFromStream(file, is_big_endian, 4, m_command_id);
	//file.read(m_dataChunk, 4);
	//m_dataChunk[4] = '\0';

	// get the body data pointer
	file.seekg(m_address, ios::beg);
	file.seekg(0x14, ios::cur);
	readIntFromStream(file, is_big_endian, 4, m_data_pointer);

	// get data chunk
	file.seekg(m_address, ios::beg);
	file.seekg(0x2c, ios::cur);
	file.read(m_guid, 16);
	m_guid[16] = '\0';
}


