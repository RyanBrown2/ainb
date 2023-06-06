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
	DataBlock(BlockType type);
	~DataBlock();
	char* data_dump;
	int m_address;
	void load(std::fstream& file);
private:
	BlockType m_blockType;
protected:
	char* m_data_dump;
	static const bool is_big_endian = false;
};
