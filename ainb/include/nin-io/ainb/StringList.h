#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include "nin-io/util/util.h"


class StringList
{
public:
	StringList(std::fstream& file);
	~StringList();

private:
	char* m_raw_data;
};