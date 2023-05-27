#pragma once
#include <iostream>
#include <fstream>
#include "Util.h"

struct HeaderData {
	bool isBigEndian;
	int version;
	int hashOffset;
	int stringOffset;
	int rootNodeOffset;
};

class Parser
{
public:
	Parser(std::fstream& file);
	~Parser();
	HeaderData headerData;
private:
	HeaderData parseHeaderData(std::fstream& file);
};

