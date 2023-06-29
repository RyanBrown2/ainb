#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ocidl.h>
#include "StringList.h"
#include "util.h"

namespace ainb {

class ParameterHandler {
public:
	ParameterHandler(StringList* string_list);
	~ParameterHandler();

	struct TableParameter {
		int address = -1;
		int index = -1;
		std::string name = "";
		int value = 0;
		
		void load(LPSTREAM stream, StringList* string_list, int section_num);
	};

	struct StructureParameter {
		int address = -1;
		int index = -1;
		std::string name = "";
		int tag = -1;
		std::string second_string = "";
		int section_num = -1;

		void load(LPSTREAM stream, StringList* string_list, int section_num);
	};

	void loadTableParameters(LPSTREAM stream, int end_address);
	void loadStructureParameters(LPSTREAM stream, int end_address);

	std::vector<int>* getActiveTables() { return &m_active_tables; }
	std::vector<int>* getActiveStructures() { return &m_active_structures; }

	TableParameter* getParameterFromTable(int table_num, int parameter_num);
	StructureParameter* getParameterFromStructure(int section_num, int parameter_num);

	std::vector<TableParameter>* getTableParameters(int table_num);
	std::vector<StructureParameter>* getStructureParameters(int section_num);

private:
	StringList* m_string_list;

	// table_num to vector of parameters
	std::map<int, std::vector<TableParameter>> m_table_parameters;

	std::map<int, std::vector<StructureParameter>> m_structure_parameters;

	std::vector<int> m_active_tables;
	std::vector<int> m_active_structures;

	static std::map<int, int> table_entry_lengths;
	static std::map<int, int> structure_entry_lengths;
};

}