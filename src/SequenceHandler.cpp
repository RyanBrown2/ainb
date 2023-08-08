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
	return m_sequence_nodes.at(index);
}

void SequenceHandler::loadFromStream(fstream& stream, int entry_count, int execution_count)
{
	stream.seekg(0x74, ios::beg);
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

void SequenceHandler::finalize()
{
	updateCommandIndices();
}

void SequenceHandler::updateCommandIndices()
{
	for (int i = 0; i < m_sequence_nodes.size(); i++)
	{
		SequenceNode* node = m_sequence_nodes.at(i);
		node->setIndex(i);
	}
}

void SequenceHandler::writeEntryCommandHeadsToStream(fstream& stream)
{
	stream.seekg(0x74, ios::beg);

	for (int i = 0; i < m_entry_commands.size(); i++)
	{
		EntryCommand* entry_command = m_entry_commands.at(i);

		// name offset
		int name_offset = m_string_list->getOffsetOfString(entry_command->name);
		writeIntToStream(stream, 4, name_offset);

		// guid
		stream.write(entry_command->guid, 16);

		// entry node index
		int entry_node_index = entry_command->entry_node->getIndex();
		writeIntToStream(stream, 4, entry_node_index);
	}
}

void SequenceHandler::writeExecutionCommandHeadsToStream(fstream& stream)
{
	stream.seekg(0x74 + 0x18 * m_entry_commands.size(), ios::beg);

	for (int i = 0; i < m_sequence_nodes.size(); i++)
	{
		int start_pos = stream.tellg();
		SequenceNode* node = m_sequence_nodes.at(i);
		node->writeHeadToStream(stream, m_string_list);
		stream.seekg(start_pos + 0x3c, ios::beg);
	}
}

// todo
void SequenceHandler::writeCommandBodiesToStream(fstream& stream)
{
	for (int i = 0; i < m_sequence_nodes.size(); i++)
	{
		SequenceNode* node = m_sequence_nodes.at(i);
		node->setBodyPos(stream.tellg());

		node->writeBodyToStream(stream, m_string_list);
	}
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

		//m_sequence_nodes[index] = node;
		m_sequence_nodes.push_back(node);
	}
}

void SequenceHandler::loadEntryCommandHeads(fstream& stream, int count)
{
	for (int i = 0; i < count; i++)
	{
		EntryCommand* entry_command = new EntryCommand();

		int name_offset;
		readIntFromStream(stream, 4, name_offset);
		entry_command->name = m_string_list->getStringFromOffset(name_offset);

		stream.read(entry_command->guid, 16);
		entry_command->guid[16] = '\0';

		int entry_node_index;
		readIntFromStream(stream, 4, entry_node_index);
		entry_command->entry_node = m_sequence_nodes.at(entry_node_index);

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

			int value;
			readIntFromStream(stream, 4, value);
			node->setInternalParameter(m_parameter_handler->getInternalParameter(section_num, param_index), value, section_num);

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

			int value;
			readIntFromStream(stream, 4, value);

			node->setCommandParameter(m_parameter_handler->getCommandParameter(section_num, param_index), value, section_num);
		}

		// goto beginning of call table
		stream.seekg(body_start_pos + (0x08 * 18), ios::beg);

		int table_entry_count = 0;

		for (int i = 0; i < 10; i++)
		{
			int section_entry_count;
			readIntFromStream(stream, 1, section_entry_count);

			// skip the second number
			stream.seekg(1, ios::cur);
			
			table_entry_count += section_entry_count;
		}

		//stream.seekg(body_start_pos + 0xa3, ios::beg);



		if (table_entry_count == 0)
		{
			continue;
		}

		vector<int> entry_addresses;
		for (int j = 0; j < table_entry_count; j++)
		{
			int entry_address;
			readIntFromStream(stream, 4, entry_address);
			entry_addresses.push_back(entry_address);
		}

		for (int j = 0; j < entry_addresses.size(); j++)
		{
			stream.seekg(entry_addresses.at(j), ios::beg);

			int callee_index;
			readIntFromStream(stream, 4, callee_index);
			SequenceNode* callee = m_sequence_nodes.at(callee_index);

			int param_string_offset;
			readIntFromStream(stream, 4, param_string_offset);
			string param_string = m_string_list->getStringFromOffset(param_string_offset);

			node->addCall(callee, param_string);
			callee->addCaller(node);

		}
	}
}
