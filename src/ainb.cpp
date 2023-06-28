#include "ainb/ainb.h"

using namespace std;

using namespace ainb;

AINB::AINB(LPSTREAM stream)
{
	m_stream = stream;
	parseHeader();

	m_string_list = new StringList(&m_header_data.string_list_start_pos);
	m_string_list->loadFromStream(m_stream);

	m_parameter_handler = new ParameterHandler(m_string_list);
	parseParameters();

	streamSeek(m_stream, 0x74, START);

	m_sequence_handler = new SequenceHandler(m_string_list);
	m_sequence_handler->loadFromStream(m_stream, m_header_data.entry_command_count, m_header_data.execution_command_count);
}

AINB::~AINB() {
	m_stream->Release();
}

void AINB::parseHeader()
{
	streamSeek(m_stream, 0, START);
	m_stream->Read(m_header_data.type, 3, NULL);
	m_header_data.type[3] = '\0';

	streamSeek(m_stream, 0x0c, START);
	readIntFromStream(m_stream, m_header_data.entry_command_count);
	readIntFromStream(m_stream, m_header_data.execution_command_count);

	streamSeek(m_stream, 0x24, START);
	readIntFromStream(m_stream, m_header_data.string_list_start_pos);

	// parameter table start
	streamSeek(m_stream, 0x2c, START);
	readIntFromStream(m_stream, m_header_data.parameter_table_start);

	// parameter structure start
	streamSeek(m_stream, 0x34, START);
	readIntFromStream(m_stream, m_header_data.parameter_structure_start);

	// parameter structure end
	readIntFromStream(m_stream, m_header_data.parameter_structure_end);

}

void AINB::parseParameters()
{
	streamSeek(m_stream, m_header_data.parameter_table_start, START);
	m_parameter_handler->loadTableParameters(m_stream, m_header_data.parameter_structure_start);

	streamSeek(m_stream, m_header_data.parameter_structure_start, START);
	m_parameter_handler->loadStructureParameters(m_stream, m_header_data.parameter_structure_end);

	return;
	// temp for debugging
	vector<int>* active_tables = m_parameter_handler->getActiveTables();
	for (int i = 0; i < active_tables->size(); i++)
	{
		int table_index = active_tables->at(i);
		vector<ParameterHandler::TableParameter>* table_parameters = m_parameter_handler->getTableParameters(table_index);
		for (int j = 0; j < table_parameters->size(); j++)
		{
			ParameterHandler::TableParameter table_parameter = table_parameters->at(j);
			printf("table_index: %d, parameter_index: %d, parameter_name: %s, parameter_value: %d\n", table_index, table_parameter.index, table_parameter.name.c_str(), table_parameter.value);
		}
	}
	vector<int>* active_structures = m_parameter_handler->getActiveStructures();
	for (int i = 0; i < active_structures->size(); i++)
	{
		int structure_index = active_structures->at(i);
		vector<ParameterHandler::StructureParameter>* structure_parameters = m_parameter_handler->getStructureParameters(structure_index);
		for (int j = 0; j < structure_parameters->size(); j++)
		{
			ParameterHandler::StructureParameter structure_parameter = structure_parameters->at(j);
			printf("structure_index: %d, parameter_index: %d, parameter_name: %s, parameter_tag: %d\n", structure_index, structure_parameter.index, structure_parameter.name.c_str(), structure_parameter.tag);
		}
	}

}

int AINB::getEntryCommandCount()
{
	return m_header_data.entry_command_count;
}

int AINB::getExecutionCommandCount()
{
	return m_header_data.execution_command_count;
}

void AINB::writeToStream(LPSTREAM stream)
{
	stream->Write(m_header_data.type, 3, NULL);
}

SequenceNode* AINB::getSequenceNode(int index)
{
	return m_sequence_handler->getSequenceNode(index);
}

AINB::ParameterStruct AINB::getTableParameter(int section_num, int index)
{
	ParameterStruct return_struct;
	ParameterHandler::TableParameter* parameter = m_parameter_handler->getParameterFromTable(section_num, index);
	return_struct.address = parameter->address;
	_bstr_t converted_name(parameter->name.c_str());
	return_struct.name = SysAllocString(converted_name);
	return_struct.value = parameter->value;
	return return_struct;
}

AINB::ParameterStruct AINB::getStructureParameter(int section_num, int index)
{
	ParameterStruct return_struct;
	ParameterHandler::StructureParameter* parameter = m_parameter_handler->getParameterFromStructure(section_num, index);
	return_struct.address = parameter->address;
	_bstr_t converted_name(parameter->name.c_str());
	return_struct.name = SysAllocString(converted_name);
	return_struct.value = parameter->tag;
	return return_struct;
}

AINB* Create(LPSTREAM stream)
{
	return new AINB(stream);
}

void Destroy(AINB* ainb)
{
	delete ainb;
}

int GetEntryCommandCount(AINB* ainb)
{
	return ainb->getEntryCommandCount();
}

int GetExecutionCommandCount(AINB* ainb)
{
	return ainb->getExecutionCommandCount();
}

void Write(AINB* ainb, LPSTREAM stream)
{
	ainb->writeToStream(stream);
}

SequenceNode::NodeData GetSequenceNodeData(AINB* ainb, int index)
{
	SequenceNode* node = ainb->getSequenceNode(index);
	return node->getData();
}

AINB::ParameterStruct GetTableParameter(AINB* ainb, int section_num, int param_num)
{
	return ainb->getTableParameter(section_num, param_num);
}

AINB::ParameterStruct GetStructureParameter(AINB* ainb, int section_num, int param_num)
{
	return ainb->getStructureParameter(section_num, param_num);
}
