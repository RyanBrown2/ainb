#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "StringList.h"
#include "ParameterHandler.h"
#include "SequenceNode.h"
#include "util.h"

namespace ainb
{
class SequenceHandler
{
public:
	SequenceHandler(ParameterHandler* parameter_handler, StringList* string_list);
	~SequenceHandler();

	// todo, struct for entry command
	struct EntryCommand {
		std::string name = "";
		char* guid = new char[17];
		SequenceNode* entry_node = nullptr;
	};

	void addSequenceNode(SequenceNode* sequence_node);

	SequenceNode* getSequenceNode(int index);
	
	void loadFromStream(std::fstream& stream, int entry_count, int execution_count);

	void writeCommandBodiesToStream(std::fstream& stream);

	std::vector<EntryCommand*> getEntryCommands() { return m_entry_commands; }
	std::vector<SequenceNode*> getSequenceNodes() { return m_sequence_nodes; }

	void finalize();

private:
	ParameterHandler* m_parameter_handler;
	StringList* m_string_list;

	std::vector<SequenceNode*> m_sequence_nodes;
	//std::unordered_map<int, SequenceNode*> m_sequence_nodes;
	std::vector<EntryCommand*> m_entry_commands;

	void loadExecutionCommandHeads(std::fstream& stream, int count);
	void loadEntryCommandHeads(std::fstream& stream, int count);
	void loadCommandBodies(std::fstream& stream);

	void updateCommandIndices();
};
}