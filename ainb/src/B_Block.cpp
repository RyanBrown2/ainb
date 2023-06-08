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

	//int length = 0xa4/4;

	file.seekg(0x90, ios::cur);
	readIntFromStream(file, is_big_endian, 2, m_array_length);




	//Table table;

	//table.address = file.tellg();
	//table.length = m_array_length;

	if (m_array_length == 0) {
		file.seekg(current_pos, ios::beg);
		return;
	}

	file.seekg(0x12, ios::cur);

	cout << "Table Starting at: " << hex << file.tellg() << endl;

	m_table = loadBodyTable(file, m_array_length, string_list);

	cout << "Table Ending at: " << hex << file.tellg() << endl;

	file.seekg(current_pos, ios::beg);

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
