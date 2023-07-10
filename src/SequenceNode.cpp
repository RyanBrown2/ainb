#include "ainb/SequenceNode.h"

using namespace std;
using namespace ainb;

SequenceNode::SequenceNode()
{
	m_guid = nullptr;
	m_body_pos = -1;
	m_name = "";
}

SequenceNode::~SequenceNode()
{

}

void SequenceNode::writeHeadToStream(fstream& stream, int index)
{

}

void SequenceNode::writeBodyToStream(fstream& stream, StringList* string_list)
{
	//for (auto& entry : m_internal_parameters) {
	//	entry.f
	//}
	int body_start_pos = stream.tellg();

	for (auto& param_pair : m_internal_parameters)
	{
		int section_num = param_pair.first;
		stream.seekg(body_start_pos + (8 * section_num), ios::beg);
		int param_index = param_pair.second->index;
		stream.write(convertIntToCharArray(param_index, 4), 4);
		stream.write(convertIntToCharArray(m_internal_parameter_values.at(param_pair.second), 4), 4);

	}

	//for (int i = 0; i < 6; i++)
	//{
	//	if (m_internal_parameters[i] != nullptr) {
	//		int param_index = m_internal_parameters.at(i)->index;
	//		stream.write(convertIntToCharArray(param_index ,4), 4);
	//		stream.write(convertIntToCharArray(m_internal_parameter_values.at(m_internal_parameters.at(i)), 4), 4);
	//	}
	//	else {
	//		stream.seekg(8, ios::cur);
	//	}
	//}

	int command_param_start_pos = body_start_pos + (6 * 0x8);

	for (auto& param_pair : m_command_parameters)
	{
		int section_num = param_pair.first;
		stream.seekg(command_param_start_pos + (8 * section_num), ios::beg);
		int param_index = param_pair.second->index;
		stream.write(convertIntToCharArray(param_index, 4), 4);
		stream.write(convertIntToCharArray(m_command_parameter_values.at(param_pair.second), 4), 4);

	}

	//for (int i = 0; i < 12; i++)
	//{
	//	if (m_command_parameters[i] != nullptr) {
	//		int param_index = m_command_parameters.at(i)->index;
	//		stream.write(convertIntToCharArray(param_index, 4), 4);
	//		stream.write(convertIntToCharArray(m_command_parameter_values.at(m_command_parameters.at(i)), 4), 4);
	//	}
	//	else {
	//		stream.seekg(8, ios::cur);
	//	}
	//}

	int call_table_header_start_pos = command_param_start_pos + (12 * 0x8);
	stream.seekg(call_table_header_start_pos, ios::beg);

	// todo: figure out call table header
	stream.seekg(0x14, ios::cur);

	int call_table_size = m_call_table.size();

	if (call_table_size == 0) {
		return;
	}

	int call_table_address_start_pos = stream.tellg();

	// skip the table address, will write afterwards
	stream.seekg(4 * call_table_size, ios::cur);

	vector<int> call_table_addresses;
	// write call table entries
	for (int i = 0; i < m_call_table.size(); i++)
	{
		call_table_addresses.push_back(stream.tellg());

		int callee_index = m_call_table.at(i).callee->getIndex();
		string call_string = m_call_table.at(i).call_string;
		int string_offset = string_list->getOffsetOfString(call_string);

		stream.write(convertIntToCharArray(callee_index, 4), 4);
		stream.write(convertIntToCharArray(string_offset, 4), 4);
	}

	// write call table addresses
	stream.seekg(call_table_address_start_pos, ios::beg);

	for (int i = 0; i < call_table_addresses.size(); i++)
	{
		stream.write(convertIntToCharArray(call_table_addresses.at(i), 4), 4);
	}

	stream.seekg(call_table_addresses.back() + 0x8, ios::beg);

	//stream.flush();
}

int SequenceNode::getIndex()
{
	return m_index;
}

void SequenceNode::setIndex(int index)
{
	m_index = index;
}

string SequenceNode::getName()
{
	return m_name;
}

void SequenceNode::setName(string name)
{
	m_name = name;
}

char* SequenceNode::getGUID()
{
	return m_guid;
}

void SequenceNode::setGUID(char* guid)
{
	m_guid = guid;
}

int SequenceNode::getBodyPos()
{
	return m_body_pos;
}

void SequenceNode::setBodyPos(int pos)
{
	m_body_pos = pos;
}

void SequenceNode::setInternalParameter(InternalParameterBase* parameter, int value, int position)
{
	m_internal_parameters[position] = parameter;
	m_internal_parameter_values[parameter] = value;
}

void SequenceNode::setCommandParameter(CommandParameterBase* parameter, int value, int position)
{
	m_command_parameters[position] = parameter;
	m_command_parameter_values[parameter] = value;
}

void SequenceNode::addCall(SequenceNode* node, string param)
{
	CallTableEntry entry;
	entry.callee = node;
	entry.call_string = param;
	m_call_table.push_back(entry);
}


