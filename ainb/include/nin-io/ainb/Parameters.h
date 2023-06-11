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
		int table_num;
		std::vector<TableEntry> entries;
	};
	struct StructureSectionData {
		std::map<int, std::vector<ParameterNode>> structure_entries;
		//std::vector<ParameterNode> parameter_nodes;
	};

	struct ParameterData {
		std::vector<TableData> tables;
		std::vector<TableEntry> table_entries;
		std::vector<ParameterNode> parameters;
	};

	ParameterData getData() { return m_data; }


	TableEntry getTableEntry(int table_num, int index);
	ParameterNode getParameterNode(int section_num, int index);

	// temporary to see which subsections of structure section are being used
	std::map<int, int> t_structure_subsection_addresses;

private:
	int m_table_section_start;
	int m_structure_section_start;
	int m_section_3_start;

	// this contains all the tables mapped to which number they are
	std::map<int, TableData> m_table_data_map;

	// this contains all the structure nodes
	//StructureSectionData m_structure_section_data;
	std::map<int, std::vector<ParameterNode>> m_structure_section_data;

	void loadTableSection(std::fstream& file);
	void loadStructureSection(std::fstream& file);




	// old stuff below, cleaning up



	StringList* m_string_list;

	//void printTableSectionData(TableSectionData table_section_data);

	ParameterData m_data;

	static const bool is_big_endian = false;

	static std::map<int, int> structure_entry_lengths;
};