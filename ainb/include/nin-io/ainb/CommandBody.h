#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "nin-io/util/util.h"
#include "StringList.h"
#include "ExecutionCommand.h"

class CommandBody
{
public:
	CommandBody();
	CommandBody(StringList* string_list);
	~CommandBody();

	void setStringList(StringList* string_list) { m_string_list = string_list; }
	void load(std::fstream& file);
	int getAddress() { return m_address; };

	void setCommandReference(ExecutionCommand* command) { m_command = command; }
	ExecutionCommand* getCommandReference() { return m_command; }

	struct TableKeyValuePair {
		std::string parameter;
		int index;
	};

	std::vector<TableKeyValuePair> getCallTable() { return m_call_table; }

private:
	int m_address;

	StringList* m_string_list;

	ExecutionCommand *m_command;

	std::map<int, int> m_table_parameters;
	std::map<int, int> m_table_values;
	std::map<int, int> m_structure_parameters;
	std::map<int, int> m_structure_values;

	std::vector<TableKeyValuePair> m_call_table;

	static const bool is_big_endian = false;
};