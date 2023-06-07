#include "nin-io/ainb/DataFoot.h"
#include "nin-io/ainb/StringList.h"

using namespace std;

DataFoot::DataFoot(fstream& file, StringList* string_list)
{
	m_string_list = string_list;


	// find start of the table section
	file.seekg(0x2c, ios::beg);
	readIntFromStream(file, is_big_endian, 4, m_table_section_start);

	cout << "Table Section Start: " << hex << m_table_section_start << endl;

	// find start of the structure section
	file.seekg(0x34, ios::beg);
	readIntFromStream(file, is_big_endian, 4, m_structure_section_start);

	cout << "Structure Section Start: " << hex << m_structure_section_start << endl;

	file.seekg(m_table_section_start, ios::beg);
	m_table_section_data = loadTableSection(file);

	return;

}

DataFoot::~DataFoot()
{

}

// 6 addresses
DataFoot::TableSection DataFoot::loadTableSection(fstream& file)
{
	TableSection table_section;
	
	int current_pos = file.tellg();
	for (int i = 0; i < 6; i++) {
		file.seekg(current_pos, ios::beg);
		file.seekg(4*i, ios::cur);

		int entry_address;
		readIntFromStream(file, is_big_endian, 4, entry_address);
		file.seekg(entry_address, ios::beg);

		int string_tag;
		readIntFromStream(file, is_big_endian, 4, string_tag);
		table_section.strings[i] = m_string_list->getString(string_tag);

		cout << "Entry " << to_string(i) << " Address: " << hex << entry_address << endl;
		cout << "String Tag: " << to_string(string_tag) << endl;
		cout << "String " << i << ": " << table_section.strings[i] << endl;
		cout << endl;
	}

	return table_section;
}

DataFoot::StructureEntry DataFoot::loadEntry(fstream& file)
{
	StructureEntry entry;

	int start_pos = file.tellg();

	int name_string_tag;
	readIntFromStream(file, is_big_endian, 4, name_string_tag);
	entry.name = m_string_list->getString(name_string_tag);

	file.seekg(start_pos + 0x10, ios::beg);

	cout << "Name: " << entry.name << endl;

	return entry;
}