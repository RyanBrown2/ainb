#include "nin-io/ainb/StringList.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

StringList::StringList(fstream& file)
{
	string raw_string;
	while (!file.eof()) {
		raw_string.push_back(file.get());
	}
	file.clear();

	int null_pos = 0;
	int current_pos = 0;
	int last_null = false;
	while ((null_pos = raw_string.find('\0', null_pos)) != string::npos) {
		string current_string = raw_string.substr(current_pos, null_pos - current_pos);

		if (last_null) {
			m_string_loaded_map[current_pos - 1] = current_string;
			last_null = false;
		}

		if (current_string.length() == 0) {
			last_null = true;
		}

		m_string_loaded_map[current_pos] = current_string;
		null_pos++;
		current_pos = null_pos;
	}	
}

StringList::~StringList()
{
}

string StringList::getStringFromPos(int pos)
{
	return m_string_loaded_map[pos];
}

int* StringList::getPosFromString(string str)
{
	if (!m_string_data.count(str)) {
		int* pos = new int;
		*pos = -1;
		m_string_data[str] = pos;
	}
	return m_string_data[str];
}

void StringList::finalize()
{
	int last_pos = 0;
	for (auto it = m_string_data.begin(); it != m_string_data.end(); it++) {
		string str = it->first;
		*it->second = last_pos;
		last_pos += str.length() + 1;
	}
}

void StringList::writeToFile(fstream& file)
{
	for (auto pair : m_string_data) {
		string str = pair.first;
		file.write(str.c_str(), str.length() + 1);
	}
}
