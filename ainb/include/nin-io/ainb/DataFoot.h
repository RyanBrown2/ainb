#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "StringList.h"
#include "ParameterNode.h"
#include "nin-io/util/util.h"

class DataFoot {
public:
	DataFoot(std::fstream& file, StringList* string_list);
	~DataFoot();

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

	TableSectionData loadTableSection(std::fstream& file);

	struct StructureEntry {
		std::string name;
		std::vector<StructureEntry> children;
	};
	struct StructureSectionData {
		int count = 0;
		std::vector<std::string> strings;
	};
	StructureSectionData loadStructureSection(std::fstream& file);
private:
	int m_table_section_start;
	int m_structure_section_start;
	int m_section_three_start;
	TableSectionData m_table_section_data;
	StructureSectionData m_structure_section_data;

	StructureEntry parseStructureEntry(std::fstream& file);

	StringList* m_string_list;

	static void printTableSectionData(TableSectionData table_section_data);

	static const bool is_big_endian = false;

	static std::map<int, int> structure_entry_lengths;
};