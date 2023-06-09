#include "nin-io/ainb/B_Command.h"

using namespace std;

ostream& operator<<(ostream& os, const B_Command command) {
	os << command.m_name << endl;
	os << "Address: " << command.m_address << endl;
	os << "Index: " << hex << command.m_index << endl;
	os << "Data Pointer: " << hex << command.m_dataPointer << endl;
	os << "Unknown 1: " << command.m_unknown1 << endl;
	os << "Unknown 2: " << command.m_unknown2 << endl;
	os << "Command ID: " << hex << command.m_command_id << endl;
	os << "GUID: ";
	displayCharArrayAsHex(os, command.m_guid, 16);
	return os;
}

B_Command::B_Command() : BaseCommand(BlockType::B)
{
	m_index = -1;
	m_dataPointer = -1;
	m_unknown1 = -1;
	m_unknown2 = -1;

	//m_dataChunk = new char[5];
	m_command_id = -1;

	m_array_length = -1;

	
}

B_Command::~B_Command()
{

}

void B_Command::load(fstream& file)
{
	BaseCommand::load(file);

	// unknown 1
	readIntFromStream(file, is_big_endian, 2, m_unknown1);

	// get index
	readIntFromStream(file, is_big_endian, 4, m_index);

	// unknown 2
	//file.seekg(m_address, ios::beg);
	//file.seekg(0x6, ios::cur);
	readIntFromStream(file, is_big_endian, 2, m_unknown2);

	// string pointer
	readIntFromStream(file, is_big_endian, 4, m_string_pointer);

	// 4 byte data chunk
	file.seekg(m_address, ios::beg);
	file.seekg(0xc, ios::cur);
	readIntFromStream(file, is_big_endian, 4, m_command_id);
	//file.read(m_dataChunk, 4);
	//m_dataChunk[4] = '\0';

	// get the body data pointer
	file.seekg(m_address, ios::beg);
	file.seekg(0x14, ios::cur);
	readIntFromStream(file, is_big_endian, 4, m_dataPointer);

	// get data chunk
	file.seekg(m_address, ios::beg);
	file.seekg(0x2c, ios::cur);
	file.read(m_guid, 16);
	m_guid[16] = '\0';
}

void B_Command::loadBody(fstream& file, StringList string_list)
{
	int current_pos = file.tellg();
	file.seekg(m_dataPointer, ios::beg);

	//map<int, int> position_to_key;
	//map<int, int> value_map;

	int value_count = 0;
	for (int i = 0; i < 18; i++) {
		int key;
		readIntFromStream(file, is_big_endian, 4, key);
		int value;
		readIntFromStream(file, is_big_endian, 4, value);
		
		if (key != 0) {
			m_body_data.position_to_key[i] = key;
			m_body_data.value_map[key] = value;
			value_count += 1;
		}
	}

	// todo - figure out proper way to determine length
	file.seekg(m_dataPointer, ios::beg);
	file.seekg(0xa3, ios::cur);

	// check for array
	readIntFromStream(file, is_big_endian, 1, m_array_length);

	if (m_array_length == 0) {
		file.seekg(current_pos, ios::beg);
		return;
	}

	if (m_array_length > 0) {
		loadBodyTable(file, m_array_length, string_list);
	}

	file.seekg(current_pos, ios::beg);

}

void B_Command::loadBodyTable(fstream& file, int length, StringList string_list)
{
	//Table table;

	m_body_data.table.address = file.tellg();
	m_body_data.table.length = length;

	int* addresses = new int[length];
	// load table addresses
	for (int i = 0; i < m_array_length; i++) {
		readIntFromStream(file, is_big_endian, 4, addresses[i]);
	}

	for (int i = 0; i < length; i++) {
		file.seekg(addresses[i], ios::beg);

		// read data
		TableValuePair entry;
		readIntFromStream(file, is_big_endian, 4, entry.value1);
		//readIntFromStream(file, is_big_endian, 4, entry.value2);
		int string_offset;
		readIntFromStream(file, is_big_endian, 4, string_offset);
		entry.value2 = string_list.getString(string_offset);
		
		//entry.value2 = string_offset;
	
		m_body_data.table.entries.push_back(entry);
	}


}
