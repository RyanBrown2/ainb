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
		std::cout << "A Block at address: " << block.m_address << std::endl;
		std::cout << "Unknown 1: " << block.m_unknown1 << std::endl;
		std::cout << "Unknown 2:" << block.m_unknown2 << std::endl;
		return os;
	}

private:
	int m_unknown1;
	int m_unknown2;
};