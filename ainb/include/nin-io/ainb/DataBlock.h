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
	void load(std::fstream& file);
private:
	BlockType m_blockType;
protected:
	int m_address;
	char* m_data_dump;
	int m_string_pointer;

	static const bool is_big_endian = false;
};
