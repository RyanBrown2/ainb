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

	// temp public for testing
	int m_unknown1; // 0x00

	friend std::ostream& operator<<(std::ostream& os, const B_Block block) {
		std::cout << block.m_name << std::endl;
		std::cout << "Address: " << block.m_address << std::endl;
		std::cout << "Index: " << std::to_string(block.m_index) << std::endl;
		std::cout << "Data Pointer: " << std::hex << block.m_dataPointer << std::endl;
		std::cout << "Unknown 1: " << block.m_unknown1 << std::endl;
		std::cout << "Unknown 2: " << block.m_unknown2 << std::endl;
		std::cout << "4 Byte Data Chunk: " << std::hex << block.m_data_chunk << std::endl;
		//std::cout << "String Pointer: " << std::to_string(block.m_string_pointer) << std::endl;
		if (block.m_array_length > 0) {
			std::cout << "Array Length: " << std::to_string(block.m_array_length) << std::endl;
		}
		return os;
	}
private:
	int m_index; // 0x02
	int m_dataPointer; // 0x14
	int m_data_chunk; // 0x0c
	// unknown values

	int m_unknown2; // 0x06
	int m_array_length;
};
