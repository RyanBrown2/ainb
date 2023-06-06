#pragma once
#include <iostream>
#include <fstream>
#include <string>
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
	int getStringPointer() { return m_string_pointer; }
	void setString(std::string name) { m_name = name; }
private:
	BlockType m_blockType;
protected:
	int m_address;
	char* m_data_dump;
	int m_string_pointer;
	std::string m_name;

	static const bool is_big_endian = false;
};
