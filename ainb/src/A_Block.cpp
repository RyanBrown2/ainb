#include "nin-io/ainb/A_Block.h"

using namespace std;

A_Block::A_Block() : DataBlock(BlockType::A)
{
	m_unknown2 = -1;
}

A_Block::~A_Block()
{

}

void A_Block::load(fstream& file)
{
	DataBlock::load(file);

	// unknown 1
	readIntFromStream(file, is_big_endian, 4, m_string_pointer);

	// data
	file.read(m_data_dump, 16);

	// unknown 2
	readIntFromStream(file, is_big_endian, 4, m_unknown2);
}
