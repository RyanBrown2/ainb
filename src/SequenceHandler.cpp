#include "ainb/SequenceHandler.h"

using namespace std;
using namespace ainb;

SequenceHandler::SequenceHandler(StringList* string_list)
{
	m_string_list = string_list;

}

SequenceHandler::~SequenceHandler()
{

}

void SequenceHandler::addSequenceNode(SequenceNode* node)
{
	//m_sequence_nodes.push_back(node);
	//m_sequence_nodes.at(node->getIndex()) = node;
}

SequenceNode* SequenceHandler::getSequenceNode(int index)
{
	return m_sequence_nodes[index];
}

SequenceNode* SequenceHandler::parseExecutionHead(LPSTREAM stream)
{
	//SequenceNode* node = new SequenceNode();
	streamSeek(stream, 2, CURRENT);
	int index;
	readIntFromStream(stream, index);
	// 0x06

	streamSeek(stream, 2, CURRENT);
	int string_offset;
	readIntFromStream(stream, string_offset);
	// 0x0c

	std::string name = m_string_list->getStringFromOffset(string_offset);

	streamSeek(stream, 0x20, CURRENT);
	// 0x2c
	char* guid = new char[17];
	stream->Read(guid, 16, NULL);

	SequenceNode* node = new SequenceNode(index, name, guid);

	// only for debug
	//cout << "node " << index << " : " << string_offset << " | " << node->getName() << endl;

	return node;

}

void SequenceHandler::loadFromStream(LPSTREAM stream, int entry_count, int execution_count)
{
	// load entry commands
	for (int i = 0; i < entry_count; i++)
	{
		// todo, for now just skipping
		streamSeek(stream, 0x18, CURRENT);
	}

	// load execution commands
	for (int i = 0; i < execution_count; i++)
	{
		// todo
		SequenceNode* node = parseExecutionHead(stream);
		m_sequence_nodes[node->getIndex()] = node;

	}
	
}

// todo
void SequenceHandler::writeToStream(LPSTREAM stream)
{
	
}
