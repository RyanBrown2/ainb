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

void StringList::loadFromStream(LPSTREAM stream)
{
	streamSeek(stream, 0, END);
	int end_pos = streamTell(stream);
	streamSeek(stream, *m_start_pos, START);

	string raw_string;
	char buff[1];
	while(streamTell(stream) < end_pos) {
		stream->Read(buff, 1, 0);
		raw_string.push_back(buff[0]);
	}

	int null_pos = 0;
	int current_pos = 0;
	int last_null = false;
	while ((null_pos = raw_string.find('\0', null_pos)) != string::npos) {
		string current_string = raw_string.substr(current_pos, null_pos - current_pos);

		//if (last_null) {
		//	m_loaded_string_data[current_pos - 1] = current_string;
		//	last_null = false;
		//}

		//if (current_string.length() == 0) {
		//	last_null = true;
		//}

		m_loaded_string_data[current_pos] = current_string;
		null_pos++;
		current_pos = null_pos;
		//cout << current_string << endl;
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
		m_string_data.insert(pair<string, int>(str, m_next_offset));
		m_next_offset = str.length() + 1;
		return m_string_data.at(str);
	}
}

void StringList::writeToStream(LPSTREAM stream)
{
	streamSeek(stream, *m_start_pos, START);
	for (auto pair : m_string_data) {
		string str = pair.first;
		stream->Write(str.c_str(), str.length() + 1, NULL);
	}
}
