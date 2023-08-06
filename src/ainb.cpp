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

	stream.seekg(0x74, ios::beg);
	m_sequence_handler = new SequenceHandler(m_parameter_handler, m_string_list);
	m_sequence_handler->loadFromStream(m_stream, m_header_data.entry_command_count, m_header_data.execution_command_count);

	m_name = m_string_list->getStringFromOffset(0);
}

AINB::~AINB() {
	m_stream.close();
}

void AINB::parseHeader()
{
	m_stream.seekg(0, ios::beg);
	m_stream.read(m_header_data.type, 4);
	m_header_data.type[4] = '\0';

	m_stream.seekg(0x0c, ios::beg);
	readIntFromStream(m_stream, 4, m_header_data.entry_command_count);
	readIntFromStream(m_stream, 4, m_header_data.execution_command_count);

	m_stream.seekg(0x20, ios::beg);
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
	readIntFromStream(m_stream, 4, m_header_data.file_category);
	return;
}

void AINB::parseParameters()
{
	//streamSeek(m_stream, m_header_data.internal_parameters_start, START);
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
	// update header data
	m_header_data.entry_command_count = m_sequence_handler->getEntryCommands().size();
	m_header_data.execution_command_count = m_sequence_handler->getSequenceNodes().size();

	// end of command heads
	m_header_data.command_heads_end = 0x74 + (0x18 * m_header_data.entry_command_count) + (0x3c * m_header_data.execution_command_count);

	// write command bodies
	stream.seekg(m_header_data.command_heads_end + 0x30, ios::beg);
	m_sequence_handler->writeCommandBodiesToStream(stream);

	// get start position of parameter section
	stream.seekg(0x14, ios::cur); // 0x14 bytes of padding
	m_header_data.parameter_section_start = stream.tellg();

	// write command heads
	m_sequence_handler->writeEntryCommandHeadsToStream(stream);
	m_sequence_handler->writeExecutionCommandHeadsToStream(stream);

	// get start position of internal parameter section
	// todo: there is potentially another section before this, need to figure out what it is
	m_header_data.internal_parameters_start = m_header_data.parameter_section_start;

	// internal parameter data
	stream.seekg(m_header_data.internal_parameters_start, ios::beg);
	m_parameter_handler->writeInternalParametersToStream(stream);

	// get start position of command parameter section
	m_header_data.command_parameters_start = stream.tellg();

	// command parameter data
	stream.seekg(m_header_data.command_parameters_start, ios::beg);
	m_parameter_handler->writeCommandParametersToStream(stream);

	// get end of parameter section
	m_header_data.command_parameters_end = stream.tellg();

	// todo: there is more data between the end of the parameter section and the start of the string list
	// for now, just pad 0x20 bytes
	// the library currently doesn't support this data, so it will be lost

	// get start position of string list
	// todo: update this once the above todo is resolved
	m_header_data.string_list_start_pos = m_header_data.command_parameters_end + 0x20;

	// write string table
	stream.seekg(m_header_data.string_list_start_pos, ios::beg);
	m_string_list->writeToStream(stream);
	
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
	m_header_data.name_offset = m_string_list->getNextOffset();

	m_sequence_handler->finalize();
	m_parameter_handler->finalize();
}

void AINB::HeaderData::writeToStream(fstream& stream)
{
	stream.seekg(0, ios::beg);

	// 0x00
	stream.write(type, 4); 

	stream.seekg(0x0c, ios::beg);

	// 0x0c
	writeIntToStream(stream, 4, entry_command_count);
	// 0x10
	writeIntToStream(stream, 4, execution_command_count);

	stream.seekg(0x20, ios::beg);

	// 0x20
	writeIntToStream(stream, 4, command_heads_end);
	// 0x24
	writeIntToStream(stream, 4, string_list_start_pos);

	// 0x28 todo: This is the address that points 4 bytes before the String List.
	stream.seekg(0x4, ios::cur);
	
	// 0x2c
	writeIntToStream(stream, 4, internal_parameters_start);

	// 0x30 todo
	stream.seekg(0x4, ios::cur);

	// 0x34
	writeIntToStream(stream, 4, command_parameters_start);

	// 0x38 todo: end of command parameters section
	stream.seekg(0x4, ios::cur);

	// 0x3c todo
	stream.seekg(0x4, ios::cur);

	// 0x40
	writeIntToStream(stream, 4, parameter_section_start);

	stream.seekg(0x60, ios::beg);

	// 0x60
	writeIntToStream(stream, 4, name_offset);

	// 0x64
	writeIntToStream(stream, 4, file_category);
}
