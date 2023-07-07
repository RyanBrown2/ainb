#include "ainb/ainb.h"

using namespace std;

using namespace ainb;

AINB* AINB::loadFromStream(fstream& file)
{
	// get size 
	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// allocate memory
	HGLOBAL hGlobal = GlobalAlloc(GHND, fileSize);
	if (hGlobal == nullptr) {
		// Handle error: memory allocation failed
		file.close();
		return nullptr;
	}

	// get pointer to allocated memory
	void* pBuffer = GlobalLock(hGlobal);
	if (pBuffer == nullptr) {
		// Handle error: memory lock failed
		GlobalFree(hGlobal);
		file.close();
		return nullptr;
	}

	// read file into allocated memory
	file.read(static_cast<char*>(pBuffer), fileSize);

	// create lpstream
	LPSTREAM pStream;
	if (CreateStreamOnHGlobal(hGlobal, FALSE, &pStream) != S_OK) {
		// Handle error: failed to create LPSTREAM
		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
		file.close();
		return nullptr;
	}

	return new AINB(pStream);

	pStream->Release();
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	file.close();
}

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

int* AINB::getCommandParameterCounts()
{
	int* counts = new int[12];
	vector<int>* sections = m_parameter_handler->getActiveCommandParameterTypes();
	for (int i = 0; i < 12; i++)
	{
		if (find(sections->begin(), sections->end(), i) != sections->end()) {
			counts[i] = m_parameter_handler->getCommandParameters(i)->size();
		}
		else {
			counts[i] = 0;
		}
	}
	return counts;
}

ParameterHandler::InternalParameterBase* AINB::getInternalParameterBase(int section_num, int index)
{
	return m_parameter_handler->getInternalParameterBase(section_num, index);
}

ParameterHandler::CommandParameterBase* AINB::getCommandParameterBase(int section_num, int index)
{
	return m_parameter_handler->getCommandParameterBase(section_num, index);
}


//SequenceNode* AINB::getSequenceNode(int index)
//{
//	return m_sequence_handler->getSequenceNode(index);
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

AINB::CommandParameter AINB::exportCommandParameter(ParameterHandler::CommandParameterBase* parameter)
{
	CommandParameter return_struct;

	CComBSTR converted_name(parameter->name.c_str());
	return_struct.name = SysAllocString(converted_name);

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

int* GetCommandParameterCounts(AINB* ainb)
{
	return ainb->getCommandParameterCounts();
}

AINB::InternalParameter GetInternalParameter(AINB* ainb, int section_num, int index)
{
	ParameterHandler::InternalParameterBase* parameter = ainb->getInternalParameterBase(section_num, index);
	return AINB::exportInternalParameter(parameter);
}

AINB::CommandParameter GetCommandParameter(AINB* ainb, int section_num, int index)
{
	ParameterHandler::CommandParameterBase* parameter = ainb->getCommandParameterBase(section_num, index);
	return AINB::exportCommandParameter(parameter);
}


