#include "nin-io/ainb/StringList.h"

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
	while ((null_pos = raw_string.find('\0', null_pos)) != string::npos) {
		string current_string = raw_string.substr(current_pos, null_pos - current_pos);
		null_pos++;
		current_pos = null_pos;
		if (current_string.length() == 0) {
			m_string_map[current_pos] = m_string_map[current_pos - 1];
			continue;
		}
		m_string_map[current_pos] = current_string;
	}	
}

StringList::~StringList()
{
}

string StringList::getString(int pos)
{
	return m_string_map[pos];
}
