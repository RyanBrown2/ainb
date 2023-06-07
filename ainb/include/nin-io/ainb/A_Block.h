#pragma once
#include <iostream>
#include <fstream>
#include "nin-io/util/util.h"
#include "DataBlock.h"

class A_Block : public DataBlock
{
public:
	A_Block();
	~A_Block();
	void load(std::fstream& file);

	friend std::ostream& operator<<(std::ostream& os, const A_Block block) {
		std::cout << block.m_name << std::endl;
		std::cout << "Address: " << block.m_address << std::endl;
		//std::cout << "String Pointer: " << std::to_string(block.m_string_pointer) << std::endl;
		std::cout << "Unknown 2: " << std::hex << block.m_unknown2 << std::endl;
		return os;
	}

private:
	int m_unknown2;
};