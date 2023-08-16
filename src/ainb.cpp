#include "ainb/ainb.h"

using namespace std;

using namespace ainb;

AINB::AINB(fstream& stream) : m_stream(stream)
{
	parseHeader();

	m_string_list = new StringList(&m_header_data.string_list_start_pos);
	m_string_list->loadFromStream(m_stream);

	m_parameter_handler = new ParameterHandler(m_string_list);
	parseParameters();

	stream.seekg(m_header_data.command_heads_end, ios::beg);
	m_external_handler = new ExternalHandler(m_string_list);
	m_external_handler->loadFromStream(m_stream);

	stream.seekg(0x74, ios::beg);
	m_sequence_handler = new SequenceHandler(m_parameter_handler, m_string_list);
	m_sequence_handler->loadFromStream(m_stream, m_header_data.entry_command_count, m_header_data.execution_command_count);

	m_name = m_string_list->getStringFromOffset(0);

	m_address_manager = new AddressManager(m_parameter_handler, m_sequence_handler, m_external_handler);
}

AINB::~AINB() {
	m_stream.close();
}

void AINB::parseHeader()
{
	m_stream.seekg(0, ios::beg);
	m_stream.read(m_header_data.head, 4);
	m_header_data.head[4] = '\0';

	m_stream.seekg(0x0c, ios::beg);
	readIntFromStream(m_stream, 4, m_header_data.entry_command_count);
	readIntFromStream(m_stream, 4, m_header_data.execution_command_count);
	readIntFromStream(m_stream, 4, m_header_data.unknown_1);
	readIntFromStream(m_stream, 4, m_header_data.unknown_2);
	readIntFromStream(m_stream, 4, m_header_data.unknown_3);

	//m_stream.seekg(0x20, ios::beg);
	readIntFromStream(m_stream, 4, m_header_data.command_heads_end); // 0x20
	readIntFromStream(m_stream, 4, m_header_data.string_list_start_pos); // 0x24

	// internal parameter start
	m_stream.seekg(0x2c, ios::beg);
	readIntFromStream(m_stream, 4, m_header_data.internal_parameters_start);

	// command parameters start
	m_stream.seekg(0x34, ios::beg);
	readIntFromStream(m_stream, 4, m_header_data.command_parameters_start);

	// command parameters end
	readIntFromStream(m_stream, 4, m_header_data.command_parameters_end);

	m_stream.seekg(0x64, ios::beg);
	readIntFromStream(m_stream, 4, m_header_data.file_type);
	return;
}

void AINB::parseParameters()
{
	m_stream.seekg(m_header_data.internal_parameters_start, ios::beg);
	m_parameter_handler->loadInternalParameters(m_stream, m_header_data.command_parameters_start);

	m_stream.seekg(m_header_data.command_parameters_start, ios::beg);
	m_parameter_handler->loadCommandParameters(m_stream, m_header_data.command_parameters_end);

	return;
}

int AINB::getEntryCommandCount()
{
	return m_header_data.entry_command_count;
}

int AINB::getExecutionCommandCount()
{
	return m_header_data.execution_command_count;
}

/**
 * Write the AINB to the stream.
 * Make sure to call finalize() before writing.
 * 
 * @param stream The stream to write to.
 */
void AINB::writeToStream(fstream& stream)
{
	stream.seekg(m_header_data.command_heads_end, ios::beg);
	m_external_handler->writeToStream(stream);

	// write command bodies
	//stream.seekg(m_header_data.command_heads_end + 0x30, ios::beg);
	m_sequence_handler->writeCommandBodiesToStream(stream);
	stream.flush();

	// write command heads
	m_sequence_handler->writeEntryCommandHeadsToStream(stream);
	stream.flush();
	m_sequence_handler->writeExecutionCommandHeadsToStream(stream);
	stream.flush();

	// internal parameter data
	stream.seekg(m_header_data.internal_parameters_start, ios::beg);
	m_parameter_handler->writeInternalParametersToStream(stream);
	stream.flush();

	// command parameter data
	stream.seekg(m_header_data.command_parameters_start, ios::beg);
	m_parameter_handler->writeCommandParametersToStream(stream);
	stream.flush();

	// write string table
	stream.seekg(m_header_data.string_list_start_pos, ios::beg);
	m_string_list->writeToStream(stream);
	stream.flush();
	
	// write file header
	m_header_data.writeToStream(stream);
}

int* AINB::getInternalParameterCounts()
{
	int* counts = new int[6];
	vector<int>* sections = m_parameter_handler->getActiveInternalParameterTypes();
	for (int i = 0; i < 6; i++)
	{
		if (find(sections->begin(), sections->end(), i) != sections->end()) {
			// subtract 1 because the first entry is always the null parameter
			counts[i] = m_parameter_handler->getInternalParameters(i)->size() - 1;
		}
		else {
			counts[i] = 0;
		}
	}
	return counts;
}

