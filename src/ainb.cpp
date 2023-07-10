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

	//streamSeek(m_stream, 0x74, START);
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

	m_stream.seekg(0x24, ios::beg);
	readIntFromStream(m_stream, 4, m_header_data.string_list_start_pos);

	// internal parameter start
	m_stream.seekg(0x2c, ios::beg);
	readIntFromStream(m_stream, 4, m_header_data.internal_parameters_start);

	// command parameters start
	m_stream.seekg(0x34, ios::beg);
	readIntFromStream(m_stream, 4, m_header_data.command_parameters_start);

	// command parameters end
	readIntFromStream(m_stream, 4, m_header_data.command_parameters_end);

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
	// make sure name gets written to string list
	m_string_list->getOffsetOfString(m_name);

	// address after the last command
	int command_end_pos = 0x74 + (0x18 * m_sequence_handler->getEntryCommands().size()) + (0x3c * m_sequence_handler->getSequenceNodes().size());

	// write command bodies
	stream.seekg(command_end_pos + 0x30, ios::beg);
	m_sequence_handler->writeCommandBodiesToStream(stream);

	// internal parameter data
	stream.seekg(m_header_data.internal_parameters_start, ios::beg);
	m_parameter_handler->writeInternalParametersToStream(stream);

	// command parameter data
	stream.seekg(m_header_data.command_parameters_start, ios::beg);
	m_parameter_handler->writeCommandParametersToStream(stream);

	// write string table
	stream.seekg(m_header_data.string_list_start_pos, ios::beg);
	m_string_list->writeToStream(stream);

	// todo: update header data
	// write header data
	stream.seekg(0, ios::beg);
	stream.write(m_header_data.type, 4);

	stream.seekg(0x0c, ios::beg);
	// write command counts
	stream.write(convertIntToCharArray(m_header_data.entry_command_count, 4), 4);
	stream.write(convertIntToCharArray(m_header_data.execution_command_count, 4), 4);

	// string list start
	stream.seekg(0x24, ios::beg);
	stream.write(convertIntToCharArray(m_header_data.string_list_start_pos, 4), 4);
}

int* AINB::getInternalParameterCounts()
{
	int* counts = new int[6];
	vector<int>* sections = m_parameter_handler->getActiveInternalParameterTypes();
	for (int i = 0; i < 6; i++)
	{
		if (find(sections->begin(), sections->end(), i) != sections->end()) {
			counts[i] = m_parameter_handler->getInternalParameters(i)->size();
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
			counts[i] = m_parameter_handler->getCommandParameters(i)->size();
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
	m_sequence_handler->finalize();
	m_parameter_handler->finalize();
}

