#include "nin-io/ainb/B_Block.h"

using namespace std;

B_Block::B_Block(int address) : DataBlock(address, BlockType::B)
{
	m_index = -1;
	m_storedAddress = -1;
}

B_Block::~B_Block()
{

}

B_Block B_Block::load(std::fstream& file)
{
	int address = file.tellg();
	B_Block block(address);

	// get index
	file.seekg(0x2, ios::cur);
	char indexChar[3];
	file.read(indexChar, 2);
	indexChar[2] = '\0';
	int index = convertHexCharArrayToInt(indexChar, false, 2);
	block.m_index = index;

	// TODO: get the rest of the data

	// get stored address
	file.seekg(address, ios::beg);
	file.seekg(0x14, ios::cur);
	char storedAddressChar[5];
	file.read(storedAddressChar, 4);
	storedAddressChar[4] = '\0';
	block.m_storedAddress = convertHexCharArrayToInt(storedAddressChar, false, 4);

	// get data chunk
	file.seekg(address, ios::beg);
	file.seekg(0x2c, ios::cur);
	file.read(block.data, 16);
	block.data[16] = '\0';

	return block;
}