#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "nin-io/util/util.h"

class BaseCommand
{
public:
	enum BlockType {
		A, B,
	};
	BaseCommand(BlockType type);
	~BaseCommand();
	void load(std::fstream& file);
	int getAddress() { return m_address; }
	int getStringPointer() { return m_string_pointer; }

	void setName(std::string name) { m_name = name; }
	std::string getName() { return m_name; }
	char* getGuid() { return m_guid; }
private:
	BlockType m_blockType;
protected:
	int m_address;
	char* m_guid;
	int m_string_pointer;
	std::string m_name;

	static const bool is_big_endian = false;
};
