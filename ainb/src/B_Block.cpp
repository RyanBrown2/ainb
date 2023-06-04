#include "nin-io/ainb/B_Block.h"

using namespace std;

B_Block::B_Block(int address) : DataBlock(address, BlockType::B)
{
	m_index = -1;
	m_storedAddress = -1;
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
	//char unknown1Char[3];
	//file.read(unknown1Char, 2);
	//unknown1Char[2] = '\0';
	//block.m_unknown1 = convertHexCharArrayToInt(unknown1Char, false, 2);
	readIntFromStream(file, false, 2, block.m_unknown1);

	// get index
	file.seekg(0x2, ios::cur);
	//char indexChar[3];
	//file.read(indexChar, 2);
	//indexChar[2] = '\0';
	//int index = convertHexCharArrayToInt(indexChar, false, 2);
	//block.m_index = index;
	readIntFromStream(file, false, 2, block.m_index);

	// unknown2
	file.seekg(address, ios::beg);
	file.seekg(0x6, ios::cur);
	//char unknown2Char[3];
	//file.read(unknown2Char, 2);
	//unknown2Char[2] = '\0';
	//block.m_unknown2 = convertHexCharArrayToInt(unknown2Char, false, 2);
	readIntFromStream(file, false, 2, block.m_unknown2);

	//unknown3
	//char unknown3Char[3];
	//file.read(unknown3Char, 2);
	//unknown3Char[2] = '\0';
	//block.m_unknown3 = convertHexCharArrayToInt(unknown3Char, false, 2);
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
	//char storedAddressChar[5];
	//file.read(storedAddressChar, 4);
	//storedAddressChar[4] = '\0';
	//block.m_storedAddress = convertHexCharArrayToInt(storedAddressChar, false, 4);
	readIntFromStream(file, false, 4, block.m_storedAddress);

	// get data chunk
	file.seekg(address, ios::beg);
	file.seekg(0x2c, ios::cur);
	file.read(block.data_dump, 16);
	block.data_dump[16] = '\0';

	return block;
}