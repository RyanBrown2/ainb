#include "ainb/parameters/CommandParameter.h"

using namespace std;
using namespace ainb;

CommandParameter<ParameterType::INT>::CommandParameter()
{

}

CommandParameter<ParameterType::BOOL>::CommandParameter()
{

}

CommandParameter<ParameterType::FLOAT>::CommandParameter()
{

}

CommandParameter<ParameterType::STRING>::CommandParameter()
{

}

CommandParameter<ParameterType::VEC3>::CommandParameter()
{

}

CommandParameter<ParameterType::UDT>::CommandParameter()
{

}


template <ParameterType T>
CommandParameter<T>::~CommandParameter()
{

}

template <ParameterType T>
void CommandParameter<T>::load(fstream& stream, StringList* string_list, bool is_input)
{
	m_address = stream.tellg();
	int section_num = is_input ? T * 2 : T * 2 + 1;
	int end_pos = m_address + CommandParamEntryLengths.at(section_num);

	m_type_num = section_num;

	int name_offset;
	readIntFromStream(stream, 2, name_offset);
	m_name = string_list->getStringFromOffset(name_offset);
	// 0x02

	stream.seekg(2, ios::cur);
	// 0x04

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	// read tag
	readIntFromStream(stream, 2, m_command_ref);
	stream.seekg(2, ios::cur);

	stream.seekg(end_pos, ios::beg);
	return;
}

void CommandParameter<ParameterType::UDT>::load(fstream& stream, StringList* string_list, bool is_input)
{
	m_address = stream.tellg();
	int section_num = is_input ? UDT * 2 : UDT * 2 + 1;
	int end_pos = m_address + CommandParamEntryLengths.at(section_num);

	m_type_num = section_num;

	int name_offset;
	readIntFromStream(stream, 2, name_offset);
	m_name = string_list->getStringFromOffset(name_offset);
	// 0x02

	stream.seekg(2, ios::cur);
	// 0x04

	int type_name_tag;
	readIntFromStream(stream, 2, type_name_tag);
	m_type_name = string_list->getStringFromOffset(type_name_tag);
	// 0x06

	stream.seekg(2, ios::cur);
	// 0x08

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	readIntFromStream(stream, 4, m_command_ref);
	// 0x0c

	// ensure that the stream is at the end of the current parameter before continuing
	stream.seekg(end_pos, ios::beg);
	return;
}

template <ParameterType T>
void CommandParameter<T>::write(fstream& stream, StringList* string_list, bool is_input)
{
	int section_num = is_input ? T * 2 : T * 2 + 1;
	int end_pos = (int)stream.tellg() + CommandParamEntryLengths.at(section_num);

	int name_offset = string_list->getOffsetOfString(m_name);
	writeIntToStream(stream, 2, name_offset);

	stream.seekg(2, ios::cur);
	// 0x04

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	writeIntToStream(stream, 2, m_command_ref);
	stream.seekg(2, ios::cur);

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	stream.seekg(end_pos, ios::beg);
}

void CommandParameter<ParameterType::UDT>::write(fstream& stream, StringList* string_list, bool is_input)
{
	int section_num = is_input ? UDT * 2 : UDT * 2 + 1;
	int end_pos = (int)stream.tellg() + CommandParamEntryLengths.at(section_num);

	int name_offset = string_list->getOffsetOfString(m_name);
	writeIntToStream(stream, 2, name_offset);

	stream.seekg(2, ios::cur);
	// 0x04

	int type_name_offset = string_list->getOffsetOfString(m_type_name);
	writeIntToStream(stream, 2, type_name_offset);

	stream.seekg(2, ios::cur);
	// 0x08

	if (stream.tellg() >= end_pos)
	{
		return;
	}

	writeIntToStream(stream, 4, m_command_ref);
	// 0x0c

	stream.seekg(end_pos, ios::beg);
}

template <ParameterType T>
map<string, string>* CommandParameter<T>::getExtras()
{
	map<string, string>* extras = new map<string, string>();
	// todo
	return extras;
}

map<string, string>* CommandParameter<ParameterType::UDT>::getExtras()
{
	map<string, string>* extras = new map<string, string>();
	extras->insert(pair<string, string>("type_name", m_type_name));
	return extras;
}
