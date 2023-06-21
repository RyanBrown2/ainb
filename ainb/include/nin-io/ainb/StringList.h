#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>

class StringList
{
public:
	StringList(int* start_pos);
	~StringList();

	void loadFromFile(std::fstream& file);

	std::string getStringFromOffset(int offset);
	int* getOffsetFromString(std::string str);
	void finalize();
	void writeToFile(std::fstream& file);
private:

	int* m_start_pos = nullptr;

	// offset to string
	std::map<int, std::string> m_loaded_string_data;

	std::map<std::string, int*> m_string_data;
};