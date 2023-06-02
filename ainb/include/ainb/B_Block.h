#pragma once
#include <iostream>
#include <fstream>
#include "ainb/DataBlock.h"

class B_Block : public DataBlock
{
public:
	B_Block(int address);
	~B_Block();
	static B_Block load(std::fstream& file);
private:
	int index;
};
