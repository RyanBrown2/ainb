#include "nin-io/ainb/DataBlock.h"

using namespace std;

DataBlock::DataBlock(BlockType type)
{
	//DataBlock::m_address = address;
	m_address = -1;
	m_data_dump = new char[17];

	m_blockType = type;
	m_string_pointer = -1;
}

DataBlock::~DataBlock()
{
	//delete[] data;
}

void DataBlock::load(fstream& file)
{
	m_address = file.tellg();
}
