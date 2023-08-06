#include "ainb/StringList.h"

using namespace std;
using namespace ainb;

StringList::StringList(int* start_pos)
{
	m_start_pos = start_pos;
}

StringList::~StringList()
{

}

void StringList::loadFromStream(fstream& stream)
{
	stream.seekg(0, ios::end);
	int end_pos = stream.tellg();
	stream.seekg(*m_start_pos, ios::beg);

	string raw_string;
	char buff[1];
	while(stream.tellg() < end_pos) {
		stream.read(buff, 1);
		raw_string.push_back(buff[0]);
	}

	int null_pos = 0;
	int current_pos = 0;
	int last_null = false;
	while ((null_pos = raw_string.find('\0', null_pos)) != string::npos) {
		string current_string = raw_string.substr(current_pos, null_pos - current_pos);
		m_loaded_string_data[current_pos] = current_string;
		null_pos++;
		current_pos = null_pos;
	}
}

string StringList::getStringFromOffset(int offset)
{
	return m_loaded_string_data[offset];
}


/**
 * Returns the offset of the string in the string list.
 * If the string is not in the string list, it is added to the string list and the offset is returned.
*/
int StringList::getOffsetOfString(string str)
{
	if (m_string_data.count(str)) {

		return m_string_data.at(str);
	}
	else {
		m_strings_ordered.push_back(str);
		m_string_data.insert(pair<string, int>(str, m_next_offset));
		m_next_offset = str.length() + 1;
		return m_string_data.at(str);
	}
}

void StringList::writeToStream(fstream& stream)
{
	//streamSeek(stream, *m_start_pos, START);
	stream.seekg(*m_start_pos, ios::beg);
	for (int i = 0; i < m_strings_ordered.size(); i++)
	{
		string str = m_strings_ordered[i];
		stream.write(str.c_str(), str.length() + 1);
	}
}

int StringList::getNextOffset()
{
	return m_next_offset;
}
