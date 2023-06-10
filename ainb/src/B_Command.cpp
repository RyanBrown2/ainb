#include "nin-io/ainb/B_Command.h"

using namespace std;

ostream& operator<<(ostream& os, ExecutionCommand command) {
	os << command.m_name << endl;
	os << "Address: " << hex << command.m_address << endl;
	os << "Index: " << hex << command.m_index << endl;
	os << "Data Pointer: " << hex << command.m_dataPointer << endl;
	os << "Unknown 1: " << hex << command.m_unknown1 << endl;
	os << "Unknown 2: " << hex << command.m_unknown2 << endl;
	os << "Command ID: " << hex << command.m_command_id << endl;
	os << "GUID: ";
	displayCharArrayAsHex(os, command.m_guid, 16);

	ExecutionCommand::BodyData body_data = command.getBodyData();
	for (const auto& pair : body_data.position_to_key) {
		int pos = pair.first;
		int key = pair.second;
		int value = body_data.value_map[key];

		cout << "Pos: " << hex << pos << " Key: " << key << " Value: " << value << endl;
	}

	cout << "Table:" << endl;
	for (int i = 0; i < body_data.table.entries.size(); i++) {
		ExecutionCommand::TableValuePair pair = body_data.table.entries[i];
		int val = pair.value1;
		string name = pair.value2;

		cout << "Name: " << name << " | Value: " << hex << val << endl;
	}
	return os;
}

ExecutionCommand::ExecutionCommand() : BaseCommand(BlockType::B)
{
	m_index = -1;
	m_dataPointer = -1;
	m_unknown1 = -1;
	m_unknown2 = -1;

	//m_dataChunk = new char[5];
	m_command_id = -1;

	m_array_length = -1;

	
}

ExecutionCommand::~ExecutionCommand()
{

}

void ExecutionCommand::load(fstream& file)
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

void ExecutionCommand::loadBody(fstream& file, StringList string_list)
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

void ExecutionCommand::loadBodyTable(fstream& file, int length, StringList string_list)
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
