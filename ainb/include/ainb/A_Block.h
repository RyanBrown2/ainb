#pragma once
#include <iostream>
#include <fstream>
#include "DataBlock.h"

class A_Block : public DataBlock
{
public:
	A_Block(int address);
	~A_Block();
	static A_Block load(std::fstream& file);
private:
	int unknown1;
	int unknown2;
};