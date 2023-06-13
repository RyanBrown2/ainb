#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "nin-io/util/util.h"
#include "StringList.h"
#include "ExecutionCommand.h"
#include "ParameterHandler.h"

class CommandBody
{
public:
	CommandBody();
	CommandBody(StringList* string_list, ParameterHandler* parameter_handler);
	~CommandBody();

	void setStringList(StringList* string_list) { m_string_list = string_list; }
	void setParameterHandler(ParameterHandler* parameter_handler) { m_parameter_handler = parameter_handler; }
	void load(std::fstream& file);
	int getAddress() { return m_address; };

	void setCommandReference(ExecutionCommand* node_command) { m_command = node_command; }
	ExecutionCommand* getCommandReference() { return m_command; }

	struct CallTableEntry {
		int index = -1; // index of the command to call
		std::string parameter = "";
	};

	std::vector<CallTableEntry>* getCallTable() { return &m_call_table; }

	// TODO: potentailly use parameter structs from ParameterHandler.h
	struct Parameter {
		int section_num = -1;
		int index = -1;
		std::string param_name = "";
		int value = -1;
	};

	std::vector<Parameter>* getTableParameters();
	std::vector<Parameter>* getStructureParameters();

private:
	int m_address;

	StringList* m_string_list = nullptr;
	ParameterHandler* m_parameter_handler = nullptr;

	ExecutionCommand* m_command = nullptr;

	std::vector<Parameter> m_table_parameters;
	std::vector<Parameter> m_section_parameters;
	std::vector<CallTableEntry> m_call_table;

	static const bool is_big_endian = false;
};