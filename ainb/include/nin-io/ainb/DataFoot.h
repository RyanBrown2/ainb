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

	struct TableSection {
		std::string strings[6];
	};

	TableSection loadTableSection(std::fstream& file);
private:
	int m_table_section_start;
	int m_structure_section_start;
	TableSection m_table_section_data;

	StringList* m_string_list;

	struct StructureEntry {
		std::string name;
		std::vector<StructureEntry> children;
	};

	StructureEntry loadEntry(std::fstream& file);

	static const bool is_big_endian = false;
};