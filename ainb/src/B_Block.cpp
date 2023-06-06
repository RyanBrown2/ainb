#include "nin-io/ainb/B_Block.h"

using namespace std;

B_Block::B_Block() : DataBlock(BlockType::B)
{
	m_index = -1;
	m_dataPointer = -1;
	m_unknown1 = -1;
	m_unknown2 = -1;

	m_dataChunk = new char[5];
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
	readIntFromStream(file, is_big_endian, 2, m_index);

	// unknown 2
	file.seekg(m_address, ios::beg);
	file.seekg(0x6, ios::cur);
	readIntFromStream(file, is_big_endian, 2, m_unknown2);

	// string pointer
	readIntFromStream(file, is_big_endian, 2, m_string_pointer);

	// 4 byte data chunk
	file.seekg(m_address, ios::beg);
	file.seekg(0xc, ios::cur);
	file.read(m_dataChunk, 4);
	m_dataChunk[4] = '\0';

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

void B_Block::loadBody(fstream& file)
{
	file.seekg(m_dataPointer, ios::beg);

	int length = 0xa4/4;

	for (int i = 0; i < length; i++) {
		int value;
		readIntFromStream(file, is_big_endian, 4, value);
		if (value > 60) {
			cout << "Value: " << value << endl;
		}
	}


}
