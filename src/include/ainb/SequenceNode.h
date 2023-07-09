#pragma once
#include <iostream>
#include <string>
#include "ParameterHandler.h"

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

	void setInternalParameter(InternalParameterBase* parameter, int position);
	void setCommandParameter(CommandParameterBase* parameter, int position);

	//vector<InternalParameterBase*> getInternalParameters();
	std::map<int, InternalParameterBase*> getInternalParameters() { return m_internal_parameters; }
	std::map<int, CommandParameterBase*> getCommandParameters() { return m_command_parameters; }

private:
	char* m_guid;
	int m_body_pos; // position of the command body, should only be used for loading
	std::string m_name;

	std::map<int, InternalParameterBase*> m_internal_parameters;
	std::map<int, CommandParameterBase*> m_command_parameters;

	// todo: body

};

}