int* AINB::getCommandParameterCounts()
{
	int* counts = new int[12];
	vector<int>* sections = m_parameter_handler->getActiveCommandParameterTypes();
	for (int i = 0; i < 12; i++)
	{
		if (find(sections->begin(), sections->end(), i) != sections->end()) {
			// subtract 1 because the first entry is always the null parameter
			counts[i] = m_parameter_handler->getCommandParameters(i)->size() - 1;
		}
		else {
			counts[i] = 0;
		}
	}
	return counts;
}

InternalParameterBase* AINB::getInternalParameter(int section_num, int index)
{
	return m_parameter_handler->getInternalParameter(section_num, index);
}

CommandParameterBase* AINB::getCommandParameter(int section_num, int index)
{
	return m_parameter_handler->getCommandParameter(section_num, index);
}

void AINB::finalize()
{
	// make sure name is the first string in string list
	m_string_list->getOffsetOfString(m_name);

	// ensure that the offset for the second string is correct
	string type_name = ainb::FileTypeNumToName.at(m_header_data.file_type);
	m_header_data.type_name_offset = m_string_list->getOffsetOfString(type_name);

	m_sequence_handler->finalize();
	m_parameter_handler->finalize();

	// update header data
	m_header_data.entry_command_count = m_sequence_handler->getEntryCommands().size();
	m_header_data.execution_command_count = m_sequence_handler->getSequenceNodes().size();

	AddressManager::AddressData* address_data = m_address_manager->calcAddressData();

	m_header_data.command_heads_end = address_data->headers_end_address;

	m_header_data.parameter_section_start = address_data->param_section_start_address;

	m_header_data.internal_parameters_start = address_data->internal_param_start_address;
	m_header_data.command_parameters_start = address_data->command_param_start_address;
	m_header_data.command_parameters_end = address_data->command_param_end_address;

	m_header_data.string_list_start_pos = address_data->string_list_start_address;
	m_header_data.string_list_4byte_padding = address_data->string_list_4byte_padding;
	m_header_data.string_list_8byte_padding = address_data->string_list_8byte_padding;

	m_header_data.func_start_address = address_data->func_start_address;
	m_header_data.func_end_address = address_data->func_end_address;
	m_header_data.end_data_4byte_padding = address_data->end_data_4byte_padding;
}

void AINB::HeaderData::writeToStream(fstream& stream)
{
	stream.seekg(0, ios::beg);

	// 0x00
	stream.write(head, 4); 
	writeIntToStream(stream, 4, 1031);

	stream.seekg(0x0c, ios::beg);

	// 0x0c
	writeIntToStream(stream, 4, entry_command_count);
	// 0x10
	writeIntToStream(stream, 4, execution_command_count);

	// 0x14 todo
	writeIntToStream(stream, 4, -1);

	// 0x18 todo
	writeIntToStream(stream, 4, -1);

	// 0x1c todo
	writeIntToStream(stream, 4, -1); 

	stream.seekg(0x20, ios::beg);

	// 0x20
	writeIntToStream(stream, 4, command_heads_end);
	// 0x24
	writeIntToStream(stream, 4, string_list_start_pos);

	// 0x28
	writeIntToStream(stream, 4, string_list_4byte_padding);
	
	// 0x2c
	writeIntToStream(stream, 4, internal_parameters_start);

	// 0x30 todo
	writeIntToStream(stream, 4, -1);

	// 0x34
	writeIntToStream(stream, 4, command_parameters_start);

	// 0x38
	writeIntToStream(stream, 4, command_parameters_end);

	// 0x3c todo
	writeIntToStream(stream, 4, -1);

	// 0x40
	writeIntToStream(stream, 4, parameter_section_start);

	// 0x44
	writeIntToStream(stream, 4, func_start_address);

	// 0x48 todo
	writeIntToStream(stream, 4, -1);

	// 0x4c todo
	writeIntToStream(stream, 4, -1);

	// 0x50 todo
	writeIntToStream(stream, 4, -1);

	stream.seekg(0x5c, ios::beg);
	writeIntToStream(stream, 4, func_end_address);

	// 0x60
	writeIntToStream(stream, 4, type_name_offset);

	// 0x64
	writeIntToStream(stream, 4, file_type);

	// 0x68
	writeIntToStream(stream, 4, end_data_4byte_padding);

	// 0x6c
	writeIntToStream(stream, 4, string_list_8byte_padding);

	// 0x70
	writeIntToStream(stream, 4, end_data_address);
}
