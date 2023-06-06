#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "nin-io/util/util.h"


class StringList
{
public:
	StringList(std::fstream& file);
	~StringList();

	std::string getString(int pos);
private:
	std::map<int, std::string> m_string_map;
};