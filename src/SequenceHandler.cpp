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

// todo
void SequenceHandler::addSequenceNode(SequenceNode* node)
{
	//m_sequence_nodes.push_back(node);
	//m_sequence_nodes.at(node->getIndex()) = node;
}

SequenceNode* SequenceHandler::getSequenceNode(int index)
{
	return m_sequence_nodes[index];
}

void SequenceHandler::loadFromStream(fstream& stream, int entry_count, int execution_count)
{
	// load entry commands
	for (int i = 0; i < entry_count; i++)
	{
		// todo, for now just skipping
		stream.seekg(0x18, ios::cur);
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

void SequenceHandler::loadExecutionCommandHeads(fstream& stream, int count)
{
	for (int i = 0; i < count; i++)
	{
		int end_pos = (int)stream.tellg() + 0x3c;
		SequenceNode* node = new SequenceNode();

		// todo: unknown 1
		stream.seekg(2, ios::cur);
		// 0x02

		// index
		int index;
		readIntFromStream(stream, 4, index);
		// 0x06

		// todo: unknown 2
		stream.seekg(2, ios::cur);
		// 0x08

		// string offset
		int string_offset;
		readIntFromStream(stream, 4, string_offset);
		node->setName(m_string_list->getStringFromOffset(string_offset));
		// 0x0c

		// todo: uid
		stream.seekg(4, ios::cur);
		// 0x10

		stream.seekg(4, ios::cur);
		// 0x14

		// command body address
		int command_body_address;
		readIntFromStream(stream, 4, command_body_address);
		node->setBodyPos(command_body_address);
		// 0x18

		stream.seekg(0x14, ios::cur);

		// guid
		char guid[17];
		stream.read(guid, 16);
		guid[16] = '\0';
		node->setGUID(guid);

		stream.seekg(end_pos, ios::beg);

		m_sequence_nodes[index] = node;
	}
}
