#pragma once
#include <iostream>
#include <string>
#include <objidl.h>

namespace ainb {

class SequenceNode
{
public:
	SequenceNode();
	~SequenceNode();

	std::string getName();
	void setName(std::string name);

	char* getGUID();
	void setGUID(char* guid);

	int getBodyPos();
	void setBodyPos(int pos);

	void writeHeadToStream(std::fstream& stream, int index);

private:
	char* m_guid;
	int m_body_pos; // position of the command body, should only be used for loading
	std::string m_name;

	// todo: body

};

}