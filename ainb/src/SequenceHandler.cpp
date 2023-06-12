#include "nin-io/ainb/SequenceHandler.h"

using namespace std;

SequenceHandler::SequenceHandler()
{
}

SequenceHandler::~SequenceHandler()
{

}

SequenceHandler::SequenceNode* SequenceHandler::loadSequence(SequenceNode* caller_node, int command_index)
{
	ExecutionCommand command = m_execution_commands[command_index];
	CommandBody command_body = m_command_bodies[command.getBodyAddress()];

	SequenceNode node;
	node.caller = caller_node;
	node.command = &command;
	node.command_body = &command_body;

	vector<CommandBody::TableKeyValuePair> call_table = command_body.getCallTable();
	vector<int> unique_command_indices;
	for (int i = 0; i < call_table.size(); i++) {
		CallTableEntry entry;
		entry.index = call_table[i].index;
		entry.parameter = call_table[i].parameter;
		node.call_table.push_back(entry);

		if (find(unique_command_indices.begin(), unique_command_indices.end(), entry.index) == unique_command_indices.end()) {
			unique_command_indices.push_back(entry.index);
		}
	}

	for (int i = 0; i < unique_command_indices.size(); i++) {
		node.callees.push_back(loadSequence(&node, unique_command_indices[i]));
	}


	return &node;
}

void SequenceHandler::setEntryPointCommands(vector<EntryPointCommand> entry_point_commands)
{
	m_entry_point_commands = entry_point_commands;
}

void SequenceHandler::setExecutionCommands(vector<ExecutionCommand> execution_commands)
{
	m_execution_commands = execution_commands;
}

void SequenceHandler::setCommandBodies(map<int, CommandBody> command_bodies)
{
	m_command_bodies = command_bodies;
}

void SequenceHandler::setParameterHandler(ParameterHandler* parameter_handler)
{
	m_parameter_handler = parameter_handler;
}