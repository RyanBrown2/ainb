#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ocidl.h>
#include <unordered_map>
#include "StringList.h"
#include "SequenceNode.h"
#include "util.h"

namespace ainb
{
class SequenceHandler
{
public:
	SequenceHandler(StringList* string_list);
	~SequenceHandler();

	// todo, struct for entry command

	void addSequenceNode(SequenceNode* sequence_node);

	SequenceNode* getSequenceNode(int index);
	
	void loadFromStream(LPSTREAM stream, int entry_count, int execution_count);
	void writeToStream(std::fstream& stream);

	std::vector<SequenceNode*> getSequenceNodes();

private:
	StringList* m_string_list;

	std::unordered_map<int, SequenceNode*> m_sequence_nodes;

	void loadExecutionCommandHeads(LPSTREAM stream, int count);

};
}