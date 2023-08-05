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
		std::string entry_string;
		SequenceNode* callee = nullptr;
	};

	SequenceNode();
	~SequenceNode();

	std::vector<SequenceNode*> getCallers();
	void addCaller(SequenceNode* caller);
	void removeCaller(SequenceNode* caller);

	// note: index should only be used for loading and saving to file
	int getIndex();
	void setIndex(int index);

	std::string getName();
	void setName(std::string name);

	char* getGUID();
	void setGUID(char* guid);

	int getBodyPos();
	void setBodyPos(int pos);

	void writeHeadToStream(std::fstream& stream, StringList* string_list);

	void setInternalParameter(InternalParameterBase* parameter, int value, int position);
	void setCommandParameter(CommandParameterBase* parameter, int value, int position);

	std::map<int, InternalParameterBase*> getInternalParameters() { return m_internal_parameters; }
	std::map<int, CommandParameterBase*> getCommandParameters() { return m_command_parameters; }

	void addCall(SequenceNode* callee, std::string param);
	std::vector<CallTableEntry> getCallTable() { return m_call_table; }

	void writeBodyToStream(std::fstream& stream, StringList* string_list);

	bool operator==(SequenceNode const& other) const {
		return this->_equals(other);
	}

	bool operator!=(SequenceNode const& other) const {
		return !this->_equals(other);
	}

protected:
	virtual bool _equals(SequenceNode const & other) const;

private:
	std::vector<SequenceNode*> m_callers;

	int m_index;
	char m_guid[17];
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