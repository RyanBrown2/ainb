#pragma once
#include <iostream>
#include <fstream>
#include "DataBlock.h"
#include "nin-io/util/util.h"

class B_Block : public DataBlock
{
public:
	B_Block();
	~B_Block();
	void load(std::fstream& file);

	void loadBody(std::fstream& file);


	friend std::ostream& operator<<(std::ostream& os, const B_Block block) {
		std::cout << "B Block at address: " << block.m_address << std::endl;
		std::cout << "Index: " << std::to_string(block.m_index) << std::endl;
		std::cout << "Data Pointer: " << std::hex << block.m_dataPointer << std::endl;
		std::cout << "Unknown 1: " << block.m_unknown1 << std::endl;
		std::cout << "Unknown 2: " << block.m_unknown2 << std::endl;
		std::cout << "String Pointer: " << std::to_string(block.m_string_pointer) << std::endl;
		return os;
	}
private:
	int m_index; // 0x02
	int m_dataPointer; // 0x14
	char *m_dataChunk; // 0x0c
	// unknown values
	int m_unknown1; // 0x00
	int m_unknown2; // 0x06
};
