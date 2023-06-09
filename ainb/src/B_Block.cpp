#include "nin-io/ainb/B_Block.h"

using namespace std;

B_Block::B_Block() : DataBlock(BlockType::B)
{
	m_index = -1;
	m_dataPointer = -1;
	m_unknown1 = -1;
	m_unknown2 = -1;

	//m_dataChunk = new char[5];
	m_data_chunk = -1;

	m_array_length = -1;

	
}

B_Block::~B_Block()
{

}

void B_Block::load(fstream& file)
{
	DataBlock::load(file);

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
	readIntFromStream(file, is_big_endian, 4, m_data_chunk);
	//file.read(m_dataChunk, 4);
	//m_dataChunk[4] = '\0';

	// get the body data pointer
	file.seekg(m_address, ios::beg);
	file.seekg(0x14, ios::cur);
	readIntFromStream(file, is_big_endian, 4, m_dataPointer);

	// get data chunk
	file.seekg(m_address, ios::beg);
	file.seekg(0x2c, ios::cur);
	file.read(m_data_dump, 16);
	m_data_dump[16] = '\0';
}

void B_Block::loadBody(fstream& file, StringList string_list)
{
	int current_pos = file.tellg();
	file.seekg(m_dataPointer, ios::beg);



	map<int, int> section_to_key;
	map<int, int> value_map;

	int value_count = 0;
	for (int i = 0; i < 18; i++) {
		int key;
		readIntFromStream(file, is_big_endian, 4, key);
		int value;
		readIntFromStream(file, is_big_endian, 4, value);
		
		if (key != 0) {
			//cout << "Position: " << hex << 4*i << " | Value: " << hex << value << endl;
			section_to_key[i] = key;
			value_map[key] = value;
			value_count += 1;
		}

	}

	cout << *this << endl;

	for (const auto& pair : section_to_key) {
		int section = pair.first;
		int key = pair.second;
		int value = value_map[key];

		cout << "Position: " << hex << section << " | Key: " << hex << key << " | Value: " << hex << value << endl;
	}



	cout << endl;

	// todo - figure out proper way to determine length
	file.seekg(m_dataPointer, ios::beg);
	file.seekg(0xa4-0x1, ios::cur);

	readIntFromStream(file, is_big_endian, 1, m_array_length);

	//Table table;

	//table.address = file.tellg();
	//table.length = m_array_length;

	if (m_array_length == 0) {
		file.seekg(current_pos, ios::beg);
		return;
	}

	// todo - figure out proper way to determine length
	//file.seekg(0x12, ios::cur);

	//cout << "Table Starting at: " << hex << file.tellg() << endl;

	if (m_array_length > 0) {
		m_table = loadBodyTable(file, m_array_length, string_list);
	}

	//cout << "Table Ending at: " << hex << file.tellg() << endl;


	if (m_array_length > 0) {
		std::cout << "Array Length: " << std::to_string(m_array_length) << std::endl;
		for (int i = 0; i < m_array_length; i++) {
			std::cout << "Table Entry " << std::to_string(i) << ": " << std::hex << m_table.entries[i].value1 << ", " << m_table.entries[i].value2 << std::endl;
		}
	}

	cout << endl;

	file.seekg(current_pos, ios::beg);

	//if (value_count > (12+ m_array_length)) {
	if (value_map.size() > 12) {
		cout << endl << endl;
		cout << "Went over: " << dec << value_count << endl;
		cout << endl << endl;
		cin.get();
	}

}

B_Block::Table B_Block::loadBodyTable(fstream& file, int length, StringList string_list)
{
	Table table;

	table.address = file.tellg();
	table.length = length;

	//cout << "Table Starting at: " << hex << file.tellg() << endl;

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
	
		table.entries.push_back(entry);
	}

	return table;

}
