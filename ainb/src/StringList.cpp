#include "nin-io/ainb/StringList.h"

using namespace std;

StringList::StringList(int* start_pos)
{
	m_start_pos = start_pos;
}

StringList::~StringList()
{

}

void StringList::loadFromFile(fstream& file)
{
	file.seekg(*m_start_pos, ios::beg);

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
			m_loaded_string_data[current_pos - 1] = current_string;
			last_null = false;
		}

		if (current_string.length() == 0) {
			last_null = true;
		}

		m_loaded_string_data[current_pos] = current_string;
		null_pos++;
		current_pos = null_pos;
	}

}

string StringList::getStringFromOffset(int offset)
{
	return m_loaded_string_data[offset];
}

int* StringList::getOffsetFromString(string str)
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

	finalize();
	file.seekg(*m_start_pos, ios::beg);
	for (auto pair : m_string_data) {
		string str = pair.first;
		file.write(str.c_str(), str.length() + 1);
	}
}