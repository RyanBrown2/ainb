#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <ocidl.h>
#include "util.h"

namespace ainb 
{

class StringList 
{
public:
	StringList(int* start_pos);
	~StringList();

	void loadFromStream(LPSTREAM stream);

	std::string getStringFromOffset(int offset);
	int getOffsetOfString(std::string str);
	void writeToStream(LPSTREAM stream);
private:
	int* m_start_pos = nullptr;

	// offset to string
	std::map<int, std::string> m_loaded_string_data;

	// this is used to keep track of the next offset to use when adding a new string to m_string_data
	int m_next_offset = 0;
	// string to offset
	std::map<std::string, int> m_string_data;
};

}