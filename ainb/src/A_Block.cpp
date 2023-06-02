#include "ainb/A_Block.h"
#include "util/util.h"

using namespace std;

A_Block::A_Block(int address) : DataBlock(address, BlockType::A)
{

}

A_Block::~A_Block()
{

}

A_Block A_Block::load(std::fstream& file)
{
	int address = file.tellg();
	A_Block block(address);

	// unknown 1
	char unknown1Char[5];
	file.read(unknown1Char, 4);
	unknown1Char[4] = '\0';
	block.unknown1 = convertHexCharArrayToInt(unknown1Char, false, 4);

	// data
	file.read(block.data, 16);
	block.data[16] = '\0';

	// unknown 2
	char unknow2Char[5];
	file.read(unknow2Char, 4);
	unknow2Char[4] = '\0';
	block.unknown2 = convertHexCharArrayToInt(unknow2Char, false, 4);

	return block;
}