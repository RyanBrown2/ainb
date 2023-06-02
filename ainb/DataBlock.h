#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../Util.h"

class DataBlock
{
public:
	DataBlock(int address);
	~DataBlock();
	void load(std::fstream& file);
	void print();
private:
	int address;
	int index;
	char *data;
};
