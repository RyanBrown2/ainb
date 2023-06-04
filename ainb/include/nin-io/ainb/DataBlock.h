#pragma once
#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"

class DataBlock
{
public:
	enum BlockType {
		A, B,
	};
	DataBlock(int address, BlockType type);
	~DataBlock();
	char* data_dump;
	int m_address;
private:
	BlockType m_blockType;

};
