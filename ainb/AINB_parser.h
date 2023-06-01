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
private:
	int unknownOneAddress;
	int unknownTwoAddress;
	int unknownThreeAddress;
	bool isBigEndian = false;
	void parse(std::fstream& file);
	void parseHead(std::fstream& file);
	void parseData(std::fstream& file, int beginAddress);
	std::vector<std::string> parseStringArray(std::fstream& file, int beginAddress);
};
