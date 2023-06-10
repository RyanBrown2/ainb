#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "StringList.h"
#include "ParameterNode.h"
#include "nin-io/util/util.h"

class Parameters {
public:
	Parameters(std::fstream& file, StringList* string_list);
	~Parameters();

	struct TableEntry {
		std::string name;
		std::string var;
	};
	struct TableData {
		int address;
		std::vector<TableEntry> entries;
	};
	struct TableSectionData {
		std::vector<TableData> tables;
	};
	struct StructureSectionData {
		std::vector<ParameterNode> parameter_nodes;
	};

	struct ParameterData {
		std::vector<TableEntry> table;
		std::vector<ParameterNode> parameters;
	};

	ParameterData getData() { return m_data; }

private:
	int m_table_section_start;
	int m_structure_section_start;
	int m_section_3_start;

	TableSectionData m_table_section_data;
	StructureSectionData m_structure_section_data;


	TableSectionData loadTableSection(std::fstream& file);
	StructureSectionData loadStructureSection(std::fstream& file);

	StringList* m_string_list;

	static void printTableSectionData(TableSectionData table_section_data);

	ParameterData m_data;

	static const bool is_big_endian = false;

	static std::map<int, int> structure_entry_lengths;
};