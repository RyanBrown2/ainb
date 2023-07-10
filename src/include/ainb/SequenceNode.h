#pragma once
#include <iostream>
#include <string>
#include "StringList.h"
#include "ParameterHandler.h"

namespace ainb {

class SequenceNode
{
public:

	struct CallTableEntry {
		SequenceNode* callee = nullptr;
		std::string call_string;
	};

	SequenceNode();
	~SequenceNode();

	// note: index should only be used for loading and saving to file
	int getIndex();
	void setIndex(int index);

	std::string getName();
	void setName(std::string name);

	char* getGUID();
	void setGUID(char* guid);

	int getBodyPos();
	void setBodyPos(int pos);

	void writeHeadToStream(std::fstream& stream, int index);

	void setInternalParameter(InternalParameterBase* parameter, int value, int position);
	void setCommandParameter(CommandParameterBase* parameter, int value, int position);

	//vector<InternalParameterBase*> getInternalParameters();
	std::map<int, InternalParameterBase*> getInternalParameters() { return m_internal_parameters; }
	std::map<int, CommandParameterBase*> getCommandParameters() { return m_command_parameters; }

	void addCall(SequenceNode* callee, std::string param);
	std::vector<CallTableEntry> getCallTable() { return m_call_table; }

	void writeBodyToStream(std::fstream& stream, StringList* string_list);

private:
	int m_index;
	char* m_guid;
	int m_body_pos; // position of the command body, should only be used for loading
	std::string m_name;

	std::map<int, InternalParameterBase*> m_internal_parameters;
	std::map<int, CommandParameterBase*> m_command_parameters;

	std::map<InternalParameterBase*, int> m_internal_parameter_values;
	std::map<CommandParameterBase*, int> m_command_parameter_values;

	// use vector to preserve order of the calls
	std::vector<CallTableEntry> m_call_table;

};

}