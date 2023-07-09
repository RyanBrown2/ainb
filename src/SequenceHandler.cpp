#include "ainb/SequenceHandler.h"

using namespace std;
using namespace ainb;

SequenceHandler::SequenceHandler(ParameterHandler* parameter_handler, StringList* string_list)
{
	m_parameter_handler = parameter_handler;
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
	// skip entry commands
	stream.seekg(0x18 * entry_count, ios::cur);

	// load execution commands
	loadExecutionCommandHeads(stream, execution_count);

	stream.seekg(0x74, ios::beg);

	// load entry commands
	loadEntryCommandHeads(stream, entry_count);
	
	// load command bodies
	loadCommandBodies(stream);

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

void SequenceHandler::loadEntryCommandHeads(fstream& stream, int count)
{
	for (int i = 0; i < count; i++)
	{
		EntryCommand entry_command;

		int name_offset;
		readIntFromStream(stream, 4, name_offset);
		entry_command.name = m_string_list->getStringFromOffset(name_offset);

		stream.read(entry_command.guid, 16);
		entry_command.guid[16] = '\0';

		int entry_node_index;
		readIntFromStream(stream, 4, entry_node_index);
		entry_command.entry_node = m_sequence_nodes.at(entry_node_index);

		m_entry_commands.push_back(entry_command);
	}
}

void SequenceHandler::loadCommandBodies(fstream& stream)
{
	for (int i = 0; i < m_sequence_nodes.size(); i++)
	{
		SequenceNode* node = m_sequence_nodes.at(i);

		stream.seekg(node->getBodyPos(), ios::beg);
		
		
		int body_start_pos = (int)stream.tellg();
		for (int j = 0; j < m_parameter_handler->getActiveInternalParameterTypes()->size(); j++)
		{
			int section_num = m_parameter_handler->getActiveInternalParameterTypes()->at(j);
			stream.seekg(body_start_pos + section_num * 0x8, ios::beg);
			int param_index;
			readIntFromStream(stream, 4, param_index);

			// if index is 0, skip
			if (param_index == 0)
			{
				continue;
			}

			node->setInternalParameter(m_parameter_handler->getInternalParameter(section_num, param_index), section_num);

		}

		int command_param_start_pos = body_start_pos + (6*0x8);
		for (int j = 0; j < m_parameter_handler->getActiveCommandParameterTypes()->size(); j++)
		{
			int section_num = m_parameter_handler->getActiveCommandParameterTypes()->at(j);
			stream.seekg(command_param_start_pos + section_num * 0x8, ios::beg);
			int param_index;
			readIntFromStream(stream, 4, param_index);

			// if index is 0, skip
			if (param_index == 0)
			{
				continue;
			}

			node->setCommandParameter(m_parameter_handler->getCommandParameter(section_num, param_index), section_num);
		}
	}
}
