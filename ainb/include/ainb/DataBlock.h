#pragma once
#include <iostream>
#include <fstream>

class DataBlock
{
public:
	enum BlockType {
		A, B,
	};
	DataBlock(int address, BlockType type);
	~DataBlock();
	static DataBlock loadTypeA(std::fstream& file);
	char* data;
private:
	BlockType m_blockType;
	int m_address;

};
