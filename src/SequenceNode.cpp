#include "ainb/SequenceNode.h"

using namespace std;
using namespace ainb;

SequenceNode::SequenceNode()
{
	m_guid = nullptr;
	m_body_pos = -1;
	m_name = "";
}

SequenceNode::~SequenceNode()
{

}

void SequenceNode::writeHeadToStream(fstream& stream, int index)
{

}

string SequenceNode::getName()
{
	return m_name;
}

void SequenceNode::setName(string name)
{
	m_name = name;
}

char* SequenceNode::getGUID()
{
	return m_guid;
}

void SequenceNode::setGUID(char* guid)
{
	m_guid = guid;
}

int SequenceNode::getBodyPos()
{
	return m_body_pos;
}

void SequenceNode::setBodyPos(int pos)
{
	m_body_pos = pos;
}

void SequenceNode::setInternalParameter(InternalParameterBase* parameter, int position)
{
	m_internal_parameters[position] = parameter;
}

void SequenceNode::setCommandParameter(CommandParameterBase* parameter, int position)
{
	m_command_parameters[position] = parameter;
}

void SequenceNode::addCall(SequenceNode* node, string param)
{
	if (m_call_table.count(node)) {
		m_call_table[node].call_strings.push_back(param);
		return;
	}

	CallTableEntry entry;
	entry.callee = node;
	entry.call_strings.push_back(param);
	m_call_table[node] = entry;
}
