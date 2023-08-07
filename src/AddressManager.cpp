#include "ainb/AddressManager.h"

using namespace std;
using namespace ainb;

AddressManager::AddressManager(ParameterHandler* parameter_handler, SequenceHandler* sequence_handler)
{
	m_parameter_handler = parameter_handler;
	m_sequence_handler = sequence_handler;
}

AddressManager::~AddressManager()
{

}

int AddressManager::calcNodeBodiesSize()
{
	vector<SequenceNode*> sequence_nodes = m_sequence_handler->getSequenceNodes();

	int node_bodies_size = 0;
	for (int i = 0; i < sequence_nodes.size(); i++)
	{
		node_bodies_size += 0xa4; // this is the base size of a command body (with empty call table)

		// todo: call table entries sizes assumed to be 8 bytes each, not always the case
		int entry_size = 8;
		// get call table size
		int call_table_size = sequence_nodes[i]->getCallTable().size() * (entry_size + 4); // +4 for pointer
		node_bodies_size += call_table_size;
	}

	return node_bodies_size;
}

int AddressManager::calcInternalParamSize()
{
	int total_size = 4*6; // size of the header
	for (int i = 0; i < m_parameter_handler->getActiveInternalParameterTypes()->size(); i++)
	{
		int type = m_parameter_handler->getActiveInternalParameterTypes()->at(i);

		int length = InternalParamEntryLengths.at(type);

		// subtract 1 because the first entry is always the null parameter
		total_size += length * (m_parameter_handler->getInternalParameters(type)->size() - 1);
	}
	return total_size;
}

int AddressManager::calcCommandParamSize()
{
	int total_size = 4 * 12; // size of the header
	for (int i = 0; i < m_parameter_handler->getActiveCommandParameterTypes()->size(); i++)
	{
		int type = m_parameter_handler->getActiveCommandParameterTypes()->at(i);

		int length = CommandParamEntryLengths.at(type);

		// subtract 1 because the first entry is always the null parameter
		total_size += length * (m_parameter_handler->getCommandParameters(type)->size() - 1);
	}
	return total_size;
}

AddressManager::AddressData* AddressManager::calcAddressData()
{
	AddressData* address_data = new AddressData();

	int entry_count = m_sequence_handler->getEntryCommands().size();
	int node_count = m_sequence_handler->getSequenceNodes().size();

	address_data->headers_end_address = 0x20 + (entry_count * 0x18) + (node_count * 0x3c);

	// todo: library currently doesn't support this data, so it will be lost
	int external_ref_section_size = 0x30;

	address_data->node_bodies_start_address = address_data->headers_end_address + external_ref_section_size;

	int node_bodies_size = calcNodeBodiesSize();

	address_data->param_section_start_address = address_data->node_bodies_start_address + node_bodies_size;

	// todo
	address_data->param_unknown_section_2_start_address = address_data->param_section_start_address;

	address_data->internal_param_start_address = address_data->param_unknown_section_2_start_address;

	address_data->command_param_start_address = address_data->internal_param_start_address + calcInternalParamSize();
	address_data->command_param_end_address = address_data->command_param_end_address + calcCommandParamSize();


	address_data->string_list_start_address = address_data->command_param_end_address + 0x20; // todo
	address_data->string_list_4byte_padding = address_data->string_list_start_address - 4;
	address_data->string_list_8byte_padding = address_data->string_list_start_address - 8;

	return address_data;
}