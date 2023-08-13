#include "ainb/parameters/InternalParameter.h"

using namespace std;
using namespace ainb;

template <>
InternalParameter<ParameterType::INT>::InternalParameter() : InternalParameterBase()
{

}

template <>
InternalParameter<ParameterType::BOOL>::InternalParameter() : InternalParameterBase()
{

}

template <>
InternalParameter<ParameterType::FLOAT>::InternalParameter() : InternalParameterBase()
{

}

template <>
InternalParameter<ParameterType::STRING>::InternalParameter() : InternalParameterBase()
{

}

template <>
InternalParameter<ParameterType::VEC3>::InternalParameter() : InternalParameterBase()
{

}

template <>
InternalParameter<ParameterType::UDT>::InternalParameter() : InternalParameterBase()
{

}

template <ParameterType T>
InternalParameter<T>::~InternalParameter()
{

}

template <ParameterType T>
void InternalParameter<T>::load(fstream& stream, StringList* string_list)
{
	InternalParameter::m_address = stream.tellg();
	int section_num = T;
	int end_pos = m_address + InternalParamEntryLengths.at(section_num);
	int name_offset;
	readIntFromStream(stream, 4, name_offset);
	InternalParameter::m_name = string_list->getStringFromOffset(name_offset);

	InternalParameter::m_type_num = T;
	stream.seekg(4, ios::cur);

	if (T == ParameterType::STRING)
	{
		int string_offset;
		readIntFromStream(stream, 4, string_offset);
		InternalParameter::m_value = string_list->getStringFromOffset(string_offset);
	}
	else {
		int val;
		readIntFromStream(stream, 4, val);
		InternalParameter::m_value = to_string(val);
	}

	stream.seekg(end_pos, ios::beg);
}

template <ParameterType T>
void InternalParameter<T>::write(fstream& stream, StringList* string_list)
{
	int name_offset = string_list->getOffsetOfString(m_name);

	writeIntToStream(stream, 4, name_offset);

	stream.seekg(4, ios::cur);

	if (T == ParameterType::STRING)
	{
		int string_offset = string_list->getOffsetOfString(m_value);
		writeIntToStream(stream, 4, string_offset);
	}
	else {
		int val = stoi(m_value);
		writeIntToStream(stream, 4, val);
	}

}
