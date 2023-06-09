#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "StringList.h"
#include "nin-io/util/util.h"

class ParameterNode
{
public:
	ParameterNode();
	~ParameterNode();
	void loadStringList(StringList* string_list);
	void load(std::fstream& file, int length, int section_num);

	int getBlockRef();

	friend std::ostream& operator<<(std::ostream& os, const ParameterNode& node) {
		os << "ParameterNode: " << node.m_name << std::endl;
		os << "Section: " << std::to_string(node.m_section_num) << std::endl;
		os << "Address: " << std::hex << node.m_address;
		os << " | Length: " <<std::dec << node.m_length << std::endl;
		os << "Terminated: " << (node.m_terminated ? "Yes" : "No") << std::endl;
		os << "Tag: " << std::hex << node.m_tag << std::endl;
		if (node.m_second_string != "") {
			os << "Second String: " << node.m_second_string << std::endl;
		}

		os << "Met Expected Format: " << (node.m_follows_expected ? "Yes" : "No") << std::endl;
		if (!node.m_follows_expected) {

		}
		return os;
	}
private:
	int m_address;
	int m_length;

	std::string m_name;


	int m_tag;
	bool m_terminated;

	bool m_follows_expected;

	StringList* m_string_list;

	int m_section_num;

	int m_second_string_tag;
	std::string m_second_string;

	static const bool is_big_endian = false;
};