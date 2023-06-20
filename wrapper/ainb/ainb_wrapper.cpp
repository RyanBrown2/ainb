#include "ainb_wrapper.h"

using namespace std;

AINB_FILE::AINB_FILE()
{
	m_ainb = new AINB();
	m_name = nullptr;
}

AINB_FILE::~AINB_FILE()
{
}

void AINB_FILE::loadFile(string file_dir)
{
	fstream file;

	file.open(file_dir, fstream::in | fstream::out | std::ios::binary);

	if (file.fail()) {
		std::cout << "Failed to open file" << std::endl;
		return;
	}


	m_ainb->load(file);

	CComBSTR temp(m_ainb->getName().c_str());
	m_name = temp.Detach();

	m_file_header_data = m_ainb->getFileHeaderData();

	m_test_node = m_ainb->getSequences()->at(0);

	file.close();
}

BSTR AINB_FILE::getName()
{
	return m_name;
}

int AINB_FILE::getEntryPointCount()
{
	return m_file_header_data.entry_point_command_count;
}

int AINB_FILE::getExecutionCommandCount()
{
	return m_file_header_data.execution_command_count;
}

AINB_FILE::ExportedSequenceNode AINB_FILE::toExportedSequenceNode(SequenceHandler::SequenceNode* sequence_node)
{
	ExportedSequenceNode exported_sequence_node;
	exported_sequence_node.name = CComBSTR(sequence_node->node_command->getName().c_str()).Detach();
	exported_sequence_node.index = sequence_node->node_command->getIndex();
	exported_sequence_node.command_id = sequence_node->node_command->getCommandID();
	exported_sequence_node.guid = CComBSTR(sequence_node->node_command->getGuid()).Detach();
	return exported_sequence_node;
}
