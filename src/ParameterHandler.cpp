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

void ParameterHandler::loadInternalParameters(fstream& stream, int end_address)
{
	vector<int> table_section_addresses;
	vector<int> section_nums;

	int index = 0;
	int current_pos = stream.tellg();
	for (int i = 0; i < 6; i++) {
		stream.seekg(current_pos, ios::beg);
		stream.seekg(4 * i, ios::cur);

		int entry_address;
		readIntFromStream(stream, 4, entry_address);

		if (entry_address == end_address) {
			break;
		}

		if (i > 0) {
			if (entry_address == table_section_addresses[index - 1]) {
				table_section_addresses.pop_back();
				section_nums.pop_back();
				index -= 1;
			}
		}
		table_section_addresses.push_back(entry_address);
		section_nums.push_back(i);
		index += 1;
	}

	for (int i = 0; i < table_section_addresses.size(); i++) {
		int current_pos = table_section_addresses[i];
		int section_number = section_nums[i];
		stream.seekg(current_pos, ios::beg);

		int end_pos;
		if (i + 1 < table_section_addresses.size()) {
			end_pos = table_section_addresses[i + 1];
		}
		else {
			end_pos = end_address;
		}

		m_active_internal_parameter_types.push_back(section_number);

		ParameterType param_type = static_cast<ParameterType>(i);

		// create null parameter at index 0
		switch (param_type) {
		case ParameterType::INT:
			m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::INT>>());
			break;
		case ParameterType::BOOL:
			m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::BOOL>>());
			break;
		case ParameterType::FLOAT:
			m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::FLOAT>>());
			break;
		case ParameterType::STRING:
			m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::STRING>>());
			break;
		case ParameterType::VEC3:
			m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::VEC3>>());
			break;
		case ParameterType::UDT:
			m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::UDT>>());
			break;
		}

		m_internal_parameters[section_number][0]->index = 0;
		m_internal_parameters[section_number][0]->name = "Null Parameter";

		while (stream.tellg() < end_pos) {
			createAndLoadInternalParameter(stream, section_number);
		}
	}
}

void ParameterHandler::loadCommandParameters(fstream& stream, int end_address)
{
	vector<int> section_addresses;
	map<int, int> address_to_section_number;

	// load section addresses
	int last_vector_index = 0;
	for (int i = 0; i < 12; i++) {
		int entry_address;
		readIntFromStream(stream, 4, entry_address);
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

		stream.seekg(section_address, ios::beg);

		int end_pos;
		if (i + 1 < section_addresses.size()) {
			end_pos = section_addresses[i + 1];
		}
		else {
			end_pos = end_address;
		}

		m_active_command_parameter_types.push_back(section_number);

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
		case ParameterType::UDT:
			m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::UDT>>());
			break;
		}

		m_command_parameters[section_number][0]->index = 0;
		m_command_parameters[section_number][0]->name = "Null Parameter";

		while (stream.tellg() < end_pos) {
			createAndLoadCommandParameter(stream, section_number);
		}
	}
}

ParameterHandler::InternalParameterBase* ParameterHandler::getInternalParameter(int section_num, int parameter_num)
{
	try {
		InternalParameterBase* param = m_internal_parameters.at(section_num).at(parameter_num).get();
		return param;
		//return &(InternalParameterBase)m_internal_parameters.at(section_num).at(parameter_num);
	}
	catch (out_of_range) {
		cerr << "Tried getting internal parameter index " << to_string(parameter_num) << " from section" << to_string(section_num) << endl;
		throw std::invalid_argument("Invalid parameter index");
	}
}

ParameterHandler::CommandParameterBase* ParameterHandler::getCommandParameter(int section_num, int parameter_num)
{
	try {
		CommandParameterBase* param = m_command_parameters.at(section_num).at(parameter_num).get();
		return param;
	}
	catch (out_of_range) {
		cerr << "Tried getting command parameter index " << to_string(parameter_num) << " from section" << to_string(section_num) << endl;
		throw std::invalid_argument("Invalid parameter index");
	}
}

// get a vector of all the parameters in a section
vector<unique_ptr<ParameterHandler::InternalParameterBase>>* ParameterHandler::getInternalParameters(int section_number)
{
	return &m_internal_parameters[section_number];
}

vector<unique_ptr<ParameterHandler::CommandParameterBase>>* ParameterHandler::getCommandParameters(int section_num)
{
	return &m_command_parameters[section_num];
}

