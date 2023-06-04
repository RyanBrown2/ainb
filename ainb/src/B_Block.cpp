#include "nin-io/ainb/B_Block.h"

using namespace std;

B_Block::B_Block(int address) : DataBlock(address, BlockType::B)
{
	m_index = -1;
	m_dataPointer = -1;
	m_unknown1 = -1;
	m_unknown2 = -1;
	m_unknown3 = -1;

	m_dataChunk = new char[5];
}

B_Block::~B_Block()
{

}

B_Block B_Block::load(std::fstream& file)
{
	int address = file.tellg();
	B_Block block(address);

	// unknown1
	readIntFromStream(file, false, 2, block.m_unknown1);

	// get index
	//file.seekg(0x2, ios::cur);
	readIntFromStream(file, false, 2, block.m_index);

	// unknown2
	file.seekg(address, ios::beg);
	file.seekg(0x6, ios::cur);
	readIntFromStream(file, false, 2, block.m_unknown2);

	//unknown3
	readIntFromStream(file, false, 2, block.m_unknown3);

	// 4 byte data chunk
	file.seekg(address, ios::beg);
	file.seekg(0xc, ios::cur);
	char dataChunkChar[5];
	file.read(dataChunkChar, 4);
	dataChunkChar[4] = '\0';
	block.m_dataChunk = dataChunkChar;

	// get stored address
	file.seekg(address, ios::beg);
	file.seekg(0x14, ios::cur);
	readIntFromStream(file, false, 4, block.m_dataPointer);

	// get data chunk
	file.seekg(address, ios::beg);
	file.seekg(0x2c, ios::cur);
	file.read(block.data_dump, 16);
	block.data_dump[16] = '\0';

	return block;
}