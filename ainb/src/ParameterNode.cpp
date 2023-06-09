#include "nin-io/ainb/ParameterNode.h"

using namespace std;

ParameterNode::ParameterNode()
{
	m_address = -1;
	m_length = -1;

	m_name = "";

	m_tag = -1;
	m_terminated = false;

	m_follows_expected = true;

	m_string_list = NULL;

	m_second_string_tag = -1;
	m_second_string = "";
	m_section_num = -1;
}

ParameterNode::~ParameterNode()
{

}

void ParameterNode::load(fstream& file, int length, int section_num)
{

	m_address = file.tellg();
	m_length = length;
	m_section_num = section_num;

	int end_pos = (int)file.tellg() + length;
	
	int string_tag;
	readIntFromStream(file, is_big_endian, 2, string_tag);
	// 0x02 - current pos

	m_name = m_string_list->getString(string_tag);

	file.seekg(0x1, ios::cur);
	// 0x03 - current pos

	// check the last byte for terminator (0x80)
	int terminator;
	readIntFromStream(file, is_big_endian, 1, terminator);
	// 0x04 - current pos
	if (terminator == 0x80) {
		m_terminated = true;
		return;
	}

	if (file.tellg() >= end_pos) {
		return;
	}
	

	// get the tag
	readIntFromStream(file, is_big_endian, 2, m_tag);
	// 0x06 - current pos

	file.seekg(0x2, ios::cur);
	// 0x08 - current pos

	if (file.tellg() >= end_pos) {
		return;
	}

	file.seekg(0x2, ios::cur);
	// 0x0A - current pos

	// check for 0x80
	int check_value;
	readIntFromStream(file, is_big_endian, 1, check_value);
	// 0x0B - current pos

	//if (check_value != 0x80) {
	//	m_follows_expected = false;
	//}

	// check if second string is expected
	int check_second_string;
	readIntFromStream(file, is_big_endian, 1, check_second_string);
	// 0x0C - current pos

	if (file.tellg() >= end_pos) {
		return;
	}

	file.seekg(0x4, ios::cur);
	// 0x10 - current pos

	if (file.tellg() >= end_pos) {
		return;
	}

	// needs to be at 0x10 to read the second string tag
	readIntFromStream(file, is_big_endian, 4, m_second_string_tag);

	if (check_second_string == 0x01) {
		if (m_second_string_tag != 0x00000000 && m_second_string_tag != 0xffffffff) {
			m_follows_expected = false;
		}
	}

	if (m_tag == 0xFFFF) {

	}

	m_second_string = m_string_list->getString(m_second_string_tag);

	file.seekg(end_pos, ios::beg);

}


void ParameterNode::loadStringList(StringList* string_list)
{
	m_string_list = string_list;
}

int ParameterNode::getBlockRef() {
	return m_tag;
}