void ParameterHandler::createAndLoadInternalParameter(fstream& stream, int section_number)
{
	ParameterType param_type = static_cast<ParameterType>(section_number);

	// index of the new parameter (haven't added it yet so we don't -1 from the size)
	int index = m_internal_parameters[section_number].size();

	switch (param_type) {
	case ParameterType::INT:
		m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::INT>>());
		break;
	case ParameterType::BOOL:
		m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::BOOL>>());
		break;
	case ParameterType::FLOAT:
		m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::FLOAT>>());
		break;
	case ParameterType::STRING:
		m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::STRING>>());
		break;
	case ParameterType::VEC3:
		m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::VEC3>>());
		break;
	case ParameterType::UDT:
		m_internal_parameters[section_number].push_back(make_unique<InternalParameter<ParameterType::UDT>>());
		break;
	}
	m_internal_parameters[section_number][index]->index = index;
	m_internal_parameters[section_number][index]->load(stream, m_string_list);
}

void ParameterHandler::createAndLoadCommandParameter(fstream& stream, int section_number)
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
	case ParameterType::UDT:
		m_command_parameters[section_number].push_back(make_unique<CommandParameter<ParameterType::UDT>>());
		break;
	}
	m_command_parameters[section_number][index]->index = index;
	m_command_parameters[section_number][index]->load(stream, m_string_list, is_input);
}

void ParameterHandler::writeInternalParametersToStream(fstream& stream)
{
	// writing the internal parameters
	vector<int> section_offsets;

	int start_pos = stream.tellg();

	stream.seekg(24, ios::cur); // skip over the section header for now (we'll come back to it later)

	for (int i = 0; i < m_active_internal_parameter_types.size(); i++)
	{
		section_offsets.push_back(stream.tellg());
		int section_num = m_active_internal_parameter_types.at(i);
		ParameterHandler::ParameterType param_type = static_cast<ParameterHandler::ParameterType>(section_num);

		vector<unique_ptr<ParameterHandler::InternalParameterBase>>* section = &m_internal_parameters[section_num];
		for (int j = 1; j < section->size(); j++) // start at one to avoid writing null parameter
		{
			InternalParameterBase* param = getInternalParameter(section_num, j);
			param->write(stream, m_string_list);
		}
	}
	int end_pos = stream.tellg();

	// write the section header
	stream.seekg(start_pos, ios::beg);

	for (int i = 0; i < 6; i++)
	{
		if (section_offsets.size() == 0) {
			stream.write(convertIntToCharArray(end_pos, 4), 4);
			continue;
		}
		stream.write(convertIntToCharArray(section_offsets.at(0), 4), 4);
		if (count(m_active_internal_parameter_types.begin(), m_active_internal_parameter_types.end(), i)) {
			//section_address[i] = section_offsets.begin();
			section_offsets.erase(section_offsets.begin());
		}
	}
}

