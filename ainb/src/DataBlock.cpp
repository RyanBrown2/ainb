#include "nin-io/ainb/DataBlock.h"

using namespace std;

DataBlock::DataBlock(int address, BlockType type)
{
	DataBlock::m_address = address;
	data = new char[17];
	data[16] = '\0';

	m_blockType = type;
}

DataBlock::~DataBlock()
{
	//delete[] data;
}

DataBlock DataBlock::loadTypeA(fstream& file)
{
	int address = file.tellg();
	DataBlock dataBlock(address, BlockType::A);

	// get index
	file.seekg(0x2, ios::cur);
	char indexChar[3];
	file.read(indexChar, 2);
	indexChar[2] = '\0';
	int index = convertHexCharArrayToInt(indexChar, false, 2);

	file.read(dataBlock.data, 16);

	return dataBlock;

}
