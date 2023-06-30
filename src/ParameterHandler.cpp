#include "ainb/ParameterHandler.h"

using namespace std;
using namespace ainb;

ParameterHandler::ParameterHandler(StringList* string_list)
{
	m_string_list = string_list;
}

ParameterHandler::~ParameterHandler()
{

}

void ParameterHandler::loadTableParameters(LPSTREAM stream, int end_address)
{
	vector<int> table_section_addresses;
	vector<int> table_nums;

	int index = 0;
	int current_pos = streamTell(stream);
	for (int i = 0; i < 6; i++) {
		streamSeek(stream, current_pos, START);
		streamSeek(stream, 4 * i, CURRENT);

		int entry_address;
		readIntFromStream(stream, entry_address);
		streamSeek(stream, entry_address, START);

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
		int section_num = table_nums[i];
		streamSeek(stream, current_pos, START);

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
		null_parameter.index = 0;
		m_table_parameters[table_nums[i]].push_back(null_parameter);

		int index = 1;
		while (current_pos < end_pos) {
			TableParameter table_parameter;

			table_parameter.load(stream, m_string_list, section_num);
			table_parameter.index = index;
			index += 1;

			m_table_parameters[table_nums[i]].push_back(table_parameter);
			current_pos = streamTell(stream);
		}
	}
}

void ParameterHandler::loadCommandParameters(LPSTREAM stream, int end_address)
{
	vector<int> section_addresses;
	map<int, int> address_to_section_number;

	// load section addresses
	int last_vector_index = 0;
	for (int i = 0; i < 12; i++) {
		int entry_address;
		readIntFromStream(stream, entry_address);
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

	// load parameters
	for (int i = 0; i < section_addresses.size(); i++) {
		int section_address = section_addresses[i];
		// get entry lengths for section
		int section_number = address_to_section_number[section_address];
		int entry_length = parameter_entry_lengths[section_number];

		streamSeek(stream, section_address, START); // go to beginning of section

		int end_pos;
		if (i + 1 < section_addresses.size()) {
			end_pos = section_addresses[i + 1];
		}
		else {
			end_pos = end_address;
		}

		m_active_paramter_lists.push_back(section_number);

		// get the parameter type for this section
		ParameterType param_type = static_cast<ParameterType>(i / 2);

		switch (param_type) {
			case ParameterType::INT:
				m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::INT>>());
				break;
			case ParameterType::BOOL:
				m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::BOOL>>());
				break;
			case ParameterType::FLOAT:
				m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::FLOAT>>());
				break;
			case ParameterType::STRING:
				m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::STRING>>());
				break;
			case ParameterType::VEC3:
				m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::VEC3>>());
				break;
			case ParameterType::UDF:
				m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::UDF>>());
				break;
		}

		m_command_parameters[section_number][0]->index = 0;
		m_command_parameters[section_number][0]->name = "Null Parameter";

		while (streamTell(stream) < end_pos) {
			createAndLoadCommandParameter(stream, section_number);
		}

	}

}

ParameterHandler::TableParameter* ParameterHandler::getParameterFromTable(int table_num, int parameter_num)
{
	try {
		return &m_table_parameters.at(table_num).at(parameter_num);
	}
	catch (out_of_range) {
		cerr << "Tried getting parameter index " << to_string(parameter_num) << " from table " << to_string(table_num) << endl;
		throw std::invalid_argument("Invalid parameter index");
	}
}

vector<ParameterHandler::TableParameter>* ParameterHandler::getTableParameters(int table_num)
{
	return &m_table_parameters[table_num];
}

vector<unique_ptr<ParameterHandler::CommandParameterBase>>* ParameterHandler::getCommandParameters(int section_num)
{
	return &m_command_parameters[section_num];
}

void ParameterHandler::createAndLoadCommandParameter(LPSTREAM stream, int section_number)
{
	bool is_input = section_number % 2 == 0;
	ParameterType param_type = static_cast<ParameterType>(section_number / 2);

	// index of the new parameter (haven't added it yet so we don't -1 from the size)
	int index = m_command_parameters[section_number].size();
	
	switch (param_type) {
		case ParameterType::INT:
			m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::INT>>());
			break;
		case ParameterType::BOOL:
			m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::BOOL>>());
			break;
		case ParameterType::FLOAT:
			m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::FLOAT>>());
			break;
		case ParameterType::STRING:
			m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::STRING>>());
			break;
		case ParameterType::VEC3:
			m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::VEC3>>());
			break;
		case ParameterType::UDF:
			m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::UDF>>());
			break;
	}

	m_command_parameters[section_number][index]->index = index;
	m_command_parameters[section_number][index]->load(stream, m_string_list, is_input);
	return;
}

map<int, int> ParameterHandler::table_entry_lengths = {
	{0, 0x0c},
	{1, 0x0c},
	{2, 0x0c},
	{3, 0x0c},
	{4, 0x14}
};

map<int, int> ParameterHandler::parameter_entry_lengths = {
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
	{11, 0x8}
};

void ParameterHandler::TableParameter::load(LPSTREAM stream, StringList* string_list, int section_num)
{
	TableParameter::address = streamTell(stream);
	int end_pos = address + table_entry_lengths[section_num];
	int name_offset;
	readIntFromStream(stream, name_offset);
	TableParameter::name = string_list->getStringFromOffset(name_offset);

	streamSeek(stream, 4, CURRENT);

	readIntFromStream(stream, TableParameter::value);

	streamSeek(stream, end_pos, START);
}

template <ParameterHandler::ParameterType T>
void ParameterHandler::CommandParameter<T>::load(LPSTREAM stream, StringList* string_list, bool is_input)
{
	address = streamTell(stream);
	int section_num = is_input ? T * 2 : T * 2 + 1;
	int end_pos = address + ParameterHandler::parameter_entry_lengths[section_num];

	int name_offset;
	read2ByteIntFromStream(stream, name_offset);
	name = string_list->getStringFromOffset(name_offset);
	// 0x02

	streamSeek(stream, 2, CURRENT);
	// 0x04

	if (streamTell(stream) >= end_pos)
	{
		return;
	}

	//if(T == UDF)
	//{
	//	int second_string_tag;
	//	readIntFromStream(stream, second_string_tag);
	//	//type_name = string_list->getStringFromOffset(second_string_tag);
	//	// 0x08

	//	readIntFromStream(stream, command_ref);
	//	// 0x0c

	//}
	//else {
		read2ByteIntFromStream(stream, command_ref);
		streamSeek(stream, 2, CURRENT);
	//}

	streamSeek(stream, end_pos, START);
	return;
}

void ParameterHandler::CommandParameter<ParameterHandler::ParameterType::UDF>::load(LPSTREAM stream, StringList* string_list, bool is_input)
{
	address = streamTell(stream);
	int section_num = is_input ? UDF * 2 : UDF * 2 + 1;
	int end_pos = address + ParameterHandler::parameter_entry_lengths[section_num];

	int name_offset;
	read2ByteIntFromStream(stream, name_offset);
	name = string_list->getStringFromOffset(name_offset);
	// 0x02

	streamSeek(stream, 2, CURRENT);
	// 0x04

	int type_name_tag;
	read2ByteIntFromStream(stream, type_name_tag);
	type_name = string_list->getStringFromOffset(type_name_tag);
	// 0x06

	streamSeek(stream, 2, CURRENT);
	// 0x08

	if (streamTell(stream) >= end_pos)
	{
		return;
	}

	readIntFromStream(stream, command_ref);
	// 0x0c

	// ensure that the stream is at the end of the current parameter before continuing
	streamSeek(stream, end_pos, START);
	return;
}
