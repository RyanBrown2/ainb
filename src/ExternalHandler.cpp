#include "ainb/ExternalHandler.h"

using namespace std;
using namespace ainb;

ExternalHandler::ExternalHandler(StringList* string_list)
{
	m_string_list = string_list;
}

ExternalHandler::~ExternalHandler()
{

}

void ExternalHandler::loadFromStream(fstream& stream)
{
	// first load header
	vector<HeaderEntry> header_entries;

	for (int i = 0; i < 6; i++)
	{
		HeaderEntry entry;
		readIntFromStream(stream, 2, entry.entry_count);
		readIntFromStream(stream, 2, entry.start_index);
		readIntFromStream(stream, 4, entry.allocation_offset);
		header_entries.push_back(entry);
	}

	// check if the file has data in this section
	// if not, return
	if (header_entries[5].start_index == 0)
	{
		return;
	}

	// now load data
	for (int i = 0; i < 6; i++)
	{
		vector<unique_ptr<ExternalParameterBase>> parameters;

		HeaderEntry entry = header_entries[i];
		for (int j = 0; j < entry.entry_count; j++)
		{
			int index = entry.start_index + j;
			createAndLoadParameter(stream, index, (ParameterType) i);
		}
	}

	// move to end of allocated space
	int alloc_size = header_entries[5].allocation_offset + header_entries[5].entry_count * ExternalParamAllocSizes.at(5);
	stream.seekg(alloc_size, ios::cur);

	// load external files
	// todo: currently just determines the count, doesn't store data for each reference
	int entry_terminator = -1;
	while (entry_terminator != 0x0b68a737)
	{
		stream.seekg(0x0c, ios::cur);
		m_external_file_count += 1;
		readIntFromStream(stream, 4, entry_terminator);
	}
}

void ExternalHandler::writeToStream(fstream& stream)
{
	int start_pos = stream.tellg();

	int index = 0;
	int alloc_size = 0;

	vector<HeaderEntry> header_entries;

	// skip the header for now
	stream.seekg(0x30, ios::cur);

	for (int i = 0; i < m_parameters.size(); i++)
	{
		HeaderEntry entry;
		entry.entry_count = m_parameters[i].size();
		entry.start_index = index;
		entry.allocation_offset = alloc_size;

		int alloc_entry_size = ExternalParamAllocSizes.at(i); // how much space does each entry take up
		for (int j = 0; j < m_parameters[i].size(); j++)
		{
			m_parameters[i][j]->write(stream, m_string_list);
			index += 1;
			alloc_size += alloc_entry_size;
		}
		header_entries.push_back(entry);
	};

	int end_pos = stream.tellg();

	// write header
	stream.seekg(start_pos, ios::beg);

	for (int i = 0; i < header_entries.size(); i++)
	{
		HeaderEntry entry = header_entries[i];
		writeIntToStream(stream, 2, entry.entry_count);
		writeIntToStream(stream, 2, entry.start_index);
		writeIntToStream(stream, 4, entry.allocation_offset);
	}

	stream.seekg(end_pos + alloc_size, ios::beg);

	// todo: write external files
	for (int i = 0; i < m_external_file_count; i++)
	{
		writeIntToStream(stream, 0x10, -1);
	}
}


void ExternalHandler::createAndLoadParameter(fstream& stream, int index, ParameterType type)
{

	switch (type) {
	case ParameterType::INT:
		m_parameters[ParameterType::INT].push_back(make_unique<ExternalParameter<ParameterType::INT>>());
		break;
	case ParameterType::BOOL:
		m_parameters[ParameterType::BOOL].push_back(make_unique<ExternalParameter<ParameterType::BOOL>>());
		break;
	case ParameterType::FLOAT:
		m_parameters[ParameterType::FLOAT].push_back(make_unique<ExternalParameter<ParameterType::FLOAT>>());
		break;
	case ParameterType::STRING:
		m_parameters[ParameterType::STRING].push_back(make_unique<ExternalParameter<ParameterType::STRING>>());
		break;
	case ParameterType::VEC3:
		m_parameters[ParameterType::VEC3].push_back(make_unique<ExternalParameter<ParameterType::VEC3>>());
		break;
	case ParameterType::UDT:
		m_parameters[ParameterType::UDT].push_back(make_unique<ExternalParameter<ParameterType::UDT>>());
		break;
	}
	
	m_parameters[type].back()->setIndex(index);
	m_parameters[type].back()->load(stream, m_string_list);
}

int ExternalHandler::getSize()
{
	int total_size = 0x30; // header size

	// entries + allocation space
	//for (int i = 0; i < 6; i++)
	//{
	//	vector<unique_ptr<ExternalParameterBase>>& parameters = m_parameters.at(i);
	//	int entry_count = parameters.size();
	//	int size = entry_count * (0x8 + ExternalParamAllocSizes.at(i));
	//	total_size += size;
	//}
	for (auto& pair : m_parameters)
	{
		int type_num = pair.first;
		vector<unique_ptr<ExternalParameterBase>>& parameters = pair.second;
		int entry_count = parameters.size();
		int size = entry_count * (0x8 + ExternalParamAllocSizes.at(type_num));
		total_size += size;
	}

	// external files
	total_size += m_external_file_count * 0x10;

	return total_size;
}