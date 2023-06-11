#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nin-io/util/util.h"
#include "BaseCommand.h"
#include "EntryPointCommand.h"
#include "ExecutionCommand.h"
#include "StringList.h"
#include "ParameterNode.h"
#include "Parameters.h"

class AINB
{
public:
	AINB();
	~AINB();
	void load(std::fstream& file);

	struct AinbHeaderData {
		char type[4]; // 0x00 | 3
		int a_commands; // 0x0C | 4
		int b_commands; // 0x10 | 4
		int data_body_start; // 0x20 | 4
		int string_section_start; // 0x24 | 4

		int footer_start; // 0x2c | 4
	};

	std::string getName();
	AinbHeaderData getHeaderData();

	std::vector<EntryPointCommand> getACommands();
	std::vector<ExecutionCommand> getBCommands();

	Parameters::ParameterData getParameterData() { return m_parameter_data; }
	
private:
	static const bool is_big_endian = false;
	void loadFooter(std::fstream& file);
	void loadDataBody(std::fstream& file);

	std::string m_name;
	AinbHeaderData m_header_data;

	std::vector<EntryPointCommand> m_a_commands;
	std::vector<ExecutionCommand> m_b_commands;

	Parameters::ParameterData m_parameter_data;

};