#include "ainb/B_Block.h"
#include "util/util.h"

using namespace std;

B_Block::B_Block(int address) : DataBlock(address, BlockType::B)
{
	index = -1;
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
	block.index = index;

	// TODO: get the rest of the data

	file.seekg(address, ios::beg);
	file.seekg(0x2c, ios::cur);
	file.read(block.data, 16);
	block.data[16] = '\0';

	return block;
}