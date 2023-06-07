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

	// find start of section three
	file.seekg(0x30, ios::beg);
	readIntFromStream(file, is_big_endian, 4, m_section_three_start);

	cout << "Section Three Start: " << hex << m_section_three_start << endl;


	// find start of potential function section
	file.seekg(0x5c, ios::beg);
	int section_four_start;
	readIntFromStream(file, is_big_endian, 4, section_four_start);

	cout << "Section Four Start: " << hex << section_four_start << endl;

	cout << endl;

	// load table section
	file.seekg(m_table_section_start, ios::beg);
	m_table_section_data = loadTableSection(file);

	cout << "Table Section Loaded" << endl;

	//printTableSectionData(m_table_section_data);

	// load structure section
	file.seekg(m_structure_section_start, ios::beg);
	m_structure_section_data = loadStructureSection(file);


	return;

}

DataFoot::~DataFoot()
{

}

DataFoot::TableSectionData DataFoot::loadTableSection(fstream& file)
{
	TableSectionData table_section_data;
	
	vector<int> table_section_addresses;

	int current_pos = file.tellg();
	for (int i = 0; i < 6; i++) {
		file.seekg(current_pos, ios::beg);
		file.seekg(4*i, ios::cur);

		int entry_address;
		readIntFromStream(file, is_big_endian, 4, entry_address);
		file.seekg(entry_address, ios::beg);

		if (entry_address == m_structure_section_start) {
			break;
		}
		table_section_addresses.push_back(entry_address);
	}

	for (int i = 0; i < table_section_addresses.size(); i++) {
		TableData table_data;
		int current_pos = table_section_addresses[i];
		file.seekg(current_pos, ios::beg);
		table_data.address = current_pos;

		int end_pos;
		if (i + 1 < table_section_addresses.size()) {
			end_pos = table_section_addresses[i+1];
		}
		else {
			end_pos = m_structure_section_start;
		}

		while (current_pos < end_pos) {
			TableEntry entry;

			// get name
			int name_string_tag;
			readIntFromStream(file, is_big_endian, 4, name_string_tag);
			entry.name = m_string_list->getString(name_string_tag);

			file.seekg(0x4, ios::cur); // skip 4 bytes, assumed to be 0x00000000
			int var_string_tag;
			readIntFromStream(file, is_big_endian, 4, var_string_tag);
			entry.var = m_string_list->getString(var_string_tag);

			table_data.entries.push_back(entry);
			current_pos = file.tellg();
		}

		table_section_data.tables.push_back(table_data);

	}

	return table_section_data;
}

void DataFoot::printTableSectionData(TableSectionData table_section_data) {
	cout << "Table Section Data" << endl;
	for (int i = 0; i < table_section_data.tables.size(); i++) {
		cout << "Table " << i << endl;
		for (int j = 0; j < table_section_data.tables[i].entries.size(); j++) {
			cout << "Entry " << j << ": " << table_section_data.tables[i].entries[j].name << " | ";
			cout << table_section_data.tables[i].entries[j].var << endl;
		}
	}

}

// 12 addresses
DataFoot::StructureSectionData DataFoot::loadStructureSection(fstream& file)
{
	StructureSectionData structure_section;

	//int first_entry_address;
	//readIntFromStream(file, is_big_endian, 4, first_entry_address);
	//file.seekg(first_entry_address, ios::beg);

	vector<int> section_addresses;

	// load addresses
	for (int i = 0; i < 12; i++) {
		int entry_address;
		readIntFromStream(file, is_big_endian, 4, entry_address);
		if (entry_address == m_section_three_start) {
			break;
		}
		section_addresses.push_back(entry_address);
	}

	map<int, int> final_section_addresses;
	cout << "Sections: " << to_string(section_addresses.size()) << endl;
	int actual_sections = 0;
	int last_address = 0;
	for (int i = 0; i < section_addresses.size(); i++) {
		if (section_addresses[i] != last_address) {
			actual_sections++;
			final_section_addresses[i] = section_addresses[i];
			cout << "Section " << to_string(i) << " Address: " << hex << section_addresses[i] << endl;
		}
		last_address = section_addresses[i];
	}

	//cout << "Actual Sections: " << to_string(actual_sections) << endl;

	// section one
	int section_one_address = final_section_addresses[0];
	int section_two_address = final_section_addresses[1];
	file.seekg(section_one_address, ios::beg);

	while (file.tellg() < section_two_address) {
		StructureEntry entry = parseStructureEntry(file);

	}


	return structure_section;
}

DataFoot::StructureEntry DataFoot::parseStructureEntry(fstream& file)
{
	StructureEntry entry;

	int string_offset;
	readIntFromStream(file, is_big_endian, 4, string_offset);
	entry.name = m_string_list->getString(string_offset);

	file.seekg(0xc, ios::cur);

	return entry;
}

//DataFoot::StructureEntry DataFoot::loadEntry(fstream& file)
//{
//	StructureEntry entry;
//
//	int start_pos = file.tellg();
//
//	int name_string_tag;
//	readIntFromStream(file, is_big_endian, 4, name_string_tag);
//	entry.name = m_string_list->getString(name_string_tag);
//
//	file.seekg(start_pos + 0x10, ios::beg);
//
//	cout << "Name: " << entry.name << endl;
//
//	return entry;
//}