void ParameterHandler::writeCommandParametersToStream(fstream& stream)
{
	// writing the internal parameters
	vector<int> section_offsets;

	int start_pos = stream.tellg();

	stream.seekg(4*12, ios::cur); // skip over the section header for now (we'll come back to it later)

	for (int i = 0; i < m_active_command_parameter_types.size(); i++)
	{
		section_offsets.push_back(stream.tellg());
		int section_num = m_active_command_parameter_types.at(i);
		ParameterHandler::ParameterType param_type = static_cast<ParameterHandler::ParameterType>(section_num/2);

		vector<unique_ptr<ParameterHandler::CommandParameterBase>>* section = &m_command_parameters[section_num];
		for (int j = 1; j < section->size(); j++) // start at one to avoid writing null parameter
		{
			CommandParameterBase* param = getCommandParameter(section_num, j);
			param->write(stream, m_string_list, section_num%2 == 0);
		}
	}
	int end_pos = stream.tellg();

	// write the section header
	stream.seekg(start_pos, ios::beg);

	for (int i = 0; i < 12; i++)
	{
		if (section_offsets.size() == 0) {
			stream.write(convertIntToCharArray(end_pos, 4), 4);
			continue;
		}
		stream.write(convertIntToCharArray(section_offsets.at(0), 4), 4);
		if (count(m_active_command_parameter_types.begin(), m_active_command_parameter_types.end(), i)) {
			section_offsets.erase(section_offsets.begin());
		}
	}



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

template <ParameterHandler::ParameterType T>
void ParameterHandler::InternalParameter<T>::load(fstream& stream, StringList* string_list)
{
	InternalParameter::address = stream.tellg();
	int section_num = T;
	int end_pos = address + table_entry_lengths[section_num];
	int name_offset;
	readIntFromStream(stream, 4, name_offset);
	InternalParameter::name = string_list->getStringFromOffset(name_offset);

	stream.seekg(4, ios::cur);

	if (type == ParameterType::STRING)
	{
		int string_offset;
		readIntFromStream(stream, 4, string_offset);
		InternalParameter::value = string_list->getStringFromOffset(string_offset);
	}
	else {
		int val;
		readIntFromStream(stream, 4, val);
		InternalParameter::value = to_string(val);
	}

	stream.seekg(end_pos, ios::beg);
}

template <ParameterHandler::ParameterType Type>
void ParameterHandler::InternalParameter<Type>::write(fstream& stream, StringList* string_list)
{
	int name_offset = string_list->getOffsetOfString(name);

	stream.write(convertIntToCharArray(name_offset, 4), 4);

	stream.seekg(4, ios::cur);

	if (type == ParameterType::STRING)
	{
		int string_offset = string_list->getOffsetOfString(value);
		stream.write(convertIntToCharArray(string_offset, 4), 4);
	}
	else {
		int val = stoi(value);
		stream.write(convertIntToCharArray(val, 4), 4);
	}

}

template <ParameterHandler::ParameterType T>
void ParameterHandler::CommandParameter<T>::load(fstream& stream, StringList* string_list, bool is_input)
{
	address = stream.tellg();
	int section_num = is_input ? T * 2 : T * 2 + 1;
	int end_pos = address + ParameterHandler::parameter_entry_lengths[section_num];

	int name_offset;
	readIntFromStream(stream, 2, name_offset);
	name = string_list->getStringFromOffset(name_offset);
	// 0x02

	stream.seekg(2, ios::cur);
	// 0x04

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	// read tag
	readIntFromStream(stream, 2, command_ref);
	stream.seekg(2, ios::cur);

	stream.seekg(end_pos, ios::beg);
	return;
}

void ParameterHandler::CommandParameter<ParameterHandler::ParameterType::UDT>::load(fstream& stream, StringList* string_list, bool is_input)
{
	address = stream.tellg();
	int section_num = is_input ? UDT * 2 : UDT * 2 + 1;
	int end_pos = address + ParameterHandler::parameter_entry_lengths[section_num];

	int name_offset;
	readIntFromStream(stream, 2, name_offset);
	name = string_list->getStringFromOffset(name_offset);
	// 0x02

	stream.seekg(2, ios::cur);
	// 0x04

	int type_name_tag;
	readIntFromStream(stream, 2, type_name_tag);
	type_name = string_list->getStringFromOffset(type_name_tag);
	// 0x06

	stream.seekg(2, ios::cur);
	// 0x08

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	readIntFromStream(stream, 4, command_ref);
	// 0x0c

	// ensure that the stream is at the end of the current parameter before continuing
	stream.seekg(end_pos, ios::beg);
	return;
}

template <ParameterHandler::ParameterType T>
void ParameterHandler::CommandParameter<T>::write(fstream& stream, StringList* string_list, bool is_input)
{
	int section_num = is_input ? T * 2 : T * 2 + 1;
	int end_pos = (int)stream.tellg() + ParameterHandler::parameter_entry_lengths[section_num];

	int name_offset = string_list->getOffsetOfString(name);
	stream.write(convertIntToCharArray(name_offset, 2), 2);

	stream.seekg(2, ios::cur);
	// 0x04

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	stream.write(convertIntToCharArray(command_ref, 2), 2);
	stream.seekg(2, ios::cur);

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	stream.seekg(end_pos, ios::beg);
}

void ParameterHandler::CommandParameter<ParameterHandler::ParameterType::UDT>::write(fstream& stream, StringList* string_list, bool is_input)
{
	int section_num = is_input ? UDT * 2 : UDT * 2 + 1;
	int end_pos = (int)stream.tellg() + ParameterHandler::parameter_entry_lengths[section_num];

	int name_offset = string_list->getOffsetOfString(name);
	stream.write(convertIntToCharArray(name_offset, 2), 2);

	stream.seekg(2, ios::cur);
	// 0x04

	int type_name_offset = string_list->getOffsetOfString(type_name);
	stream.write(convertIntToCharArray(type_name_offset, 2), 2);

	stream.seekg(2, ios::cur);
	// 0x08

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	stream.write(convertIntToCharArray(command_ref, 4), 4);

	stream.seekg(end_pos, ios::beg);
}
