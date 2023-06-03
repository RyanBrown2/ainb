#pragma once
#include <iostream>
#include <fstream>
#include "DataBlock.h"
#include "nin-io/util/util.h"

class B_Block : public DataBlock
{
public:
	B_Block(int address);
	~B_Block();
	static B_Block load(std::fstream& file);
private:
	int m_index;
	int m_storedAddress;
};
