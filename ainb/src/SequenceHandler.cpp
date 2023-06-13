#include "nin-io/ainb/SequenceHandler.h"

using namespace std;

SequenceHandler::SequenceHandler()
{
}

SequenceHandler::~SequenceHandler()
{

}

SequenceHandler::SequenceNode* SequenceHandler::createEntryNode(EntryPointCommand* command)
{
	SequenceNode* node = new SequenceNode();
	node->caller = nullptr;
	int command_index = command->getExecutionIndex();
	node->node_command = &m_execution_commands->at(command_index);
	node->node_body = &m_command_bodies->at(node->node_command->getBodyAddress());
	node->call_table = node->node_body->getCallTable();

	// create new caller history
	node->caller_history = new unordered_map<int, SequenceNode*>();
	
	// create callees
	loadCallTable(node);

	return node;
}

SequenceHandler::SequenceNode* SequenceHandler::createExecutionNode(ExecutionCommand* command)
{
	SequenceNode* node = new SequenceNode();
	node->caller = nullptr;
	node->node_command = command;
	node->node_body = &m_command_bodies->at(node->node_command->getBodyAddress());
	node->call_table = node->node_body->getCallTable();

	// create new caller history
	node->caller_history = new unordered_map<int, SequenceNode*>();

	// create callees
	loadCallTable(node);

	return node;
}


SequenceHandler::SequenceNode* SequenceHandler::loadSequence(SequenceNode* caller_node, int command_index)
{
	SequenceNode* node = new SequenceNode();
	node->caller = caller_node;
	node->node_command = &m_execution_commands->at(command_index);
	node->node_body = &m_command_bodies->at(node->node_command->getBodyAddress());
	node->call_table = node->node_body->getCallTable();
	node->caller_history = caller_node->caller_history;

	// create callees
	loadCallTable(node);

	return node;
}

void SequenceHandler::loadCallTable(SequenceNode* node)
{
	//vector<int> ordered_indices; // store the order that indices are called in
	for (int i = 0; i < node->call_table->size(); i++) {
		CommandBody::CallTableEntry call_table_entry = node->call_table->at(i);

		node->sorted_call_table[call_table_entry.index].push_back(call_table_entry.parameter);

		// check if the last index called is the same as this one
		if (node->ordered_param_indices.size() > 0 && node->ordered_param_indices.back() != call_table_entry.index) {
			node->ordered_param_indices.push_back(call_table_entry.index);
		} else if (node->ordered_param_indices.size() == 0) {
			node->ordered_param_indices.push_back(call_table_entry.index);
		}
	}

	// add current node to caller history
	node->caller_history->insert({ node->node_command->getIndex(), node });

	// iterate through the call table in order
	for (int i = 0; i < node->ordered_param_indices.size(); i++) {
		int index = node->ordered_param_indices[i];
		vector<string> parameters = node->sorted_call_table.at(index);

		// create callee

		// make sure no loops are created
		auto it = node->caller_history->find(index);
		if (it != node->caller_history->end()) {
			// loop detected
			node->return_callees.push_back(node->caller_history->at(index));
		} else {
			try {
				node->callees.push_back(loadSequence(node, index)); // recursively create nodes
			} catch (const std::out_of_range& oor) {
				int caller_index = node->caller->node_command->getIndex();
				cerr << "Potentially Bad Data | ";
				cerr << "Command with index " << caller_index << " tried calling command with index " << index << endl;
			}
		}
	}

	// once all callees are created, remove current node from caller history
	node->caller_history->erase(node->node_command->getIndex());

}

void SequenceHandler::setEntryPointCommands(vector<EntryPointCommand>* entry_point_commands)
{
	m_entry_point_commands = entry_point_commands;
}

void SequenceHandler::setExecutionCommands(vector<ExecutionCommand>* execution_commands)
{
	m_execution_commands = execution_commands;
}

void SequenceHandler::setCommandBodies(map<int, CommandBody>* command_bodies)
{
	m_command_bodies = command_bodies;
}

void SequenceHandler::setParameterHandler(ParameterHandler* parameter_handler)
{
	m_parameter_handler = parameter_handler;
}