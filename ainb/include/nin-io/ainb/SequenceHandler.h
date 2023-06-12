#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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

	void setEntryPointCommands(std::vector<EntryPointCommand> entry_point_commands);
	void setExecutionCommands(std::vector<ExecutionCommand> execution_commands);
	void setCommandBodies(std::map<int, CommandBody> command_bodies);
	void setParameterHandler(ParameterHandler* parameter_handler);

	struct CallTableEntry {
		std::string parameter;
		int index;
	};

	struct SequenceNode {
		SequenceNode* caller = nullptr;
		ExecutionCommand* command = nullptr;
		CommandBody* command_body = nullptr;
		std::vector<CallTableEntry> call_table;
		std::vector<SequenceNode*> callees;
	};

	SequenceNode* loadSequence(SequenceNode* caller_node, int command_index);

private:

	std::vector<EntryPointCommand> m_entry_point_commands;

	std::vector<ExecutionCommand> m_execution_commands;

	// address to body
	std::map<int, CommandBody> m_command_bodies;
	
	ParameterHandler* m_parameter_handler = nullptr;
};