#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "DataBlock.h"


class AINB
{
public:
	AINB(std::fstream& file);
	~AINB();
	//struct DataBlock {
	//	int beginAddress;
	//	int index;
	//	char data[17];
	//};
	struct SpecialDataBlock {
		int unknown1;
		char data[17];
		int unknown2;
	};
private:
	int headerOne;
	int headerTwo;
	int headerThree;
	bool isBigEndian = false;
	void parse(std::fstream& file);
	void parseHead(std::fstream& file);
	void parseData(std::fstream& file, int beginAddress);
	//DataBlock parseDataBlock(std::fstream& file, int beginAddress);
	//SpecialDataBlock parseSpecialDataBlock(std::fstream& file, int beginAddress);
	std::vector<std::string> parseStringArray(std::fstream& file, int beginAddress);
};
