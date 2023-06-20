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
		std::string name = "";
		int value = -1;
		int address = -1;

		void load(std::fstream& file, StringList* string_list);
	};

	struct StructureParameter {
		std::string name;
		int tag = -1;
		std::string second_string = "";
		int address = -1;
		int section_num = -1;

		void load(std::fstream& file, StringList* string_list, int section_num);

	};

	void loadTableParameters(std::fstream& file, int end_address);
	void loadStructureParameters(std::fstream& file, int end_address);

	std::vector<int>* getActiveTables() { return &m_active_tables; }
	std::vector<int>* getActiveStructures() { return &m_active_structures; }

	TableParameter getParameterFromTable(int table_num, int parameter_num);
	StructureParameter getParameterFromStructure(int section_num, int parameter_num);

	std::vector<TableParameter>* getTableParameters(int table_num);
	std::vector<StructureParameter>* getStructureParameters(int section_num);
private:

	StringList* m_string_list;

	// table_num to vector of parameters
	std::map<int, std::vector<TableParameter>> m_table_parameters;

	std::map<int, std::vector<StructureParameter>> m_structure_parameters;

	std::vector<int> m_active_tables;
	std::vector<int> m_active_structures;

	static const bool is_big_endian = false;
	static std::map<int, int> structure_entry_lengths;
};
