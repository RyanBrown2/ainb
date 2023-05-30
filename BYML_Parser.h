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

class BYML
{
public:
	BYML(std::fstream& file);
	~BYML();
	HeaderData headerData;
private:
	HeaderData parseHeaderData(std::fstream& file);
};

