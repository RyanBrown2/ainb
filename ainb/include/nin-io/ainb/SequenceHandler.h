#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "nin-io/util/util.h"
#include "StringList.h"
#include "ParameterHandler.h"
#include "EntryPointCommand.h"
#include "ExecutionCommand.h"
#include "CommandBody.h"

class SequenceHandler
{
public:
	SequenceHandler();
	~SequenceHandler();

	void setEntryPointCommands(std::vector<EntryPointCommand>* entry_point_commands);
	void setExecutionCommands(std::vector<ExecutionCommand>* execution_commands);
	void setCommandBodies(std::map<int, CommandBody>* command_bodies);
	void setParameterHandler(ParameterHandler* parameter_handler);

	struct SequenceNode {
		SequenceNode* caller = nullptr;
		ExecutionCommand* node_command = nullptr;
		CommandBody* node_body = nullptr;
		std::vector<CommandBody::CallTableEntry>* call_table = nullptr;
		// index to parameters
		std::vector<int> ordered_param_indices; // store the order that indices are called in
		std::map<int, std::vector<std::string>> sorted_call_table;
		std::vector<SequenceNode*> callees;
		std::vector<SequenceNode*> return_callees;
		std::unordered_map<int, SequenceNode*>* caller_history;
	};

	SequenceNode* createEntryNode(EntryPointCommand* command);
	SequenceNode* createExecutionNode(ExecutionCommand* command);
	SequenceNode* loadSequence(SequenceNode* caller_node, int command_index);

private:

	std::vector<EntryPointCommand>* m_entry_point_commands = nullptr;

	std::vector<ExecutionCommand>* m_execution_commands = nullptr;

	// address to body
	std::map<int, CommandBody>* m_command_bodies = nullptr;
	
	ParameterHandler* m_parameter_handler = nullptr;

	void loadCallTable(SequenceNode* node);
};