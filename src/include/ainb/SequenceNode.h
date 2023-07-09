#pragma once
#include <iostream>
#include <string>
#include "ParameterHandler.h"

namespace ainb {

class SequenceNode
{
public:

	struct CallTableEntry {
		SequenceNode* callee = nullptr;
		std::vector<std::string> call_strings;
	};

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

	void addCall(SequenceNode* callee, std::string param);
	std::map<SequenceNode*, CallTableEntry> getCallTable() { return m_call_table; }

private:
	char* m_guid;
	int m_body_pos; // position of the command body, should only be used for loading
	std::string m_name;

	std::map<int, InternalParameterBase*> m_internal_parameters;
	std::map<int, CommandParameterBase*> m_command_parameters;

	//std::vector<CallTableEntry> m_call_table;
	std::map<SequenceNode*, CallTableEntry> m_call_table;

	// todo: body

};

}