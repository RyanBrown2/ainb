#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>




class AINB
{
public:
	AINB(std::fstream& file);
	~AINB();
	struct DataBlock {
		int version;
		int beginAddress;
		int index;
		char data[17];
	};
private:
	int headerOne;
	int headerTwo;
	int headerThree;
	bool isBigEndian = false;
	void parse(std::fstream& file);
	void parseHead(std::fstream& file);
	void parseData(std::fstream& file, int beginAddress);
	DataBlock parseDataBlock(std::fstream& file, int beginAddress);
	std::vector<std::string> parseStringArray(std::fstream& file, int beginAddress);
};
