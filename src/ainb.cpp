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

	// internal parameter start
	streamSeek(m_stream, 0x2c, START);
	readIntFromStream(m_stream, m_header_data.internal_parameters_start);

	// parameter structure start
	streamSeek(m_stream, 0x34, START);
	readIntFromStream(m_stream, m_header_data.command_parameters_start);

	// parameter structure end
	readIntFromStream(m_stream, m_header_data.command_parameters_end);

}

void AINB::parseParameters()
{
	streamSeek(m_stream, m_header_data.internal_parameters_start, START);
	m_parameter_handler->loadInternalParameters(m_stream, m_header_data.command_parameters_start);

	streamSeek(m_stream, m_header_data.command_parameters_start, START);
	//m_parameter_handler->loadStructureParameters(m_stream, m_header_data.command_parameters_end);
	m_parameter_handler->loadCommandParameters(m_stream, m_header_data.command_parameters_end);

	return;
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

int* AINB::getInternalParameterCounts()
{
	int* counts = new int[6];
	vector<int>* sections = m_parameter_handler->getActiveInternalParameterTypes();
	for (int i = 0; i < 6; i++)
	{
		if (find(sections->begin(), sections->end(), i) != sections->end()) {
			counts[i] = m_parameter_handler->getInternalParameters(i)->size();
		}
		else {
			counts[i] = 0;
		}
	}
	return counts;
}

ParameterHandler::InternalParameterBase* ainb::AINB::getInternalParameterBase(int section_num, int index)
{
	return m_parameter_handler->getInternalParameterBase(section_num, index);
}


//SequenceNode* AINB::getSequenceNode(int index)
//{
//	return m_sequence_handler->getSequenceNode(index);
//}

//ParameterHandler::InternalParameter AINB::getInternalParameterBase(int section_num, int index)
//{
	//ParameterStruct return_struct;
	//ParameterHandler::InternalParameter* parameter = m_parameter_handler->getInternalParameterBase(section_num, index);
	//return_struct.address = parameter->address;
	//_bstr_t converted_name(parameter->name.c_str());
	//return_struct.name = SysAllocString(converted_name);
	//return_struct.value = parameter->value;
	//return return_struct;
	
//}

//AINB::ParameterStruct AINB::getStructureParameter(int section_num, int index)
//{
//	ParameterStruct return_struct;
//	ParameterHandler::StructureParameter* parameter = m_parameter_handler->getParameterFromStructure(section_num, index);
//	return_struct.address = parameter->address;
//	_bstr_t converted_name(parameter->name.c_str());
//	return_struct.name = SysAllocString(converted_name);
//	return_struct.value = parameter->tag;
//	return return_struct;
//}

AINB::InternalParameter AINB::exportInternalParameter(ParameterHandler::InternalParameterBase* parameter)
{
	InternalParameter return_struct;

	CComBSTR converted_name(parameter->name.c_str());
	return_struct.name = SysAllocString(converted_name);

	CComBSTR converted_value(parameter->value.c_str());
	return_struct.value = SysAllocString(converted_value);
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

int* GetInternalParameterCounts(AINB* ainb)
{
	return ainb->getInternalParameterCounts();
}

AINB::InternalParameter GetInternalParameter(AINB* ainb, int section_num, int index)
{
	ParameterHandler::InternalParameterBase* parameter = ainb->getInternalParameterBase(section_num, index);
	return AINB::exportInternalParameter(parameter);
}

//AINB::InternalParameter GetInternalParameter(AINB* ainb, int section_num, int param_num)
//{
//	return ainb->getInternalParameterBase(section_num, param_num);
//}

