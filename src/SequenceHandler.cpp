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

void SequenceHandler::loadFromStream(LPSTREAM stream, int entry_count, int execution_count)
{
	// load entry commands
	for (int i = 0; i < entry_count; i++)
	{
		// todo, for now just skipping
		streamSeek(stream, 0x18, CURRENT);
	}

	// load execution commands
	loadExecutionCommandHeads(stream, execution_count);
	
}

// todo
void SequenceHandler::writeToStream(fstream& stream)
{
	
}

vector<SequenceNode*> SequenceHandler::getSequenceNodes()
{
	vector<SequenceNode*> nodes;
	for (auto it = m_sequence_nodes.begin(); it != m_sequence_nodes.end(); it++)
	{
		nodes.push_back(it->second);
	}
	return nodes;
}

void SequenceHandler::loadExecutionCommandHeads(LPSTREAM stream, int count)
{
	for (int i = 0; i < count; i++)
	{
		int end_pos = streamTell(stream) + 0x3c;
		SequenceNode* node = new SequenceNode();

		// todo: unknown 1
		streamSeek(stream, 2, CURRENT);
		// 0x02

		// index
		int index;
		readIntFromStream(stream, index);
		// 0x06

		// todo: unknown 2
		streamSeek(stream, 2, CURRENT);
		// 0x08

		// string offset
		int string_offset;
		readIntFromStream(stream, string_offset);
		node->setName(m_string_list->getStringFromOffset(string_offset));
		// 0x0c

		// todo: uid
		streamSeek(stream, 0x4, CURRENT);
		// 0x10


		streamSeek(stream, 0x4, CURRENT);
		// 0x14

		// command body address
		int command_body_address;
		readIntFromStream(stream, command_body_address);
		node->setBodyPos(command_body_address);
		// 0x18

		streamSeek(stream, 0x14, CURRENT);

		// guid
		char guid[17];
		stream->Read(guid, 16, NULL);
		guid[16] = '\0';
		node->setGUID(guid);

		streamSeek(stream, end_pos, START);

		m_sequence_nodes[index] = node;
	}
}
