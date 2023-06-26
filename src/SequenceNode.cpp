#include "ainb/SequenceNode.h"

using namespace std;
using namespace ainb;

SequenceNode::SequenceNode()
{
	m_index = -1;
	m_guid = nullptr;
	m_body_pos = -1;
	m_name = "";
}

SequenceNode::SequenceNode(int index, string name, char* guid)
{
	m_index = index;
	m_guid = guid;
	m_guid[16] = '\0';
	m_body_pos = -1;
	m_name = name;
}

SequenceNode::~SequenceNode()
{

}

int SequenceNode::getIndex()
{
	return m_index;
}

void SequenceNode::setIndex(int index)
{
	m_index = index;
}

string SequenceNode::getName()
{
	return m_name;
}

SequenceNode::NodeData SequenceNode::getData()
{
	NodeData data;
	data.index = m_index;

	_bstr_t guid(m_guid);
	data.guid = SysAllocString(guid);

	_bstr_t name(m_name.c_str());
	data.name = SysAllocString(name);

	return data;
}