#include "ainb/ainb.h"

using namespace std;

using namespace ainb;

AINB::AINB(LPSTREAM stream)
{
	m_stream = stream;
	parseHeader();

	m_string_list = new StringList(&m_header_data.string_list_start_pos);
	m_string_list->loadFromStream(m_stream);

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

