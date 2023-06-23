#include "ainb/ainb.h"

using namespace std;

using namespace ainb;

AINB::AINB(LPSTREAM stream) {
	m_stream = stream;
	parseHeader();
}

AINB::~AINB() {
	m_stream->Release();
}

void AINB::parseHeader() {
	streamSeek(m_stream, 0, BEGIN);
	m_stream->Read(m_header_data.type, 3, NULL);
	m_header_data.type[3] = '\0';

	streamSeek(m_stream, 0x0c, BEGIN);
	readIntFromStream(m_stream, m_header_data.entry_command_count);
	readIntFromStream(m_stream, m_header_data.execution_command_count);
}

int AINB::getEntryCommandCount() {
	return m_header_data.entry_command_count;
}

int AINB::getExecutionCommandCount() {
	return m_header_data.execution_command_count;
}

AINB* Create(LPSTREAM stream) {
	return new AINB(stream);
}

void Destroy(AINB* ainb) {
	delete ainb;
}

int GetEntryCommandCount(AINB* ainb) {
	return ainb->getEntryCommandCount();
}

int GetExecutionCommandCount(AINB* ainb) {
	return ainb->getExecutionCommandCount();
}
