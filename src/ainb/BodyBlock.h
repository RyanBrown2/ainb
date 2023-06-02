#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../Util.h"

class BodyBlock
{
public:
	BodyBlock(int address);
	~BodyBlock();
	void load(std::fstream& file);
	void print();
private:
	int address;
	int index;
	char *data;
};
