#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "StringList.h"
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

	struct StructureSection {
		int count = 0;
		std::vector<std::string> strings;
	};
	StructureSection loadStructureSection(std::fstream& file);
private:
	int m_table_section_start;
	int m_structure_section_start;
	int m_section_three_start;
	TableSectionData m_table_section_data;
	StructureSection m_structure_section_data;

	StringList* m_string_list;

	struct StructureEntry {
		std::string name;
		std::vector<StructureEntry> children;
	};

	StructureEntry loadEntry(std::fstream& file);

	static void printTableSectionData(TableSectionData table_section_data);

	static const bool is_big_endian = false;

};