#include "nin-io/ainb/DataBlock.h"

using namespace std;

DataBlock::DataBlock(int address, BlockType type)
{
	DataBlock::m_address = address;
	data_dump = new char[17];
	data_dump[16] = '\0';

	m_blockType = type;
}

DataBlock::~DataBlock()
{
	//delete[] data;
}

