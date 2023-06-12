#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "nin-io/util/util.h"
#include "StringList.h"

class ParameterHandler
{
public:
	ParameterHandler(StringList* string_list);
	~ParameterHandler();

	struct TableParameter {
		std::string name;
		int value;

		void load(std::fstream& file, StringList* string_list);
	};

	struct StructureParameter {
		std::string name;
		int tag;
		std::string second_string = "";

		void load(std::fstream& file, StringList* string_list, int section_num);

	};

	void loadTableParameters(std::fstream& file, int end_address);
	void loadStructureParameters(std::fstream& file, int end_address);

	TableParameter getParameterFromTable(int table_num, int parameter_num);
	StructureParameter getParameterFromStructure(int section_num, int parameter_num);
private:

	StringList* m_string_list;

	std::map<int, std::vector<TableParameter>> m_table_parameters;

	std::map<int, std::vector<StructureParameter>> m_structure_parameters;

	static const bool is_big_endian = false;
	static std::map<int, int> structure_entry_lengths;
};
