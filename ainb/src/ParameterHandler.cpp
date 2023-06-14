#include "nin-io/ainb/ParameterHandler.h"
#include <stdexcept>

using namespace std;

ParameterHandler::ParameterHandler(StringList* string_list)
{
	m_string_list = string_list;
}

ParameterHandler::~ParameterHandler()
{

}

void ParameterHandler::loadTableParameters(fstream& file, int end_address)
{
	vector<int> table_section_addresses;
	vector<int> table_nums;

	int index = 0;
	int current_pos = file.tellg();
	for (int i = 0; i < 6; i++) {
		file.seekg(current_pos, ios::beg);
		file.seekg(4 * i, ios::cur);

		int entry_address;
		readIntFromStream(file, is_big_endian, 4, entry_address);
		file.seekg(entry_address, ios::beg);

		if (entry_address == end_address) {
			break;
		}

		if (i > 0) {
			if (entry_address == table_section_addresses[index - 1]) {
				table_section_addresses.pop_back();
				table_nums.pop_back();
				index -= 1;
			}
		}
		table_section_addresses.push_back(entry_address);
		table_nums.push_back(i);
		index += 1;
	}

	for (int i = 0; i < table_section_addresses.size(); i++) {
		int current_pos = table_section_addresses[i];
		file.seekg(current_pos, ios::beg);

		int end_pos;
		if (i + 1 < table_section_addresses.size()) {
			end_pos = table_section_addresses[i + 1];
		}
		else {
			end_pos = end_address;
		}

		m_active_tables.push_back(table_nums[i]);

		// create null parameter at index 0
		TableParameter null_parameter;
		m_table_parameters[table_nums[i]].push_back(null_parameter);

		while (current_pos < end_pos) {
			TableParameter table_parameter;

			table_parameter.load(file, m_string_list);

			m_table_parameters[table_nums[i]].push_back(table_parameter);
			current_pos = file.tellg();
		}
	}

}

void ParameterHandler::loadStructureParameters(fstream& file, int end_address)
{
	vector<int> section_addresses;
	map<int, int> address_to_section_number;

	// load addresses
	int last_vector_index = 0;
	for (int i = 0; i < 12; i++) {
		int entry_address;
		readIntFromStream(file, is_big_endian, 4, entry_address);
		if (entry_address == end_address) {
			break;
		}

		// handle repeating addresses
		if (i > 0) {
			// if this address is repeated
			if (entry_address == section_addresses[last_vector_index]) {
				address_to_section_number[entry_address] = i;
			}
			else {
				last_vector_index += 1;
				section_addresses.push_back(entry_address);
				address_to_section_number[entry_address] = i;
			}
		}
		else {
			last_vector_index = i;
			section_addresses.push_back(entry_address);
			address_to_section_number[entry_address] = i;
		}

	}

	for (int i = 0; i < section_addresses.size(); i++) {

		int section_address = section_addresses[i];
		// get entry lengths for section
		int section_number = address_to_section_number[section_address];
		int entry_length = structure_entry_lengths[section_number];

		file.seekg(section_address, ios::beg);

		int end_pos;
		if (i + 1 < section_addresses.size()) {
			end_pos = section_addresses[i + 1];
		}
		else {
			end_pos= end_address;
		}

		m_active_structures.push_back(section_number);
		

		// create null parameter at index 0
		StructureParameter null_parameter;
		m_structure_parameters[section_number].push_back(null_parameter);

		while (file.tellg() < end_pos) {
			StructureParameter parameter;
			parameter.load(file, m_string_list, section_number);
			m_structure_parameters[section_number].push_back(parameter);
		}
	}
}


ParameterHandler::TableParameter ParameterHandler::getParameterFromTable(int table_num, int parameter_num)
{
	try {
		return m_table_parameters.at(table_num).at(parameter_num);
	}
	catch (out_of_range) {
		cerr << "Tried getting parameter index " << to_string(parameter_num) << " from table " << to_string(table_num) << endl;
		throw std::invalid_argument("Invalid parameter index");
	}
}

ParameterHandler::StructureParameter ParameterHandler::getParameterFromStructure(int section_num, int parameter_num)
{
	return m_structure_parameters.at(section_num).at(parameter_num);
}

vector<ParameterHandler::TableParameter>* ParameterHandler::getTableParameters(int table_num)
{
	return &m_table_parameters[table_num];
}

vector<ParameterHandler::StructureParameter>* ParameterHandler::getStructureParameters(int section_num)
{
	return &m_structure_parameters[section_num];
}


map<int, int> ParameterHandler::structure_entry_lengths = {
	{0, 0x10},
	{1, 0x4},
	{2, 0x10},
	{3, 0x4},
	{4, 0x10},
	{5, 0x4},
	{6, 0x10},
	{7, 0x4},
	{8, 0x18},
	{9, 0x4},
	{10, 0x14},
	{11, 0x4}
};

void ParameterHandler::TableParameter::load(fstream& file, StringList* string_list)
{
	address = file.tellg();

	int name_tag;
	readIntFromStream(file, is_big_endian, 4, name_tag);
	TableParameter::name = string_list->getString(name_tag);

	file.seekg(4, ios::cur);

	readIntFromStream(file, is_big_endian, 4, TableParameter::value);
}

void ParameterHandler::StructureParameter::load(fstream& file, StringList* string_list, int section_num)
{

	int end_pos = (int)file.tellg() + ParameterHandler::structure_entry_lengths[section_num];

	address = (int)file.tellg();
	StructureParameter::section_num = section_num;

	int name_tag;
	readIntFromStream(file, is_big_endian, 2, name_tag);
	StructureParameter::name = string_list->getString(name_tag);
	// 0x02 - current pos

	file.seekg(0x2, ios::cur);
	// 0x04 - current pos

	if (file.tellg() >= end_pos) {
		return;
	}

	// if this is section 10, then we have a special case
	if (section_num == 10) {
		int second_string_tag;
		readIntFromStream(file, is_big_endian, 2, second_string_tag);
		StructureParameter::second_string = string_list->getString(second_string_tag);
	} else {
		// get the tag
		readIntFromStream(file, is_big_endian, 2, tag);
	}
	// 0x06 - current pos

	file.seekg(0x2, ios::cur);
	// 0x08 - current pos

	if (file.tellg() >= end_pos) {
		return;
	}

	// TODO: finish loading the parameter data

	file.seekg(end_pos, ios::beg);
	return;
}


