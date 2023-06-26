#pragma once
#include <iostream>
#include <string>
#include <comdef.h>

namespace ainb {

class SequenceNode
{
public:
	SequenceNode();
	SequenceNode(int index, std::string name, char* guid);
	~SequenceNode();

	struct NodeData {
		int index;
		BSTR guid;
		BSTR name;
	};
	
	int getIndex();
	void setIndex(int index);

	std::string getName();

	NodeData getData();
private:
	int m_index;
	char* m_guid;
	int m_body_pos; // position of the command body
	std::string m_name;

};

}