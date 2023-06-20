#include "nin-io/ainb/ainb.h"
#include "nin-io/ainb/StringList.h"

using namespace std;

AINB::AINB()
{

}

AINB::~AINB()
{

}

void AINB::loadFileHeaderData(fstream& file) {
	file.clear();
	file.seekg(0, ios::beg);

	// type
	file.read(m_file_header_data.type, 3);
	m_file_header_data.type[3] = '\0';

	// address section pointer
	readIntFromStream(file, is_big_endian, 1, m_file_header_data.address_section_pointer);

	// entry point command count
	file.seekg(0x0c, ios::beg);
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.entry_point_command_count);

	// execution command count
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.execution_command_count);

	// unknown value
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.unknown_value);

	// command body start address
	file.seekg(m_file_header_data.address_section_pointer, ios::beg);
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.command_body_start_address);

	// string list start address
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.string_list_start_address);

	// four bytes before string list start address
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.four_bytes_before_string_list);

	// parameter table start address
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.parameter_table_start_address);
	
	// parameter subsection 3
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.parameter_subsection_three_start_address);

	// parameter structure start address
	readIntFromStream(file, is_big_endian, 4, m_file_header_data.parameter_structure_start_address);

}

void AINB::load(fstream& file)
{
	// load the file's header data
	loadFileHeaderData(file);

	// load the string list
	file.seekg(m_file_header_data.string_list_start_address, ios::beg);
	m_string_list = new StringList(file);

	m_name = m_string_list->getStringFromPos(0);

	// PARAMETERS
	m_parameter_handler = new ParameterHandler(m_string_list);

	// load the parameter table
	file.seekg(m_file_header_data.parameter_table_start_address, ios::beg);
	m_parameter_handler->loadTableParameters(file, m_file_header_data.parameter_structure_start_address);
	file.seekg(m_file_header_data.parameter_structure_start_address, ios::beg);
	m_parameter_handler->loadStructureParameters(file, m_file_header_data.parameter_subsection_three_start_address);

	// TODO: there's a potential function section after this that needs to be handled

	// COMMANDS

	file.seekg(0x74, ios::beg);

	for (int i = 0; i < m_file_header_data.entry_point_command_count; i++) {
		EntryPointCommand command;
		command.load(file, m_string_list);
		m_entry_point_commands.push_back(command);
	}

	for (int i = 0; i < m_file_header_data.execution_command_count; i++) {
		ExecutionCommand command;
		command.load(file, m_string_list);
		m_execution_commands.push_back(command);
	}

	// COMMAND BODIES
	for (int i = 0; i < m_execution_commands.size(); i++) {
		ExecutionCommand* command = &m_execution_commands[i];
		CommandBody body(m_string_list, m_parameter_handler);
		file.seekg(command->getBodyAddress(), ios::beg);
		body.setCommandReference(command);
		body.load(file);

		m_command_bodies[body.getAddress()] = body;
	}

	// replace command indices with command pointers
	for (int i = 0; i < m_command_bodies.size(); i++) {
		CommandBody* body = &m_command_bodies[i];

		for (int j = 0; j < body->getCallTable()->size(); j++) {
			CommandBody::CallTableEntry* entry = &body->getCallTable()->at(j);
			if (entry->index >= m_execution_commands.size()) {
				continue;
			}
			entry->command = &m_execution_commands[entry->index];
		}
	}

	// load the sequences
	m_sequence_handler = new SequenceHandler();
	loadSequences();

	// if there are no entry points, load manually
	if (m_sequences.size() == 0) {

		for (int i = 0; i < m_file_header_data.execution_command_count; i++)
		{
			if (m_sequence_handler->getLoadedNodes()->count(i)) {
				continue;
			}
			m_sequences.push_back(m_sequence_handler->createExecutionNode(&m_execution_commands[i]));
		}
	}

	return;
}

AINB::FileHeaderData AINB::getFileHeaderData()
{
	return m_file_header_data;
}

void AINB::loadSequences()
{
	m_sequence_handler->setEntryPointCommands(&m_entry_point_commands);
	m_sequence_handler->setExecutionCommands(&m_execution_commands);
	m_sequence_handler->setCommandBodies(&m_command_bodies);
	m_sequence_handler->setParameterHandler(m_parameter_handler);

	for (int i = 0; i < m_entry_point_commands.size(); i++) {
		m_sequences.push_back(m_sequence_handler->createEntryNode(&m_entry_point_commands[i]));
	}
}

vector<SequenceHandler::SequenceNode*>* AINB::getSequences()
{
	return &m_sequences;
}

string AINB::getName()
{
	return m_name;
}

void AINB::writeTo(fstream& file)
{

	// finalize data

	// entry points
	for (int i = 0; i < m_entry_point_commands.size(); i++) {
		EntryPointCommand* command = &m_entry_point_commands[i];
		string name = command->getName();
		m_string_list->getPosFromString(name); // load the string into the string list
	}

	// execution commands
	for (int i = 0; i < m_execution_commands.size(); i++) {
		ExecutionCommand* command = &m_execution_commands[i];
		string name = command->getName();
		m_string_list->getPosFromString(name); // load the string into the string list
	}

	// write data to file
	m_string_list->finalize();

	int start_pos = file.tellg();

	file.write("AIB", 3);

	// todo: header data
	file.seekg(0x74, ios::beg);

	// writing entry point data
	for (int i = 0; i < m_entry_point_commands.size(); i++) {
		EntryPointCommand* command = &m_entry_point_commands[i];
		int name_pos = *m_string_list->getPosFromString(command->getName());
		file.write(reinterpret_cast<char*>(&name_pos), 4);
		file.write(command->getGUID(), 16);
		int execution_index = command->getExecutionIndex();
		file.write(reinterpret_cast<char*>(&execution_index), 4);
	}

	// writing execution command data
	for (int i = 0; i < m_execution_commands.size(); i++) {
		ExecutionCommand* command = &m_execution_commands[i];
		int unknown1 = command->getUnknown1();
		file.write(reinterpret_cast<char*>(&unknown1), 2);
		int command_index = command->getIndex();
		file.write(reinterpret_cast<char*>(&command_index), 4);
		int unknown2 = command->getUnknown2();
		file.write(reinterpret_cast<char*>(&unknown2), 2);
		int name_pos = *m_string_list->getPosFromString(command->getName());
		file.write(reinterpret_cast<char*>(&name_pos), 4);
		int command_id = command->getCommandID();
		file.write(reinterpret_cast<char*>(&command_id), 4);
		file.seekg(0x4, ios::cur);
		int command_body_address = command->getBodyAddress(); // todo: this is wrong, need to calculate it
		file.write(reinterpret_cast<char*>(&command_body_address), 4);
		
		file.seekg(0x14, ios::cur);
		file.write(command->getGuid(), 16);

	}


	// string list
	m_string_list->writeToFile(file);

}

