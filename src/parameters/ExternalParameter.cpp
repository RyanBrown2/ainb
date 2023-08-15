#include "ainb/parameters/ExternalParameter.h"

using namespace std;
using namespace ainb;

template <>
ExternalParameter<ParameterType::INT>::ExternalParameter()
{
	m_type_num = ParameterType::INT;
	m_alloc_size = ExternalParamAllocSizes.at(ParameterType::INT);
}

template <>
ExternalParameter<ParameterType::BOOL>::ExternalParameter()
{
	m_type_num = ParameterType::BOOL;
	m_alloc_size = ExternalParamAllocSizes.at(ParameterType::BOOL);
}

template <>
ExternalParameter<ParameterType::FLOAT>::ExternalParameter()
{
	m_type_num = ParameterType::FLOAT;
	m_alloc_size = ExternalParamAllocSizes.at(ParameterType::FLOAT);
}

template <>
ExternalParameter<ParameterType::STRING>::ExternalParameter()
{
	m_type_num = ParameterType::STRING;
	m_alloc_size = ExternalParamAllocSizes.at(ParameterType::STRING);
}

template <>
ExternalParameter<ParameterType::VEC3>::ExternalParameter()
{
	m_type_num = ParameterType::VEC3;
	m_alloc_size = ExternalParamAllocSizes.at(ParameterType::VEC3);
}

template <>
ExternalParameter<ParameterType::UDT>::ExternalParameter()
{
	m_type_num = ParameterType::UDT;
	m_alloc_size = ExternalParamAllocSizes.at(ParameterType::UDT);
}

template <ParameterType T>
ExternalParameter<T>::~ExternalParameter()
{

}

template <ParameterType T>
void ExternalParameter<T>::load(fstream& stream, StringList* string_list)
{
	// todo
	int name_offset;
	readIntFromStream(stream, 2, name_offset);

	// todo - figure out what the 80 represents
	stream.seekg(2, ios::cur);

	int second_string_offset;
	readIntFromStream(stream, 4, second_string_offset);

	m_name = string_list->getStringFromOffset(name_offset);
	m_second_string = string_list->getStringFromOffset(second_string_offset);
}

template <ParameterType T>
void ExternalParameter<T>::write(fstream& stream, StringList* string_list)
{
	// todo
}
