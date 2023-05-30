#pragma once
#include <iostream>


class AINB
{
public:
	AINB(std::fstream& file);
	~AINB();

	void Parse(const char* filename);
};
