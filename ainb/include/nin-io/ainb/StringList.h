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

	std::string getStringFromPos(int pos);
	int* getPosFromString(std::string str);
	void finalize();
private:
	std::map<int, std::string> m_string_loaded_map;

	std::map<std::string, int*> m_string_data;

};