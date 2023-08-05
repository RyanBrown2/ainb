#include "ainb/SequenceNode.h"

#include <cstring>

using namespace std;
using namespace ainb;

SequenceNode::SequenceNode()
{
	//m_guid = new char[17];
	m_body_pos = -1;
	m_name = "";
}

SequenceNode::~SequenceNode()
{

}

void SequenceNode::writeHeadToStream(fstream& stream, StringList* string_list)
{
	// todo unknown 1
	stream.seekg(0x2, ios::cur);
	// 0x2

	// index
	writeIntToStream(stream, 4, m_index);
	// 0x6

	// todo unknown 2
	stream.seekg(0x2, ios::cur);
	// 0x8

	// string offset
	int string_offset = string_list->getOffsetOfString(m_name);
	writeIntToStream(stream, 4, string_offset);
	// 0xC

	// todo uid
	stream.seekg(0x4, ios::cur);
	// 0x10

	stream.seekg(0x4, ios::cur);
	// 0x14

	// command body address
	writeIntToStream(stream, 4, m_body_pos);
	// 0x18

	stream.seekg(0x14, ios::cur);
	// 0x2C

	// guid
	stream.write(m_guid, 16);
	// 0x3C
}

void SequenceNode::writeBodyToStream(fstream& stream, StringList* string_list)
{
	int body_start_pos = stream.tellg();
	m_body_pos = body_start_pos;

	for (auto& param_pair : m_internal_parameters)
	{
		int section_num = param_pair.first;
		stream.seekg(body_start_pos + (8 * section_num), ios::beg);
		int param_index = param_pair.second->index;
		stream.write(convertIntToCharArray(param_index, 4), 4);
		stream.write(convertIntToCharArray(m_internal_parameter_values.at(param_pair.second), 4), 4);

	}

	int command_param_start_pos = body_start_pos + (6 * 0x8);

	for (auto& param_pair : m_command_parameters)
	{
		int section_num = param_pair.first;
		stream.seekg(command_param_start_pos + (8 * section_num), ios::beg);
		int param_index = param_pair.second->index;
		stream.write(convertIntToCharArray(param_index, 4), 4);
		stream.write(convertIntToCharArray(m_command_parameter_values.at(param_pair.second), 4), 4);

	}

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
		string entry_string = m_call_table.at(i).entry_string;
		int string_offset = string_list->getOffsetOfString(entry_string);

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
}



vector<SequenceNode*> SequenceNode::getCallers()
{
	return m_callers;
}

void SequenceNode::addCaller(SequenceNode* caller)
{
	m_callers.push_back(caller);
}

void SequenceNode::removeCaller(SequenceNode* caller)
{
	m_callers.erase(remove(m_callers.begin(), m_callers.end(), caller), m_callers.end());
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
	memcpy(m_guid, guid, 16);
	m_guid[16] = '\0';
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
	entry.entry_string = param;
	m_call_table.push_back(entry);
}

bool ainb::SequenceNode::_equals(SequenceNode const& other) const
{
	if (this->m_guid != other.m_guid) {
		return false;
	}

	return true;
}
