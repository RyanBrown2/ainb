#include "nin-io/ainb/DataBlock.h"

using namespace std;

DataBlock::DataBlock(BlockType type)
{
	//DataBlock::m_address = address;
	DataBlock::m_address = -1;
	data_dump = new char[17];

	m_data_dump = new char[17];

	m_blockType = type;
}

DataBlock::~DataBlock()
{
	//delete[] data;
}

void DataBlock::load(fstream& file)
{
	m_address = file.tellg();
}
